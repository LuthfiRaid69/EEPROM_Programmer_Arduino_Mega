/*
   Parallel EEPROM Programmer 29LV640 (tsop48) w/ SD card (2GB) and Arduino DUE
*/

#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

File myFile;

// change this to match your SD shield or module;
//     Arduino Ethernet shield W5100: pin 6
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int SdCardChipSelect = 4;


#define P_LED 13

#define data_buff_size 512 //1024 //2048 //4096 //8192 //16384 //32768
byte data_buff[data_buff_size];

#include "Parallel_EEPROM.h"

// set up variables using the SD utility library functions:

bool busyState = false;
bool ledPwmUp = true;
byte ledPwm = 0;

String inputString = "";         // a String to hold incoming data

#include "SdCard_EEPROM.h"
#include "Serial_Print.h"

void setup()
{
  delay(1); // EEPROM Power-Up

  pinMode(13, OUTPUT);

  EEPROM_init();

  Serial.begin(115200);

  Serial.println(F("\r\nStart"));

  if (SdCard_init()) return; // true: ERROR

  print_SD_info();

  //print_help_list();
}

void loop()
{
  if (!busyState) {
    analogWrite(13, ledPwm);
    if (ledPwmUp) {
      if (ledPwm < 128) {
        ledPwm++;
      } else {
        ledPwmUp = false;
      }
    } else {
      if (ledPwm > 16) {
        ledPwm--;
      } else {
        ledPwmUp = true;
      }
    }
  } else {
    ledPwm = 0;
    ledPwmUp = true;
  }

  delay(10);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    busyState = true;

    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    Serial.write(inChar);
    inputString += inChar;

    if (inChar == '\n') {
      trim_CRLF();

      if ((inputString.indexOf(F("blank")) == 0) && (inputString.length() == 5)) {
        EEPROM_blank();
      } else if (inputString.indexOf(F("Burn")) == 0) { // 4 + 1 + 8 + 4
        SdCard_EEPROM_burn();
      } else if (inputString.indexOf(F("compare")) == 0) { // 7 + 1 + 8 + 4
        SdCard_EEPROM_compare();
      } else if (inputString.indexOf(F("delete")) == 0) { // 6 + 1 + 8 + 4
        SdCard_file_delete();
      } else if ((inputString.indexOf(F("dump")) == 0) && (inputString.length() == 4)) {
        EEPROM_dump();
      } else if ((inputString.indexOf(F("Erase")) == 0) && (inputString.length() == 5)) {
        EEPROM_erase();
      } else if ((inputString.indexOf(F("id")) == 0) && (inputString.length() == 2)) {
        print_ID();
      } else if ((inputString.indexOf(F("inc_pattern")) == 0) && (inputString.length() == 11)) {
        EEPROM_fill_pattern_inc();
      } else if ((inputString.indexOf(F("help")) == 0) && (inputString.length() == 4)) {
        print_help_list();
      } else if ((inputString.indexOf(F("ls")) == 0) && (inputString.length() == 2)) {
        print_SD_info();
      } else if (inputString.indexOf(F("pattern")) == 0) { // patern 0x00 6 + 1 + 4 = 11
        burn_pattern();
      } else if (inputString.indexOf("save") == 0) { // 4 + 1 + 8 + 4
        SdCard_EEPROM_save();
      } else {
        print_Unknown();
      }

      inputString = "";
    }

    busyState = false;
  }
}
