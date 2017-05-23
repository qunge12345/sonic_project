#include "Sonic.h"

void Sonic_Init(void)
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_USART1, ENABLE);	//GPIOA,B,Cʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4,ENABLE);//
	
  //USART2_TX   PA.2 			USART1_Tx PA.9		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_9; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART2_RX	  PA.3			USART1_Rx PA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_10;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART3_Tx	PB.10				USART4_Tx PC.10
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//USART3_Rx	PB.11				USART4_Rx PC.11
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//USART1-4 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//|USART2_IRQn|USART3_IRQn|UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
	NVIC_Init(&NVIC_InitStructure);
  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	//USART1
	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	//USART2
  USART_Init(USART2, &USART_InitStructure); //��ʼ������
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	//USART3
	USART_Init(USART3,&USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
	//UART4
	USART_Init(UART4,&USART_InitStructure);
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART4,ENABLE);
	USART_GetITStatus(USART1, USART_FLAG_TC);
	USART_GetITStatus(USART2, USART_FLAG_TC);
	USART_GetITStatus(USART3, USART_FLAG_TC);
	USART_GetITStatus(UART4, USART_FLAG_TC);
}


