/* Includes ------------------------------------------------------------------*/
#include "My_function.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
char Rec_data[100];
int Rec_count=0;
int Rec_finsh;
int Test_AT=0;
int Connect=0;
char Send_data[10];
int Value=10;
int Water_Status=0;
int qos=0;
int retain=0;


//Nhan dulieu
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {
      Rec_data[Rec_count] = USART_ReceiveData8(USART1);
      USART_ClearITPendingBit(USART1,USART_IT_RXNE);
      if( Rec_data[Rec_count] == 10)
      { 
          Rec_finsh = 1;
          Rec_count=0;
          
      }
      else
        Rec_count++;
    }
  USART_ClearFlag(USART1,USART_FLAG_RXNE);
  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
void Send_AT_Check()
{
    if (Test_AT==0)
    {    
      Delay_ms(20);
      Rec_finsh = 0;
      Send_Cmd("AT+CSGS?\r\n");
      Delay_ms(20);      
      if(Rec_finsh == 1)
        {  
          GPIO_SetBits(GPIOE,GPIO_Pin_7);
          if (Rec_data[7]=='1')
            {
              GPIO_SetBits(GPIOD,GPIO_Pin_6);          
              Test_AT=1;
              Rec_finsh = 0;   
            }
        }
      }
 }

void Send_ConnectMQTT()
{
      if (Test_AT==1)
    {
      Delay_ms(5000);
      Send_Cmd("ATE0\r");
      Delay_ms(5000);
      Send_Cmd("AT+CNMP=13\r");     
      Delay_ms(2000);
      Send_Cmd("AT+CSQ\r");
      Delay_ms(5000);
      Send_Cmd("AT+CPSI?\r");
      Delay_ms(2000);
      Send_Cmd("AT+SMCONF=\"URL\",\"broker.hivemq.com\",1883\r");
      Delay_ms(2000);
      Send_Cmd("AT+SMCONF=\"USERNAME\",\"CHUNGEPI\"\r");
      Delay_ms(2000);
      Send_Cmd("AT+SMCONF=\"PASSWORD\",\"123456a@\"\r");
      Delay_ms(2000);
      Send_Cmd("AT+SMCONF=\"CLIENTID\",\"CNHN\"\r");
      Delay_ms(5000);
      Send_Cmd("AT+SMCONF=\"KEEPTIME\",120\r");
      Delay_ms(5000);
      Send_Cmd("AT+SMCONF=\"CLEANSS\",1\r"); 
      Delay_ms(2000);
      Send_Cmd("AT+CNACT?\r");
      Delay_ms(2000);
      Send_Cmd("AT+CNACT=0,1\r");
      Delay_ms(2000);
      Send_Cmd("AT+SMCONN\r");
      Delay_ms(10000);
      if(Rec_finsh == 1)
        {  
          if (Rec_data[0]=='O' && Rec_data[1]=='K')
            {
              GPIO_ResetBits(GPIOD,GPIO_Pin_6);          
              Test_AT=2;
              Rec_finsh = 0;   
            }
          else
            {         
              Test_AT=0;
              Rec_finsh = 0;
            }
        }
      }
     
}
void Calc()
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0 && Water_Status==0)
  {
    Value++;
    Delay_ms(10);
    Water_Status=1;
  }
   if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)!=0 && Water_Status==1)
  {
    Delay_ms(10);
    Water_Status=0;
  }
}
void Send_PublicMQTT()
{
      if (Test_AT==2)
    {   
      sprintf(Send_data,"{\"key\":%d\r\n",Value);
      Send_Cmd("AT+SMPUB=\"tantan3\",string(11),0,0\r");
      Delay_ms(2000);
    //  Send_Cmd("{\"key\":");
     
      //sprintf(Send_data,"%d\r\n",Value);
      Send_Cmd(Send_data);
      //Send_Cmd("56789\r"); 
      Send_Cmd("}\r");
      Delay_ms(5000);

     // Send_Cmd("{\"key\":1}\r");
     // Delay_ms(2000);
    }
}
void Hengio()
{  GPIO_SetBits(GPIOB,GPIO_Pin_7);
  Delay_ms(2000);
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);
  Delay_ms(2000);
}
void Disconnect()
{ 
  Send_Cmd("AT+SMDISC\r");
  Delay_ms(2000);
  Send_Cmd("AT+CNACT=0,0");
  Delay_ms(2000);
}

  
  
void main(void)
{
  Config_HSI();
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOD);
  GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);
  //GPIO_Init(GPIOE,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
  //GPIO_Init(GPIOC,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
  UART_CONFIG();

  //GPIO_SetBits(GPIOB,GPIO_Pin_7);
  //Delay_ms(2000);
  //GPIO_ResetBits(GPIOB,GPIO_Pin_7);
 // Delay_ms(2000);
  while (1)
  {
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
  Delay_ms(500);
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);
  Delay_ms(500);
    for(int i =0;i<3;i++)
    {
    //Hengio();

    Send_AT_Check();
  //  Send_ConnectMQTT();
    Delay_ms(500);
      Send_Cmd("ATE0\r");
      Delay_ms(10);
      Send_Cmd("AT+CNMP=13\r");     
      Delay_ms(10);
      Send_Cmd("AT+CSQ\r");
      Delay_ms(10);
      Send_Cmd("AT+CPSI?\r");
      Delay_ms(10);
      Send_Cmd("AT+SMCONF=\"URL\",\"137.59.47.201\",1883\r");
      Delay_ms(10);
      Send_Cmd("AT+SMCONF=\"USERNAME\",\"CHUNGEPI\"\r");
      Delay_ms(10);
      Send_Cmd("AT+SMCONF=\"PASSWORD\",\"123456a@\"\r");
      Delay_ms(10);
      Send_Cmd("AT+SMCONF=\"CLIENTID\",\"Dh4\"\r");
      Delay_ms(10);
      Send_Cmd("AT+SMCONF=\"KEEPTIME\",120\r");
      Delay_ms(100);
      Send_Cmd("AT+SMCONF=\"CLEANSS\",1\r"); 
      Delay_ms(100);
      Send_Cmd("AT+CNACT?\r");
      Delay_ms(100);
      Send_Cmd("AT+CNACT=0,1\r");
      Delay_ms(3000);
      Send_Cmd("AT+SMCONN\r");
      Delay_ms(200);

    Calc();
   
   
      Send_PublicMQTT();

      sprintf(Send_data,"{\"key\":%d}\r\n",Value);

      Send_Cmd("AT+SMPUB=\"v1/devices/me/telemetry\",11,0,0\r");
      Delay_ms(100);
    //  Send_Cmd("{\"key\":");
     
       //sprintf(Send_data,"%d\r\n",Value);
      Send_Cmd(Send_data);
      //Send_Cmd("56789\r"); 
      //Send_Cmd("}\r");
      Delay_ms(200);
     // Send_Cmd("AT+SMDISC\r");
     // Delay_ms(3000);
     // Send_Cmd("AT+CNACT=0,0\r");
      //Delay_ms(3000);
    }
      Send_Cmd("AT+CPOWD=1\r");
    //for(int j =0;j<72;j++)
   // {
     Delay_ms(30000); //6h=12*30000(5phut)
   // }

  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

