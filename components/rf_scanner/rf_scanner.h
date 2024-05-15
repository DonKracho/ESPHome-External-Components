#pragma once

#define RF_SCANNER
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace rf_scanner {

class RfSensor;

struct SensorMessage
{
  uint8_t txid{0};
  uint8_t channel{0};
  float   temperature{NAN};
  float   humidity{NAN};
  float   precipitation{NAN};
  bool    battery{true};
};

class RfScanner : public Component
{
 public:
  RfScanner() = default;

  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void register_sensor(RfSensor *sensor) { sensors_.push_back(sensor); }
  
  void set_datapin(InternalGPIOPin *pin) { dpin_ = pin; };
  void set_scanpin(InternalGPIOPin *pin) { spin_ = pin; };
  void set_ledpin(InternalGPIOPin *pin)  { lpin_ = pin; };

 protected:
  bool scan_data(struct SensorMessage *message);
  std::vector<RfSensor*> sensors_;
  InternalGPIOPin *dpin_{nullptr};
  InternalGPIOPin *spin_{nullptr};
  InternalGPIOPin *lpin_{nullptr};
};

}  // namespace rf_scanner
}  // namespace esphome
