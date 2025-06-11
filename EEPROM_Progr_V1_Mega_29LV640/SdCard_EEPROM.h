
bool SdCard_init();
void SdCard_EEPROM_compare();
void SdCard_EEPROM_compare_file(String filename);
void SdCard_EEPROM_save();
void SdCard_EEPROM_save_file(String filename);
void SdCard_EEPROM_burn();
void SdCard_EEPROM_file_burn(String filename);
void SdCard_file_delete();

uint32_t byte_cnt;
uint32_t millis1;

bool SdCard_init() {
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
 // pinMode(SS, OUTPUT);

  if (!SD.begin(SdCardChipSelect) & !card.init(SPI_HALF_SPEED, SdCardChipSelect)) {
    Serial.println("initialization failed!");
    return true;
  }

  Serial.println("initialization done.");

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return true;
  }

  return false;
}

void SdCard_EEPROM_compare() {
  if (inputString.length() <= 20) {
    if (inputString.length() > 8) {
      Serial.print("filename: '");
      Serial.print(inputString.substring(8));
      Serial.println("'");
      if (!SD.exists(inputString.substring(8))) {
        Serial.println("file not found, try again");
      } else {
        SdCard_EEPROM_compare_file(inputString.substring(8));
      }
    } else {
      Serial.println("Please type a filename, try again");
    }
  } else {
    Serial.println("Please use file name in 8.3 format, try again");
  }
}
void SdCard_EEPROM_compare_file(String filename) {
  uint32_t addr = 0;
  uint16_t dat = 0;
  uint32_t EEPROM_size8 = EEPROM_size; // 8388608 bytes to word; 16 bits
  uint32_t EEPROM_size16 = EEPROM_size8 / 2; // 8388608 bytes to word; 16 bits
  boolean error = false;

  myFile = SD.open(filename);

  Serial.println("EEPROM_compare_bin begin");

  EEPROM_reset_cmd();

  delay(100);

  byte_cnt = 0;

  EEPROM_set_data_input();

  if (myFile) {
    while (myFile.available() && (error == false)) {
      myFile.read(data_buff, data_buff_size);

      for (uint16_t i = 0; i < 256; i++) {
        dat = EEPROM_read(addr);
        if (addr < EEPROM_size16) {
          addr++;
        } else {
          break;
        }
        byte_cnt += 2;

        if ((data_buff[i * 2] != (dat & 0xFF)) || (data_buff[(i * 2) + 1] != ((dat >> 8) & 0xFF))) {
          Serial.print("Error, data is different: 0x");
          Serial.print(data_buff[i], HEX);
          Serial.print(": 0x");
          Serial.print(dat & 0xFF, HEX);
          Serial.print("; 0x");
          Serial.print(data_buff[i + 1], HEX);
          Serial.print(": 0x");
          Serial.println((dat >> 8) & 0xFF, HEX);
          error = true;
          break;
        }
      }

      if ((millis1 + 1000) < millis()) {
        millis1 = millis();

        digitalWrite(P_LED, !digitalRead(P_LED));

        Serial.print("Comparing EEPROM data");
        if (print_Progress(byte_cnt, EEPROM_size8)) return;
      }
    }
  }

  myFile.close();

  Serial.println("EEPROM_compare_bin finish");

  //reset_cmd();
}

void SdCard_EEPROM_save() {
  if (inputString.length() <= 17) {
    if (inputString.length() > 5) {
      Serial.print("Filename: '");
      Serial.print(inputString.substring(5));
      Serial.println("'");
      if (SD.exists(inputString.substring(5))) {
        Serial.println("File exists, please type a different filename, try again");
      } else {
        SdCard_EEPROM_save_file(inputString.substring(5));
      }
    } else {
      Serial.println("Please type a filename, try again");
    }
  } else {
    Serial.println("Please use file name in 8.3 format, try again");
  }
}

void SdCard_EEPROM_save_file(String filename) {
  uint32_t addr = 0;
  //  uint16_t dat = 0;
  uint16_t dat_buff16[256];
  uint32_t EEPROM_size8 = EEPROM_size; // 8388608 bytes to word; 16 bits
  uint32_t EEPROM_size16 = EEPROM_size8 / 2; // 8388608 bytes to word; 16 bits

  myFile = SD.open(filename, FILE_WRITE);

  Serial.println("EEPROM_to_bin begin");

  EEPROM_reset_cmd();

  delay(100);

  byte_cnt = 0;

  EEPROM_set_data_input();

  if (myFile) {
    while (addr < EEPROM_size16) {
      for (uint16_t i = 0; i < 256; i++) {
        dat_buff16[i] = EEPROM_read(addr);
        addr++;
        byte_cnt += 2;

        data_buff[i * 2] = dat_buff16[i] & 0xFF;
        data_buff[(i * 2) + 1] = (dat_buff16[i] >> 8) & 0xFF;
      }

      myFile.write(data_buff, data_buff_size);

      if ((millis1 + 1000) < millis()) {
        millis1 = millis();

        digitalWrite(P_LED, !digitalRead(P_LED));

        Serial.print("Saving data to the SD card");
        if (print_Progress(byte_cnt, EEPROM_size8)) return;
      }
    }
  }

  myFile.close();

  Serial.println("EEPROM_to_bin finish");

  //reset_cmd();
}

void SdCard_EEPROM_burn() {
  if (inputString.length() <= 17) {
    if (inputString.length() > 5) {
      Serial.print("Filename: '");
      Serial.print(inputString.substring(5));
      Serial.println("'");
      if (!SD.exists(inputString.substring(5))) {
        Serial.println("File not found, try again");
      } else {
        SdCard_EEPROM_file_burn(inputString.substring(5));
      }
    } else {
      Serial.println("Please type a filename, try again");
    }
  } else {
    Serial.println("Please use file name in 8.3 format, try again");
  }
}

void SdCard_EEPROM_file_burn(String filename) {
  uint32_t addr = 0;
  uint16_t dat = 0;
  uint32_t EEPROM_size8 = EEPROM_size; // 8388608 bytes to word; 16 bits
  uint32_t EEPROM_size16 = EEPROM_size8 / 2; // 8388608 bytes to word; 16 bits
  float progress_value = 0.0;

  myFile = SD.open(filename);

  Serial.println("bin_to_EEPROM begin");

  EEPROM_reset_cmd();

  delay(100);

  byte_cnt = 0;

  if (myFile) {
    while (myFile.available()) {
      myFile.read(data_buff, data_buff_size);
      for (uint16_t i = 0; i < 256; i++) {

        dat = data_buff[i * 2] | (data_buff[(i * 2) + 1] << 8);
        EEPROM_update(addr, dat);
        if (addr < EEPROM_size16) {
          addr++;
          byte_cnt += 2;
        } else {
          break;
        }
      }

      if ((millis1 + 1000) < millis()) {
        millis1 = millis();


        digitalWrite(P_LED, !digitalRead(P_LED));

        Serial.print("Writing data in EEPROM");
        if (print_Progress(byte_cnt, EEPROM_size8)) return;
      }
    }
  }

  myFile.close();

  Serial.println("bin_to_EEPROM finish");

  //reset_cmd();
}

void SdCard_file_delete() {
  if (inputString.length() <= 19) {
    if (inputString.length() > 7) {
      Serial.print("Filename: '");
      Serial.print(inputString.substring(7));
      Serial.println("'");
      if (!SD.exists(inputString.substring(7))) {
        Serial.println("File not found, try again");
      } else {
        SD.remove(inputString.substring(7));

        if (!SD.exists(inputString.substring(7))) {
          Serial.println("The file has been removed");
        } else {
          Serial.println("The file has not been removed, try again");
        }
      }
    } else {
      Serial.println("Please type a filename, try again");
    }
  } else {
    Serial.println("Please use file name in 8.3 format, try again");
  }
}
