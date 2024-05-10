#include "esphome/core/log.h"
#include "rf_switch.h"

namespace esphome {
namespace rf_switch {

static const char *TAG = "rf_switch.switch";

void RfSwitch::write_state(bool state) {
    // This will be called every time the user requests a state change.
	
	float value = (state) ? 0.5f : 0.0f;
    output_->set_level(value + (0.001f * channel_));

    // Acknowledge new state by publishing it
    publish_state(state);
}

void RfSwitch::dump_config() {
    ESP_LOGCONFIG(TAG, "RF custom switch");
	// LOG_BINARY_OUTPUT(output_); //can't be used because it accesses protected members.
	ESP_LOGCONFIG(TAG, "  channel: %d", channel_);
}

} //namespace rf_switch
} //namespace esphome