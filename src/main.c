
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
USART_InitTypeDef USART_InitStruct;


void delay(uint32_t time);

void GPIO_Config();
void USART_Config();
void ADC_Config();
void USART_Puts(USART_TypeDef* USARTx, volatile char*s);

uint16_t readADC_Voltage();
uint16_t readADC_Current();
uint16_t readADC_Termistor();
double ConversionToTemperature(int analogValue);

char buffer[100];
double   reel_voltage;
double	 reel_current;
int		 reel_temperature;

int main(void)
{
	GPIO_Config();
	ADC_Config();
	USART_Config();
  while (1)
  {

	  reel_voltage = ((double)10/4095)*readADC_Voltage();                //PA0- 0-10V giriþ deðerini okur.

	  reel_current= (((double)3.3/4095)*readADC_Current())/165;             // PA1- 165k ohm direncin üzerinden geçen akým

	  reel_temperature= ConversionToTemperature(readADC_Termistor());       //PA5- adc den sýcaklýða dönüþtürme


	   sprintf(buffer, "Voltage : %.3f Current : %.3f Temperature %d\n", reel_voltage, reel_current, reel_temperature);
	   USART_Puts(USART2,buffer);             //PA2

	   delay(16800000);

  }
}

void delay(uint32_t time)
{
	while(time--);
}


void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin= GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, & GPIO_InitStruct);



	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;

	GPIO_Init(GPIOA,&GPIO_InitStruct);

}
void USART_Config(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitStruct.USART_BaudRate=9600;
	USART_InitStruct.USART_Mode=USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No ;
	USART_InitStruct.USART_StopBits=USART_StopBits_1 ;
	USART_InitStruct.USART_WordLength= USART_WordLength_8b;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

	USART_Init(USART2,&USART_InitStruct);

	USART_Cmd(USART2,ENABLE);
}

void USART_Puts(USART_TypeDef* USARTx, volatile char*s){

	while(*s){

		while(!(USARTx->SR & 0x00000040));

			USART_SendData(USARTx,*s);
			*s++;


	}

}

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4 ;

	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;

	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_Cmd(ADC1, ENABLE);
}
uint16_t readADC_Voltage()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0,1,ADC_SampleTime_56Cycles);


	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);

	return ADC_GetConversionValue(ADC1);
}

uint16_t readADC_Current()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1,1,ADC_SampleTime_56Cycles);


	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);

	return ADC_GetConversionValue(ADC1);
}

uint16_t readADC_Termistor()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5,1,ADC_SampleTime_56Cycles);


	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);

	return ADC_GetConversionValue(ADC1);
}

double ConversionToTemperature(int analogValue)
{
		double lnr=0,kelvin=0,temperature=0;
		lnr= log(((40950000 / analogValue) - 10000));
		kelvin = 1 / (0.006810807013 + (-0.0009491784599*lnr) + (0.0000002913204729 * lnr * lnr * lnr));
		temperature = kelvin - 273.15;
		return temperature;
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
