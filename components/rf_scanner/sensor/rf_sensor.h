#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"

#include "../rf_scanner.h"

namespace esphome {
namespace rf_scanner {

class RfSensor : public PollingComponent, public Parented<RfScanner>, public sensor::Sensor
{
 public:
  RfSensor(uint8_t txid);

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }
  void set_precipitation_sensor(sensor::Sensor *precipitation_sensor) { precipitation_sensor_ = precipitation_sensor; }
  uint8_t get_txid() { return txid_; }
  void update_data(struct SensorMessage *data) { data_ = *data; hasdata_ = true; update(); }

 protected:
  bool hasdata_ = false;
  struct SensorMessage data_;
  uint8_t txid_{0};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};  
  sensor::Sensor *precipitation_sensor_{nullptr};  
};

}  // namespace rf_scanner
}  // namespace esphome
