/*
 * traffic.h
 *
 * Created: 2019. 09. 21. 18:20:44
 *  Author: Ghostman
 */ 


#ifndef TRAFFIC_H_
#define TRAFFIC_H_

#define LA_P	PA0
#define LA_S	PA1
#define LA_Z	PA2

#define LB_P	PA3
#define LB_S	PA4
#define LB_Z	PA5
#define LB_Z2	PA6

#define LD_P	PA7
#define LD_S	PC0
#define LD_Z	PC1

#define LC_P	PC2
#define LC_S	PC3
#define LC_Z	PC4

#define LE_P	PC5
#define LE_S	PC6
#define LE_Z	PC7
#define LE_Z2	PF0

#define LV_P1	PF1
#define LV_P2	PF2
#define LV_F	PF3

#define LG1_P	PE2
#define LG1_Z	PE3

#define LG2_P	PE6
#define LG2_Z	PE7

#define BTNV1	PE4
#define BTNV2	PE5
#define BTNV1_PRESSED()		(!(PINE & (1<<BTNV1)))
#define BTNV2_PRESSED()		(!(PINE & (1<<BTNV2)))

#define PORTA_MASK	0xFF
#define PORTC_MASK	0xFF
#define PORTF_MASK	0x0F
#define PORTE_MASK	0xCC

////SAJÁT

#define SLA_P 0b00000001		//port A
#define SLA_S 0b00000010		//port A
#define SLA_Z 0b00000100		//port A
#define SLA_PS 0b00000011		//port A

#define SLB_P 0b00001000		//port A
#define SLB_S 0b00010000		//port A
#define SLB_Z 0b00100000		//port A
#define SLB_PS 0b00011000		//port A

#define SLB_Z2 0b01000000		//port A

#define SLD_P 0b10000000		//port A
#define SLD_S 0b00000001		//port C
#define SLD_Z 0b00000010		//port C
							//itt nem tudok összevonást alkalmazni

#define SLC_P 0b00000100		//port C
#define SLC_S 0b00001000		//port C
#define SLC_Z 0b00010000		//port C
#define SLC_PS 0b00001100		//port C

#define SLE_P 0b00100000		//port C
#define SLE_S 0b01000000		//port C
#define SLE_Z 0b10000000		//port C
#define SLE_PS 0b01100000		//port C

#define SLE_Z2 0b00000001		//port F

#define SLG1_P 0b00000100	//port E
#define SLG1_Z 0b00001000	//port E

#define SLG2_P 0b01000000	//port E
#define SLG2_Z 0b10000000	//port E

#define SBTNV1 0b00010000	//port E
#define SBTNV2 0b00100000	//port E
#define SBTNV 0b00110000

#define SLV_P1 0b00000010	//port F
#define SLV_P2 0b00000100	//port F
#define SLV_F 0b00001000	//port F


//init fgvk
void portinit();
void Timer0Init();
void Timer1Init();

//
void jon_vonat();

//start fgv
void start();

//ciklus1
void ciklus1_ps();
void ciklus1_z();
void ciklus1_s();

//ciklus2
void ciklus2_ps();
void ciklus2_z();
void ciklus2_s();

//ciklus3
void ciklus3_ps();
void ciklus3_z();
void ciklus3_s();

//ciklus4
void ciklus4_ps();
void ciklus4_z();
void ciklus4_s();

//VONAT FGVEK
void vonat_start();
void vonat_ciklus1_ps();
void vonat_ciklus2_ps();
void vonat_ciklus1_z();
void vonat_ciklus2_z();
void vonat_ciklus1_s();
void vonat_ciklus2_s();

//PWM FGV-ek
void pwm_gyalogos1();
void pwm_gyalogos2();
void pwm_vasut_feher_nz();
void pwm_vasut_piros_nz();
void pwm_vasut_piros_z();
void pwm_szerviz();


//SZERVÍZ
void szerviz_start();
void pwm_szerviz_jon_vonat();

#endif /* TRAFFIC_H_ */