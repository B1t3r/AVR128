#include <avr/io.h>
#include <avr/interrupt.h>
#include "traffic.h"

#define F_CPU 8000000UL
#include <util/delay.h>

//int ocr_ps=7811, ocr_z=39063, ocr_s=15625, ocr_gyz=23438;
int ocr_tomb[5]={ 15625, 7811, 39062, 15625, 23438 };	//7811-1sec 15625-2sec 23438-3sec 39063-5sec
	
unsigned char ocr_index=0, ciklus_valaszto=0, vonat_ciklus_valaszto=0;

unsigned char gomb_figyelo=0, gomb_figyelo_flag=0, vonat_gomb_figyelo=0, vonat_gomb_figyelo_flag=1;

unsigned char pwm_n=0, pwm_db=0, pwm_n1=0, pwm_valaszto=0;
int pwm_kesleltetes=0 ,pwm_piros_valtas=0;

unsigned char szerviz_gomb=0;

unsigned char szerviz_vonat_flag=0, szerviz_gomb_figyelo=0;

int main(void)
{
	portinit();
	Timer0Init();
	Timer1Init();
	
	ciklus_valaszto=1;
	szerviz_gomb=1;
	
	while (1)
	{
		if(PING==0b1) { szerviz_gomb=1; pwm_valaszto=0; PORTE|=0x30; start(); ocr_index=0; gomb_figyelo=0; }
		if(PING==0b10) { szerviz_gomb=2; szerviz_gomb_figyelo=0; szerviz_start(); }
			
		if(szerviz_gomb==1)
		{
			portinit();
			//PORTE|=0x30;
			
			if(PINE==SBTNV1) { gomb_figyelo=1; }
			if(PINE==SBTNV2) { gomb_figyelo=2; }
			if(gomb_figyelo_flag>0 && gomb_figyelo>0) { gomb_figyelo_flag=0; jon_vonat(); }
			vonat_gomb_figyelo=0;
			
			////PIROS
			if(ocr_index==0)
			{
				while(1)
				{
					Timer1Init();
					start();
					break;
				}
			}
			
			////PIROS-SÁRGA
			if(ocr_index==1)
			{
				gomb_figyelo_flag=0;
				
				while(ciklus_valaszto==1)
				{
					Timer1Init();
					ciklus1_ps();
					break;
				}
				
				while(ciklus_valaszto==2)
				{
					Timer1Init();
					ciklus2_ps();
					break;
				}
				
				while(ciklus_valaszto==3)
				{
					Timer1Init();
					ciklus3_ps();
					break;
				}
				
				while(ciklus_valaszto==4)
				{
					Timer1Init();
					ciklus4_ps();
					break;
				}
			}
			
			////ZÖLD
			if(ocr_index==2)
			{
				while(ciklus_valaszto==1)
				{
					Timer1Init();
					ciklus1_z();
					pwm_valaszto=1;
					break;
				}
				
				while(ciklus_valaszto==2)
				{
					Timer1Init();
					ciklus2_z();
					break;
				}

				while(ciklus_valaszto==3)
				{
					Timer1Init();
					ciklus3_z();
					pwm_valaszto=2;
					break;
				}
				
				while(ciklus_valaszto==4)
				{
					Timer1Init();
					ciklus4_z();
					break;
				}
			}
			
			////SÁRGA
			if(ocr_index==3)
			{
				while(ciklus_valaszto==1)
				{
					Timer1Init();
					ciklus1_s();
					pwm_valaszto=0;
					break;
				}
				
				while(ciklus_valaszto==2)
				{
					Timer1Init();
					ciklus2_s();
					break;
				}

				while(ciklus_valaszto==3)
				{
					Timer1Init();
					ciklus3_s();
					pwm_valaszto=0;
					break;
				}

				while(ciklus_valaszto==4)
				{
					Timer1Init();
					ciklus4_s();
					break;
				}
			}
			
			if(ocr_index>3)
			{
				ocr_index=0;
				ciklus_valaszto++;
			}
			if(ciklus_valaszto>4)
			{
				ciklus_valaszto=1;
			}
			
		}
		///////////////////////////////////////////////SZERV////////////////////////
		if(szerviz_gomb==2)
		{
			if(szerviz_gomb_figyelo==0) { pwm_valaszto=5; }
				
			if(PINE==SBTNV1) 
			{ 
				szerviz_gomb_figyelo=1;
				if(szerviz_vonat_flag==2) { szerviz_gomb_figyelo=0; }
				szerviz_vonat_flag=1;
			}
			if(PINE==SBTNV2) 
			{ 
				szerviz_gomb_figyelo=2;
				if(szerviz_vonat_flag==1) { szerviz_gomb_figyelo=0; }
				szerviz_vonat_flag=2;
			}
			
			if(szerviz_gomb_figyelo>0) { pwm_valaszto=6; }
				
		}
	}
		
}
////FOLYAMAT FGV-EK//+header////////////////////////////////////////////////////////////////////////////////////////////////
void start()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	PORTA|=SLB_P;
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	//PORTF=SLV_F;
	pwm_kesleltetes=0;
	pwm_valaszto=0;
	gomb_figyelo_flag=1;
}

////JÖN A KURVA VONAT/////////////////////////////////////////////////////////////////////////////////////////////////
void jon_vonat()
{
	Timer1Init();
	vonat_ciklus_valaszto=1;
	
	while(gomb_figyelo>0)
	{		
		portinit();
		if(PINE==SBTNV1) { vonat_gomb_figyelo=1; }
		if(PINE==SBTNV2) { vonat_gomb_figyelo=2; }
			
		if(gomb_figyelo==1 && vonat_gomb_figyelo==2 && vonat_gomb_figyelo_flag==1) 
		{ 
			gomb_figyelo=0; 
			portinit(); 
			Timer0Init(); 
			Timer1Init(); 
			ciklus_valaszto=1;
			return; 
		}
		if(gomb_figyelo==2 && vonat_gomb_figyelo==1 && vonat_gomb_figyelo_flag==1)
		{ 
			gomb_figyelo=0; 
			portinit(); 
			Timer0Init(); 
			Timer1Init(); 
			ciklus_valaszto=1;
			return; 
		}
			
		////PIROS
		if(ocr_index==0)
		{
			while(1)
			{
				Timer1Init();
				vonat_start();
				break;
			}
		}
		
		////PIROS-SÁRGA
		if(ocr_index==1)
		{
			vonat_gomb_figyelo_flag=0;
			
			while(vonat_ciklus_valaszto==1)
			{
				Timer1Init();
				vonat_ciklus1_ps();
				break;
			}
			
			while(vonat_ciklus_valaszto==2)
			{
				Timer1Init();
				vonat_ciklus2_ps();
				break;
			}
		}
		
		////ZÖLD
		if(ocr_index==2)
		{
			while(vonat_ciklus_valaszto==1)
			{
				Timer1Init();
				vonat_ciklus1_z();
				pwm_valaszto=4;
				break;
			}
			
			while(vonat_ciklus_valaszto==2)
			{
				Timer1Init();
				vonat_ciklus2_z();
				break;
			}
		}
		
		////SÁRGA
		if(ocr_index==3)
		{
			while(vonat_ciklus_valaszto==1)
			{
				Timer1Init();
				vonat_ciklus1_s();
				pwm_valaszto=3;
				break;
			}
			
			while(vonat_ciklus_valaszto==2)
			{
				Timer1Init();
				vonat_ciklus2_s();
				break;
			}
		}
		
		if(ocr_index>3)
		{
			ocr_index=0;
			vonat_ciklus_valaszto++;
		}
		
		if(vonat_ciklus_valaszto>2)
		{
			vonat_ciklus_valaszto=1;
		}
	}
	
}

////VONAT FGV-ek//+header////////////////////////////////////////////////////////////////////////////
////piros
void vonat_start()
{
	PORTA=PORTC=PORTE=0;
	PORTE|=0x30;
	
	PORTA|=SLA_P;
	PORTA|=SLB_P;
	PORTA|=SLD_P;
	
	PORTC|=SLC_P;
	PORTC|=SLE_P;
	
	PORTE|=SLG1_P;
	PORTE|=SLG2_P;
	
	PORTF=0;
	//PORTF=SLV_P1;
	//PORTF=SLV_P2;
	pwm_kesleltetes=0;
	//pwm_piros_valtas=0;
	pwm_valaszto=3;
	vonat_gomb_figyelo_flag=1;
}



////INTERRUPTOK//////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{
	switch(pwm_valaszto)
	{
		case 1: pwm_gyalogos1();			break;
		case 2: pwm_gyalogos2();			break;
		case 3: pwm_vasut_piros_nz();		break;
		case 4: pwm_vasut_piros_z();		break;
		case 5: pwm_szerviz();				break;
		case 6: pwm_szerviz_jon_vonat();	break;
		default: pwm_vasut_feher_nz();		break;
	}
}

ISR(TIMER1_COMPA_vect)	//Timer1 komparálási interrupt
{ 
	ocr_index++;
}

////INICIALIZÁLÁS////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer0Init()
{
	TCCR0 = (0 << WGM01)| (0 << WGM00) | (0<<CS02) | (1<<CS01) | (0<<CS00);
	TIMSK = (1<<TOIE0);
	sei();
}

void Timer1Init() 
{
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);	//ctc //1024
	TCCR1C = 0;
	OCR1A = ocr_tomb[ocr_index];					// max 65535 (16bit-es)
	TIMSK |= (1<<OCIE1A);							//engedélyezés
	sei();
}

void pwm_gyalogos1()
{
	if (pwm_db==10)
	{
		pwm_n1++;
		if (pwm_n1==8)
		{
			pwm_n1=0;
		}
	}
	pwm_n++;
	pwm_db++;
	pwm_kesleltetes++;
	if (pwm_n>pwm_n1)
	{
		PORTF=0;
		if(pwm_kesleltetes>10000) { PORTE=0; PORTE|=SLG2_P; }
		if(pwm_kesleltetes<10000) { PORTE|=SLG1_Z; PORTE|=SLG2_P; }
		
		if (pwm_n>8)
		{
			pwm_n=0;
		}
	}
	else
	{
		PORTF|=SLV_F;
		if(pwm_kesleltetes>10000) PORTE|=SLG1_Z;
	}
}

void pwm_gyalogos2()
{
	if (pwm_db==10)
	{
		pwm_n1++;
		if (pwm_n1==8)
		{
			pwm_n1=0;
		}
	}
	pwm_n++;
	pwm_db++;
	pwm_kesleltetes++;
	if (pwm_n>pwm_n1)
	{
		PORTF=0;
		if(pwm_kesleltetes>10000) { PORTE=0; PORTE|=SLG1_P; }
		if(pwm_kesleltetes<10000) { PORTE|=SLG2_Z; PORTE|=SLG1_P; }
			
		if (pwm_n>8)
		{
			pwm_n=0;
		}
	}
	else
	{
		PORTF|=SLV_F;
		if(pwm_kesleltetes>10000) PORTE|=SLG2_Z;
	}
}

void pwm_vasut_feher_nz()
{
	if (pwm_db==10)
	{
		pwm_n1++;
		if (pwm_n1==8)
		{
			pwm_n1=0;
		}
	}
	pwm_n++;
	pwm_db++;
	if (pwm_n>pwm_n1)
	{
		PORTF=0;
		if (pwm_n>8)
		{
			pwm_n=0;
		}
	}
	else
	{
		PORTF|=SLV_F;
	}
}

void pwm_vasut_piros_nz()
{
	pwm_piros_valtas++;
	if (pwm_db==10)
	{
		pwm_n1++;
		if (pwm_n1==8)
		{
			pwm_n1=0;
		}
	}
	pwm_n++;
	pwm_db++;
	
	if (pwm_n>pwm_n1)
	{
		PORTF=0;
		
		if (pwm_n>8)
		{
			pwm_n=0;
		}
	}
	else
	{
		if(pwm_piros_valtas/1000==1) { PORTF|=SLV_P1; }
		if(pwm_piros_valtas/2000==1) { PORTF|=SLV_P2; }
		if(pwm_piros_valtas>=3000) pwm_piros_valtas=0;;
	}
}

void pwm_vasut_piros_z()
{
	pwm_piros_valtas++;
	if (pwm_db==10)
	{
		pwm_n1++;
		if (pwm_n1==8)
		{
			pwm_n1=0;
		}
	}
	pwm_n++;
	pwm_db++;
	pwm_kesleltetes++;
	
	if (pwm_n>pwm_n1)
	{
		PORTF=0;
		if(pwm_kesleltetes>10000) { PORTE=0; PORTE|=SLG1_P; }
		if(pwm_kesleltetes<10000) { PORTE|=SLG2_Z; PORTE|=SLG1_P; }
		
		if (pwm_n>8)
		{
			pwm_n=0;
		}
	}
	else
	{
		if(pwm_kesleltetes>10000) PORTE|=SLG2_Z;
		
		if(pwm_piros_valtas/1000==1) { PORTF|=SLV_P1; }
		if(pwm_piros_valtas/2000==1) { PORTF|=SLV_P2; }
		if(pwm_piros_valtas>=3000) pwm_piros_valtas=0;
	}
}

void pwm_szerviz()
{
	if (pwm_db==10)
	{
		pwm_n1++;
		if (pwm_n1==8)
		{
			pwm_n1=0;
		}
	}
	pwm_n++;
	pwm_db++;
	if (pwm_n>pwm_n1)
	{
		PORTA=PORTC=PORTF=0;
		if (pwm_n>8)
		{
			pwm_n=0;
		}
	}
	else
	{
		PORTA|=SLB_S;
		PORTA|=SLA_S;
		PORTC|=SLC_S;
		PORTC|=SLD_S;
		PORTC|=SLE_S;
		PORTF|=SLV_F;
	}
}

void pwm_szerviz_jon_vonat()
{
	pwm_piros_valtas++;
	if (pwm_db==10)
	{
		pwm_n1++;
		if (pwm_n1==8)
		{
			pwm_n1=0;
		}
	}
	pwm_n++;
	pwm_db++;
	if (pwm_n>pwm_n1)
	{
		PORTA=PORTC=PORTF=0;
		if (pwm_n>8)
		{
			pwm_n=0;
		}
	}
	else
	{
		PORTA|=SLB_S;
		PORTA|=SLA_S;
		PORTC|=SLC_S;
		PORTC|=SLD_S;
		PORTC|=SLE_S;
		if(pwm_piros_valtas/1000==1) { PORTF|=SLV_P1; }
		if(pwm_piros_valtas/2000==1) { PORTF|=SLV_P2; }
		if(pwm_piros_valtas>=3000) pwm_piros_valtas=0;
	}
}
////TESZT////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*int main(void)
{
	portinit();
	while(1)
	{
		if(PINE==SBTNV1) PORTF=SLV_F;
		if(PINE==SBTNV2) PORTA=SLA_S;
	}
	
}*/