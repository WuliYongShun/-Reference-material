#include "jdq.h"

//////////////////////////////////////////////////////////////////////////////////	 
	  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PA2\PC4\PB0\PB11为输出口.并使能这两个口的时钟		    
//继电器 IO初始化
void JDQ_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PB,PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //FM1K-->PA.2 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.2
 GPIO_ResetBits(GPIOA,GPIO_Pin_2);						 //PA.8 输出低

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //FM1G-->PC.4 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_ResetBits(GPIOC,GPIO_Pin_4); 						 //PC.4 输出低 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //FM2K-->PB.0 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0
 GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 //PB.0 输出低

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //FM2G-->PB.11 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_ResetBits(GPIOB,GPIO_Pin_11); 						 //PB.11 输出低
 

//蜂鸣器报警
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//JLINK上的脚
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //BEEP-->PA.15 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出  GPIO_Mode_Out_OD ，GPIO_Mode_Out_PP , GPIO_Mode_AF_OD ,  GPIO_Mode_AF_PP 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				   //推挽输出 ，IO口速度为50MHz
// GPIO_ResetBits(GPIOA,GPIO_Pin_15); 						     //PA.15 输出低 
// GPIO_SetBits(GPIOA,GPIO_Pin_15); 						     //PA.15 输出高
}
 
/**
  * @brief  拨码开关初始化
  * @param  None
  * @retval None
  */
void Button_Config(void)
{
  //定义一个GPIO结构体
  GPIO_InitTypeDef  GPIO_InitStructure; 

  //初始化GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC ,ENABLE);
  //GPIOB
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void Optical_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PB,PC端口时钟
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //FM1KD-->PA.3 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.3
 GPIO_ResetBits(GPIOA,GPIO_Pin_3);						 //PA.8 输出低

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //FM1GD-->PC.5 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //根据设定参数初始化GPIOC.5
 GPIO_ResetBits(GPIOC,GPIO_Pin_5); 						 //PC.5 输出低 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //FM2KD-->PB.1 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.1
 GPIO_ResetBits(GPIOB,GPIO_Pin_1);						 //PB.1 输出低

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	    		 //FM2GD-->PB.10 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //根据设定参数初始化GPIOB.10
 GPIO_ResetBits(GPIOB,GPIO_Pin_10); 						 //PB.10 输出低
 
 }
