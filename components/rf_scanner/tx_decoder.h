#include <Arduino.h>

#define TXD_DEBUG
#define TXD_DEBUG_VERBOSE

#define MAX_CODES 30
#define MAX_RECORDS 20

namespace esphome {
namespace rf_scanner {

class TXDecoder {
public:
  struct rec
  {
    bool valid;         // record is valid
    byte txid;          // transmitter ID random when battery gets inserted
    bool battery;       // 1 if battery is low
    bool button;        // true if was triggerd by tx transmit button
    byte channel;       // channel switch of transmitter 1,2,3
    int16_t temperature;// terperature in 1/10 °C
    int16_t humidity;   // humidity in %
    uint32_t timestamp; // time this record was created
  };

  TXDecoder() {};
  ~TXDecoder() {};

  void Loop();
  void Setup();
  bool GetRecord(struct rec &record);
  char *Record2String(struct rec &record);
  char *Record2Debug(struct rec &record);

private:
  bool DecodeRecord(uint64_t value, struct rec &record);
  bool GTechDecodeRecord(uint64_t value, struct rec &record);
  bool PFR130DecodeRecord(uint64_t value, struct rec &record);
  bool DigooDecodeRecord(uint64_t value, struct rec &record);
  void StoreRecord(struct rec &record);
  int  mLastReadRec = -1;
  int  mLastStoredRec = -1;
  struct rec mRecords[MAX_RECORDS];
  char mBuffer[64];
};

extern TXDecoder txd;

}  // namespace rf_scanner
}  // namespace esphome

