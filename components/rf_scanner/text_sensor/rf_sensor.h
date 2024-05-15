#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#include "../rf_scanner.h"

namespace esphome {
namespace rf_scanner {

//class RfSensor : public PollingComponent, public Parented<RfScanner>, public sensor::Sensor
class RfSensor : public Component, public Parented<RfScanner>, public text_sensor::TextSensor
{
 public:
  RfSensor(int16_t txid, int16_t chan);

  void setup() override;
  void loop() override;
  void update();
  void dump_config() override;
  float get_setup_priority() const override;
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }
  void set_precipitation_sensor(sensor::Sensor *precipitation_sensor) { precipitation_sensor_ = precipitation_sensor; }
  int16_t get_txid() { return txid_; }
  int16_t get_chan() { return chan_; }
  void update_data(struct SensorMessage *data);
  void clear_data();
 
 protected:
  bool hasdata_ = false;
  uint32_t last_ms_{0};  // timestamp of last reception
  uint32_t timeout_ms_{600000ul}; // 10 min
  struct SensorMessage data_;
  int16_t txid_{0};
  int16_t chan_{0};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};  
  sensor::Sensor *precipitation_sensor_{nullptr};  
};

}  // namespace rf_scanner
}  // namespace esphome
