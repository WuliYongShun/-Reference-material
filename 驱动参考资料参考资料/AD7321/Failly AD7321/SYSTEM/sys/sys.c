#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK  STM32������
//ϵͳ�жϷ������û�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/10
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************  
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
//void Stm32_Clock_Init(u8 PLL)
//{
//		 unsigned char temp=0;  
//		 MYRCC_DeInit();        //
//			RCC->CR|=0x00010000;  
//		 while(!(RCC->CR>>17));
//		 RCC->CFGR=0X00000400;
//		 PLL-=2;
//		 RCC->CFGR|=PLL<<18;  
//		 RCC->CFGR|=1<<16;
//		 FLASH->ACR|=0x32;        //
//		 RCC->CR|=0x01000000;
//		 while(!(RCC->CR>>25));
//		 RCC->CFGR|=0x00000002;
//		 while(temp!=0x02)    
//		 {  
//			temp=RCC->CFGR>>2;
//			temp&=0x03;
//		 }    
//}
// 
 