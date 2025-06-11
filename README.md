# EEPROM_Programmer_Arduino_Mega
EEPROM Programmer for : 29LV160, 29LV320, 29LV640, Etc. internal memory SP404 OG , Juno D and any Pedal FX &amp; Synthesizer.
##
###
### SD Card Module Connecting

| No | SD Card Pin | Arduino Pin |
|----|-------------|-------------|
|  1 | GND         | GND         |
|  2 | VCC         | 5V          |
|  3 | MOSI        | 51          |
|  4 | MISO        | 50          |
|  5 | SCK         | 52          |
|  6 | CS          | 4           |

...
...
####
### EEPROM Pin Connecting 1 - 24

| No | EEPROM Pin | Arduino Pin |
|----|------------|-------------|
|  1 | A15        | 14          |
|  2 | A14        | 28          |
|  3 | A13        | 15          |
|  4 | A12        | 30          |
|  5 | A11        | 16          |
|  6 | A10        | 32          |
|  7 | A9         | 17          |
|  8 | A8         | 34          |
|  9 | A19        | 18          |
| 10 | A20        | 36          |
| 11 | WE         | 19          |
| 12 | RESET      | 38          |
| 13 | A21        | 67          |
| 14 | WP_ACC     | 40          |
| 15 | RY_BY      | 66          |
| 16 | A18        | 42          |
| 17 | A17        | 65          |
| 18 | A7         | 44          |
| 19 | A6         | 64          |
| 20 | A5         | 46          |
| 21 | A4         | 63          |
| 22 | A3         | 48          |
| 23 | A2         | 62          |
| 24 | A1         | 8           |

#### note:
|  29LV160                          |
|----------|----------|-------------|
| A20 : NC | A21 : NC | WP_ACC : NC |
###
29LV320 ( A21 : NC )
###
...
...
####
### EEPROM Pin Connecting 25 - 48

| No | EEPROM Pin | Arduino Pin |
|----|------------|-------------|
| 25 | A0         | 11          |
| 26 | CE         | 9           |
| 27 | GND/VSS    | GND         |
| 28 | OE         | 49          |
| 29 | Q0         | 10          |
| 30 | Q8         | 47          |
| 31 | Q1         | 68          |
| 32 | Q9         | 45          |
| 33 | Q2         | 69          |
| 34 | Q10        | 43          |
| 35 | Q3         | 31          |
| 36 | Q11        | 41          |
| 37 | VCC        | 3.3V        |
| 38 | Q4         | 39          |
| 39 | Q12        | 27          |
| 40 | Q5         | 37          |
| 41 | Q13        | 26          |
| 42 | Q6         | 35          |
| 43 | Q14        | 25          |
| 44 | Q7         | 33          |
| 45 | Q15_A_1    | 24          |
| 46 | GND/VSS    | GND         |
| 47 | BYTE       | 23          |
| 48 | A16        | 29          |
