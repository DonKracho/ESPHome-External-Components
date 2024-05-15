#include "rf_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rf_scanner {

static const char *const TAG = "RfSensor";

RfSensor::RfSensor(uint8_t txid) : txid_(txid) {}

float RfSensor::get_setup_priority() const { return setup_priority::DATA; }

void RfSensor::setup() {
	parent_->register_sensor(this);
}

void RfSensor::dump_config() {
  LOG_SENSOR("", "RF Sensor", this);
  ESP_LOGCONFIG(TAG, "  txid: %u", this->txid_);
  LOG_UPDATE_INTERVAL(this);
  
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  LOG_SENSOR("  ", "Precipitation", this->precipitation_sensor_);
}

void RfSensor::update() {
	if (hasdata_) {
		ESP_LOGD(TAG, "Got txid=%d chan=%d temp=%.1f°C humi=%.0f%% prec=%.2fmm", data_.txid, data_.channel, data_.temperature, data_.humidity, data_.precipitation);
		status_clear_warning();
	} else {
		ESP_LOGW(TAG, "Invalid readings!");
		status_set_warning();
	}

	if (temperature_sensor_ != nullptr)
		temperature_sensor_->publish_state(data_.temperature);
	if (humidity_sensor_ != nullptr)
		humidity_sensor_->publish_state(data_.humidity);
	if (precipitation_sensor_ != nullptr)
		precipitation_sensor_->publish_state(data_.precipitation);

	publish_state(0.1f * data_.channel + data_.txid); // sensor float state
}

}  // namespace rf_scanner
}  // namespace esphome
