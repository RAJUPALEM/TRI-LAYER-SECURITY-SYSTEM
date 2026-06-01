#include<lpc21xx.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
#include "uart0.h"
#include "uart1.h"
#include "defines.h"
#include "eint.h"
#include "i2c.h"
#include "i2c_eeprom.h"

#define LED 4 // P0.4

extern char card[10];
extern unsigned char j,r_flag;
int l=0;
extern char buff[50];
extern unsigned char i;

extern u8 i_flag;

u8 r_pass[5],c_pass[5],d_pass[5]="1234",r_card[10],otp[10],c_otp[5];
u8 phno[]="9398578164";

u32 num=0;

void GSM_init(void);
void send_sms(char *,u32);

/*void convert(u32 n)
{
	s32 k=0,t;
	t=n;
	while(t)
	{	
		k++;
		t/=10;
	}

	otp[0]='O';
	otp[1]='T';
	otp[2]='P';
	otp[3]=':';
	otp[4]=' ';
	otp[k--]='\0';
	while(n)
	{
		otp[k--]=n%10+48;
		n/=10;
	}
} */

int main()
{
	int r,c,time=22;
	IODIR0|=15<<LED;
	IOPIN0=IOPIN0&~(15<<LED)|(0x0f<<LED);	
	InitUART0();
	InitUART1();
	init_i2c();
	Init_LCD();
	init_kpm();
	Enable_EINT0();
	CmdLCD(0X80);
	StrLCD("Welcome TO TRI-LAYER Security System");
	delay_ms(1000);
	GSM_init();
	delay_ms(1000);
	//send_sms((char *)phno,"7745");
	CmdLCD(0x01);
	i2c_eeprom_page_write(0x50,0x00,d_pass,4);
	i2c_eeprom_page_write(0x50,0x10,"12534588",8);
	while(1)
	{
		i2c_eeprom_seq_read(0x50,0x00,r_pass,4);
		i2c_eeprom_seq_read(0x50,0x10,r_card,8);
		memset(card,'\0',10);
		memset(buff,'\0',10);
		IOPIN0=IOPIN0&~(15<<LED)|(0x0f<<LED);
		//do
		//{
			CmdLCD(0X80);
			StrLCD("Waiting for card");
			i=0,j=0,r_flag=0;
			memset(card,'\0',10);
			memset(buff,'\0',50);
			while(r_flag<2)
			{
				if(i_flag)
				{
					change_pass();
					i_flag=0;
					break;
				}
			}
			if(r_flag==2)
			{
			r_flag=0;
			CmdLCD(0X01);
			StrLCD("Validating");
			for(r=11;r<16;r++)
			{
				CmdLCD(0X80+r);
				CharLCD('.');
				delay_ms(50);
			}
			if(strcmp((const char *)card,(const char *)r_card)==0)
			{
				CmdLCD(0X01);
				StrLCD("Valid card");
				delay_ms(1000);
				CmdLCD(0X01);
				StrLCD("Enter the pass");
				Readnum(c_pass);
				CmdLCD(0X01);
				StrLCD("Validating");
				for(r=11;r<16;r++)
				{
					CmdLCD(0X80+r);
					CharLCD('.');
					delay_ms(50);
				}
				if(strcmp((const char *)c_pass,(const char *)r_pass)==0)
				{
					CmdLCD(0X01);
					StrLCD("Valid password");
					delay_ms(1000);
					CmdLCD(0X01);
					StrLCD("OTP Generating.");
					delay_ms(1000);
					i=0;
					for(l=0;l<4;l++)
					{
						srand(time++);
						num=(num*10)+(rand()%10);
					}
					/*CmdLCD(0Xc0);
					U32LCD(num);
					CharLCD(' ');*/
					//convert(num);
					//StrLCD(otp);					
					delay_ms(1000);
					send_sms((char *)phno,num);
					//send_sms((char *)phno);
					CmdLCD(0X01);
					StrLCD("Enter the OTP");
					Readnum(c_otp);
					CmdLCD(0X01);
					StrLCD("Validating");
					for(r=11;r<16;r++)
					{
						CmdLCD(0X80+r);
						CharLCD('.');
						delay_ms(50);
					}
					if(num==atoi((const char *)c_otp))
					{
						CmdLCD(0X01);
						StrLCD("Valid OTP");
						delay_ms(1000);
						CmdLCD(0X01);
						StrLCD("DOOR OPENING..");
						for(r=0;r<2;r++)
						{
							for(c=0;c<4;c++)
							{
								IOPIN0=IOPIN0&~(15<<LED)|(((1<<c)^0x0f)<<LED);
								delay_ms(100);
							}
						}
						CmdLCD(0X01);
						StrLCD("DOOR OPENED..");
						delay_ms(500);
						CmdLCD(0X01);
						StrLCD("DOOR CLOSING..");
						for(r=0;r<2;r++)
						{
							for(c=3;c>=0;c--)
							{
								IOPIN0=IOPIN0&~(15<<LED)|(((1<<c)^0x0f)<<LED);
								delay_ms(100);
							}
						}
						CmdLCD(0X01);
						StrLCD("DOOR CLOSED..");
						delay_ms(500);								
					}
				}				
				else
				{
					CmdLCD(0X01);
					StrLCD("InValid pass");
					delay_ms(1000);
					CmdLCD(0X01);					
				}
			}
			else
			{
				CmdLCD(0X01);
				StrLCD("InValid card");
				delay_ms(1000);
				CmdLCD(0X01);					
			}
			}
						
		//}while(i_flag==0);
		//i_flag=0;
		//change_pass();
	}

}

