/*#include<lpc21xx.h>
#include<string.h>
#include "types.h"
#include "delay.h"
#include "uart0.h"
#include "lcd.h"


extern char buff[50],i;
extern u8 otp[10];
u8 *gsmat[]={"AT\r\n","ATE0\r\n","AT+CMGF=1\r\n","AT+CMGS="};

u8 *gsmresp[]={"OK","ERROR"};

void GSM_init(void)									 
{	
	u32 a=0;
	CmdLCD(0x80);
	StrLCD("GSM init....");

	for(a=0;a<3;a++)
	{
		i=0;memset(buff,'\0',50);
		UART0_Str((char *)gsmat[a]);
		while(i<2);
		delay_ms(500);
		buff[i] = '\0';
		if(strstr(buff,(const char *)gsmresp[0]))
		{		
			CmdLCD(0xC0);
			StrLCD((s8 *)gsmresp[0]);
			delay_ms(500);
			CmdLCD(0xC0);
			StrLCD("  ");
		}
		else
		{
			CmdLCD(0xC0);
			//StrLCD("check the connection properly");
			StrLCD((s8 *)gsmresp[1]);
			//StrLCD("check the connection");
			delay_ms(500);
			CmdLCD(0xC0);
			StrLCD("     ");
		}
	}
}	

//void send_sms(char *num,u32 n)
//{
/*	char msg[10],k=0;
	strcpy(msg,"OTP: ");
	for(k=0;otp[k];k++)
	{
		msg[strlen(msg)]=otp[k];
	} */
/*	CmdLCD(0x01);
	StrLCD("Sending sms....");
	delay_ms(500);
	//CmdLCD(0xc0);
	//StrLCD(msg);
	i=0;memset(buff,'\0',50);
	UART0_Str((char *)gsmat[3]);
	UART0_Tx('"');
	UART0_Str((char *)num);
	UART0_Tx('"');
	UART0_Str("\r\n");
	delay_ms(500);
	i=0;
	UART0_Int(n);
    UART0_Tx(0x1A);
	delay_ms(5000);
	while(i<4);
	buff[i] = '\0';
	CmdLCD(0xc0);
	//StrLCD(buff);
	//delay_ms(3000);
	if(strstr(buff,(const char *)gsmresp[0]))
	{
		CmdLCD(0x01);
		StrLCD("Message Sent..");
		delay_ms(500);
	}
	else
	{
		CmdLCD(0x01);
		StrLCD("Message failed..");
		delay_ms(500);
	}
}
 
*/
#include<lpc21xx.h>
#include<string.h>
#include "types.h"
#include "delay.h"
#include "uart0.h"
#include "lcd.h"

extern char buff[50], i;
extern u8 otp[10];
u8 *gsmat[] = {"AT\r\n", "ATE0\r\n", "AT+CMGF=1\r\n", "AT+CMGS="};
u8 *gsmresp[] = {"OK", "ERROR"};

void GSM_init(void)                                     
{    
    u32 a = 0;
    u32 timeout = 0;
    
    CmdLCD(0x80);
    StrLCD("Welcome to GSM init....    ");

    for(a = 0; a < 3; a++)
    {
        i = 0; 
        memset(buff, '\0', 50);
        
        UART0_Str((char *)gsmat[a]);
        
        timeout = 0;
        while((i < 4) && (timeout < 2000)) 
        {
            delay_ms(1);
            timeout++;
        }
        
        buff[i] = '\0'; 

        if(strstr(buff, (const char *)gsmresp[0]))
        {        
            CmdLCD(0xC0);
            StrLCD("Command OK     ");
            delay_ms(500);
        }
        else
        {
            CmdLCD(0x01); 
            CmdLCD(0x80);
            StrLCD("GSM Error!");
            CmdLCD(0xC0);
            StrLCD("Check Connection");
            while(1); 
        }
    }
    
    CmdLCD(0x01);
    StrLCD("GSM Ready!      ");
    delay_ms(1000);
}

void send_sms(char *num, u32 n)
{
    u32 timeout = 0;

    CmdLCD(0x01);
    StrLCD("Sending sms....");
    delay_ms(500);
    
    i = 0;
    memset(buff, '\0', 50);
    
    UART0_Str((char *)gsmat[3]);
    UART0_Tx('"');
    UART0_Str((char *)num);
    UART0_Tx('"');
    UART0_Str("\r\n");
    delay_ms(500);
    
    i = 0;
    UART0_Int(n);
    UART0_Tx(0x1A);
    
    timeout = 0;
    while((i < 4) && (timeout < 5000))
    {
        delay_ms(1);
        timeout++;
    }
    
    buff[i] = '\0';
    CmdLCD(0xc0);
    
    if(strstr(buff, (const char *)gsmresp[0]))
    {
        CmdLCD(0x01);
        StrLCD("Message Sent..");
        delay_ms(500);
    }
    else
    {
        CmdLCD(0x01);
        StrLCD("Message failed..");
        delay_ms(500);
    }
} 
