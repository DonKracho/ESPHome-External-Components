#include "rf_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rf_scanner {

static const char *const TAG = "RfSensor";

RfSensor::RfSensor(uint8_t channel) : channel_(channel) {}

float RfSensor::get_setup_priority() const { return setup_priority::DATA; }

void RfSensor::dump_config() {
  LOG_SENSOR("", "RF Sensor", this);
  ESP_LOGCONFIG(TAG, "  Pin: %u", this->channel_);
  LOG_UPDATE_INTERVAL(this);
  
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
}

bool RfSensor::read_sensor_(float *temperature, float *humidity, uint8_t channel) {
	if (channel != 110)
		return false;
	*humidity = 65.0f;
	*temperature = 22.1f;
	return true;
}

float RfSensor::sample() {
	return this->parent_->read_data(this->channel_);
}

void RfSensor::update() {
	float temperature, humidity;
	bool success;

    success = this->read_sensor_(&temperature, &humidity, this->channel_);

	if (success) {
		ESP_LOGD(TAG, "Got Temperature=%.1f°C Humidity=%.1f%%", temperature, humidity);

		if (this->temperature_sensor_ != nullptr)
			this->temperature_sensor_->publish_state(temperature);
		if (this->humidity_sensor_ != nullptr)
			this->humidity_sensor_->publish_state(humidity);
		this->status_clear_warning();
	} else {
		const char *str = "";
		ESP_LOGW(TAG, "Invalid readings!");
		if (this->temperature_sensor_ != nullptr)
			this->temperature_sensor_->publish_state(NAN);
		if (this->humidity_sensor_ != nullptr)
			this->humidity_sensor_->publish_state(NAN);
		this->status_set_warning();
	}
	this->publish_state(this->sample());
}

}  // namespace rf_scanner
}  // namespace esphome
