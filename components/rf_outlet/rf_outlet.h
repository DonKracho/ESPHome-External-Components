#pragma once

#ifdef USE_ARDUINO

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace rf_outlet {

#define PULSE_COUNT_MAX 200

struct Command {
  int  group{0};
  int  channel{0};
  bool state{false};
};

class RfOutlet : public output::FloatOutput, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_tx_pin(InternalGPIOPin *tx_pin) { tx_pin_ = tx_pin; }
  void set_repeat(uint16_t repeat) { repeat_ = repeat; }
  
  bool isSending();
  bool isCountRemaining() { return mPulseCount < mCnt || mRepeatCount < mRep; };
  uint32_t loadNextPulse();

  InternalGPIOPin *tx_pin_{nullptr}; // digital output pin for driving the data input of the 433MHz sender. Do not use pin 10, it is required for timer compare

 protected:
  void write_state(float state) override;
  
  uint16_t repeat_;

 private:
  bool command(int group, int channel, bool on);
  bool prepareCode(uint32_t code);
  void transmitCode();
  bool beginPulse(int clk, int rep);
  bool addPulse(unsigned char val);
  void endPulse();

  bool isLastPulse() { if (mPulseCount > mCnt) { mSending = false; return true; } return false; }
  void dump() {};

  std::vector<Command> mCommandQueue;

  // in case of interference use the alternative channel group 16 to 32, the master channel will be 18 then
  // rolling codes            group 3,4,7,B / 1,2,9,A  house 1, Master 10         group 1,5,6,A / 2,7,8,B  house A, Master 2
  uint32_t code_odd[4][2] = { 0x158230UL, 0x180C40UL, 0x1B5A70UL, 0x1629B0UL }, { 0xA69710UL, 0xABB250UL, 0xA77E60UL, 0xA460A0UL };
  uint32_t code_evn[4][2] = { 0x12B710UL, 0x1CCE20UL, 0x193390UL, 0x14D6A0UL }, { 0xAF4F20UL, 0xAE0570UL, 0xA9E980UL, 0xAC2AB0UL };

  int nextCode{0};
  int mPulseCount{0};
  int mRepeatCount{0};

  bool mSending{false};
  bool mFilling{false};

  int mLen{250};  // maximum count
  int mCnt{0};    // send cound
  int mRep{1};    // repeat
  int mClk{2500}; // us
  unsigned char duration[PULSE_COUNT_MAX];
};

}  // namespace rf_outlet
}  // namespace esphome

#endif  // USE_ARDUINO
