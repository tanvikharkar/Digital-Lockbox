// LCD Source Code

#include "msp.h"
#include "string.h"
#include "LCD.h"
#include "Keypad.h"

void LCD_command(uint8_t command)
{
  uint8_t high_nibble = command & ~CLEAR_DATA; // Split command into high and low nibble
  uint8_t low_nibble = command << 4;
  P3->OUT &= RS_RW_ZERO; // Set RS (P3.5) and RW (P3.6) output = 0; AND w/ 1001_1111
  P4->OUT &= CLEAR_DATA; // Clear DB7 (P4.7) - DB4 (P4.4) before setting high nibble
  P4->OUT |= high_nibble; // Set DB7 (P4.7) - DB4 (P4.4) = high nibble; OR w/ high
  P3->OUT |= ~(E_ZERO); // Set E (P3.7) output = 1; OR w/ 1000_0000
  __delay_us(1); // Delay by at least 140ns
  P3->OUT &= E_ZERO; // Set E (P3.7) output = 0; AND w/ 0111_1111
  __delay_us(2); // Delay by at least 1200ns
  P4->OUT &= CLEAR_DATA; // Clear DB7 (P4.7) - DB4 (P4.4) before setting low nibble
  P4->OUT |= low_nibble; // Set DB7 (P4.7) - DB4 (P4.4) = low nibble; OR w/ low
  P3->OUT |= ~(E_ZERO); // Set E (P3.7) output = 1; OR w/ 1000_0000
  __delay_us(1); // Delay by at least 140ns
  P3->OUT &= E_ZERO; // Set E (P3.7) output = 0; AND w/ 0111_1111
  if (command <= SHORT_DELAY_COMMAND){ // Delay by 1.53ms or 39us depending on command
    __delay_us(1530);
  }
  else{
    __delay_us(39);
  }
  return;
}

void LCD_init(void)
{
  P3->SEL0 &= CTRL_ZERO; // Set RS (P3.5) , RW (P3.6) , E (P3.7) to GPIO; AND w/ 0001_1111
  P3->SEL1 &= CTRL_ZERO;
  P4->SEL0 &= CLEAR_DATA; // Set DB7 (P4.7) - DB4 (P4.4) to GPIO; AND w/ 0000_1111
  P4->SEL1 &= CLEAR_DATA;
  P3->DIR |= ~(CTRL_ZERO); // Set RS (P3.5) , RW (P3.6) , E (P3.7) as outputs; OR w/ 1110_0000
  P4->DIR |= ~(CLEAR_DATA); // Set DB7 (P4.7) - DB4 (P4.4) as outputs
  __delay_us(40000); // Delay by at least 40ms
  P3->OUT &= RS_RW_ZERO; // Set RS (P3.5) and RW (P3.6) output = 0; AND w/ 1001_1111
  P4->OUT &= CLEAR_DATA; // Clear DB7 (P4.7) - DB4 (P4.4) before setting equal to 0x30
  P4->OUT |= 0x30; // Set DB7 (P4.7) - DB4 (P4.4) = 0x30; OR w/ 0011_0000
  P3->OUT &= ~(E_ZERO); // Set E (P3.7) output = 1; OR w/ 1000_0000
  __delay_us(1); // Delay by at least 140ns
  P3->OUT &= E_ZERO; // Set E (P3.7) output = 0; AND w/ 0111_1111
  __delay_us(39); // Delay by at least 39us
  LCD_command(LCD_PARAM); // 4-bit mode, 2 line, 5x8 font
  LCD_command(LCD_PARAM); // 4-bit mode, 2 line, 5x8 font
  LCD_command(CLEAR_DATA); // Display, cursor, and blink on
  LCD_command(CLEAR_HOME); // Clear home
  LCD_command(INC_CURSOR); // Increment cursor, no shift
  return;
}

void LCD_write_char(uint8_t letter)
{
  uint8_t high_nibble = letter & ~CLEAR_DATA; // Split letter into high and low nibble
  uint8_t low_nibble = letter << 4;
  P3->OUT &= RW_OUT; // Set RW (P3.6) output = 0; AND w/ 1011_1111
  P3->OUT |= RS_OUT; // Set RS (P3.5) output = 1; OR w/ 0010_0000
  P4->OUT &= CLEAR_DATA; // Clear DB7 (P4.7) - DB4 (P4.4) before setting high nibble
  P4->OUT |= high_nibble; // Set DB7 (P4.7) - DB4 (P4.4) = high nibble; OR w/ high
  P3->OUT |= ~(E_ZERO); // Set E (P3.7) output = 1; OR w/ 1000_0000
  __delay_us(1); // Delay by at least 140ns
  P3->OUT &= E_ZERO; // Set E (P3.7) output = 0; AND w/ 0111_1111
  __delay_us(2); // Delay by at least 1200ns
  P4->OUT &= CLEAR_DATA; // Clear DB7 (P4.7) - DB4 (P4.4) before setting low nibble
  P4->OUT |= low_nibble; // Set DB7 (P4.7) - DB4 (P4.4) = low nibble; OR w/ low
  P3->OUT |= ~(E_ZERO); // Set E (P3.7) output = 1; OR w/ 1000_0000
  __delay_us(1); // Delay by at least 140ns
  P3->OUT &= E_ZERO; // Set E (P3.7) output = 0; AND w/ 0111_1111
  __delay_us(43); // Delay by at least 43us
  return;
}

void LCD_write_string(char string[])
{
  char i = 0;
  char length = strlen(string); // Get length of string
  for (i = 0; i < length; i++) // Use a for loop and LCD_write_char to print every letter of the
  string
  {
    LCD_write_char(string[i]);
  }
}
