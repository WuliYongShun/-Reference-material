#include "jdq.h"

//////////////////////////////////////////////////////////////////////////////////	 
	  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PA2\PC4\PB0\PB11Ϊ�����.��ʹ���������ڵ�ʱ��		    
//�̵��� IO��ʼ��
void JDQ_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PB,PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //FM1K-->PA.2 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.2
 GPIO_ResetBits(GPIOA,GPIO_Pin_2);						 //PA.8 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //FM1G-->PC.4 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOC,GPIO_Pin_4); 						 //PC.4 ����� 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //FM2K-->PB.0 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.0
 GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 //PB.0 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //FM2G-->PB.11 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOB,GPIO_Pin_11); 						 //PB.11 �����
 

//����������
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//JLINK�ϵĽ�
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //BEEP-->PA.15 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������  GPIO_Mode_Out_OD ��GPIO_Mode_Out_PP , GPIO_Mode_AF_OD ,  GPIO_Mode_AF_PP 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				   //������� ��IO���ٶ�Ϊ50MHz
// GPIO_ResetBits(GPIOA,GPIO_Pin_15); 						     //PA.15 ����� 
// GPIO_SetBits(GPIOA,GPIO_Pin_15); 						     //PA.15 �����
}
 
/**
  * @brief  ���뿪�س�ʼ��
  * @param  None
  * @retval None
  */
void Button_Config(void)
{
  //����һ��GPIO�ṹ��
  GPIO_InitTypeDef  GPIO_InitStructure; 

  //��ʼ��GPIOBʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC ,ENABLE);
  //GPIOB
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void Optical_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PB,PC�˿�ʱ��
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //FM1KD-->PA.3 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.3
 GPIO_ResetBits(GPIOA,GPIO_Pin_3);						 //PA.8 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //FM1GD-->PC.5 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //�����趨������ʼ��GPIOC.5
 GPIO_ResetBits(GPIOC,GPIO_Pin_5); 						 //PC.5 ����� 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //FM2KD-->PB.1 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.1
 GPIO_ResetBits(GPIOB,GPIO_Pin_1);						 //PB.1 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	    		 //FM2GD-->PB.10 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //�����趨������ʼ��GPIOB.10
 GPIO_ResetBits(GPIOB,GPIO_Pin_10); 						 //PB.10 �����
 
 }
