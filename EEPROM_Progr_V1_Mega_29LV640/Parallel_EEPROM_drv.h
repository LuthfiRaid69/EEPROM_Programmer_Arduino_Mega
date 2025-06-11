#ifdef Mode_Fast_IO

void EEPROM_set_address(uint16_t addr);
uint16_t EEPROM_get_data();
void EEPROM_set_data(uint16_t dat);
void EEPROM_set_data_input();
void EEPROM_set_data_output();
void EEPROM_reset_cmd();
uint16_t EEPROM_read_cmd();
void EEPROM_write_cmd();

void EEPROM_set_address(uint16_t addr) {
  digitalWrite(PIN_A0, addr & 1);
  digitalWrite(PIN_A1, (addr >> 1) & 1);
  digitalWrite(PIN_A2, (addr >> 2) & 1);
  digitalWrite(PIN_A3, (addr >> 3) & 1);
  digitalWrite(PIN_A4, (addr >> 4) & 1);
  digitalWrite(PIN_A5, (addr >> 5) & 1);
  digitalWrite(PIN_A6, (addr >> 6) & 1);
  digitalWrite(PIN_A7, (addr >> 7) & 1);
  digitalWrite(PIN_A8, (addr >> 8) & 1);
  digitalWrite(PIN_A9, (addr >> 9) & 1);
  digitalWrite(PIN_A10, (addr >> 10) & 1);
  digitalWrite(PIN_A11, (addr >> 11) & 1);
  digitalWrite(PIN_A12, (addr >> 12) & 1);
  digitalWrite(PIN_A13, (addr >> 13) & 1);
  digitalWrite(PIN_A14, (addr >> 14) & 1);
  digitalWrite(PIN_A15, (addr >> 15) & 1);
  digitalWrite(PIN_A16, (addr >> 16) & 1);
  digitalWrite(PIN_A17, (addr >> 17) & 1);
  digitalWrite(PIN_A18, (addr >> 18) & 1);
  digitalWrite(PIN_A19, (addr >> 19) & 1);
  digitalWrite(PIN_A20, (addr >> 20) & 1);
  digitalWrite(PIN_A21, (addr >> 21) & 1);
}

uint16_t EEPROM_get_data() {
  uint16_t val = 0;

  val |= digitalRead(PIN_Q0);
  val |= digitalRead(PIN_Q1) << 1;
  val |= digitalRead(PIN_Q2) << 2;
  val |= digitalRead(PIN_Q3) << 3;
  val |= digitalRead(PIN_Q4) << 4;
  val |= digitalRead(PIN_Q5) << 5;
  val |= digitalRead(PIN_Q6) << 6;
  val |= digitalRead(PIN_Q7) << 7;
  val |= digitalRead(PIN_Q8) << 8;
  val |= digitalRead(PIN_Q9) << 9;
  val |= digitalRead(PIN_Q10) << 10;
  val |= digitalRead(PIN_Q11) << 11;
  val |= digitalRead(PIN_Q12) << 12;
  val |= digitalRead(PIN_Q13) << 13;
  val |= digitalRead(PIN_Q14) << 14;
  val |= digitalRead(PIN_Q15_A_1) << 15;

  return val;
}

void EEPROM_set_data(uint16_t dat) {
  digitalWrite(PIN_Q0, dat & 1);
  digitalWrite(PIN_Q1, (dat >> 1) & 1);
  digitalWrite(PIN_Q2, (dat >> 2) & 1);
  digitalWrite(PIN_Q3, (dat >> 3) & 1);
  digitalWrite(PIN_Q4, (dat >> 4) & 1);
  digitalWrite(PIN_Q5, (dat >> 5) & 1);
  digitalWrite(PIN_Q6, (dat >> 6) & 1);
  digitalWrite(PIN_Q7, (dat >> 7) & 1);
  digitalWrite(PIN_Q8, (dat >> 8) & 1);
  digitalWrite(PIN_Q9, (dat >> 9) & 1);
  digitalWrite(PIN_Q10, (dat >> 10) & 1);
  digitalWrite(PIN_Q11, (dat >> 11) & 1);
  digitalWrite(PIN_Q12, (dat >> 12) & 1);
  digitalWrite(PIN_Q13, (dat >> 13) & 1);
  digitalWrite(PIN_Q14, (dat >> 14) & 1);
  digitalWrite(PIN_Q15_A_1, (dat >> 15) & 1);
}

void EEPROM_set_data_input() {
  pinMode(PIN_Q0, INPUT);
  pinMode(PIN_Q1, INPUT);
  pinMode(PIN_Q2, INPUT);
  pinMode(PIN_Q3, INPUT);
  pinMode(PIN_Q4, INPUT);
  pinMode(PIN_Q5, INPUT);
  pinMode(PIN_Q6, INPUT);
  pinMode(PIN_Q7, INPUT);
  pinMode(PIN_Q8, INPUT);
  pinMode(PIN_Q9, INPUT);
  pinMode(PIN_Q10, INPUT);
  pinMode(PIN_Q11, INPUT);
  pinMode(PIN_Q12, INPUT);
  pinMode(PIN_Q13, INPUT);
  pinMode(PIN_Q14, INPUT);
  pinMode(PIN_Q15_A_1, INPUT);
}

void EEPROM_set_data_output() {
  pinMode(PIN_Q0, OUTPUT);
  pinMode(PIN_Q1, OUTPUT);
  pinMode(PIN_Q2, OUTPUT);
  pinMode(PIN_Q3, OUTPUT);
  pinMode(PIN_Q4, OUTPUT);
  pinMode(PIN_Q5, OUTPUT);
  pinMode(PIN_Q6, OUTPUT);
  pinMode(PIN_Q7, OUTPUT);
  pinMode(PIN_Q8, OUTPUT);
  pinMode(PIN_Q9, OUTPUT);
  pinMode(PIN_Q10, OUTPUT);
  pinMode(PIN_Q11, OUTPUT);
  pinMode(PIN_Q12, OUTPUT);
  pinMode(PIN_Q13, OUTPUT);
  pinMode(PIN_Q14, OUTPUT);
  pinMode(PIN_Q15_A_1, OUTPUT);
}

void EEPROM_reset_cmd() {
  digitalWrite(PIN_RESET, LOW);

  delayMicroseconds(10);

  digitalWrite(PIN_RESET, HIGH);

  delayMicroseconds(20);
}

uint16_t EEPROM_read_cmd() {
  uint16_t dat = 0;
  // Read array action is to read the data stored in the array. While the memory device is in powered up or has been reset,
  // it will automatically enter the status of read array. If the microprocessor wants to read the data stored in the array, it has
  // to drive CE# (device enable control pin) and OE# (Output control pin) as Vil, and input the address of the data to be
  // read into address pin at the same time. After a period of read cycle (Tce or Taa), the data being read out will be
  // displayed on output pin for microprocessor to access. If CE# or OE# is Vih, the output will be in tri-state, and there will
  // be no data displayed on output pin at all.

  digitalWrite(PIN_WE, HIGH);
  digitalWrite(PIN_CE, LOW);
  digitalWrite(PIN_OE, LOW);

  //delayMicroseconds(1);
  dat = EEPROM_get_data();

  digitalWrite(PIN_OE, HIGH);
  digitalWrite(PIN_CE, HIGH);

  return dat;

}

void EEPROM_write_cmd() {
  // To write a command to the device, system must drive WE# and CE# to Vil, and OE# to Vih. In a command cycle, all
  // address are latched at the later falling edge of CE# and WE#, and all data are latched at the earlier rising edge of CE#
  // and WE#.
  digitalWrite(PIN_OE, HIGH);
  digitalWrite(PIN_CE, LOW);
  digitalWrite(PIN_WE, LOW);

  delayMicroseconds(1);

  digitalWrite(PIN_WE, HIGH);
  digitalWrite(PIN_CE, HIGH);

  delayMicroseconds(1);
}

#endif
