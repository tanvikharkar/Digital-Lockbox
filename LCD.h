// LCD Header

#ifndef LCD_H_
#define LCD_H_

#define CPU_FREQ 3000000 // Frequency of microcontroller
#define __delay_us(t_us) (__delay_cycles((((uint64_t)t_us)*CPU_FREQ) / 1000000))
#define RS_RW_ZERO 0x9F
#define RW_OUT 0xBF
#define RS_OUT 0x20
#define E_ZERO 0x7F
#define SHORT_DELAY_COMMAND 0x03
#define LCD_PARAM 0x28
#define CLEAR_DATA 0x0F
#define CLEAR_HOME 0x01
#define INC_CURSOR 0x06
#define CTRL_ZERO 0x1F
#define LINE_TWO 0xC0
#define USER_DELAY 300000
#define USER_READ 2000000

// Define Functions
void LCD_command(uint8_t);
void LCD_init(void);
void LCD_write_char(uint8_t);
void LCD_write_string(char[]);
void LCD_main(void);

#endif /* LCD_H_ */
