#include "esphome/core/log.h"
#include "rf_switch.h"

namespace esphome {
namespace rf_switch {

static const char *TAG = "rf_switch.switch";

void RfSwitch::write_state(bool state) {
  // This will be called every time the user requests a state change.
  // ESPHome float_output does allow states between 0.0 and 1.0 only.
  // Do some magic to code channel and state into a float value
  float value = (state) ? 0.5f : 0.0f;
  output_->set_level(value + (0.001f * channel_) + (0.016f  * group_));

  // optimistic acknowledge of new state by publishing it
  publish_state(state);
}

void RfSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "RF custom switch");
  // LOG_BINARY_OUTPUT(output_); //can't be used because it accesses protected members.
  ESP_LOGCONFIG(TAG, "  channel: %d", channel_);
}

} //namespace rf_switch
} //namespace esphome
