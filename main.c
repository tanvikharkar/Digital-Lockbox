#include "msp.h"
#include <stdio.h>
#include "LCD.h"
#include "Keypad.h"
#include "string.h"

void main(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
  
  LCD_init(); // Initialize LCD
  keypad_init(); // Initialize keypad
  
  LCD_write_string("LOCKED"); // Set initial LCD display screen
  LCD_command(LINE_TWO);
  LCD_write_string("ENTER KEY: ");
  
  P2->SEL0 &= ~RGB_LED; // Initialize RGB LED to GPIO and as an OUTPUT
  P2->SEL1 &= ~RGB_LED;
  P2->DIR |= RGB_LED;
  P2->OUT = RED; // Set RGB LED to red
  
  char* key = "-1"; // Key "not pressed" value
  char counter = 0;
  char password[5] = "0000"; // Initial default password
  char new_password[5] = "";
  
  typedef enum // Define FSM States
    {
    WAIT,
    KEY_PRESSED,
    UNLOCK_BOX
    } STATE_TYPE;
  
    STATE_TYPE state = WAIT; // Start at WAIT state
  
    while(1)
    {
      switch(state) // Switch statement to cycle through FSM cases
      {
        case WAIT:
          while (strcmp(key, "-1") == 0) // Wait until key press detected
          {
            key = getkey(); // Gets key from keypad press
            if (counter >= 4) // Checks if more than 4 digits (length of pin code) are entered
            {
              state = KEY_PRESSED; // Go to KEY_PRESSED state to check if the entered pin is correct
              break;
            }
          }
          
        case KEY_PRESSED:
          if (strcmp(password, new_password) != 0) // Check if passwords are the same
          { // If passwords are not the same, start over
            if (counter >= 4) // Check if more than 4 incorrect digits are entered
            {
              __delay_us(USER_DELAY);
              LCD_init(); // Try again, reset screen
              LCD_write_string("INCORRECT KEY");
              LCD_command(LINE_TWO);
              LCD_write_string("TRY AGAIN");
              __delay_us(USER_READ);
              LCD_init();
              LCD_write_string("LOCKED"); // User must enter new password to be checked, process starts over
              LCD_command(LINE_TWO);
              LCD_write_string("ENTER KEY: ");
              key = "-1";
              counter = 0;
              memset(new_password,0,strlen(new_password)); // Clears the user entered password string
              state = WAIT; // Wait for a key press
            }
          while (counter < 4) // Check if four or less keys are entered by the user
          {
            if (strcmp(key, "*") == 0) // Clear LCD display if "*" key is pressed
            {
              LCD_init();
              LCD_write_string("LOCKED");
              LCD_command(LINE_TWO);
              LCD_write_string("ENTER KEY: ");
              counter = 0;
              key = "-1";
              memset(new_password,0,strlen(new_password)); // Clears the user entered password string
              state = WAIT; // Wait for a key press
            }
            else if (strcmp(key, "*") != 0) // Write keypad character to LCD display if not a "*"
            {
              if (strcmp(key, "-1") != 0) // Check if any key is pressed
              {
                __delay_us(USER_DELAY); // Delay writing to LCD to give time for user input
                LCD_write_string(key); // Write keypad character to LCD screen
                counter = counter + 1; // Increment counter to check if entered pin is < 4 digits
                strcat(new_password, key); // Formation of the user-entered password
                key = "-1"; // Set key to default "not pressed" value
                state = WAIT; // Wait for a key press
              }
            }
            break;
          }
          break;
        }
          
      case UNLOCK_BOX:
        __delay_us(USER_DELAY);
        P2->OUT = GREEN; // Set LED to green when the box is unlocked
        LCD_init(); // LCD_init() is used to clear the LCD screen
        LCD_write_string("BOX UNLOCKED");
        LCD_command(LINE_TWO);
        LCD_write_string("WELCOME!");
        __delay_us(USER_READ); // Delay to give user time to read the previous screen message
        LCD_init();
        LCD_write_string("# TO SET NEW PIN"); // Press "#" to enter a new pin for the lockbox
        LCD_command(LINE_TWO);
        LCD_write_string("* TO LOCK BOX"); // Press "*" to relock the box with the same pin used to unlock it
        key = "-1"; // Set key to default "not pressed" value
        while (strcmp(key, "-1") == 0) // Wait until key press detected
        {
          key = getkey();
        }
        if (strcmp(key, "#") == 0) // User sets a new password for the lockbox
        {
          LCD_init();
          LCD_write_string("ENTER NEW PIN:");
          LCD_command(LINE_TWO);
          __delay_us(USER_DELAY);
          memset(password,0,strlen(password));
          memset(new_password,0,strlen(new_password));
          counter = 0;
          while (counter < 4)
          {
            key = getkey();
            if (strcmp(key, "*") == 0) // Clear LCD if "*" key is pressed
            {
              LCD_init();
              LCD_write_string("ENTER NEW PIN:");
              LCD_command(LINE_TWO);
              counter = 0;
              key = "-1";
              memset(password,0,strlen(password)); // Clear password string
            }
            if (strcmp(key, "-1") != 0)
            {
              __delay_us(USER_DELAY); // Delay writing to LCD to give time for user input
              LCD_write_string(key);
              strcat(password, key);
              counter = counter + 1; // Increment counter to check if entered pin is four or less digits
            }
          }
          __delay_us(USER_DELAY);
          LCD_init();
          LCD_write_string("NEW PIN SET");
          LCD_command(LINE_TWO);
          LCD_write_string("* TO LOCK BOX");
          counter = 1;
          while (counter == 1)
          {
            key = getkey();
            if (strcmp(key, "*") == 0)
            {
              counter = 0;
              P2->OUT = RED;
              state = KEY_PRESSED;
            }
          }
        }
          
        if (strcmp(key, "*") == 0)
        {
          counter = 0;
          memset(new_password,0,strlen(new_password));
          P2->OUT = RED;
          LCD_init();
          LCD_write_string("LOCKING BOX");
          __delay_us(USER_READ / 2); // Entering dots slowly for dramatic effect
          LCD_write_string(".");
          __delay_us(USER_READ / 2);
          LCD_write_string(".");
          __delay_us(USER_READ / 2);
          LCD_write_string(".");
          __delay_us(USER_READ / 2);
          state = KEY_PRESSED;
        }
        break;
          
        default:
          state = WAIT; // Default for safety in case code fails
    }
  }
}
