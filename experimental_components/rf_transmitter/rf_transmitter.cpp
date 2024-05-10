#include "rf_transmitter.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rf_transmitter {

static const char *TAG = "rf_transmitter";

void RfTransmitter::setup() {
	rfpin_->setup();
	rfpin_->digital_write(false);
}

void RfTransmitter::loop() {

}

void RfTransmitter::turn_on(uint16_t channel) {
	rfpin_->digital_write(true);
}

void RfTransmitter::turn_off(uint16_t channel) {
	rfpin_->digital_write(false);
}

void RfTransmitter::dump_config() {
    ESP_LOGCONFIG(TAG, "RF transmitter");
	LOG_PIN("  rf_pin: ", rfpin_);
	ESP_LOGCONFIG(TAG, "  reapeat: %d", repeat_);
}

} //namespace rf_switch
} //namespace esphome