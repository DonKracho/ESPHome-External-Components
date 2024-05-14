#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace rf_scanner {

class RfScanner : public Component
{
 public:
  RfScanner() = default;

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  uint16_t read_data(uint8_t channel);
  
  void set_datapin(InternalGPIOPin *pin) { dpin_ = pin; };
  void set_scanpin(InternalGPIOPin *pin) { spin_ = pin; };
  
 protected:
  InternalGPIOPin *dpin_{nullptr};
  InternalGPIOPin *spin_{nullptr};
};

}  // namespace rf_scanner
}  // namespace esphome
