#include "rf_scanner.h"
#include "config.h"
#include "esphome/core/log.h"
#include "esphome.h"

namespace esphome {
namespace rf_scanner {

Config cfg;

static const char *const TAG = "rf_scanner";
static uint16_t  gauge_count = 0;

float RfScanner::get_setup_priority() const { return setup_priority::HARDWARE; }

void RfScanner::setup() {
  // trasnfer settings to global Config class which is used a wrapper do old tx_decoder code
  dpin_->setup();
  cfg.Tx.RecvPin = dpin_->get_pin();
  if (spin_) {
    spin_->setup();
    cfg.Tx.ScanPin = spin_->get_pin();
  }
  if (lpin_) {
    lpin_->setup();
    cfg.Tx.LedPin = lpin_->get_pin();
  }
  cfg.Tx.Type = TX_DIGOO;   // use TX_GTECH to decode GT-WT-02 sensors
  txd.Setup();
}

void RfScanner::loop() {
#if 0  // generate a test record every 10s
  static uint32_t last_us = 0;
  uint32_t time_us = millis();

  if (time_us > last_us + 10000UL) {
    ESP_LOGI(TAG, "received data");
    last_us = time_us;
    SensorMessage data;
    scan_data(&data);
#else
    TXDecoder::rec scan;
    txd.Loop();
    if (txd.GetRecord(scan)) {
      SensorMessage data;
      data.txid = scan.txid;
      data.channel = scan.channel;
	    if (data.channel == 3) {  // channel 3 has special encoding for rain count
		    data.precipitation = scan.humidity * RAINFACTOR;
	    } else {
		    data.humidity = scan.humidity;
		    data.temperature = scan.temperature * 0.1f;
	    }
      data.battery = scan.battery;
#endif
    for (auto sensor : sensors_) {
      int16_t txid = sensor->get_txid();
      int16_t chan = sensor->get_chan();

      // ATTENTION: specyfying a sensor txid in config.yaml requires an uptate and uploading of a new configuration after battery change!  
      if ( (txid == data.txid && chan == data.channel)  // txid and chan is given (exact match)
        || (txid == data.txid && chan == -1)            // txid is given only (still very reliable because there are 255 ids available)
        || (txid == -1 && chan == data.channel)         // chan is given only (may interfer with sensors nearby)
        || (txid == -1 && chan == -1) ) {               // debug sensor (catches all sensors not found before)
        sensor->update_data(&data);
        break;
      }
    }
  }
}

void RfScanner::dump_config() {
  ESP_LOGCONFIG(TAG, "RfSensor:");
  LOG_PIN("  Data Pin: ", dpin_);
  LOG_PIN("  Scan Pin: ", spin_);
  LOG_PIN("  LED  Pin: ", lpin_);
}

bool RfScanner::scan_data(struct SensorMessage *message)
{
  // generate some test data
  uint8_t ids[8][2] { { 179, 1 }, {20, 2}, {135, 3}, {214, 4}, {39, 1}, {127, 4}, {222, 2}, {45, 3} };
  uint8_t index = rand() % 8;
  message->txid = ids[index][0];
  message->channel = ids[index][1];
	if (message->channel == 4) {
		message->precipitation = gauge_count++ * 0.25f;
	} else {
		message->humidity = 65.0f + (rand() % 100) * 0.05f;
		message->temperature = 22.1f + (rand() % 100) * 0.01f;
	}
  return true;
}

}  // namespace rf_scanner
}  // namespace esphome
