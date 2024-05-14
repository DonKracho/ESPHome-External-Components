#include "rf_scanner.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rf_scanner {

static const char *const TAG = "rf_sensor";

float RfScanner::get_setup_priority() const { return setup_priority::HARDWARE; }

void RfScanner::setup() {
  dpin_->setup();
  if (spin_) spin_->setup();
}

void RfScanner::dump_config() {
  ESP_LOGCONFIG(TAG, "RfSensor:");
  LOG_PIN("  Data Pin:", dpin_);
  if (spin_) LOG_PIN("  Scan Pin:", spin_);
}

uint16_t RfScanner::read_data(uint8_t channel) {
  uint8_t control = channel << 3;

  uint16_t digital_value = channel;

  return digital_value;
}

}  // namespace rf_scanner
}  // namespace esphome
