byte hex2byte(byte hex_in);
void printBin(uint16_t value);
bool print_Progress(uint32_t value, uint32_t max_value);
bool isHex(String data_in);
void burn_pattern();
void print_ID();
void print_Unknown();
void trim_CRLF();
void help_list();
void print_SD_info();

byte hex2byte(byte hex_in) {
  byte res = 0;

  if ((hex_in >= 97) && (hex_in <= 102)) {
    res = (hex_in - 87);
  } else if ((hex_in >= 65) && (hex_in <= 70)) {
    res = (hex_in - 55);
  } else {
    res = (hex_in - 48);
  }

  return res;
}

void printBin(uint16_t value) {
  for (int __bit = 15; __bit >= 0; __bit--) {
    Serial.print(bitRead(value, __bit));
  }

  Serial.println();
}

bool print_Progress(uint32_t value, uint32_t max_value) {
  float progress_value = 0.0;

  Serial.print(F(" - Progress: ["));
  progress_value = float(value);
  progress_value /= float(max_value);
  progress_value *= 100.0;

  uint8_t j = uint8_t((progress_value + 0.1) / 10.0);

  for (uint8_t i = 0; i < j; i++) {
    Serial.print(F("#"));
  }

  for (uint8_t i = j; i < 10; i++) {
    Serial.print(F("-"));
  }

  Serial.print(F("] "));

  Serial.print(progress_value, 1);
  Serial.print(F(" % ("));
  Serial.print(value, DEC);
  Serial.println(F(" Bytes)"));

  if (Serial.available()) {
    while (Serial.available()) {
      Serial.read();
    }

    Serial.println(F("The operation was aborted remotely"));
    return true;
  }

  return false;
}

bool isHex(String data_in) {
  if (((data_in[0] >= 48) && (data_in[0] <= 57) || (data_in[0] >= 65) && (data_in[0] <= 70) || (data_in[0] >= 97) && (data_in[0] <= 102)) &&
      ((data_in[1] >= 48) && (data_in[1] <= 57) || (data_in[1] >= 65) && (data_in[1] <= 70) || (data_in[1] >= 97) && (data_in[1] <= 102))) {
    return true;
  } else {
    return false;
  }
}

void burn_pattern() {
  if (inputString.length() == 12) {
    if ((inputString[8] == '0') && (inputString[9] == 'x') &&
        isHex(inputString.substring(10))) {
      byte i = 0;

      i = hex2byte(inputString[10]) << 4;

      i |= hex2byte(inputString[11]);

      EEPROM_fill_pattern(i);
    } else {
      Serial.println(F("Please use data in 0x00 (Hexadecimal) format, try again"));
    }
  } else {
    Serial.println(F("Please use data in 0x00 (Hexadecimal) format, try again"));
  }
}

void print_ID() {
  uint16_t Silicon_ID = EEPROM_get_Silicon_ID();
  uint16_t Device_ID = EEPROM_get_Device_ID();

  Serial.print(F("Manufacturer ID: 0x"));
  Serial.print(Silicon_ID, HEX);

  if (Silicon_ID == 0xC2) {
    Serial.print(F(" (MXIC)"));
  } else if (Silicon_ID == 0x7F) {
    Serial.print(F(" (EON)"));
  } else if (Silicon_ID == 0x01) {
    Serial.print(F(" (AMD)"));
  } else {
    Serial.print(F(" (Unknown)"));
  }

  Serial.print(F(" Device ID: 0x"));
  Serial.print(Device_ID, HEX);

  if ((Silicon_ID == 0xC2) && ((Device_ID == 0x22C9) || (Device_ID == 0x22CB))) {
    Serial.print(F(" (MX29LV640)"));
  } else if ((Silicon_ID == 0x7F) && ((Device_ID == 0x22C9) || (Device_ID == 0x22CB))) {
    Serial.print(F(" (EN29LV640)"));
  } else if ((Silicon_ID == 0x01) && (Device_ID == 0x227E)) {
    Serial.print(F(" (AM29LV640)"));
  } else {
    Serial.print(F(" (Unknown)"));
  }

  Serial.println();
}

void print_Unknown() {
  Serial.println(F("Unknown command, try again"));
  Serial.println();
  Serial.println(F("Please, type help to get valid command list"));
}

void trim_CRLF() {
  if (inputString.indexOf('\r') >= 0) {
    inputString = inputString.substring(0, inputString.indexOf('\r'));
  }
  if (inputString.indexOf('\n') >= 0) {
    inputString = inputString.substring(0, inputString.indexOf('\n'));
  }
}

void print_help_list() {
  Serial.println();
  Serial.println(F("<<< Help list - Begin >>>"));
  Serial.println();
  Serial.println(F("Command - Description:"));
  Serial.println();
  Serial.println(F("blank - Read the EEPROM memory and check if there is any data"));
  Serial.println(F("Burn [filename] - Reads the file from the SD card and writes it to the EEPROM memory"));
  Serial.println(F("compare [filename] - Reads the EEPROM memory and compares the data with the SD card file"));
  Serial.println(F("delete - Removes the file from the sd card"));
  Serial.println(F("dump - reads data from EEPROM and prints data"));
  Serial.println(F("Erase - Erase the entire contents of the EEPROM memory"));
  Serial.println(F("file compare [filename1] [filename2] - Compares data from two sd card files"));
  Serial.println(F("id - Shows Manufacturer ID and Device ID"));
  Serial.println(F("inc_pattern - fills EEPROM memory with incremental pattern data (from 0x00 to 0xFF)"));
  Serial.println(F("ls - Shows list of files from SD card"));
  Serial.println(F("pattern [data] - fills EEPROM memory with test pattern data (0x00 to 0xFF)"));
  Serial.println(F("save [filename] - Reads the EEPROM memory and saves the data to the SD card file"));
  Serial.println();
  Serial.println(F("Note: It is always necessary to perform the Erase operation before writing"));
  Serial.println();
  Serial.println(F("<<< Help list - End >>>"));
}

void print_SD_info() {
  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  double volumesize_KiB;
  double volumesize_MiB;
  double volumesize_GiB;
  Serial.print(F("\nVolume type is FAT"));
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  //volumesize *= 512;                            // SD card blocks are always 512 bytes

  volumesize_KiB = volumesize / 2;
  volumesize_MiB = volumesize_KiB / 1024.0;
  volumesize_GiB = volumesize_MiB / 1024.0;

  Serial.print(F("Volume size: "));

  if (volumesize_GiB > 0.0) {
    Serial.print(volumesize_GiB, 3);
    Serial.println(F(" GiB"));
  } else if (volumesize_MiB > 0.0) {
    Serial.print(volumesize_MiB, 3);
    Serial.println(F(" MiB"));
  } else if (volumesize_KiB > 0.0) {
    Serial.print(volumesize_KiB, 3);
    Serial.println(F(" kiB"));
  } else {
    Serial.print(volumesize);
    Serial.println(F(" Bytes"));
  }

  Serial.println(F("\nFiles found on the card (name, date and size in bytes): "));
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}
