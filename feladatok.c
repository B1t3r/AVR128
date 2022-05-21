////1. feladat////
/*Készítse el a következő futófényt:
7 6 5 4 3 2 1 0
x x x x x x x 0
x x x x x x 0 0
x x x x x 0 0 0
x x x x 0 0 0 0
x x x 0 0 0 0 x
x x 0 0 0 0 x x
x 0 0 0 0 x x x
0 0 0 0 x x x x
x 0 0 0 0 x x x
…
x x x x 0 0 0 0
x x x x x 0 0 0
x x x x x x 0 0
x x x x x x x 0
x x x x x x 0 0*/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>


void init();

int main(void)
{
	init();
	int led=1, s=0;

	while(1)
	{
		if (s==0)
		{
			PORTD=led;
			PORTB=(led<<4);
			_delay_ms(100);
			led=led<<1;

			if (led<=16)
			{
				led=led+1;
			}
			
			if(led==240)
			{
				s=1;
			}
		}
		
		else
		{
			PORTD=led;
			PORTB=(led<<4);
			_delay_ms(100);
			led=led>>1;

			if(led==1)
			{
				s=0;
			}
		}
	}
}

void init()
{
	DDRD=DDRB=0xF0;

}

////2. feladat////
/*Készítsen a következők szerint számvizsgálót:
- Ha a bevitt szám nem 3jegyű, akkor világítson az RGB piros LED
- Ha a bevitt szám 3jegyű és páros, akkor világítson az RGB kék LED
- Ha a bevitt szám 3jegyű és páratlan, akkor világítson az RGB zöld LED
- Ha a bevitt szám 3jegyű és >500, kapcsolja be a páros számú LED-eket
- Ha a bevitt szám 3jegyű <=500, kapcsolja be a páratlan számú LED-eket
- Ha a bevitt szám 910, akkor kapcsolja be az összes LED-et
Kijelzés Lenyomott szám
_ 0 0 0 7
_ 0 0 7 1
_ 0 7 1 5
_ 7 1 5 4
_ 1 5 4 9
_ 5 4 9 …

A bevitt számot mindig a * vagy # gomb lenyomásával vizsgálja meg!

- Kijelzés a 7szemenses kijelzőn az előzőekben szemléltetett módon valósítsa meg.*/

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void matrix();
void portinit();
void vizsgal();

volatile unsigned char ell=0, szam=0, red=0;
const unsigned char billtomb[]={69,14,13,11,22,21,19,38,37,35,70,67};
//								 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, #, *
unsigned char tol_tomb[4]={0};

int main()
{
	portinit();
	
	unsigned char sor=0b1000, olvas=0, index=0, i=0;	//sor-inic olvas-beadott bill i-tömb index
	
	while(1)
	{
		PORTC=sor;
		_delay_ms(30);
		olvas= PINC & 0x7F;
		
		for(index=0;index<12;index++)
		{
			if(olvas==billtomb[index]) 
			{
				if(index==10 || index==11) vizsgal();
				else
				{
					if(i<4)
					{
						tol_tomb[3]=tol_tomb[2];
						tol_tomb[2]=tol_tomb[1];
						tol_tomb[1]=tol_tomb[0];
						tol_tomb[0]=index;
						i++;
					}
					else i=0;
				}
			}
		}
		
		if(red==1)PORTC=0b10000000;
		
		if(sor==0x40) sor=0x08;
		else {sor=sor<<1;}
			
	}
	
	return 0;
}

ISR(TIMER0_OVF_vect)
{
	//PORTA = 0x80 | (hova<<4) | mit; // 0x80 a kezd? címe a 0-nak , a 
									//hova<<4 megadja hogy melyik digitr?l beszélünk(0,1,2,3), a mit az értéket rakja ki (0...9) 
		switch(ell)
		{
			//case 0: PORTA = 0x80 | (0<<4) | tol_tomb[0];	break;
			//case 1: PORTA = 0x80 | (1<<4) | tol_tomb[1];	break;
			//case 2: PORTA = 0x80 | (2<<4) | tol_tomb[2];	break;
			//case 3: PORTA = 0x80 | (3<<4) | tol_tomb[3];	break;
			case 0: PORTA = 0X80 + tol_tomb[0];	break;
			case 1: PORTA = 0X90 + tol_tomb[1];	break;
			case 2: PORTA = 0XA0 + tol_tomb[2];	break;
			case 3: PORTA = 0XB0 + tol_tomb[3];	break;
		}
	
	ell++;
	if(ell>3)ell=0;
}

void vizsgal()
{
	unsigned char led=0xFF;
	
	if( tol_tomb[2]==0 && tol_tomb[3]==0 )											{ red=1; PORTE=PORTB=PORTD=0; }
	else red=0;
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) &&  (tol_tomb[0]%2)==0 )					PORTE=0b00000100;
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) &&  (tol_tomb[0]%2)!=0 )					PORTE=0b00001000;
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) && (tol_tomb[2]<5 && tol_tomb[3]==0)  )	{ PORTB=0b01010101; PORTD=0b01010101; }
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) && (tol_tomb[2]>4 || tol_tomb[3]!=0)  )	{ PORTB=0b10101010; PORTD=0b10101010; }
	if( tol_tomb[0]==0 && tol_tomb[1]==1 && tol_tomb[2]==9 && tol_tomb[3]==0)		{ PORTD=led; PORTB=led<<4; }
	
}

void portinit()
{
	DDRA=0xFF;
	DDRC=0b11111000;
	DDRE=0b00001100;
	PORTA=PORTC=PORTE=0;
	
	TCCR0=(1<<CS01) | (1<<WGM00);
	TIMSK=(1<<TOIE0);
	sei();
	
	DDRB=DDRD=0b11110000;
	PORTB=PORTD=0;
}

////3.feladat////
/*Készítsen stoppert, melyet G0 lenyomására indít el és G1 lenyomására állít meg.
o A kijelzés a 7szegmenses kijelző alsó 3 digitjén történjen.
o A számláló léptetése 0,1s-onként történjen.
o A menübe belépve mindig 0-ról induljon a számlálás.
o Nullázni a G2-mal lehessen.*/

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void portinit();
void feltetel();

volatile unsigned char ell=0, null=0, i=0;
unsigned char tomb[3]={0};


int main()
{
	portinit();
	
	unsigned char gomb=0;
	
	while(1)
	{
		if(PING==0b1) gomb=1;
		if(gomb==1) feltetel();
		
		if(PING==0b10) gomb=2;
		
		if(PING==0b100) tomb[0]=tomb[1]=tomb[2]=0;
	}
}

void feltetel()
{
	
	if(tomb[0]>9)
	{
		tomb[0]=0;
		tomb[1]++;
	}
	if(tomb[1]>9)
	{
		tomb[1]=0;
		tomb[2]++;
	}
	
	tomb[0]=i;
	_delay_ms(100);
	i++;
	if(i>10) i=0;
}

ISR(TIMER0_OVF_vect)
{
	switch(ell)
	{
		case 0: PORTA = 0X80 + tomb[0];	break;
		case 1: PORTA = 0X90 + tomb[1];	break;
		case 2: PORTA = 0XA0 + tomb[2];	break;
	}
	
	ell++;
	if(ell>3)ell=0;
}

void portinit()
{
	DDRA=0xFF;
	DDRG=0;
	PORTA=PORTG=0;
	
	//TCCR0 =	  (1<<CS02) //órajelosztás [1024]
	//		| (1<<CS01)
	//		| (1<<CS00)
	//		| (1<<WGM01); //CTC mód
	//OCR0 = 1024; //Timer maximális értéke
	
	TCCR0=(1<<CS01) | (1<<WGM00);
	TIMSK=(1<<TOIE0);
	sei();
	
	DDRB=DDRD=0b11110000;
	PORTB=PORTD=0;
}

////4. feladat////
/*Készítsen RGB LED színkeverőt:
- Az értékeket a mátrix billentyűzetről lehessen bevinni 000-999(RGB) között.
- A 0 érték jelenti azt, hogy az adott színű nem világít, a 9-es pedig azt, hogy maximális fényerővel.
Kijelzés Lenyomott szám
_ 0 0 0 7
_ 0 0 7 1
_ 0 7 1 5
_ 7 1 5 4
_ 1 5 4 9
_ 5 4 9 …

A bevitt számot mindig a * vagy # gomb lenyomásával vizsgálja meg!*/

//Ezt csak SOFTWARE-es PWM-mel lehet megcsinálni!!!!

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void matrix();
void portinit();
void Timer3_Init();
void vizsgal();

volatile unsigned char ell=0, szam=0, red=0;
unsigned char R=0, G=0, B=0;

const unsigned char billtomb[]={69,14,13,11,22,21,19,38,37,35,70,67};
//								 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, #, *
unsigned char tol_tomb[4]={0};

int main()
{
	portinit();
	
	unsigned char sor=0b1000, olvas=0, index=0, i=0;	//sor-inic olvas-beadott bill i-tömb index
	
	while(1)
	{
		PORTC=sor;
		_delay_ms(30);
		olvas= PINC & 0x7F;
		
		for(index=0;index<12;index++)
		{
			if(olvas==billtomb[index])
			{
				if(index==10 || index==11) vizsgal();
				else
				{
					if(i<4)
					{
						tol_tomb[3]=tol_tomb[2];
						tol_tomb[2]=tol_tomb[1];
						tol_tomb[1]=tol_tomb[0];
						tol_tomb[0]=index;
						i++;
					}
					else i=0;
				}
			}
		}
		
		if(red==1)PORTC=0b10000000;
		
		if(sor==0x40) sor=0x08;
		else {sor=sor<<1;}
		
	}
	
	return 0;
}

ISR(TIMER0_OVF_vect)
{
	//PORTA = 0x80 | (hova<<4) | mit; // 0x80 a kezd? címe a 0-nak , a
	//hova<<4 megadja hogy melyik digitr?l beszélünk(0,1,2,3), a mit az értéket rakja ki (0...9)
	switch(ell)
	{
		//case 0: PORTA = 0x80 | (0<<4) | tol_tomb[0];	break;
		//case 1: PORTA = 0x80 | (1<<4) | tol_tomb[1];	break;
		//case 2: PORTA = 0x80 | (2<<4) | tol_tomb[2];	break;
		//case 3: PORTA = 0x80 | (3<<4) | tol_tomb[3];	break;
		case 0: PORTA = 0X80 + tol_tomb[0];	break;
		case 1: PORTA = 0X90 + tol_tomb[1];	break;
		case 2: PORTA = 0XA0 + tol_tomb[2];	break;
		case 3: PORTA = 0XB0 + tol_tomb[3];	break;
	}
	
	ell++;
	if(ell>3)ell=0;
}

void vizsgal()
{
	unsigned char led=0xFF;
	
	/*if( tol_tomb[2]==0 && tol_tomb[3]==0 )											{ red=1; PORTE=PORTB=PORTD=0; }
	else red=0;
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) &&  (tol_tomb[0]%2)==0 )					PORTE=0b00000100;
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) &&  (tol_tomb[0]%2)!=0 )					PORTE=0b00001000;
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) && (tol_tomb[2]<5 && tol_tomb[3]==0)  )	{ PORTB=0b01010101; PORTD=0b01010101; }
	if( (tol_tomb[2]!=0 || tol_tomb[3]!=0) && (tol_tomb[2]>4 || tol_tomb[3]!=0)  )	{ PORTB=0b10101010; PORTD=0b10101010; }
	if( tol_tomb[0]==0 && tol_tomb[1]==1 && tol_tomb[2]==9 && tol_tomb[3]==0)		{ PORTD=led; PORTB=led<<4; }*/
	
	if(tol_tomb[0]==0) led1=0;
	if(tol_tomb[0]==1) led1=20;
	if(tol_tomb[0]==2) led1=30;
	if(tol_tomb[0]==3) led1=40;
	if(tol_tomb[0]==4) led1=50;
	if(tol_tomb[0]==5) led1=60;
	if(tol_tomb[0]==6) led1=70;
	if(tol_tomb[0]==7) led1=80;
	if(tol_tomb[0]==8) led1=90;
	if(tol_tomb[0]==9) led1=100;
	
	if(tol_tomb[1]==0) led2=0;
	if(tol_tomb[1]==1) led2=20;
	if(tol_tomb[1]==2) led2=30;
	if(tol_tomb[1]==3) led2=40;
	if(tol_tomb[1]==4) led2=50;
	if(tol_tomb[1]==5) led2=60;
	if(tol_tomb[1]==6) led2=70;
	if(tol_tomb[1]==7) led2=80;
	if(tol_tomb[1]==8) led2=90;
	if(tol_tomb[1]==9) led2=100;
	
	if(tol_tomb[2]==0) led3=0;
	if(tol_tomb[2]==1) led3=15;
	if(tol_tomb[2]==2) led3=25;
	if(tol_tomb[2]==3) led3=35;
	if(tol_tomb[2]==4) led3=45;
	if(tol_tomb[2]==5) led3=55;
	if(tol_tomb[2]==6) led3=65;
	if(tol_tomb[2]==7) led3=78;
	if(tol_tomb[2]==8) led3=90;
	if(tol_tomb[2]==9) led3=100;
		
	void Timer3_Init();
	
}

void portinit()
{
	DDRA=0xFF;
	DDRC=0b11111000;
	DDRE=0b00001100;
	PORTA=PORTC=PORTE=0;
	
	TCCR0=(1<<CS01) | (1<<WGM00);
	TIMSK=(1<<TOIE0);
	sei();
	
	DDRB=DDRD=0b11110000;
	PORTB=PORTD=0;
}

////1. feladat////
/*Készítsen fényerő szabályozót a TIM1 PWM csatornáit felhasználva.
A LED1-LED3 LED-ek fényerejét gombnyomásra változtassa (maximum elérésekor kezdje előlről).
G0: LED1 fényerő nő
G1: LED2 fényerő nő
G2: LED3 fényerő nő
G3: LED-ek kikapcsolása (egyik sem világít)
G4: LED-ek bekapcsolása (mindhárom teljes fényerőn világít)*/

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char i=0, led1=0, led2=0, led3=0;

void Timer1_Init();
void portinit();
void button();

int main()
{
	portinit();
	Timer1_Init();
	
	while(1) 
	{
		//button();
		_delay_ms(5);
		if(PING==0b1) led1++;
		if(PING==0b10) led2++;
		if(PING==0b100) led3++;
		if(PING==0b1000) led1=led2=led3=100;
		if(PING==0b10000) led1=led2=led3=0;
		
		if(led1==101) led1=0;
		if(led2==101) led2=0;
		if(led3==101) led3=0;
		
		Timer1_Init();
	}
}

void button()		//nem tudom hogy ez vagy a fentebbi "if"-ezés az optimálisabb ezért benne hagytam mindkettőt
{
	i=PING&0x1F;
	_delay_ms(5);
	
	switch(i)
	{
		case 1: led1++; break;
		case 2: led2++; break;
		case 4: led3++; break;
		case 8: led1=led2=led3=100; break;
		case 16: led1=led2=led3=0; break;
	}
}

void Timer1_Init()
{
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) |		//clear
			 (1<<COM1B1) | (0<<COM1B0) |			//Clear OCnA/OCnB/OCnC on compare
			 (1<<COM1C1) | (0<<COM1C0) |			//match (set output to low level).
			 (1<<WGM11) | (0<<WGM10);			//fast pwm ICRn + beállítás alább
	
	TCCR1B = (0 << ICNC1) | (0 << ICES1) |			//Input Capture Noise Canceler & Input Capture Edge Select
			 (1 << WGM13) | (1 << WGM12) |		//fast pwm ICRn + beállítás fentebb
			 (1<<CS11) | (1<<CS00);					//clock source 64bites előosztással
	
	TCCR1C = 0;
	
	ICR1 = 250;				//komparálási érték ami az ICR1-ben van eltárolva ami az Input Capture Register
	
	OCR1A = led1; //duty 0-250		//Output Compare Register
	OCR1B = led2; //duty 0-250
	OCR1C = led3; //duty 0-250
	//A kimenetek értéke, például OC1A esetén 1 lesz az OCR1Aban
	//beállított komparálási értékig, majd utána nulla, a számláló alsó 
	//értékének elérésekor ismét 1. Ez igaz a B-re és C-re is.
	
	sei();
}

void portinit()
{
	DDRB=DDRD=0xF0;
	DDRG=0;
	
	PORTB=PORTD=0;
}

////2. feladat////
/*Készítsen mátrix billentyűzetre menüt! Az RGB LED zöld színének fényerejét (TIM3) a mátrix billentyűzettel változtassa a 
következő beállítások szerint:
0: kikapcsolt állapot
1: 10%
2: 20%
3: 30%
…
9: 90%
*: 100%
#: 0,5s villogás*/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void portinit();
void Timer3Init();

void villog(unsigned char fenyero);

const unsigned char billtomb[]={0,69,14,13,11,22,21,19,38,37,35,70,67};	//13

int pwm=0;	

int main()
{
	portinit();
	Timer3Init();
	
	unsigned char sor=0b1000, olvas=0, index=0, feny=0;
	
	while(1)
	{
		PORTC=sor;
		_delay_ms(10);
		olvas= PINC & 0x7F;
		
		for(index=0;index<13;index++)
		{
			if(olvas==billtomb[index])
			{
				switch(index)
				{
					case 1: pwm=0; break;
					case 2: pwm=100; break;
					case 3: pwm=200; break;
					case 4: pwm=300; break;
					case 5: pwm=400; break;
					case 6: pwm=500; break;
					case 7: pwm=600; break;
					case 8: pwm=700; break;
					case 9: pwm=800; break;
					case 10: pwm=900; break;
					case 11: pwm=1000; break;
					case 12: feny=pwm; villog(feny); break;
				}
			}
		}
		
		Timer3Init();
		
		if(sor==0x40) sor=0x08;
		else {sor=sor<<1;}
	}
}

void villog(unsigned char fenyero)
{
	pwm=0;
	Timer3Init();
	_delay_ms(50);
	pwm=fenyero;
	Timer3Init();
	_delay_ms(50);
}

void Timer3Init()
{
	TCCR3A = (1<<COM3A1)|(1<< WGM31); //phase correct pwm ICR3
	TCCR3B = (1<< WGM33) |(1<<CS30)| (1<<CS31) ; //F_CPU/64
	
	ICR3=1000;
	
	OCR3A=pwm;
}

void portinit()
{
	//DDRD = 0xF0; //LEDek beállítása
	//DDRB = 0xF0;
	DDRC=0b11111000;
	DDRE=0b1000;
	PORTC=PORTE=0;
}

////3. feladat////
/*Készítsen fényerő szabályozót az RGB zöld LED-re.
A G0-val növelje a kitöltési tényezőt, a G1 lenyomására csökkentse azt. 
Egy gombnyomásra ~2%-ot változzon. 
Az aktuális kitöltés megközelítő értékét %-ban írja a 7szegmenses kijelzőre.*/

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void portinit();
void Timer3Init();

int tol_tomb[3]={0};

volatile unsigned char ell=0;
int pwm=0;

int main()
{
	portinit();
	Timer3Init();
	
	while(1)
	{
		////felfele
		if(PING==0b1) { pwm=pwm+20; tol_tomb[0]=tol_tomb[0]+2; }
		if(tol_tomb[0]==10) { tol_tomb[1]++; tol_tomb[0]=0; }
		if(tol_tomb[1]==10) { tol_tomb[2]++; tol_tomb[1]=0; }
			
		if(tol_tomb[2]==1) { tol_tomb[0]=0; tol_tomb[1]=0; tol_tomb[2]=1; pwm=1000; }
		
		////lefele
		if(PING==0b10) 
		{ 
			if(tol_tomb[0]>=0 || tol_tomb[1]>0 || tol_tomb[1]>0)
			{
				pwm=pwm-20;
				tol_tomb[0]=tol_tomb[0]-2;
			}
			if(tol_tomb[2]==1 && PING==0b10) 	{ tol_tomb[2]=0; tol_tomb[1]=10; }
			if(tol_tomb[0]<0 && tol_tomb[1]>0) { tol_tomb[1]--; tol_tomb[0]=8; }
		}
		if(tol_tomb[2]<=0 && tol_tomb[1]<=0 && tol_tomb[0]<=0) { tol_tomb[0]=0; tol_tomb[1]=0; tol_tomb[2]=0; pwm=0; }
		
		Timer3Init();
		
		_delay_ms(50);
	}
}

ISR(TIMER0_OVF_vect)
{
	switch(ell)
	{
		case 0: PORTA = 0X80 + tol_tomb[0];	break;
		case 1: PORTA = 0X90 + tol_tomb[1];	break;
		case 2: PORTA = 0XA0 + tol_tomb[2];	break;
	}
	
	ell++;
	if(ell>3)ell=0;
}

void Timer3Init()
{
	TCCR3A = (1<<COM3A1)|(1<< WGM31); //phase correct pwm ICR3
	TCCR3B = (1<< WGM33) |(1<<CS30)| (1<<CS31) ; //F_CPU/64
	
	ICR3=1000;
	
	OCR3A=pwm;
}

void portinit()
{
	DDRC=0b10000000;
	DDRA=0xFF;
	DDRE=0b1000;
	DDRG=0;
	
	TCCR0=(1<<CS01) | (1<<WGM00);
	TIMSK=(1<<TOIE0);
	sei();
	
	PORTA=PORTE=PORTG=PORTC=0;	
}

////4. feladat////
/*Készítsen menüt az LCD kijelzőre!
LCD-re:
Menu1 <
Menu2
Menu3
Menu4

- 1-es nyomógomb lefele lépteti a nyilat
- 2-es nyomógomb felfelé lépteti a nyilat

- Mindig az a LED világít, amelyik menüponton állunk éppen.

- Egy gombnyomásra csak egyet lépjen a nyíl és lehessen körbe is léptetni.
Azaz:
ha a Menu1-en állunk és felfele lépünk, akkor a Menu4-re ugorjon
ha a Menu4-en állunk és lefelé lépünk, akkor a Menu1-re lépjen.*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "LCD.h"
#define F_CPU 8000000UL
#include <util/delay.h>

void portinit();
void torles();
void fgv1();
void fgv2();
void fgv3();
void fgv4();

char check=0, shift=0;

////pointerek és stringek////
char *p1=0;
char string1[]="Menu1";

char *p2=0;
char string2[]="Menu2";

char *p3=0;
char string3[]="Menu3";

char *p4=0;
char string4[]="Menu4";
/////////////////////////////////////////

int main(void)
{
	portinit();
	LCD_init();
	LCD_creatctg();		//egyedi karakter
	
	p1=string1;
	p2=string2;
	p3=string3;
	p4=string4;

	check=5;

	while(1)
	{
		if(PING==0b1) check++;
		_delay_ms(50);
		if(PING==0b10) check-=3;
	}
	
}

void fgv1()
{
	torles();
	
	LCD_command(0x80);
	
	for(shift=0; shift<5; shift++) LCD_command(0x14);
	
	LCD_data(0);		//<
	
	PORTD=0b00000001;
	PORTB=0b00000001<<4;
	check++;
}

void fgv2()
{
	torles();
	
	LCD_command(0xC0);
	
	for(shift=0; shift<5; shift++) LCD_command(0x14);
	
	LCD_data(0);		//<
	
	PORTD=0b00000010;
	PORTB=0b00000010<<4;
	check++;
}

void fgv3()
{
	torles();
	
	LCD_command(0x80);
	
	for(shift=0; shift<21; shift++) LCD_command(0x14);
	
	LCD_data(0);		//<
	
	PORTD=0b00000100;
	PORTB=0b00000100<<4;
	check++;
}

void fgv4()
{
	torles();
	
	LCD_command(0xC0);
	
	for(shift=0; shift<21; shift++) LCD_command(0x14);
	
	LCD_data(0);		//<
	
	PORTD=0b00001000;
	PORTB=0b00001000<<4;
	check++;
}
///////////////////////////////////
void torles()
{
	LCD_init();
	////1. és 3. sor////
	while(*p1)
	{
		LCD_data(*p1);
		p1++;
	}
	if(*p1==0) p1=string1;
	
	for(shift=0; shift<11; shift++) LCD_data(' ');
	
	while(*p3)
	{
		LCD_data(*p3);
		p3++;
	}
	if(*p3==0) p3=string3;
	////////////////////////////////
	
	////2. és 3. sor////
	for(shift=0; shift<19; shift++) LCD_data(' ');
	
	while(*p2)
	{
		LCD_data(*p2);
		p2++;
	}
	if(*p2==0) p2=string2;
	
	for(shift=0; shift<11; shift++) LCD_data(' ');
	
	while(*p4)
	{
		LCD_data(*p4);
		p4++;
	}
	if(*p4==0) p4=string4;
	////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{
	if(check==13) check=5;
	if(check<5 && check>0) check=11;
	
	if(check==5) fgv1();
	
	if(check==7) fgv2();
	
	if(check==9) fgv3();
	
	if(check==11) fgv4();
}

void portinit()
{
	DDRB=DDRD=0xF0;
	DDRG=0;
	PORTB=PORTD=PORTG=0;
	
	TCCR0=(1<<CS01) | (1<<WGM00);
	TIMSK=(1<<TOIE0);
	sei();
}

////5. feladat////
/*Készítsen fényújságot az LCD kijelzőre!
LCD kijelzőn a Hello World felirat fényújságként fusson az első (0.) sorban folyamatosan. Ha
a végén egy betű kifutott (elsőként d), az jelenjen meg a sor elején.*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "LCD.h"
#define F_CPU 8000000UL
#include <util/delay.h>

void visszatolas();

char *p1=0;
char string1[]="Hello World";

int main(void)
{
	LCD_init();
	
	unsigned char shift=0;
	
	
	p1=string1;
	
	while(*p1)
	{
		LCD_data(*p1);
		p1++;
	}
	//if(*p1==0) p1=string1;
	
	while(1)
	{
		LCD_command(0x1c);
		shift++;
		
		_delay_ms(500);
		
		if(shift==4)
		{
			LCD_command(0x10);
			LCD_command(0x1c);
			visszatolas();
			shift=0;
			//while(1);
		}
	}
}

void visszatolas()
{
	char index=0;
	
	LCD_command(0x80);
	
	while(1)
	{
		
	}
}

////3. feladata////
/*A Gomb0 lenyomásával jelenjen meg a terminálprogramban a következő:
MENU_1: Buzzer ON [1]
MENU_2: LCD_ON [2]
MENU_3: PWM_ON [3]
MENU_4: Szamlalo_ON [4]

Ha a számítógép billentyűzetén lenyomjuk a(z):
- ’1’-es billentyűt, akkor szólaljon meg a Buzzer (PE2)
- ’2’-es billentyűt, akkor az LCD-re írja ki: LCD ON
- ’3’-as billentyűt, akkor LED3 fényereje folyamatosan változzon
- ’4’-es billentyűt, akkor a LED7-4-en 0-15ig folyamatosan kerüljenek ki a számok
Egyéb leütött billentyű esetén hibaüzenet jelenjen meg: Nincs ilyen MENU
Súgó:
- 1: Buzzert úgy lehet kezelni, mintha a RGB zöld (valakinél a kék) LED-et
- 2: LCD kiírás: sztringként
- 3: TIMER1 PWM
- 4: Timer használata kötelező*/

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include "LCD.h"
#define FOSC 8000000
#define BAUD 9600
#define MYUBRR (FOSC/(8*BAUD))-1
#define F_CPU 8000000UL
#include <util/delay.h>

void portinit();
void Timer1_Init();
void Timer3_Init();

void USART0_Init( unsigned int ubrr );
void USART0_Transmit( unsigned char data );
unsigned char USART0_Receive();

void errorfgv();
void fgv1();
void fgv2();
void fgv3();
void fgv4();

char signal=0;
unsigned char led1=0, led2=0, led3=0;
unsigned char ocr_index=0;
int szam=0;

int main(void)
{
	portinit();
	USART0_Init ( 103 );
	
	//LCD_data('A');
	
	////POINTEREK, STRINGEK////
	char *p1=0;
	char string1[]="\n\rMENU_1: Buzzer ON [1]\n\r";
	p1=string1;
	
	char *p2=0;
	char string2[]="MENU_2: LCD_ON [2]\n\r";
	p2=string2;
	
	char *p3=0;
	char string3[]="MENU_3: PWM_ON [3]\n\r";
	p3=string3;
	
	char *p4=0;
	char string4[]="MENU_4: Szamlalo_ON [4]\n\r";
	p4=string4;
	///////////////////////////
	unsigned char kapott_ertek=0;
	
    while (1) 
    {
		////GOMB nyomás kiirítás////
		if(PING==0b1)
		{
			while(*p1!=0)
			{
				USART0_Transmit(*p1);
				p1++;
				_delay_ms(50);
			}
			if(*p1==0) p1=string1;
			
			while(*p2!=0)
			{
				USART0_Transmit(*p2);
				p2++;
				_delay_ms(50);
			}
			if(*p2==0) p2=string2;
			
			while(*p3!=0)
			{
				USART0_Transmit(*p3);
				p3++;
				_delay_ms(50);
			}
			if(*p3==0) p3=string3;
			
			while(*p4!=0)
			{
				USART0_Transmit(*p4);
				p4++;
				_delay_ms(50);
			}
			if(*p4==0) p4=string4;
			
			signal=1;
		}
		///////////////////////////
		if(signal==1)
		{
			kapott_ertek=USART0_Receive();
			
			switch(kapott_ertek)
			{
				case '1': fgv1(); break;
				case '2': fgv2(); break;
				case '3': fgv3(); break;
				case '4': fgv4(); break;
				default: errorfgv(); break;
			}
		}
    }
}


void fgv1()
{
	PORTE=0b1000;
	_delay_ms(200);
	PORTE=0;
}

void fgv2()
{
	LCD_init();
	char *lcds=0;
	char lcdst[]="LCD ON";
	lcds=lcdst;

	while(*lcds)
	{
		LCD_data(*lcds);
		lcds++;
	}
	if(*lcds==0) lcds=lcdst;
}

void fgv3()
{
	Timer1_Init();
	unsigned char i=0, b=0;
	while(b<4)
	{
		while(i<251) { i++; led3++; _delay_ms(5); Timer1_Init(); }
		while(i!=0) { i--; led3--; Timer1_Init(); }
		b++;
	}
}

void fgv4()
{
	Timer3_Init();
	DDRD=0xf0;
	PORTD=0;
	szam=0;
	while(szam<=255)
	{
		PORTD=szam;
	}
	PORTD=0;
}

void errorfgv()
{
	char *error=0;
	char errorst[]="Nincs ilyen ertek!";
	error=errorst;
	
	while(*error!=0)
	{
		USART0_Transmit(*error);
		error++;
		_delay_ms(50);
	}
	if(*error==0) error=errorst;
	USART0_Transmit(9);
}

////USART és initek////
void USART0_Transmit(unsigned char data )
{
	//Wait for empty transmit buffer
	while (!( UCSR0A & (1<< UDRE0)));
	
	//Put data into buffer, sends the data
	UDR0 = data;
}

unsigned char USART0_Receive()
{
	// Wait for data to be received
	while ( !(UCSR0A & (1<< RXC0)));
	
	// Get and return received data from buffer
	return UDR0;
}

////INICIALIZÁLÁSOK////
void portinit()
{
	DDRB=0xf0;
	DDRG=0;
	DDRE=0b1100;
	PORTB=PORTE=0;
}

void Timer1_Init()
{
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) |		//clear
	(1<<COM1B1) | (0<<COM1B0) |			//Clear OCnA/OCnB/OCnC on compare
	(1<<COM1C1) | (0<<COM1C0) |			//match (set output to low level).
	(1<<WGM11) | (0<<WGM10);			//fast pwm ICRn + beállítás alább
	
	TCCR1B = (0 << ICNC1) | (0 << ICES1) |			//Input Capture Noise Canceler & Input Capture Edge Select
	(1 << WGM13) | (1 << WGM12) |		//fast pwm ICRn + beállítás fentebb
	(1<<CS11) | (1<<CS00);					//clock source 64bites előosztással
	
	TCCR1C = 0;
	
	ICR1 = 250;				//komparálási érték ami az ICR1-ben van eltárolva ami az Input Capture Register
	
	OCR1A = led1; //duty 0-250		//Output Compare Register
	OCR1B = led2; //duty 0-250
	OCR1C = led3; //duty 0-250
	//A kimenetek értéke, például OC1A esetén 1 lesz az OCR1Aban
	//beállított komparálási értékig, majd utána nulla, a számláló alsó
	//értékének elérésekor ismét 1. Ez igaz a B-re és C-re is.
	
	sei();
}

ISR(TIMER3_COMPA_vect)	//Timer1 komparálási interrupt
{
	szam++;
}

void Timer3_Init()
{
	TCCR3B = (1<<WGM32) | (1<<CS32) | (1<<CS30);	//ctc //1024
	TCCR3C = 0;
	OCR3A = 50;					// max 65535 (16bit-es)
	ETIMSK |= (1<<OCIE3A);							//engedélyezés
	sei();
}

void USART0_Init( unsigned int ubrr )
{
	//Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//Enable double transmission speed
	UCSR0A = (1<<U2X0);
	
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	//Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

////4. feladat////
/*Készítsen UART számológépet!
- Számok bevitele: mátrix billentyűzet (0-9)
- Művelet bevitele: gombok
o gomb0: összeadás (+)
o gomb1: kivonás (-)
o gomb2: szorzás (*)
o gomb3: osztás (/)
o gomb4: hatványozás (^)
- Eredmény: mátrix billentyűzet * vagy # gombjának lenyomására
- Műveletvégzés maximum 4jegyű számokon, ellenkező esetben hibaüzenet
Kerüljön kiíratásra (UART): mindkét operandus, műveleti jel, eredmény*/

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void portinit();

void USART0_Init( unsigned int ubrr );
void USART0_Transmit( unsigned char data );
unsigned char USART0_Receive();

void osszead();
void kivon();
void szoroz();
void oszt();
void hatvany();
void errorfgv();

void read();
void read2();

const unsigned char billtomb[]={69,14,13,11,22,21,19,38,37,35,70,67};
//								 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, #, *
unsigned char lep=0;

int szam=0, szam1=0, szam2=0;

char rake=0;

char ki_string[]={};
char *ki;

int main(void)
{
	portinit();
	USART0_Init ( 103 );
	
	char gomb=0;
	
	lep=1;
	
	while(1)
	{
		if(lep==1) read();
		if(rake==1) { szam1=szam; lep=2; } //PORTB=0;
		
		if(PING==0b1)		{ gomb=1; USART0_Transmit('+'); }
		if(PING==0b10)		{ gomb=2; USART0_Transmit('-'); } 
		if(PING==0b100)		{ gomb=3; USART0_Transmit('*'); } 
		if(PING==0b1000)	{ gomb=4; USART0_Transmit('/'); } 
		if(PING==0b10000)	{ gomb=5; USART0_Transmit('^'); } 
		
		if(lep==2 && gomb>0) read();
		
		if(rake==2) { szam2=szam; lep=3; read2(); }
		
		if(rake==3)
		{
			switch(gomb)
			{
				case 1: osszead();	gomb=0;	break;
				case 2: kivon();	gomb=0;	break;
				case 3: szoroz();	gomb=0; break;
				case 4: oszt();		gomb=0; break;
				case 5: hatvany();	gomb=0; break;
			}
		}
		
	}
}

void osszead()
{
	szam=szam1+szam2;
	
	USART0_Transmit('=');
	
	itoa(szam, ki_string, 10);
	ki=ki_string;
	while(*ki)
	{
		USART0_Transmit(*ki);
		ki++;
	}
	
	USART0_Transmit('\n');
	USART0_Transmit('\r');
	
	lep=1;
	rake=0;
}

void kivon()
{
	szam=szam1-szam2;
	
	USART0_Transmit('=');
	
	itoa(szam, ki_string, 10);
	ki=ki_string;
	while(*ki)
	{
		USART0_Transmit(*ki);
		ki++;
	}
	
	USART0_Transmit('\n');
	USART0_Transmit('\r');
	
	lep=1;
	rake=0;
}

void szoroz()
{
	szam=szam1*szam2;
	
	USART0_Transmit('=');
	
	itoa(szam, ki_string, 10);
	ki=ki_string;
	while(*ki)
	{
		USART0_Transmit(*ki);
		ki++;
	}
	
	USART0_Transmit('\n');
	USART0_Transmit('\r');
	
	lep=1;
	rake=0;
}

void oszt()
{
	szam=(double)szam1/szam2;
	
	USART0_Transmit('=');
	
	itoa(szam, ki_string, 10);
	ki=ki_string;
	while(*ki)
	{
		USART0_Transmit(*ki);
		ki++;
	}
	
	USART0_Transmit('\n');
	USART0_Transmit('\r');
	
	lep=1;
	rake=0;
}

void hatvany()
{
	char hjk=1;
	szam=1;
	for (hjk=1;hjk<=szam2;hjk++)
	{
		szam=szam1*szam;
	}
	
	USART0_Transmit('=');
	
	itoa(szam, ki_string, 10);
	ki=ki_string;
	while(*ki)
	{
		USART0_Transmit(*ki);
		ki++;
	}
	
	USART0_Transmit('\n');
	USART0_Transmit('\r');
	
	lep=1;
	rake=0;
}

void errorfgv()
{
	
}

void read()
{
	int sor=0b1000, olvas=0, i=0, index=0;
	char string[]={0};
	char *p;
	szam=0;
	//PORTB=0xf0;
	while(lep==1 || lep==2)
	{
		PORTC=sor;
		_delay_ms(30);
		olvas= PINC & 0x7F;
		
		for(index=0;index<12;index++)
		{
			if(olvas==billtomb[index])
			{
				if(index==10 || index==11);
				else
				{
					
					szam=(szam*10)+index;
					
					itoa(index,string,10);
					p=string;
					USART0_Transmit(*p);
					i++;
					
					if(i>3) 
					{
						//USART0_Transmit('E');
						lep=0;
						rake++;
						//return;
					}
				}
			}
		}
		
		if(sor==0x40) sor=0x08;
		else {sor=sor<<1;}
	}
}

void read2()
{
	int sor=0b1000, olvas=0, index=0;
	szam=0;
	//PORTB=0xf0;
	while(lep==3)
	{
		PORTC=sor;
		_delay_ms(30);
		olvas= PINC & 0x7F;
		
		for(index=0;index<12;index++)
		{
			if(olvas==billtomb[index])
			{
				if(index==10 || index==11) { lep=0; rake=3; return; }
			}
		}
		
		if(sor==0x40) sor=0x08;
		else {sor=sor<<1;}
	}
}

////
////
////USART////////////////////////////////////////////////
void USART0_Transmit(unsigned char data )
{
	//Wait for empty transmit buffer
	while (!( UCSR0A & (1<< UDRE0)));
	
	//Put data into buffer, sends the data
	UDR0 = data;
}

unsigned char USART0_Receive()
{
	// Wait for data to be received
	while ( !(UCSR0A & (1<< RXC0)));
	
	// Get and return received data from buffer
	return UDR0;
}

////INICIALIZÁLÁS//////////////////////////////////////////////////
void portinit()
{
	DDRC=0b11111000;
	DDRG=0b11111;
	
	PORTC=PORTG=0;
	
	DDRB=DDRD=0;
	PORTB=PORTD=0;
}

void USART0_Init( unsigned int ubrr )
{
	//Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//Enable double transmission speed
	UCSR0A = (1<<U2X0);
	
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	//Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

/*
Készítsen RTC (nem kötelező RTC) órát!
-	Idő megadása mátrix billentyűzettel.
-	Kijelzés: LCD 0. sor
-	LCD 1. sorban az éppen aktuális pozícióban a nyíl (^)
-	Bevitel kezdete: G0
-	Bevitel: G1 (csak akkor, ha megfelelő a formátum: 00:00:00 - 23:59:59)
o	Elindul az óra
-	Törlés: G2 (1 gombnyomásra csak egyet töröl) az aktuális pozícióban
-	Ha végig értünk a nyíllal és nem történt bevitel (G1 nyomás) vagy hibás a formátum, akkor az elejéről indul újra a beírás.



G0-G4: az 5db nyomógomb jelölése, NEM a MÁTRIX billentyűzet gombjai!!!

Az LCD kijelző sorának számozása 0-tól indul és 3-ig tart.
*/
#include <avr/io.h>
#include <string.h>
#include "LCD.h"

#define F_CPU 8000000UL
#include <util/delay.h>

void portinit();
void bevitel();
void start();
void kiir();
void ellenorzes();
void szamol();

const unsigned char billtomb[]={69,14,13,11,22,21,19,38,37,35,70,67};
//								 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, #, *

char *p1=NULL;
char string1[]="00:00:00";

char gomb=0, valto=0, i_check=0, elem=0, error=0;
unsigned char sor=0b1000, olvas=0, index=0, i=0;

int main(void)
{
	portinit();
	
    LCD_init();
	LCD_creatctg();
	
	start();
	
    while (1) 
    {
		PORTD=0;
		if(PING==0b1) { gomb=1; bevitel(); }
		if(PING==0b10) { gomb=2; ellenorzes(); }

		if(error>0) start();
    }
}

void ellenorzes()
{
		if(string1[0]>=51) error=1;
		
		if(string1[3]>=54) error=1;
		
		if(string1[6]>=54) error=1;
		
		if(error==0) szamol();
}

void szamol()
{
	
}

void bevitel()
{
	p1=string1;
	PORTD=0xF0;
	while(gomb==1)
	{
		PORTC=sor;
		_delay_ms(30);
		olvas= PINC & 0x7F;
		
		for(index=0;index<12;index++)
		{
			if(olvas==billtomb[index])
			{
				if(index==10 || index==11);
				else
				{
					if(i<8)
					{
						switch(index)
						{
							case 0: string1[elem]='0'; break;
							case 1: string1[elem]='1'; break;
							case 2: string1[elem]='2'; break;
							case 3: string1[elem]='3'; break;
							case 4: string1[elem]='4'; break;
							case 5: string1[elem]='5'; break;
							case 6: string1[elem]='6'; break;
							case 7: string1[elem]='7'; break;
							case 8: string1[elem]='8'; break;
							case 9: string1[elem]='9'; break;
						}
						
						i++;
						if(i==2 || i==5) { i++; elem++; }
						elem++;
						kiir();
					}
				}
			}
		}
		if(sor==0x40) sor=0x08;
		else {sor=sor<<1;}
		if(i>7) gomb=0;
	}
}

void kiir()
{
	i_check=i;
	
	LCD_command(0x80);
	for(char step=0; step<8; step++) LCD_command(0x14);
	
	p1=string1;
	while(*p1)
	{
		LCD_data(*p1);
		p1++;
	}
	if(*p1==0) { p1=string1; p1=p1+elem; } // 
		
	LCD_command(0xC0);
	for(char step=0; step<8; step++) LCD_command(0x14);
	while(i_check!=0) { LCD_command(0x14); i_check--; }
	LCD_data(0);
}

void start()
{
	LCD_command(0x01);
	gomb=0;
	error=0;
	i=0;
	////
	for(char step=0; step<8; step++) LCD_command(0x14);
	p1=string1;
	while(*p1)
	{
		LCD_data(*p1);
		p1++;
	}
	if(*p1==0) p1=string1;
	////
	LCD_command(0xC0);
	for(char step=0; step<8; step++) LCD_command(0x14);
	LCD_data(0);
}

void portinit()
{
	DDRC=0b11111000;
	DDRG=0;
	
	PORTC=0;
	
	DDRB=DDRD=0b11110000;
	PORTB=PORTD=0;
}

