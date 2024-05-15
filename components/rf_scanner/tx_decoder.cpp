/*
  Kompatible 433MHz Funk Temperatur/Luftfeuchte Sensoren:
  Funksensor GT-WT-02 (Globaltronics GmbH & Co. KG, DomStr. 19, 20095 Hamburg)
  Funksensor NT-1959 (Latupo GmbH, Waterloohain 5, 22769 Hamburg PJN-Nr.: EL201523-01) (Pollin)
  und baugleiche Sensoren

  für Wetter Stationen wie GT-WS-08 und GT-WS-09 (Aldi)

  Ein Sendeimpuls (Modulation aktiv) ist zwischen 480 und 600us lang gefolgt von einer Pause (Modulation ausgeschaltet)
  Eine Puls Pause Folge entspricht einem Bit oder Sync Impuls
  die Länge der Pause ist hier definiert zu:
  2070us 0
  4140us 1
  9060us Start/End
  Sync zwischen den Ütertragungen der 37 Bit Pakete 9060 20180 9060
     _
  0 | |_ _ _ 2700us             ca. 2/7 * 300us
     _
  1 | |_ _ _ _ _ _  4730 us     ca. 2/14 * 300us
     _                                 _       _
  S | |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _| |_ _ _| |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ Beginn der Übertragung

  Protokoll: 37 Bit pro Daten Paket
  Zwischen den Datenpaketen ist ein Sync Impuls und wird insgesamt 6 mal wiederholt
  Ca. alle 56s wird ein neues Datenpaket gesendet
  Der Anfang der ersten Übertragung kann stark gestört sein bis die AGC des Empfängers (Pendel-Audion) eingeregelt ist.

  Beispiel LOG 6 aufeinander folgender 6 Datenpakete mit Programm RTL_433:

  [01] {37} d9 01 07 61 20 : 11011001 00000001 00000111 01100001 00100000
  [02] {37} d9 01 07 61 20 : 11011001 00000001 00000111 01100001 00100000
  [03] {37} d9 01 07 61 20 : 11011001 00000001 00000111 01100001 00100000
  [04] {37} d9 01 07 61 20 : 11011001 00000001 00000111 01100001 00100000
  [05] {37} d9 01 07 61 20 : 11011001 00000001 00000111 01100001 00100000
  [16] {37} d9 01 07 61 20 : 11011001 00000001 00000111 01100001 00100000

  Codierung des Signals:

  0000000001111111111222222222233333333
  0123456789012345678901234567890123456
  KKKKKKKKBSCCTTTTTTTTTTTTHHHHHHHPPPPPP
  OOOOOOOOAEHHEEEEEEEEEEEEUUUUUUURRRRRR
  DDDDDDDDTNNNMMMMMMMMMMMMMMMMMMMÜÜÜÜÜÜ
  EEEEEEEETDLLPPPPPPPPPPPPIIIIIIIFFFFFF

  K = 00-07,  8 Bit = Wechselnder Code bei Batteriewechsel
  B = 08-08,  1 Bit = Batteriestatus 0 = ok, 1 = low
  S = 09-09,  1 Bit = Sendtaste gedrückt = 1, pairing?
  C = 10-11,  2 Bit = Kanal, 00 = K1, 01 = K2, 10 = K3
  T = 12-24, 12 Bit = Temperatur, binär MSB->LSB o. Komma 26,0=260, 2er Komplement
                Bit 12 MSB 0=positiv, 1=negativ (4095+1 - Bit13-23 bin2dez)
  H = 25-30,  7 Bit = Feuchte, binär, Bereich 20-90% LL=10%, HH=110%
  P = 31-36,  6 Bit = Prüfsumme 8 Nibbles aufsummiert
                      bin2Dez Bit 00-03 +
                      bin2Dez Bit 04-07 +
                      bin2Dez Bit 08-11 +
                      bin2Dez Bit 12-15 +
                      bin2Dez Bit 16-21 +
                      bin2Dez Bit 22-23 +
                      bin2Dez Bit 24-27 +
                      bin2Dez Bit 28-30 + Fill Bit 0 =
                      Summe
                      Prüfsumme = Summe Modulo 64
*/

/*
  Digoo DG-TH8898 Wetterstation (433MHz Funk Temperatur/Luftfeuchte Sensoren und Regenmesser)

  Ein Sendeimpuls (Modulation aktiv) ist 520 lang gefolgt von einer Pause (Modulation ausgeschaltet)
  Eine Puls Pause Folge entspricht einem Bit oder Sync Impuls
  die Länge der Pause ist hier definiert zu:
     _
  0 | |_ _ _ 1480 us
     _
  1 | |_ _ _ _ _ _  2440 us
     _
  S | |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 4400 us Beginn eine Datenpaketes

  Protokoll: 36 Bit pro Daten Paket
  Zwischen den Datenpaketen ist ein Sync Impuls. Das Paket wird insgesamt 10 mal wiederholt.
  Der Anfang der ersten Übertragung kann stark gestört sein bis die AGC des Empfängers (Pendel-Audion) eingeregelt ist.

  Codierung des Signals:

  000000000111111111122222222223333333
  012345678901234567890123456789012345
  KKKKKKKKBSCCTTTTTTTTTTTTXXXXHHHHHHHH

  Temperatursensor:
  K = 00-07,  8 Bit = Wechselnder Code bei Batteriewechsel (nur Temperatur/Luftfeuchte Sensor)
  B = 08-08,  1 Bit = Batteriestatus 0 = ok, 1 = low
  S = 09-09,  1 Bit = 0 (eine Sendtaste gibt es nicht)
  C = 10-11,  2 Bit = Kanal, 00 = K1, 01 = K2, 10 = K3, (11 = K4 Regensensor)
  T = 12-23, 12 Bit = Temperatur, binär MSB->LSB o. Komma 26,0=260, 2er Komplement
                      Bit 12 MSB 0=positiv, 1=negativ (4095+1 - Bit13-23 bin2dez)
  X = 24-27   4 Bit = Sensor Typ 1111 für Temperatur/Luftfeuchte Sensor
  H = 28-35,  8 Bit = Luftfeuchte in %, binär Wert (Werte unter 20 zeigt das Display als 20% an)

  Regensensor:
  K = 00-07   8 Bit = ID (wechselt nicht bei Batterieweechsel, aber abweichende, feste Codes je Regenmesser)
  B = 08-08,  1 Bit = Batteriestatus 0 = ok, 1 = low
  S = 09-09,  1 Bit = 0 (eine Sendtaste gibt es nicht)
  C = 10-11,  2 Bit = 11 (Kanal 4)
  R = 12-35  20 Bit = Anzahl der Wippenschläge seit einlegen der Batterien (1 Wipenschlag == 0.3mm)

  Vereinzelt kam es zu einem Reset des Zählers auf 0. Das war aber auf die wackelige Batteriehalterung zurückzuführen.

  Öberfläche 49x109,5mm - (11*11 - 2pi*5.5) (abgerundete Ecken) = 5340 mm² = 0,00534m²

  Achtung: Es gibt keine Checksumme, die Übertragung ist daher sehr unsicher.
  Im Decoder Loop müssen daher 2 gesendete Pakete gleich sein bevor sie decodiert werden.
*/

#include "tx_decoder.h"
#include "config.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rf_scanner {

TXDecoder txd;

static const char *const TAG = "tx_decoder";

// global variables to be accessed from interrupt handler and Loop
uint64_t Codes[MAX_CODES];
volatile bool dataReady = false;
volatile byte CodeBeg = 0;
volatile byte CodeEnd = 0;
volatile bool scanning = false;

byte CodesIndexIncrement(byte curr)
{
  // increments index of CodesBuffer
  if (++curr >= MAX_CODES) curr = 0;
  return curr;
}

// interrupt service routines for GTech 433MHz transmitters

void IRAM_ATTR GTechRecvSignal()
{
  static unsigned long lastRise = 0;
  static unsigned long lastTime = 0;
  static unsigned long lastSync = 0;
  static uint64_t code = 0;
  static byte id = 0;
  static byte cnt = 0;
  bool valid = false;

  unsigned long time_us = micros();

  if (digitalRead(cfg.Tx.RecvPin)) {            // rising edge
    lastRise = time_us;                         // store for measuring active modulation pulse length
    if (time_us - lastSync > 200000ul) {
      CodeBeg = CodeEnd;
    }
  } else {                                      // falling edge
    unsigned long duration = time_us - lastRise;
    if (duration > 460ul && duration < 800ul) { // detected pulse between 400 and 800us
      // check for valid pulse length
      duration = lastRise - lastTime;           // calculate duration to last rising edge
      lastTime = lastRise;                      // store current rising edge
      if (duration > 2500ul && duration < 5000ul) {
        code <<= 1;                             // 0 detected
        if (duration > 4500ul) {                // 1 detected
          code |= 1;                            // just put a 1 to LSB
        }
        valid = true;
        cnt++;
      } else {                                  // wrong length or sync
        if (duration >= 9500ul) {               // sync detected
          if (cnt == 37) {                      // rereived data of 36 bit length
            Codes[id] = code;
            id = CodesIndexIncrement(id);
            CodeEnd = id;                       // set current id one beind last valid for loop()
            valid = true;
            dataReady = true;                   // loop() has to analyze last transmissions
            lastSync = time_us;
          }
          cnt = 0;                              // reset count and code
          code = 0;
        }
      }
    }
  }
  if (cfg.HasLedPin()) digitalWrite(cfg.Tx.LedPin, !valid); // LED signals stable reception with long flashes for > 6x120 ms
}

// interrupt service routines for Digoo 433MHz transmitters

void IRAM_ATTR DigooScanSignal()
{
  scanning = digitalRead(cfg.Tx.ScanPin);
  if (cfg.HasLedPin()) digitalWrite(cfg.Tx.LedPin, !scanning);  // LED signals stable reception with long flashes for > 6x120 ms
  if (scanning) {
    CodeBeg = CodeEnd;                          // remember index of last transmission
  } else {
    dataReady = true;                           // loop() has to analyze last transmissions
  }
}

void IRAM_ATTR DigooRecvSignal()
{
  static unsigned long lastRise = 0;
  static unsigned long lastTime = 0;
  static unsigned long lastSync = 0;
  static uint64_t code = 0;
  static byte id = 0;
  static byte cnt = 0;
  bool valid = false;

  unsigned long time_us = micros();

  if (digitalRead(cfg.Tx.RecvPin)) {            // rising edge
    lastRise = time_us;                         // store for measuring active modulation pulse length
    if (!cfg.HasScanPin() && (time_us - lastSync) > 100000ul) {
      CodeBeg = CodeEnd;
    }
  } else {                                      // falling edge
    unsigned long duration = time_us - lastRise;
    if (duration > 400ul && duration < 600ul) { // detected pulse between 400 and 800us
      // check for valid pulse length
      duration = lastRise - lastTime;           // calculate duration to last rising edge
      lastTime = lastRise;                      // store current rising edge
      if (duration > 1200ul && duration < 4000ul) {
        code <<= 1;                             // 0 detected
        if (duration > 2000ul) {                // 1 detected
          code |= 1;                            // just put a 1 to LSB
        }
        valid = true;
        cnt++;
      } else {                                  // wrong length or sync
        if (duration >= 4000ul) {               // sync detected
          if (cnt == 36) {                      // rereived data of 36 bit length
            Codes[id] = code;
            id = CodesIndexIncrement(id);
            CodeEnd = id;                       // set current id one beind last valid for loop()
            valid = true;
            if (!cfg.HasScanPin()) {
              dataReady = true;                 // loop() has to analyze last transmissions
            }
          }
          cnt = 0;                              // reset count and code
          code = 0;
          lastSync = time_us;
        }
      }
    }
  }
  if (!cfg.HasScanPin() && cfg.HasLedPin()) {
    digitalWrite(cfg.Tx.LedPin, !valid);         // LED signals stable reception with long flashes for > 6x120 ms
  }
}

void TXDecoder::Setup()
{
  ESP_LOGD(TAG, "Setup Type=%d Recv=%d Scan=%d Led=%d\r\n", cfg.Tx.Type, cfg.Tx.RecvPin, cfg.Tx.ScanPin, cfg.Tx.LedPin);
  if (cfg.HasLedPin()) {
    pinMode(cfg.Tx.LedPin, OUTPUT);
    digitalWrite(cfg.Tx.LedPin, !scanning);      // LED signals stable reception with long flashes for > 6x120 ms
  }

  if (cfg.Tx.Type == TX_GTECH) {
    if (cfg.HasRecvPin()) {
      pinMode(cfg.Tx.RecvPin, INPUT);
      attachInterrupt(digitalPinToInterrupt(cfg.Tx.RecvPin), GTechRecvSignal, CHANGE);
    }
  }
  else if (cfg.Tx.Type == TX_DIGOO) {
    if (cfg.HasScanPin()) {
      pinMode(cfg.Tx.ScanPin, INPUT);
      attachInterrupt(digitalPinToInterrupt(cfg.Tx.ScanPin), DigooScanSignal, CHANGE);
    }
    if (cfg.HasRecvPin()) {
      pinMode(cfg.Tx.RecvPin, INPUT);
      attachInterrupt(digitalPinToInterrupt(cfg.Tx.RecvPin), DigooRecvSignal, CHANGE);
    }
  }
}

class Histogram
{
public:
  struct histdata {
    uint64_t value;
    int count;
  } data = { 0, 0 };
  int size = 0;

  void addValue(uint64_t value) {
    size++;
    for (auto &record : histogram) {
      if (record.value == value) {
        record.count++;
        return;
      }
    }
    struct histdata record = { value, 1 };
    histogram.push_back(record);
  };

  struct histdata getMaxCount() {
    data.count = 0;
    data.value = 0ul;
    for (auto &record : histogram) {
      if (record.count > data.count) {
        data = record;
      }
    }
    return data;
  };

  struct histdata getMaxCountExcludeValue(uint64_t value) {
    data.count = 0;
    data.value = 0ul;
    for (auto &record : histogram) {
      if (record.value != value && record.count > data.count) {
        data = record;
      }
    }
    return data;
  };

private:
  std::vector<struct histdata> histogram;
};

void TXDecoder::Loop()
{
  static byte LastBeg = MAX_CODES;
  bool decode = dataReady;
  byte beg = CodeBeg;
  byte end = CodeEnd;
  dataReady = false;

  if (!cfg.HasScanPin()) {  // emulation of Digoo scan signal on std ESP to be able ta analyze a transfer burst
    static uint64_t lastDataReady = 0;
    static bool recvData = false;
    uint64_t time_us = micros();
    if (decode) {
      if (LastBeg != CodeBeg) {
        LastBeg = beg;
        recvData = true;
      }
      lastDataReady = time_us;
      decode = false; // wait for transfer burst to be finished
    }
    else {
      decode = (time_us - lastDataReady > 500000ul) && recvData; // wait for 0.5 seconds analyzing the data
      if (decode) {
        recvData = false;
        beg = LastBeg;
        LastBeg = MAX_CODES;
      }
    }
  }

  if (decode)
  {
    //time_t t = now();
    //ESP_LOGD(TAG, "===== %02d:%02d:%02d =====\r\n", hour(t), minute(t), second(t));
    Histogram hist;
    do {
#ifdef TXD_DEBUG_VERBOSE
      ESP_LOGD(TAG, "Received: 0x%s %2d %2d\r\n", String((unsigned long)Codes[beg], 16).c_str(), beg, end);
#endif
      hist.addValue(Codes[beg]);
      beg = CodesIndexIncrement(beg);
    } while (beg != end);

    hist.getMaxCount();
    ESP_LOGD(TAG, "MaxOccur: 0x%s %2d %2d ", String((unsigned long)hist.data.value, 16).c_str(), hist.size, hist.data.count);
    if (hist.data.count >= 2) {
      struct rec data;
      if (DecodeRecord(hist.data.value, data)) {
        StoreRecord(data);
        ESP_LOGD(TAG, "valid ");
        //ESP_LOGD(TAG, Record2Debug(data));
      }
      else {
        ESP_LOGD(TAG, "error");
      }
    }
    else {
      ESP_LOGD(TAG, "ignored");
    }
    ESP_LOGD(TAG, "");;

    // check for interleaved records
    if (hist.size - hist.data.count >= 2) {
      hist.getMaxCountExcludeValue(hist.data.value);
      ESP_LOGD(TAG, "TxSecond: 0x%s %2d %2d ", String((unsigned long)hist.data.value, 16).c_str(), hist.size, hist.data.count);
      if (hist.data.count >= 2) {
        struct rec data;
        if (DecodeRecord(hist.data.value, data)) {
          StoreRecord(data);
          ESP_LOGD(TAG, "valid ");
          //ESP_LOGD(TAG, Record2Debug(data));
        }
        else {
          ESP_LOGD(TAG, "error");
        }
      }
      else {
        ESP_LOGD(TAG, "ignored");
      }
      ESP_LOGD(TAG, "");
    }
  }
}

bool TXDecoder::GetRecord(struct rec &record)
{
  bool ret = false;
  if (mLastStoredRec >= 0) {
    int last_read_rec = mLastReadRec + 1;
    if (last_read_rec >= MAX_RECORDS) last_read_rec = 0;
    if (last_read_rec != mLastStoredRec) {
      record = mRecords[last_read_rec];
      mLastReadRec = last_read_rec;
      ret = true;
    }
  }
  return ret;
}

char *TXDecoder::Record2String(struct rec &record)
{
  if (record.channel == 3) {
    sprintf(mBuffer, "Sensor %d %fmm (%d%s%s)",
      record.channel + 1,
      RAINFACTOR * record.humidity,
      record.txid,
      record.battery ? ", LOW BAT" : "",
      record.button ? ", TX BUT" : ""
    );
  }
  else {
    sprintf(mBuffer, "Sensor %d %0.1f %d (%d%s%s)",
      record.channel + 1,
      (float) record.temperature / 10.0,
      record.humidity,
      record.txid,
      record.battery ? ", LOW BAT" : "",
      record.button ? ", TX BUT" : ""
    );
  }
  return mBuffer;
}

char *TXDecoder::Record2Debug(struct rec &record)
{
  if (record.channel == 3) {
    sprintf(mBuffer, "Sensor %d %d (%d%s%s)",
      record.channel + 1,
      record.humidity,
      record.txid,
      record.battery ? ", LOW BAT" : "",
      record.button ? ", TX BUT" : ""
    );
  }
  else {
    sprintf(mBuffer, "Sensor %d %0.1f°C %d%% (%d%s%s)", //"Sensor %d : %0.1f&deg;C %d%% (%02x%s%s)"
      record.channel + 1,
      (float) record.temperature / 10.0,
      record.humidity,
      record.txid,
      record.battery ? ", LOW BAT" : "",
      record.button ? ", TX BUT" : ""
    );
  }
  return mBuffer;
}

void TXDecoder::StoreRecord(struct rec &record)
{
  if (record.valid) {
    if (mLastStoredRec < 0) mLastStoredRec++;
    int last_stored_rec = mLastStoredRec + 1;
    if (last_stored_rec >= MAX_RECORDS) last_stored_rec = 0;
    if (last_stored_rec == mLastReadRec) mLastReadRec++;
    if (mLastReadRec >= MAX_RECORDS) mLastReadRec = 0;
    mRecords[mLastStoredRec] = record;
    mLastStoredRec = last_stored_rec;
  }
}

bool TXDecoder::DecodeRecord(uint64_t value, struct rec &record)
{
  if (cfg.Tx.Type == TX_GTECH) {
    return GTechDecodeRecord(value, record);
  }
  else if (cfg.Tx.Type == TX_DIGOO) {
    return DigooDecodeRecord(value, record);
  }
  return false;
}

bool TXDecoder::GTechDecodeRecord(uint64_t value, struct rec &record)
{
  int check_calc = 0;
  int check_recv = value & 0x3Full;
  unsigned long tmp = (value>>5) & ~1l;         // clear LSB

  // calulate check sum
  for (int i = 0; i < 8; i++)
  {
    check_calc += (tmp & 0xFl);
    tmp >>= 4;
  }
  check_calc &= 0x3F;

  record.valid = check_recv == check_calc;

  if (record.valid)
  {
    tmp = value>>6;                            // skip checksum
    record.humidity = tmp & 0x7F;              // 7 bit humidity
    tmp >>= 7;
    int16_t val = (tmp&0xFFF)<<4;              // temperature is given in 12 bit 2th complement
    record.temperature = val / 16;             // preserve sign bit by shifting to MSB and dividing by shift factor
    tmp >>= 12;
    record.channel = tmp & 0x3;                // 2 bit channel
    tmp >>= 2;
    record.button = tmp & 0x1;                 // 1 bit button
    tmp >>= 1;
    record.battery = tmp & 0x1;                // 1 bit battery status
    tmp >>= 1;
    record.txid = tmp & 0xFF;                  // 8 bit transmitter ID

    record.timestamp = 0ul; //ntp.UtcTime();

    // validate channel (exclude false positives)
    record.valid = record.channel >= 0 && record.channel <= 2;
  }
  return record.valid;
}

bool TXDecoder::DigooDecodeRecord(uint64_t value, struct rec &record)
{
  uint64_t tmp = value;

  tmp = value;
  record.humidity = tmp & 0xFFul;            // 8 bit humidity
  tmp >>= 8;
  unsigned int type = tmp & 0xFuL;
  tmp >>= 4;
  int16_t val = (tmp & 0xFFFul)<<4;          // temperature is given in 12 bit 2th complement
  record.temperature = val / 16;             // preserve sign bit by shifting to MSB and dividing by shift factor
  tmp >>= 12;
  record.channel = (tmp & 0x3ul);            // 00, 01, 10 channel switch
  tmp >>= 2;
  record.button = tmp & 0x1ul;               // 1 bit button
  tmp >>= 1;
  record.battery = !(tmp & 0x1ul);           // 1 bit battery status invertiert?
  tmp >>= 1;
  record.txid = tmp & 0xFFul;                // 8 bit transmitter ID

  if (record.channel == 3) {                 // rain gauge
    record.humidity = value & 0xFFFFFul;
    record.temperature = 0;
    type = 0;
  }

  record.timestamp = 0UL; //ntp.UtcTime();

  record.valid = type == 0xF || (record.channel == 3);

  return record.valid;
}

}  // namespace rf_scanner
}  // namespace esphome
