#pragma once

//#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace rf_transmitter {

class RfTransmitter : public Component {
 public:
  void setup() override;
  void loop() override;
  void turn_on(uint16_t channel);
  void turn_off(uint16_t channel);
  void dump_config() override;
  
  float get_setup_priority() const { return setup_priority::HARDWARE; }
  void set_rfpin(InternalGPIOPin *pin) { rfpin_ = pin; }
  void set_channel(uint16_t channel) { channel_ = channel; }
  void set_repeat(uint16_t repeat) { repeat_ = repeat; }

 protected:
  InternalGPIOPin *rfpin_{nullptr};
  uint16_t channel_{0};
  uint16_t repeat_{0};
};

} //namespace rf_switch
} //namespace esphome