#include "project.h"
#include "stdio.h"
volatile uint8 S1=0, S2=0, S3=0,S4=0,S5=0,S6=0; //contact status
void action(uint8 control_number)//read from the contact TX channel, called after an interrupt
                                 //is triggered an shows that we need to read, i.e. high final logic
{
    char buffer[14]="";
    char ch=0;
    char a=0;
    char b=0;
    char c=0;
    char d=0;
    char e=0;
    char f=0;
    Control_Reg_Write(control_number-1);
    while (ch!='@')//Nico, I need you to help me change it into a
                   //for loop that accepts arbitrary number of characters from UART.
    {    
        ch=UART_GetChar();
        if (ch=='#') //beginning byte for a sentence
        {
            while(a==0)//while to wait for a non-null char is important,
                       //otherwise it will print out a lot of blanks
            {
                a=UART_GetChar();
            }
            buffer[0]=a;
            while(b==0)
            {
                b=UART_GetChar();
            }
            buffer[1]=b;
            while(c==0)
            {
                c=UART_GetChar();
            }
            buffer[2]=c;
            while(d==0)
            {
                d=UART_GetChar();
            }
            buffer[3]=d;
            while(e==0)
            {
                e=UART_GetChar();
            }
            buffer[4]=e;
            while(f==0)
            {
                f=UART_GetChar();
            }
            buffer[5]=f;
            
            buffer[6]=':';
            buffer[7]=control_number+'0';
            buffer[8]='.';
            buffer[9]='.';
            buffer[10]='.';
            buffer[11]='\0';
            UART_computer_PutString(buffer);
            Control_Reg_Write(7);
            ch='@';
        }       
    }    
}

CY_ISR(C1_Int_Handler)//interrupt for determining state of connection and serial read request
                      //upon change in contact pin logics 
{
    CyDelay(30);
    S1=contact1_Read();
    if (S1==1)
    {
        action(1);
    }
    contact1_ClearInterrupt();
}
CY_ISR(C2_Int_Handler)
{
    CyDelay(30);//delay allows it to read the actual status of contact.
                //In either new contact triggered interrupt or change info triggered interrupt,
                //which needs to be read, th final state is always high
    S2=contact2_Read();
    if (S2==1)
    {
        action(2);
    }
    contact2_ClearInterrupt();
}
CY_ISR(C3_Int_Handler)
{   
    CyDelay(30);
    S3=contact3_Read();
    if (S3==1)
    {
        action(3);
    }
    contact3_ClearInterrupt();
}
CY_ISR(C4_Int_Handler)
{
    CyDelay(30);
    S4=contact4_Read();
    if (S4==1)
    {
        action(4);
    }
    contact4_ClearInterrupt();
}
CY_ISR(C5_Int_Handler)
{
    CyDelay(30);
    S5=contact5_Read();
    if (S5==1)
    {
        action(5);
    }
    contact5_ClearInterrupt();
}
CY_ISR(C6_Int_Handler)
{
    CyDelay(30);
    S6=contact6_Read();
    if (S6==1)
    {
        action(6);
    }
    contact6_ClearInterrupt();
}

int main(void)
{
    
    UART_Start();
    UART_computer_Start();
    CyDelay(500);
    
    CyGlobalIntEnable;
    C1_Int_StartEx(C1_Int_Handler);
    C2_Int_StartEx(C2_Int_Handler);
    C3_Int_StartEx(C3_Int_Handler);
    C4_Int_StartEx(C4_Int_Handler);
    C5_Int_StartEx(C5_Int_Handler);
    C6_Int_StartEx(C6_Int_Handler);
    S1=contact1_Read();
    S2=contact2_Read();
    S3=contact3_Read();
    S4=contact4_Read();
    S5=contact5_Read();
    S6=contact6_Read();
}
