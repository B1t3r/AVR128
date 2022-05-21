/*
 * LCD.c
 *
 * Created: 2019. 10. 14. 21:37:16
 *  Author: Ghostman
 */ 
#ifndef F_CPU
#define F_CPU (8000000UL)
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

void LCD_init()
{
	_delay_ms(15);
	//LCD port inicializálás rész
	LCD_DATA_DDR |= 0xF0;									//data7...4 out
	LCD_CMD_DDR |= (1<<LCD_RS) | (1<<LCD_RW) | (1<<LCD_EN); //RS, R/W, EN out
	
	//bekapcs után várni kell
	//_delay_ms(20);	//várj több mint 40-et ha 2.7V a táp (4.5V esetán több mint 15ms)
	
	//parancs írás
	LCD_CMD_PORT &= (0<<LCD_RW);	//R/W<-0 ->write
	LCD_CMD_PORT &= (0<<LCD_RS);	//RS <-0 ->parancs
	
	// 0  0  1  1
	//D7 D6 D5 D4
	LCD_DATA_PORT |= 0x30;			//8bites mód
	LCD_enable();
	
	//min 4.1ms
	_delay_ms(5);
	
	// 0  0  1  1
	//D7 D6 D5 D4
	LCD_DATA_PORT |= 0x30;
	LCD_enable();
	
	//min 100ms
	_delay_ms(101);
	
	// 0  0  1  1
	//D7 D6 D5 D4
	LCD_DATA_PORT |= 0x30;	//delay
	LCD_enable();			//__| |__
	
	// 0  0  1  1
	//D7 D6 D5 D4
	LCD_DATA_PORT |= 0x20;
	LCD_enable();
	
	//4bit 2 sor 8x5pixel 0x28 beállítása
	//üzemmód választás, mert nem tudjuk, hogy bekapcsolás
	//után éppen milyenben van, így a biztos
	LCD_command(0x20);
	LCD_command(0x28);
	
	//LCD home és LCD clear parancsok
	LCD_command(0x02);		//lcd alaphelyzetbe
	LCD_command(0x01);		//ldc torlese
	
	//kurzor beállítása
	LCD_command(0x0E);	//nincs villogtatás
}

void LCD_busy()
{										//BF olvasása
	uint8_t busy;
	LCD_DATA_DDR &= ~(1<<PE7);			//ott olvassuk majd a BF-et (D7-PE7)
	LCD_CMD_PORT &= ~(1<<LCD_RS);		//Sátuszinfó
	LCD_CMD_PORT |= (1<<LCD_RW);		//olvasás
	
	do
	{
		busy = 0;
		LCD_CMD_PORT |= (1<<LCD_EN);	//EN<-1
		_delay_us(1);					//felfutó
		
		busy = (LCD_DATA_PIN & (1<<PE7));	//átadjuk a BF értékét
		LCD_CMD_PORT &= ~(1<<LCD_EN);	//EN<-0
		_delay_us(1);
		LCD_CMD_PORT |= (1<<LCD_EN);	//EN<-1
		_delay_us(1);
		LCD_CMD_PORT &= ~(1<<LCD_EN);	//EN<-0
		_delay_us(1);

	}
	while(busy);

	LCD_CMD_PORT &= ~(1<<LCD_RW);		//R/W<-0 write
	LCD_DATA_DDR |= (1<<PE7);			//PE7<-1
}

void LCD_enable()
{
	//LCD_EN lábon _| |_ impulzus
	LCD_CMD_PORT |= (1<<LCD_EN);
	_delay_us(2);
	LCD_CMD_PORT &= ~(1<<LCD_EN);
	_delay_us(2);
}

void LCD_command(uint8_t cmd)
{
	LCD_busy();		//Megvárjuk míg felszabadul
	//parancs küldés esetén: R/W <- 0	RS <- 0		EN <- 0
	LCD_CMD_PORT &= (0<<LCD_RS);	//Parancs
	LCD_CMD_PORT &= (0<<LCD_RW);	//Küldés
	LCD_CMD_PORT &= (0<<LCD_EN);	//EN<-0
	
	//1 parancs az 8 bit de 4 bites módban 2*4. Elõször a felsõ 4- utána az alsó 4 bit. Közte és a végén kell az EN _| |_
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |= (cmd & 0xF0);		//fels? 4 bit küldése
	LCD_enable();						//__| |__
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |= ((cmd<<4)&0xF0);	//alsó 4 bit küldése
	LCD_enable();						//__| |__
}

void LCD_data(uint8_t data)
{
	LCD_busy();			//Megvárjuk még felszabadul
	//parancs küldés esetén: R/W <- 0	RS <- 0		EN <- 0
	LCD_CMD_PORT |= (1<<LCD_RS);	//Adatregiszter
	LCD_CMD_PORT &= ~(1<<LCD_RW);	//írás
	LCD_CMD_PORT &= ~(1<<LCD_EN);	//EN<-0
	
	//1 parancs az 8 bit de 4 bites módban 2*4. Elõször a felsõ 4- utána az alsó 4 bit. Közte és a végén kell az EN _| |_
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |= (data & 0xF0);		//felsõ 4 bit kitétele
	LCD_enable();						//__| |__
	LCD_DATA_PORT &= (0x0F);
	LCD_DATA_PORT |= ((data<<4)&0xF0);	//alsó 4 bit kitétele
	LCD_enable();						//__| |__
}

customChar[8]={

	0b00010,
	0b00100,
	0b01000,
	0b10000,
	0b10000,
	0b01000,
	0b00100,
	0b00010
};

customChar2[8]={
	0b00010,
	0b00100,
	0b01000,
	0b10000,
	0b10000,
	0b01000,
	0b00100,
	0b00010
};

void LCD_creatctg()
{
	uint8_t i=0;
	LCD_command (0x40);
	while(i<8) LCD_data(customChar[i++]);
	LCD_command(0x80);
	i=0;
	LCD_command (0x48);
	while(i<8) LCD_data(customChar2[i++]);
	LCD_command(0x80);
}