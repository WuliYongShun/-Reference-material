#include"CAT9555_I2C.h"
#include "delay.h"

/* Ӳ��IIC����
  ����  �ӻ���ַ addr
*/
void ucb0_config(uchar sla)  
{  
  uchar i=0;
  P1SEL &= ~SCL;
  P1DIR |= SCL;  
  P1OUT |= SCL;  
                                 // ���9��ʱ���Իָ�I2C����״̬  
  for( i= 0; i <9 ; i++)  
  {  
    P1OUT |= SCL;  
    delay_US(80);
    P1OUT &= ~SCL;  
    delay_US(80);
  }  
  P1SEL |= SDA|SCL; 
  P1SEL2 |= SDA|SCL;
  //�Ѿ������ⲿ���������Բ��ü��ڲ�������
  
  //I2Cģʽ�趨
  UCB0CTL1 |= UCSWRST;           //USCI Software Reset 
  UCB0CTL0=UCMST|UCMODE_3|UCSYNC;//master mode ,i2c mode,ͬ��Mode
  UCB0CTL1=UCSSEL_2|UCTR|UCSWRST;// Use SMCLK=16MHz,ransmit Select ,keep SW reset
  UCB0BR0=40;                    // fSCL = SMCLK/40 = 400kHz
  UCB0BR1=0;
  UCB0I2CSA=sla;          //Ӳ������slave address
  UCB0I2COA=0x66;         //Ӳ������own address
  UCB0CTL1 &=~UCSWRST;    // Clear SW reset  
}  

/*д�ֽ� 
  ���� ����ʹ��������� , ���͵����ݳ��� len
  ����  0 ��ʾ�ɹ� 1 ��ʾʧ��
*/
uchar IIC_Writes( uchar *buf, uchar len)  
{  
  uchar i;
  while( UCB0CTL1& UCTXSTP );  
  UCB0CTL1 |= UCTR;                // дģʽ  
  UCB0CTL1 |= UCTXSTT;             // ��������λ  
  while(!(IFG2& UCB0TXIFG)) ; 
  UCB0TXBUF = buf[0];              // �����ֽڵ�ַ    
  while(!(IFG2& UCB0TXIFG))  
  {  
    if( UCB0CTL1& UCTXNACK)      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 1;  
    }  
  }    
   
  for( i=0; i < len; i++)  
  {  
    UCB0TXBUF = buf[i+1];        // ���ͼĴ������� buffer[1] �ǵ�һ������
    while(!(IFG2& UCB0TXIFG));   // �ȴ�UCTXIFG=1     
  }  
  UCB0CTL1 |= UCTXSTP;  
  while(UCB0CTL1& UCTXSTP);       // �ȴ��������  
   
  return 0;  
}  


/*���ֽ�
  ���� ����ͷ������� , ���ص����ݳ��� len
  ���� 1 ʧ�� 0 �ɹ�

*/
uchar IIC_Reads( uchar *buf, uchar len )  
{  
  uchar i;
  while( UCB0CTL1& UCTXSTP );  
  UCB0CTL1 |= UCTR;               // дģʽ  
  UCB0CTL1 |= UCTXSTT;            // ��������λ��д�����ֽ�  
   
  UCB0TXBUF = buf[0];            // �����ֽڵ�ַ  
                                 // �ȴ�UCTXIFG=1��UCTXSTT=0 ͬʱ�仯�ȴ�һ����־λ����  
  while(!(IFG2& UCB0TXIFG))  
  {  
    if( UCB0CTL1& UCTXNACK )      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 1;  
    }  
  }    
   
  UCB0CTL1 &= ~UCTR;               // ��ģʽ  
  UCB0CTL1 |= UCTXSTT;             // ��������λ�Ͷ������ֽ�  
   
  while(UCB0CTL1& UCTXSTT);       // �ȴ�UCTXSTT=0  
  // ����Ӧ�� UCNACKIFG = 1  
   
  for( i= 0; i< len -1 ; i++)  
  {  
    while(!(IFG2& UCB0RXIFG));   // ��ȡ�ֽ����ݣ����������һ���ֽ�����  
    buf[i]= UCB0RXBUF;           //buffer[1]���յ�һ������
  }  
  UCB0CTL1 |= UCTXSTP;             // �ڽ������һ���ֽ�֮ǰ����ֹͣλ
  
  while(!(IFG2& UCB0RXIFG));       // ��ȡ���һ���ֽ����� 
  buf[i+1]=UCB0RXBUF;              //�������һ���ֽ�
  
  while( UCB0CTL1& UCTXSTP );  
  return 0;  
}  

//  ���� P0 �����뷽��  dir ��������
void IIC_P0DIR(uchar dir)
{
  uchar buffer[2];      //buffer[0]:�Ĵ�����ַ(������)  buffer[1]:CAT9555_P0�ڷ������ã������ã�
  buffer[0]=0x06;       //CAT9555_P0�ڷ���Ĵ�����ַ:   0x02
  buffer[1]=dir;
  IIC_Writes(buffer,1); //��buffer[0]��ַwrite buffer[1]

}
//���� P1�����뷽��  dir ��������
void IIC_P1DIR(uchar dir)
{
  uchar buffer[2];      //buffer[0]:�Ĵ�����ַ(������)  buffer[1]:CAT9555_P1�ڷ������ã������ã�
  buffer[0]=0x07;       //CAT9555_P1�ڷ���Ĵ�����ַ:   0x02
  buffer[1]=dir;
  IIC_Writes(buffer,1); //��buffer[0]��ַwrite buffer[1]
}

//P0������� data
void IIC_P0OUT(uchar data)
{
  uchar buffer[2];      //buffer[0]:�Ĵ�����ַ(������)  buffer[1]:���͵�CAT9555_P0�ڣ������ã�
  buffer[0]=0x02;       //CAT9555_P1������Ĵ�����ַ:   0x02
  buffer[1]=data;       
  IIC_Writes(buffer,1); //��buffer[0]��ַwrite buffer[1]
}

//P1������� data
void IIC_P1OUT(uchar data)
{
  uchar buffer[2];      //buffer[0]:�Ĵ�����ַ(������)  buffer[1]:���͵�CAT9555_P1�ڣ������ã�
  buffer[0]=0x03;       //CAT9555_P0������Ĵ�����ַ:   0x03
  buffer[1]=data;       
  IIC_Writes(buffer,1); //��buffer[0]��ַwrite buffer[1]
}

//���� P0IN�ڵ�ֵ
uchar IIC_P0IN()
{
  
  uchar buffer[2];      //buffer[0]:�Ĵ�����ַ(������)  buffer[1]:CAT9555_P1�����ݣ������ã�
  buffer[0]=0x00;       //CAT9555_P0������Ĵ�����ַ: 0x00
  IIC_Reads(buffer,1);  //��buffer[0]��ַreadһ��byte��buffer[1]
  return buffer[1];     //���ؽ��յ���P0������
}

//���� P1IN�ڵ�ֵ
uchar IIC_P1IN()
{
  
  uchar buffer[2];      //buffer[0]:�Ĵ�����ַ(������)  buffer[1]:CAT9555_P1�����ݣ������ã�
  buffer[0]=0x01;       //CAT9555_P1������Ĵ�����ַ: 0x01
  IIC_Reads(buffer,1);  //��buffer[0]��ַreadһ��byte��buffer[1]
  return buffer[1];     //���ؽ��յ���P1������
}

