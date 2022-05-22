/*
 * traffic.c
 *
 * Created: 2019. 09. 21. 18:20:30
 *  Author: Ghostman
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "traffic.h"

#define F_CPU 8000000UL
#include <util/delay.h>

void portinit()
{
	DDRG=0b11;
	
	DDRA=0xff;
	DDRC=0xff;
	DDRE=0b11001111;	//ne basztasd, ez ÍGY JÓ
	DDRF=0b00001111;
	
	PORTA=PORTC=PORTF=PORTG=0;
	PORTE|=0x30; //FELHÚZÓ ELLENÁLLÁS BEKAPCSOLÁSA A GOMBOKRA
	
	//ellenõrzésnek
	DDRB=DDRD=0xF0;
	PORTB=PORTD=0;
}

////SIMA/////////////////////////////////////////////////////////////////////////////////
////1. ciklus /////////////////////////////////////////
void ciklus1_ps()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_PS;
	
	PORTA|=SLD_P;
	PORTC|=SLD_S;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

void ciklus1_z()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_Z;
	PORTA|=SLB_Z2;
	
	PORTC|=SLD_Z;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG2_P;
	//PORTE=SLG1_Z;
	
	//PORTF=SLV_F;
}

void ciklus1_s()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_S;
	
	PORTC|=SLD_S;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

////2. ciklus /////////////////////////////////////////
void ciklus2_ps()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_PS;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

void ciklus2_z()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_Z;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_P;
	PORTF|=SLE_Z2;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

void ciklus2_s()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_S;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_P;
	PORTF=0;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

////3. ciklus /////////////////////////////////////////
void ciklus3_ps()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_PS;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

void ciklus3_z()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_Z;
	
	PORTC|=SLE_P;
	PORTF|=SLE_Z2;
	
	PORTE|=SLG1_P;
	//PORTE=SLG2_Z;
	
	//PORTF=SLV_F;
}

void ciklus3_s()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_S;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	PORTF=0;
	
	//PORTF=SLV_F;
}

////4. ciklus /////////////////////////////////////////
void ciklus4_ps()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_PS;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

void ciklus4_z()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	PORTA|=SLB_Z2;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_Z;
	PORTF|=SLE_Z2;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
}

void ciklus4_s()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_S;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	PORTF=0;
	//PORTF=SLV_F;
}

////VONAT/////////////////////////////////////////////////////////////////////////////////////////////////
////PIROS-SÁRGA
void vonat_ciklus1_ps()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_PS;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_P1;
	//PORTF=SLV_P2;
}

void vonat_ciklus2_ps()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_PS;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_P1;
	//PORTF=SLV_P2;
}

////zöld
void vonat_ciklus1_z()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_Z;
	
	PORTC|=SLE_P;
	
	//PORTE=SLG2_Z;
	PORTE|=SLG1_P;
	
	//PORTF=SLV_P1;
	//PORTF=SLV_P2;
}

void vonat_ciklus2_z()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	PORTA|=SLB_Z2;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_Z;
	
	PORTE|=SLG2_P;
	PORTE|=SLG1_P;
	
	//PORTF=SLV_P1;
	//PORTF=SLV_P2;
}

////sárga
void vonat_ciklus1_s()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_S;
	
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_P1;
	//PORTF=SLV_P2;
}

void vonat_ciklus2_s()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	
	PORTA|=SLB_P;
	
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	
	PORTC|=SLE_S;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_P1;
	//PORTF=SLV_P2;
}

void szerviz_start()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLB_S;
	PORTA|=SLA_S;
	PORTC|=SLC_S;
	PORTC|=SLD_S;
	PORTC|=SLE_S;
}