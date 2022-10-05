#include "stm8l15x.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

void Config_HSI(void)
{
  CLK_DeInit();           //Khoi tao cac thanh ghi ve mac dinh
  CLK_HSEConfig(CLK_HSE_OFF);// Tat Thach Anh Ngaoi Tan So Cao
  CLK_LSEConfig(CLK_LSE_OFF);// Tat Thach Anh Ngoai Tan So Thap
  CLK_LSICmd(DISABLE);//Tat Thach Aanh Noi Tan So Thap
  CLK_HSICmd (ENABLE);      //Chon nguon thach anh noi HSI
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//Dung nguon thach anh noi cho SYS
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8); 
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
}

void Delay_ms(uint16_t time_delay)      // Ham delay ms
{
  TIM4_TimeBaseInit(TIM4_Prescaler_64, 250);		    // set frequence timer 4 la 16*10^6/64/250 = 1000Hz
  TIM4_SetCounter(0);					    // set counter value = 0    //set counter ve gia tri ban dau =0	
  TIM4_ClearFlag(TIM4_FLAG_Update);                         // clear flag update timer4(when over timer = 1) clear 0
  TIM4_Cmd(ENABLE);					    // enable timer		// enable timer hoat dong
  while(time_delay--)					   // tan so la 1000Hz -> khi lap lai 1000 lan la 1Hz = 1s
  {
    while(TIM4_GetFlagStatus(TIM4_FLAG_Update)== 0) ;	   // watting over timer exit whiles
    TIM4_ClearFlag(TIM4_FLAG_Update);			   // clear timer for next counter    									
  }
  TIM4_Cmd(DISABLE);					// sau khi delay xong, disable timer.
}
void UART_CONFIG(void)
{
  USART_DeInit(USART1);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Rx);  
  USART_ITConfig(USART1, USART_IT_RXNE,ENABLE );        // Cho phep ngat nhan
  USART_ClockInit(USART1, USART_Clock_Disable, USART_CPOL_Low, USART_CPHA_1Edge, USART_LastBit_Disable);
  USART_Cmd(USART1,ENABLE);
  enableInterrupts();  
}
// Gui chuoi
void Send_Cmd(char *char_value)
{                        
  disableInterrupts();
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Tx);
  USART_ITConfig(USART1, USART_IT_RXNE,DISABLE );        // Cho phep ngat nhan
  for(int i=0;i<strlen(char_value);i++)
  {
     USART_SendData8(USART1,char_value[i]);
     while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);            // Wait for transmition to complete
  }
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Rx);  
  USART_ITConfig(USART1, USART_IT_RXNE,ENABLE );        // Cho phep ngat nhan
  enableInterrupts();
}

void Power_save_mode()
{

}



