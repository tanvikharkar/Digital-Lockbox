#ifndef KEYPAD_H_
#define KEYPAD_H_

#define COLUMNS (BIT0 | BIT1 | BIT2 | BIT3) // Set column bits
#define ROWS (BIT4 | BIT5 | BIT6 | BIT7) // Set row bits
#define RGB_LED 0x07 // Determines LED color of a specific button press
#define RED 0x01
#define GREEN 0x02

// Define Functions
void keypad_init(void);
const char* getkey(void);
void keypad_main(void);

#endif /* KEYPAD_H_ */
