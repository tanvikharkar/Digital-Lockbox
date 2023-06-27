// Keypad Source Code

#include "msp.h"
#include "Keypad.h"

void keypad_init(void){
  P5->SEL0 &= ~ROWS; // Set rows and columns to GPIO
  P5->SEL1 &= ~ROWS;
  P4->SEL0 &= ~COLUMNS;
  P4->SEL1 &= ~COLUMNS;
  P4->DIR &= ~COLUMNS; // Set P4.0 - 4.3 as inputs (columns)
  P5->DIR |= ROWS; // Set P5.4 - 5.7 as outputs (rows)
  P4->REN |= COLUMNS; // Enable pull down resistor on inputs (columns)
  P4->OUT &= ~COLUMNS;
  P5->REN |= ROWS; // Set all rows = 1
  P5->OUT |= ROWS;
  return;
}

const char* getkey(void){
  char columns, columns_read, rows; // Initialize variables
  
  P5->OUT |= ROWS;
  columns = P4->IN & COLUMNS; // Read columns to detect button press
  
  if (columns == 0x00){ // No button press detected
    char *key = "-1";
    return key;
  }
  
  rows = 0;
  P5->OUT &= ~ROWS; // Set all row outputs to 0
  
  while(rows < 4){
    P5->OUT = 0x10 << rows; // Parse through each row
    __delay_cycles(25); // Delay
    columns_read = P4->IN & COLUMNS; // Reading columns
    if (columns_read != 0){ // Keypad press not detected
    break;
  }
  rows++; // Increment rows by 1 if key not found
}
  
if (rows == 0){ // Returns value of key pressed
  if (columns == 0x01){
    char *key = "1";
    return key;
  }
  if (columns == 0x02){
    char *key = "2";
    return key;
  }
  if (columns == 0x04){
    char *key = "3";
    return key;
  }
  if (columns == 0x08){
    char *key = "A";
    return key;
  }
}
  
if (rows == 1){
  if (columns == 0x01){
    char *key = "4";
    return key;
  }
  if (columns == 0x02){
    char *key = "5";
    return key;
  }
  if (columns == 0x04){
    char *key = "6";
    return key;
  }
  if (columns == 0x08){
    char *key = "B";
    return key;
  }
}
  
if (rows == 2){
  if (columns == 0x01){
    char *key = "7";
    return key;
  }
  if (columns == 0x02){
    char *key = "8";
    return key;
  }
  if (columns == 0x04){
    char *key = "9";
    return key;
  }
  if (columns == 0x08){
    char *key = "C";
    return key;
  }
}
  
if (rows == 3){
  if (columns == 0x01){
    char *key = "*";
    return key;
  }
  if (columns == 0x02){
    char *key = "0";
    return key;
  }
  if (columns == 0x04){
    char *key = "#";
    return key;
  }
  if (columns == 0x08){
    char *key = "D";
    return key;
  }
}
  
if (rows > 4){
  char *key = "-1"; // Safety if a key press was missed
  return key;
}
  
char *key = "-1"; // Safety if a key press was missed
return key;
}
