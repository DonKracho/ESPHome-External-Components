#include "rf_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rf_scanner {

static const char *const TAG = "RfSensor";

RfSensor::RfSensor(int16_t txid, int16_t chan) : txid_(txid), chan_(chan) {}

float RfSensor::get_setup_priority() const { return setup_priority::DATA; }

void RfSensor::setup() {
	parent_->register_sensor(this);
    last_ms_ = millis();	// initialize timeout counter
	clear_data();			// report status sensor offline
}

void RfSensor::loop() {
  uint32_t time_ms = millis();

  // check for sensor being offline
  if (time_ms > last_ms_ + timeout_ms_) {
    clear_data();
  }
}

void RfSensor::dump_config() {
  LOG_TEXT_SENSOR("", "RF Sensor", this);
  ESP_LOGCONFIG(TAG, "  txid: %d", this->txid_);
  ESP_LOGCONFIG(TAG, "  chan: %d", this->chan_);
  
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  LOG_SENSOR("  ", "Precipitation", this->precipitation_sensor_);
}

void RfSensor::update() {
	String res;
    
	last_ms_ = millis();	// update timeout counter
	
	if (hasdata_) {
		status_clear_warning();
		res = "{ txid: " + String(data_.txid) + ", chan: " + String(data_.channel);
		if (data_.channel == 3) {
	  		res += ", perc: " + String(data_.precipitation, 2);
		} else {
	  		res += ", temp: " + String(data_.temperature, 1) + ", humi: " + String(data_.humidity, 0);
		}
		res += ", batt: " + String(data_.battery ? "LOW" : "OK") + " }" ;
	} else {
		status_set_warning();
		res = "sensor offline";
	}

	if (temperature_sensor_ != nullptr)
		temperature_sensor_->publish_state(data_.temperature);
	if (humidity_sensor_ != nullptr)
		humidity_sensor_->publish_state(data_.humidity);
	if (precipitation_sensor_ != nullptr)
		precipitation_sensor_->publish_state(data_.precipitation);

	publish_state(res.c_str());
}

void RfSensor::update_data(struct SensorMessage *data) {
	data_ = *data;
	hasdata_ = true;
	update();
}

void RfSensor::clear_data() {
	SensorMessage data;
	data_ = data;
	hasdata_ = false;
	update();
}

}  // namespace rf_scanner
}  // namespace esphome
