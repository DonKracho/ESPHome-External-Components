#pragma once

#include "rf_transmitter.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace rf_transmitter {

template<typename... Ts> class TurnOnAction : public Action<Ts...> {
 public:
  explicit TurnOnAction(RfTransmitter *rftransmitter) : rftransmitter_(rftransmitter) {}

  void play(Ts... x) override { this->rftransmitter_->turn_on(channel_); }

 protected:
    RfTransmitter *rftransmitter_{nullptr};
    uint16_t channel_{0};
};

template<typename... Ts> class TurnOffAction : public Action<Ts...> {
 public:
  explicit TurnOffAction(RfTransmitter *rftransmitter) : rftransmitter_(rftransmitter) {}

  void play(Ts... x) override { this->rftransmitter_->turn_off(channel_); }

 protected:
    RfTransmitter *rftransmitter_{nullptr};
    uint16_t channel_{0};
};

}  // namespace rf_transmitter
}  // namespace esphome