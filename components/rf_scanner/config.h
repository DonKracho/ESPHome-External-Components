#pragma once

namespace esphome {
namespace rf_scanner {

#define RAINFACTOR 0.25f

enum eTx433Type {
  TX_Unknown = -1,
  TX_GTECH,
  TX_DIGOO
};

class Config
{
  public:

  Config() = default;
  virtual ~Config() {};

  bool UseTx() const { return Tx.Type >= 0; }
  bool HasScanPin() const { return Tx.ScanPin >= 0; }
  bool HasRecvPin() const { return Tx.RecvPin >= 0; }
  bool HasLedPin() const { return Tx.LedPin >= 0; }

  // Tx 433 parameters
  struct {
    eTx433Type Type = TX_Unknown;
    int RecvPin = -1;
    int ScanPin = -1;
    int LedPin  = -1;
  } Tx;
};

extern Config cfg;

}  // namespace rf_scanner
}  // namespace esphome
