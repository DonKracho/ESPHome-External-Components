#ifdef USE_ARDUINO

#include "rf_outlet.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cmath>

#ifdef USE_ESP8266
#include "esphome.h"	// get full control for timer1
#endif
#ifdef USE_ESP32_FRAMEWORK_ARDUINO
#include <esp32-hal-timer.h>
#endif

namespace esphome {
namespace rf_outlet {

static const char *const TAG = "rf_outlet";

class RfOutlet *pwm{nullptr};

void IRAM_ATTR HOT timer_interrupt()
{
  if (pwm->isCountRemaining()) pwm->tx_pin_->digital_write(!pwm->tx_pin_->digital_read());  // Toggle tx_pin_
  pwm->loadNextPulse();
}

#ifdef USE_ESP32
// ESP32 implementation, uses basically the same code but needs to wrap
// timer_interrupt() function to auto-reschedule
static hw_timer_t *rf_timer = nullptr;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
void IRAM_ATTR HOT s_timer_intr() { timer_interrupt(); }
#endif

void RfOutlet::setup() {
  pwm = this; // expose this class to isr routine

  tx_pin_->setup();
  tx_pin_->digital_write(false);
}

void RfOutlet::loop() {
  // sending a command code squence takes a while, subsequent commands have to wait until transmitting has finished
  // Check for command in the queue when transmitter is not sending
  if (!isSending() && !mCommandQueue.empty()) {
    Command cmd = mCommandQueue.front();
    mCommandQueue.erase(mCommandQueue.begin());
    command(cmd.channel, cmd.state);
    ESP_LOGD(TAG, "channel %d %s", cmd.channel, cmd.state ? "on": "off");
  }
}

void RfOutlet::write_state(float state) {
  //ESP_LOGD(TAG, "write_state(%f)", state);
  Command cmd;
  cmd.channel = 1000.0f * state + 0.5f;	// some wired float estimations;)
  cmd.state = cmd.channel >= 500;
  if (cmd.state) cmd.channel -= 500;
  mCommandQueue.push_back(cmd);
}

void RfOutlet::dump_config() {
  ESP_LOGCONFIG(TAG, "RfOutlet:");
  LOG_PIN("  tx_pin: ", tx_pin_);
  ESP_LOGCONFIG(TAG, "   repeat %d", mRepeatCount);
  LOG_FLOAT_OUTPUT(this);
}

uint32_t RfOutlet::loadNextPulse()
{
  uint32_t pulse_us{0};
  
  if (mPulseCount < mCnt) {
    timer1_write(duration[mPulseCount++] * mClk);
  } else if (mRepeatCount < mRep) {
    mRepeatCount++;
    mPulseCount = 0;
    pulse_us = duration[mPulseCount++] * mClk;
    timer1_write(pulse_us);
  } else {
    tx_pin_->digital_write(false);    // esure to disable RF transmitter
    timer1_detachInterrupt();
    timer1_disable();
    mSending = false;
  }
  return pulse_us;
}

bool RfOutlet::command(int channel, bool state)
{
  bool ret = !isSending();  
  if (ret) {
    if (channel < 0 || channel > 15) return false;
    
    uint32_t code = (((channel & 2) && state) || (!(channel & 2) && !state)) ? code_evn[nextCode] : code_odd[nextCode];
    nextCode = (nextCode < 3) ? nextCode + 1 : 0;
    prepareCode(code | channel);
    transmitCode(); // sending the code is interrupt driven. The main loop should continue. Use isSending() for checking status.
  }
  return ret;
}

bool RfOutlet::prepareCode(uint32_t code)
{
  ESP_LOGD(TAG, "code: 0x%s", String(code,HEX).c_str());
  beginPulse(345, repeat_);
  addPulse(1);
  addPulse(7);
  for (int i = 23; i >= 0; i--)
  {
    if (code & (1L<<i))
    {
      addPulse(3);
      addPulse(1);
    }
    else
    {
      addPulse(1);
      addPulse(3);
    }
  }
  endPulse();
  return true;
}

void RfOutlet::transmitCode()
{
  if (mSending)
  {
    //dump();                                         // for http://test.sui.li/oszi/
    mPulseCount = 0;                                  // reset sending counts
    mRepeatCount = 0;                                 // reset repeat count
    tx_pin_->digital_write(true);                     // start RF sending, Interrupt will toggle
    // now start timer 1 to send prepared code
    timer1_attachInterrupt(timer_interrupt);          // interrupt service routine
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);   // TIM_DIV16 = 5MHz; 0.2us
    timer1_write(duration[mPulseCount++] * mClk);     // peload first pulse length
  }
}

bool RfOutlet::isSending()
{
  return mSending || mFilling;
}

bool RfOutlet::beginPulse(int clkus, int repeat)
{
  mFilling = true;
  mCnt = 0;
  mRep = repeat - 1;  // starts counting at 0
  mClk = clkus * 5;
  return true;
}

bool RfOutlet::addPulse(unsigned char val)
{
  if (mCnt >= mLen) return false;
  duration[mCnt++] = val;
  return true;
}

void RfOutlet::endPulse()
{
  if (mCnt <= 0 || (mCnt & 1) != 0)
  {
    ESP_LOGCONFIG(TAG, "Warning: invalid pulse count of %d", mCnt);
  }
  else
  {
    mSending = true;
  }
  mFilling = false;
}

}  // namespace rf_outlet
}  // namespace esphome

#endif  // USE_ARDUINO
