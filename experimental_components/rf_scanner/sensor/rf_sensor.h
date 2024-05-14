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
  RfSensor(uint8_t channel);

  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;
  float sample();
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }

 protected:
  bool read_sensor_(float *temperature, float *humidity, uint8_t channel);

  uint8_t channel_;
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};  
};

}  // namespace rf_scanner
}  // namespace esphome
