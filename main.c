#include "project.h"
#include "stdio.h"
volatile uint8 S1=0, S2=0, S3=0,S4=0,S5=0,S6=0; //contact status
char ch;
char buffer[14];
void action(uint8 control_number)//read from the contact, called after interrupt
{
    Control_Reg_Write(control_number-1);
    for (;;)    
    {    
        ch=UART_GetChar();
        if (ch!=0)
        {
            if (ch=='#') //beginning byte for a sentence
            {
                buffer[0]=UART_GetChar();
                buffer[1]=UART_GetChar();
                buffer[2]=UART_GetChar();
                buffer[3]=UART_GetChar();
                buffer[4]=UART_GetChar();
                buffer[5]=UART_GetChar();
                buffer[6]=S1;
                buffer[7]=S2;
                buffer[8]=S3;
                buffer[9]=S4;
                buffer[10]=S5;
                buffer[11]=S6;
                buffer[12]=control_number;
                buffer[13]='\0';
                UART_computer_PutString(buffer);
                Control_Reg_Write(7);
                break;
            }    
        }    
    }    
}
CY_ISR(C1_Int_Handler)//interrupt for determining state of connection and serial read request
                      //upon change in contact pin logics 
{
    UART_computer_PutString("interrupt\n");
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
    UART_computer_PutString("interrupt\n");
    CyDelay(30);
    S2=contact2_Read();
    if (S2==1)
    {
        action(2);
    }
    contact2_ClearInterrupt();
}
CY_ISR(C3_Int_Handler)
{   
    UART_computer_PutString("interrupt\n");
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
    UART_computer_PutString("interrupt\n");
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
    UART_computer_PutString("interrupt\n");
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
    UART_computer_PutString("interrupt\n");
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
    CyGlobalIntEnable;
    UART_Start();
    UART_computer_Start();

    
    C1_Int_StartEx(C1_Int_Handler);
    C2_Int_StartEx(C2_Int_Handler);
    C3_Int_StartEx(C3_Int_Handler);
    C4_Int_StartEx(C4_Int_Handler);
    C5_Int_StartEx(C5_Int_Handler);
    C6_Int_StartEx(C6_Int_Handler);
    
    for(;;)
    {
        CyDelay(1000);
        UART_computer_PutString("asdfdsf\n");
    }    
}

/* [] END OF FILE */
