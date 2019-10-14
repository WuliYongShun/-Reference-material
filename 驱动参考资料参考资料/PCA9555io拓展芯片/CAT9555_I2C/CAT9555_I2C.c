#include"CAT9555_I2C.h"
#include "delay.h"

/* 硬件IIC配置
  参数  从机地址 addr
*/
void ucb0_config(uchar sla)  
{  
  uchar i=0;
  P1SEL &= ~SCL;
  P1DIR |= SCL;  
  P1OUT |= SCL;  
                                 // 输出9个时钟以恢复I2C总线状态  
  for( i= 0; i <9 ; i++)  
  {  
    P1OUT |= SCL;  
    delay_US(80);
    P1OUT &= ~SCL;  
    delay_US(80);
  }  
  P1SEL |= SDA|SCL; 
  P1SEL2 |= SDA|SCL;
  //已经加了外部上拉，所以不用加内部上拉了
  
  //I2C模式设定
  UCB0CTL1 |= UCSWRST;           //USCI Software Reset 
  UCB0CTL0=UCMST|UCMODE_3|UCSYNC;//master mode ,i2c mode,同步Mode
  UCB0CTL1=UCSSEL_2|UCTR|UCSWRST;// Use SMCLK=16MHz,ransmit Select ,keep SW reset
  UCB0BR0=40;                    // fSCL = SMCLK/40 = 400kHz
  UCB0BR1=0;
  UCB0I2CSA=sla;          //硬件设置slave address
  UCB0I2COA=0x66;         //硬件设置own address
  UCB0CTL1 &=~UCSWRST;    // Clear SW reset  
}  

/*写字节 
  参数 命令和待发送数据 , 发送的数据长度 len
  返回  0 表示成功 1 表示失败
*/
uchar IIC_Writes( uchar *buf, uchar len)  
{  
  uchar i;
  while( UCB0CTL1& UCTXSTP );  
  UCB0CTL1 |= UCTR;                // 写模式  
  UCB0CTL1 |= UCTXSTT;             // 发送启动位  
  while(!(IFG2& UCB0TXIFG)) ; 
  UCB0TXBUF = buf[0];              // 发送字节地址    
  while(!(IFG2& UCB0TXIFG))  
  {  
    if( UCB0CTL1& UCTXNACK)      // 若无应答 UCNACKIFG=1  
    {  
      return 1;  
    }  
  }    
   
  for( i=0; i < len; i++)  
  {  
    UCB0TXBUF = buf[i+1];        // 发送寄存器内容 buffer[1] 是第一个数据
    while(!(IFG2& UCB0TXIFG));   // 等待UCTXIFG=1     
  }  
  UCB0CTL1 |= UCTXSTP;  
  while(UCB0CTL1& UCTXSTP);       // 等待发送完成  
   
  return 0;  
}  


/*读字节
  参数 命令和发送数据 , 读回的数据长度 len
  返回 1 失败 0 成功

*/
uchar IIC_Reads( uchar *buf, uchar len )  
{  
  uchar i;
  while( UCB0CTL1& UCTXSTP );  
  UCB0CTL1 |= UCTR;               // 写模式  
  UCB0CTL1 |= UCTXSTT;            // 发送启动位和写控制字节  
   
  UCB0TXBUF = buf[0];            // 发送字节地址  
                                 // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可  
  while(!(IFG2& UCB0TXIFG))  
  {  
    if( UCB0CTL1& UCTXNACK )      // 若无应答 UCNACKIFG=1  
    {  
      return 1;  
    }  
  }    
   
  UCB0CTL1 &= ~UCTR;               // 读模式  
  UCB0CTL1 |= UCTXSTT;             // 发送启动位和读控制字节  
   
  while(UCB0CTL1& UCTXSTT);       // 等待UCTXSTT=0  
  // 若无应答 UCNACKIFG = 1  
   
  for( i= 0; i< len -1 ; i++)  
  {  
    while(!(IFG2& UCB0RXIFG));   // 读取字节内容，不包括最后一个字节内容  
    buf[i]= UCB0RXBUF;           //buffer[1]接收第一个数据
  }  
  UCB0CTL1 |= UCTXSTP;             // 在接收最后一个字节之前发送停止位
  
  while(!(IFG2& UCB0RXIFG));       // 读取最后一个字节内容 
  buf[i+1]=UCB0RXBUF;              //接收最后一个字节
  
  while( UCB0CTL1& UCTXSTP );  
  return 0;  
}  

//  设置 P0 的输入方向  dir 方向命令
void IIC_P0DIR(uchar dir)
{
  uchar buffer[2];      //buffer[0]:寄存器地址(发送用)  buffer[1]:CAT9555_P0口方向配置（发送用）
  buffer[0]=0x06;       //CAT9555_P0口方向寄存器地址:   0x02
  buffer[1]=dir;
  IIC_Writes(buffer,1); //向buffer[0]地址write buffer[1]

}
//设置 P1的输入方向  dir 方向命令
void IIC_P1DIR(uchar dir)
{
  uchar buffer[2];      //buffer[0]:寄存器地址(发送用)  buffer[1]:CAT9555_P1口方向配置（发送用）
  buffer[0]=0x07;       //CAT9555_P1口方向寄存器地址:   0x02
  buffer[1]=dir;
  IIC_Writes(buffer,1); //向buffer[0]地址write buffer[1]
}

//P0输出数据 data
void IIC_P0OUT(uchar data)
{
  uchar buffer[2];      //buffer[0]:寄存器地址(发送用)  buffer[1]:发送到CAT9555_P0口（发送用）
  buffer[0]=0x02;       //CAT9555_P1口输出寄存器地址:   0x02
  buffer[1]=data;       
  IIC_Writes(buffer,1); //向buffer[0]地址write buffer[1]
}

//P1输出数据 data
void IIC_P1OUT(uchar data)
{
  uchar buffer[2];      //buffer[0]:寄存器地址(发送用)  buffer[1]:发送到CAT9555_P1口（发送用）
  buffer[0]=0x03;       //CAT9555_P0口输出寄存器地址:   0x03
  buffer[1]=data;       
  IIC_Writes(buffer,1); //向buffer[0]地址write buffer[1]
}

//返回 P0IN内的值
uchar IIC_P0IN()
{
  
  uchar buffer[2];      //buffer[0]:寄存器地址(发送用)  buffer[1]:CAT9555_P1口数据（接收用）
  buffer[0]=0x00;       //CAT9555_P0口输入寄存器地址: 0x00
  IIC_Reads(buffer,1);  //从buffer[0]地址read一个byte到buffer[1]
  return buffer[1];     //返回接收到的P0口数据
}

//返回 P1IN内的值
uchar IIC_P1IN()
{
  
  uchar buffer[2];      //buffer[0]:寄存器地址(发送用)  buffer[1]:CAT9555_P1口数据（接收用）
  buffer[0]=0x01;       //CAT9555_P1口输入寄存器地址: 0x01
  IIC_Reads(buffer,1);  //从buffer[0]地址read一个byte到buffer[1]
  return buffer[1];     //返回接收到的P1口数据
}

