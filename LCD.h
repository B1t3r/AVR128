/*
 * LCD.h
 *
 * Created: 2019. 10. 14. 21:37:29
 *  Author: Ghostman
 */ 

#ifndef LCD_H_
#define LCD_H_

//LCD ir�ny regiszterek
#define LCD_CMD_DDR		DDRF
#define LCD_DATA_DDR	DDRE
//LCD portok
#define LCD_CMD_PORT	PORTF
#define LCD_DATA_PORT	PORTE
//LCD data olvas�shoz
#define LCD_DATA_PIN	PINE
//LCD vez�rl� bitek
#define LCD_RS			PF1
#define LCD_RW			PF2
#define LCD_EN			PF3

//LCD f�ggv�nyek
void LCD_init();
void LCD_enable();
void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);

#endif /* LCD_H_ */