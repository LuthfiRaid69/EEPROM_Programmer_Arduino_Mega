#define Mode_Fast_IO 0

#include "Parallel_EEPROM_pins.h"
#include "Parallel_EEPROM_drv.h"



uint32_t eeprom_millis = 0;
uint32_t eeprom_millis1 = 0;
uint32_t eeprom_byte_cnt = 0;

uint32_t EEPROM_size = 2097152; // 8MB: 8388608, 1MB: 1048576, 2MB: 2097152, 4MB: 4194304
#define data_buff_size 512 //1024 //2048 //4096 //8192 //16384 //32768

bool print_Progress(uint32_t value, uint32_t max_value);
void EEPROM_init();
uint16_t EEPROM_read(uint32_t addr);
void EEPROM_write(uint32_t addr, uint16_t dat);
void EEPROM_program(uint32_t addr, uint16_t dat);
void EEPROM_update(uint32_t addr, uint16_t dat);
uint16_t EEPROM_get_Silicon_ID();
uint16_t EEPROM_get_Device_ID();
void EEPROM_blank();
void EEPROM_erase();
void EEPROM_dump();
void EEPROM_fill_pattern(byte data);
void EEPROM_fill_pattern_inc();

void EEPROM_init() {
  digitalWrite(PIN_WE, HIGH);
  digitalWrite(PIN_CE, LOW);
  digitalWrite(PIN_OE, HIGH);
  digitalWrite(PIN_RESET, HIGH);
  digitalWrite(PIN_WP_ACC, HIGH);
  digitalWrite(PIN_BYTE, HIGH); // Word mode: 16 bits (HIGH)

  pinMode(PIN_CE, OUTPUT);
  pinMode(PIN_WE, OUTPUT);
  pinMode(PIN_OE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_WP_ACC, OUTPUT);
  pinMode(PIN_RY_BY, INPUT_PULLUP);
  pinMode(PIN_BYTE, OUTPUT);

  pinMode(PIN_A0, OUTPUT);
  pinMode(PIN_A1, OUTPUT);
  pinMode(PIN_A2, OUTPUT);
  pinMode(PIN_A3, OUTPUT);
  pinMode(PIN_A4, OUTPUT);
  pinMode(PIN_A5, OUTPUT);
  pinMode(PIN_A6, OUTPUT);
  pinMode(PIN_A7, OUTPUT);
  pinMode(PIN_A8, OUTPUT);
  pinMode(PIN_A9, OUTPUT);
  pinMode(PIN_A10, OUTPUT);
  pinMode(PIN_A11, OUTPUT);
  pinMode(PIN_A12, OUTPUT);
  pinMode(PIN_A13, OUTPUT);
  pinMode(PIN_A14, OUTPUT);
  pinMode(PIN_A15, OUTPUT);
  pinMode(PIN_A16, OUTPUT);
  pinMode(PIN_A17, OUTPUT);
  pinMode(PIN_A18, OUTPUT);
  pinMode(PIN_A19, OUTPUT);
 // pinMode(PIN_A20, OUTPUT);
 // pinMode(PIN_A21, OUTPUT);

  EEPROM_set_data_output();
}

uint16_t EEPROM_read(uint32_t addr) {
  EEPROM_set_address(addr);
  return EEPROM_read_cmd();
}

void EEPROM_write(uint32_t addr, uint16_t dat) {
  EEPROM_set_address(addr);
  EEPROM_set_data(dat);
  EEPROM_write_cmd();
}

void EEPROM_program(uint32_t addr, uint16_t dat) {
  EEPROM_write(0x555, 0xAA);
  EEPROM_write(0x2AA, 0x55);
  EEPROM_write(0x555, 0xA0);
  EEPROM_write(addr, dat);
}

void EEPROM_update(uint32_t addr, uint16_t dat) {
  EEPROM_set_data_input();

  if (dat != EEPROM_read(addr)) {
    EEPROM_set_data_output();
    EEPROM_write(addr, dat);
  }
}

uint16_t EEPROM_get_Silicon_ID() {
  // Write Data AAH Address 555H
  // Write Data 55H Address 2AAH
  // Write Data 90H Address 555H
  // Read  Data xxH Address 000H

  EEPROM_set_data_output();

  EEPROM_write(0x555, 0xAA);
  EEPROM_write(0x2AA, 0x55);
  EEPROM_write(0x555, 0x90);

  EEPROM_set_data_input();
  return EEPROM_read(0x00);
}

uint16_t EEPROM_get_Device_ID() {
  // Write Data AAH Address 555H
  // Write Data 55H Address 2AAH
  // Write Data 90H Address 555H
  // Read  Data xxH Address 001H

  EEPROM_set_data_output();

  EEPROM_write(0x555, 0xAA);
  EEPROM_write(0x2AA, 0x55);
  EEPROM_write(0x555, 0x90);

  EEPROM_set_data_input();
  return EEPROM_read(0x01);
}

void EEPROM_blank() {
  uint32_t addr = 0;
  uint16_t dat = 0;
  uint32_t EEPROM_size8 = EEPROM_size; // 8388608 bytes to word; 16 bits
  uint32_t EEPROM_size16 = EEPROM_size8 / 2; // 8388608 bytes to word; 16 bits
  boolean error = false;

  EEPROM_reset_cmd();

  delay(100);

  Serial.println(F("EEPROM_blank begin"));

  eeprom_byte_cnt = 0;

  EEPROM_set_data_input();

  while ((addr < EEPROM_size16) && (error == false)) {
    for (uint16_t i = 0; i < 256; i += 2) {
      dat = EEPROM_read(addr);
      addr++;
      eeprom_byte_cnt += 2;

      if ((0xFF != (dat & 0xFF)) || (0xFF != ((dat >> 8) & 0xFF))) {
        Serial.print(F("Error (data is different) Address: 0x"));
        Serial.print(addr, HEX);
        Serial.print(F(" Value: 0x"));
        Serial.print(0xFFFF, HEX);
        Serial.print(F(": 0x"));
        Serial.println(dat, HEX);
        error = true;
        break;
      }
    }

    if ((eeprom_millis + 1000) < millis()) {
      eeprom_millis = millis();

      digitalWrite(13, !digitalRead(13));

      Serial.print(F("Checking if EEPROM is clean"));
      if (print_Progress(eeprom_byte_cnt, EEPROM_size8)) return;
    }
  }

  Serial.println(F("EEPROM_blank finish"));
}

void EEPROM_erase() {
  // Write Data AAH Address 555H
  // Write Data 55H Address 2AAH
  // Write Data 80H Address 555H
  // Write Data AAH Address 555H
  // Write Data 55H Address 2AAH
  // Write Data 10H Address 555H
  // Data=FFFFh ? YES: Auto Chip Erase Completed

  Serial.println(F("EEPROM erase begin"));

  EEPROM_reset_cmd();

  delay(100);

  EEPROM_set_data_output();

  EEPROM_write(0x555, 0xAA);
  EEPROM_write(0x2AA, 0x55);
  EEPROM_write(0x555, 0x80);
  EEPROM_write(0x555, 0xAA);
  EEPROM_write(0x2AA, 0x55);
  EEPROM_write(0x555, 0x10);

  EEPROM_set_data_input();

  uint8_t timeout = 0;
  uint8_t read_cnt = 0;

  while (read_cnt < 3) {
    if ((eeprom_millis + 1000) < millis()) {
      eeprom_millis = millis();

      digitalWrite(13, !digitalRead(13));

      Serial.print(F("Erasing EEPROM..."));
      Serial.print(timeout, DEC);
      Serial.println(F("s"));

      if (timeout < 60) {
        timeout++;
      } else {
        Serial.println(F("Error: Exceed time limit"));
        break;
      }
    }

    if (EEPROM_get_data() == 0xFFFF) {
      read_cnt++;
    } else {
      read_cnt = 0;
    }
  }

  Serial.println(F("EEPROM erase finish"));
}

void EEPROM_dump() {
  uint32_t addr = 0;
  uint16_t dat = 0;
  //  uint16_t dat_buff16[8];
  uint32_t EEPROM_size8 = EEPROM_size; // 8388608 bytes to word; 16 bits
  uint32_t EEPROM_size16 = EEPROM_size8 / 2; // 8388608 bytes to word; 16 bits

  Serial.println(F("EEPROM_dump begin"));

  EEPROM_reset_cmd();

  delay(100);

  eeprom_byte_cnt = 0;

  EEPROM_set_data_input();

  eeprom_millis1 = millis();

  while (addr < EEPROM_size16) {

    Serial.print(millis() - eeprom_millis1);

    Serial.print(F(" "));

    Serial.print(eeprom_byte_cnt, HEX);

    Serial.print(F(" "));

    for (byte i = 0; i < 8; i++) {
      dat = EEPROM_read(addr);

      Serial.print(dat, HEX);

      Serial.print(F(" "));

      addr++;
      eeprom_byte_cnt += 2;
    }

    if (print_Progress(eeprom_byte_cnt, EEPROM_size8)) return;

    if ((eeprom_millis + 1000) < millis()) {
      eeprom_millis = millis();

      digitalWrite(P_LED, !digitalRead(P_LED));
    }
  }

  Serial.println(F("EEPROM_dump finish"));

  //EEPROM_reset_cmd();
}

void EEPROM_fill_pattern(byte data) {
  uint32_t addr = 0;
  uint16_t __dat = 0;
  uint32_t EEPROM_size8 = EEPROM_size; // 8388608 bytes to word; 16 bits
  uint32_t EEPROM_size16 = EEPROM_size8 / 2; // 8388608 bytes to word; 16 bits
  float progress_value = 0.0;

  Serial.println(F("EEPROM_fill_pattern begin"));

  EEPROM_reset_cmd();

  delay(100);

  eeprom_byte_cnt = 0;

  __dat = (data << 8) | data;

  EEPROM_set_data_output();

  while (addr < EEPROM_size16) {
    for (uint16_t i = 0; i < 256; i++) {
      EEPROM_program(addr, __dat);

      if (addr < EEPROM_size16) {
        addr++;
        eeprom_byte_cnt += 2;
      } else {
        break;
      }
    }

    if ((eeprom_millis + 1000) < millis()) {
      eeprom_millis = millis();

      digitalWrite(P_LED, !digitalRead(P_LED));

      Serial.print(F("Writing 0x"));
      Serial.print(data, HEX);
      Serial.print(F(" in EEPROM"));
      if (print_Progress(eeprom_byte_cnt, EEPROM_size8)) return;
    }
  }

  Serial.println(F("EEPROM_fill_pattern finish"));
}

void EEPROM_fill_pattern_inc() {
  uint32_t addr = 0;
  uint16_t __dat = 0;
  uint32_t EEPROM_size8 = EEPROM_size; // 8388608 bytes to word; 16 bits
  uint32_t EEPROM_size16 = EEPROM_size8 / 2; // 8388608 bytes to word; 16 bits
  float progress_value = 0.0;

  Serial.println(F("EEPROM_fill_pattern begin"));

  EEPROM_reset_cmd();

  delay(100);

  eeprom_byte_cnt = 0;

  EEPROM_set_data_output();

  while (addr < EEPROM_size16) {
    for (uint16_t i = 0; i < 256; i++) {
      __dat = (i << 8) | i;
      
      EEPROM_program(addr, __dat);

      if (addr < EEPROM_size16) {
        addr++;
        eeprom_byte_cnt += 2;
      } else {
        break;
      }
    }

    if ((eeprom_millis + 1000) < millis()) {
      eeprom_millis = millis();

      digitalWrite(P_LED, !digitalRead(P_LED));

      Serial.print(F("Writing incremental test pattern in EEPROM"));
      if (print_Progress(eeprom_byte_cnt, EEPROM_size8)) return;
    }
  }

  Serial.println(F("EEPROM_fill_pattern finish"));
}
