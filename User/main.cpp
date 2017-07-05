#include "main.h"
#include "usart_print.h"

/*********** Global Variables ************/
uint16_t timer;

int main(int argc, char* argv[])
{
	/******** Initial *********/
	GPIO_Configuration();
  RCC_Configuration();
  DMA_Configuration();
  ADC_Configuration();
  USART_Configuration();
  TIMER_Configuration();
	NVIC_Configuration();
	while (1)
	{
		U_Print(USART1, ADC_DualConvertedValueTab[0]);
		U_Print_Char(USART1, "\n");
		U_Print(USART1, ADC_DualConvertedValueTab[1]);
		U_Print_Char(USART1, "\n");
		U_Print(USART1, ADC_DualConvertedValueTab[2]);
		U_Print_Char(USART1, "\n");
		U_Print(USART1, ADC_DualConvertedValueTab[3]);
		U_Print_Char(USART1, "\n");
		U_Print_Char(USART1, " - ***\r\n");
		timer = 0;
		while (timer < 300) ;
		led_toggle();
	}
}

extern "C" void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
				led_toggle();
        /* If received 't', toggle LED and transmit 'T' */
        if((char)USART_ReceiveData(USART1) == 't')
        {
            led_toggle();
            USART_SendData(USART1, 'T');
            /* Wait until Tx data register is empty, not really 
             * required for this example but put in here anyway.
             */
            
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            {
            }
        }
    }
     
    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}   
/*------- Handler -------*/
		

extern "C" void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        //led_toggle();
    }
}

extern "C" void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				timer ++;
				//time_now++;
    }
}

extern "C" void EXTI0_IRQHandler()
{
		if(EXTI_GetITStatus(EXTI_Line0) != RESET)//Judge whether a line break
		{
			if (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_0))
			{
				//mode_flip_pitch = - mode_flip_pitch;
			}
			EXTI_ClearITPendingBit(EXTI_Line0);   //Remove LINE interrupt flag bit
			led_toggle();
		} 	    
} 

extern "C" void EXTI1_IRQHandler()
{
		if(EXTI_GetITStatus(EXTI_Line1) != RESET)//Judge whether a line break
		{
			if (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_1))
			{
				//mode_flip_pitch = - mode_flip_yaw;
			}
			EXTI_ClearITPendingBit(EXTI_Line1);   //Remove LINE interrupt flag bit
			led_toggle();
		} 	    
} 

extern "C" void EXTI2_IRQHandler()
{
		if(EXTI_GetITStatus(EXTI_Line2) != RESET)//Judge whether a line break
		{
			/*---- Send Signal clear to bluetooth ----*/
			EXTI_ClearITPendingBit(EXTI_Line2);   //Remove LINE interrupt flag bit
			led_toggle();
		} 	    
} 
