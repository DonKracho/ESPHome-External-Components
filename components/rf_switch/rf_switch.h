#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace rf_switch {

class RfSwitch : public switch_::Switch, public Component {
 public:
  void write_state(bool state) override;
  void dump_config() override;
  
  float get_setup_priority() const { return setup_priority::HARDWARE; }
  void set_output(output::FloatOutput *output) { output_ = output; }
  void set_group(uint16_t group) { group_ = group; }
  void set_channel(uint16_t channel) { channel_ = channel; }

 protected:
  output::FloatOutput *output_{nullptr};
  uint16_t group_{0};
  uint16_t channel_{0};
};

} //namespace rf_switch
} //namespace esphome
