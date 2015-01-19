//-----------------------------------------------------------------
//文件名称：i2c.c
//功能概要：模拟i2c底层驱动程序文件
//版本更新：2013-12-15
//
//I2C通信协议：（除起始信号，停止信号外，SDA在SCL_H时需维持不变，只允许在SCL_L时跳变。所以发送数据时，SDA应该在SCL_L时改变状态，接收数据时，应该在SCL_H时读取SDA有效状态）
//   起始信号：SCL_H时，SDA出现下降沿。
//   停止信号：SCL_H时，SDA出现上升沿。
//   应答信号：原则：I2C每传输1个字节(先传MSB,再传LSB)后都要跟一个应答位的传输！应答位SCL始终由主机产生，而SDA则由接收方产生。
//             应答信号协议：第9个SCL_H时检查SDA：SDA_L--ack;SDA_H--no_ack。发送方接收到了no_ack时会终止发送。
//   基本时序：写：start>slave_address_w>A>data>A...>data>NA>stop
//             读：start>slave_address_r>A>DATA>a...>DATA>na>stop  (小写：数据由主机产生；大写：数据由从机产生)
//-----------------------------------------------------------------
#include "i2c.h"
#include "Delay.h"
//------------------------------ 内部函数 ------------------------------//

//条件编译

//----------------------------------------------------------------------//
#if defined MCS_51  //定义了MCS51芯片（不用设置 SDA 输入输出方向）
//----------------------------------------------------------------------//

//-----------------------------------------------------------------
//函数名称：I2C_start
//功能概要：启动总线,SCL_H时，SDA出现下降沿。
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
static void I2C_start(void)                         
{
    SDA_H;          
    delay_1us();
    SCL_H;
    delay_5us();   
    SDA_L;          
    delay_5us();          
    SCL_L;          
    delay_2us();
}

//-----------------------------------------------------------------
//函数名称：I2C_stop
//功能概要：结束总线,SCL_H时，SDA出现上升沿。
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
static void I2C_stop(void)   
{
    SDA_L;         
    delay_1us();   
    SCL_H;         
    delay_5us();
    SDA_H;         
    delay_4us();
}


//-----------------------------------------------------------------
//函数名称：I2C_ack
//功能概要：主机发送应答位，SCL_H时，保持SDA_L状态
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
static void I2C_ack(void)     
{ 
    SDA_L;     
    delay_1us();      
    SCL_H;
    delay_5us(); 
    SCL_L;              
    delay_2us();    
}


//-----------------------------------------------------------------
//函数名称：I2C_no_ack
//功能概要：主机发送非应位，SCL_H时，保持SDA_H状态
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
static void I2C_no_ack(void)   
{ 
    SDA_H;
    delay_1us();     
    SCL_H;
    delay_5us(); 
    SCL_L;               
    delay_2us();   
}

//-----------------------------------------------------------------
//函数名称：I2C_check_ack
//功能概要：主机接收从机应答位，SCL_H时读取SDA，如果SDA_L则接收到应答位，否则接收到非应答位。
//          如果无应答，则主机要结束I2C总线
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
static void I2C_check_ack(void)
{    
    delay_3us();
    SCL_L;
    delay_3us();
	
    SDA_H;
    delay_3us();
    SCL_H;
	delay_5us(); 	
	
	if(SDA==1)
    {   
        I2C_stop();  //SDA!=0，未接收到应答位，结束总线
    }  
    else  
    {
        SCL_L;       //SDA==0，接收到应答位，拉低时钟线
    }   	 
}

//-----------------------------------------------------------------
//函数名称：I2C_send_one_char
//功能概要：发送一个字节。
//          将数据c发送出去,可以是地址,也可以是数据,发送完成后需检查应答位
//函数返回：void
//参数说明：c :发送字节
//-----------------------------------------------------------------
static void I2C_send_one_char(uint8_t c)
{
    uint8_t i;

    for (i=0; i<8; i++)         //8位计数器
    {
        if(c&0x80)
          SDA_H;                
        else
          SDA_L;                //送数据口
        
        c <<= 1;                //移出数据的最高位
        
        SCL_H;                  
        delay_3us();
        SCL_L;                  
        delay_3us();            //产生SCL脉冲，在SCL_L时改变SDA状态！
    } 
}

//-----------------------------------------------------------------
//函数名称：I2C_recv_one_char
//功能概要：接收一个字节。
//          用来接收从器件传来的一个字节。接收完成后需检查应答位
//函数返回：无
//参数说明：c :接收字节存放的地址
//-----------------------------------------------------------------
static void I2C_recv_one_char(uint8_t *c)
{
    uint8_t i;
	uint8_t retc = 0;

    SDA_H;             

    for(i=0;i<8;i++)
    {
         delay_1us();          
         SCL_L;       
         delay_5us();
         SCL_H;       
         delay_2us();     //产生SCL脉冲，再SCL_H时读取SDA状态！
		
         retc=retc<<1;
         if(SDA==1)
         retc=retc+1;     //读数据位,接收的数据位放入retc中
         delay_2us(); 
    } 
    SCL_L;

    *c = retc;	
}

//----------------------------------------------------------------------//
#elif defined XS_128 || defined STM32_F4XX || defined TM4C123GH6PM //定义了XS128芯片（需要设置 SDA 输入输出方向）
//----------------------------------------------------------------------//

//-----------------------------------------------------------------
//函数名称：I2C_start
//功能概要：启动总线,SCL_H时，SDA出现下降沿。
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
 void I2C_start(void)                     
{
    SDA_O;

    SDA_H;          
    delay_1us();
    SCL_H;
    delay_5us();    
    SDA_L;          
    delay_5us();     
    SCL_L;         
    delay_2us();
}

//-----------------------------------------------------------------
//函数名称：I2C_stop
//功能概要：结束总线,SCL_H时，SDA出现上升沿。
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
 void I2C_stop(void)  
{
    SDA_O;

    SDA_L;         
    delay_1us();   
    SCL_H;         
    delay_5us();
    SDA_H;
    delay_4us();
}


//-----------------------------------------------------------------
//函数名称：I2C_ack
//功能概要：主机发送应答位，SCL_H时，保持SDA_L状态
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
 void I2C_ack(void)     
{ 
    SDA_O;

    SDA_L;     
    delay_1us();      
    SCL_H;
    delay_5us(); 
    SCL_L;               
    delay_2us();    
}

//-----------------------------------------------------------------
//函数名称：I2C_no_ack
//功能概要：主机发送非应位，SCL_H时，保持SDA_H状态
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
 void I2C_no_ack(void)   
{ 
    SDA_O;

    SDA_H;
    delay_1us();     
    SCL_H;
    delay_5us(); 
    SCL_L;                
    delay_2us();   
}

//-----------------------------------------------------------------
//函数名称：I2C_check_ack
//功能概要：主机接收从机应答位，SCL_H时读取SDA，如果SDA_L则接收到应答位，否则接收到非应答位。
//          如果无应答，则主机要结束I2C总线
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
 void I2C_check_ack(void)
{   
    SDA_O;
    
    delay_3us();
    SCL_L;
    delay_3us();
    SDA_I;           //设置为输入模式
    
    SDA_H;
    delay_3us();
    SCL_H;
    delay_5us();
    
    if(SDA==1)
    {   
        I2C_stop();  //SDA!=0，未接收到应答位，结束总线
    }  
    else  
    {
        SCL_L;       //SDA==0，接收到应答位，拉低时钟线
    }   
}

//-----------------------------------------------------------------
//函数名称：I2C_send_one_char
//功能概要：发送一个字节。
//          将数据c发送出去,可以是地址,也可以是数据,发送完成后需检查应答位
//函数返回：void
//参数说明：c :发送字节
//-----------------------------------------------------------------
 void I2C_send_one_char(uint8_t c)
{
    uint8_t i;
    
    SDA_O;
    
    for (i=0; i<8; i++)        //8位计数器
    {
        if(c&0x80)
          SDA_H;               
        else
          SDA_L;               //送数据口
        
        c <<= 1;               //移出数据的最高位
        
        SCL_H;                
        delay_3us();
        SCL_L;                 //产生SCL脉冲，在SCL_L时改变SDA状态！
        delay_3us();
    } 
}

//-----------------------------------------------------------------
//函数名称：I2C_recv_one_char
//功能概要：接收一个字节。
//          用来接收从器件传来的一个字节。接收完成后需检查应答位
//函数返回：无
//参数说明：c :接收字节存放的地址
//-----------------------------------------------------------------
 void I2C_recv_one_char(uint8_t *c)
{
    uint8_t i;
	uint8_t retc = 0;

    SDA_O;

    SDA_H;             

    SDA_I;            

    for(i=0;i<8;i++)
    {
         delay_1us();          
         SCL_L;       
         delay_5us();
         SCL_H;       
         delay_2us();		//产生SCL脉冲，再SCL_H时读取SDA状态！
		
         retc=retc<<1;
         if(SDA==1)
         retc=retc+1;       //读数据位,接收的数据位放入retc中
         delay_2us(); 
    } 
    SCL_L; 
	
	*c = retc;
}

#endif


//------------------------------ 外部接口函数 ------------------------------//

//-----------------------------------------------------------------
//函数名称：i2c_init
//功能概要：i2c初始化，初始化模拟i2c的IO口
//函数返回：void
//参数说明：无
//-----------------------------------------------------------------
void i2c_init(void)
{
	#ifdef MCS_51
	
    #elif defined XS_128
	
        DDRH_DDRH0 = 1;
        DDRH_DDRH1 = 1;
	
    #elif defined STM32_F4XX
		#if 1 //X型传感器(PD2,PD3)
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //使能时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3;  //模拟i2c用PD2、PD3口！
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //初始化为输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         //开漏输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //端口速率100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无上下拉
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		#else //十字型传感器(PB11,PB13)
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |GPIO_Pin_13;//模拟i2c用PB11、PB13口！
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //初始化为输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         //开漏输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //端口速率100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无上下拉
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		#endif

		#elif defined TM4C123GH6PM_1

		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);		//使能PORTE
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);//模拟i2c用PE4、PE5口,初始化为输出模式

    #endif	
}

//-----------------------------------------------------------------
//函数名称：I2C_send_str
//功能概要：向有寄存器地址器件发送一个字符串。               
//          从启动总线到发送地址，寄存器地址,发送数据，结束总线的全过程。
//          从机地址slave，寄存器地址reg,发送的内容是s指向的内容，发送num个字节。
//函数返回：void
//参数说明：slave:从机地址
//          reg  :寄存器地址
//          s    :发送字符串首地址
//          num  :字节个数
//-----------------------------------------------------------------
/*
void I2C_send_str(uint8_t slave,uint8_t reg,uint8_t *s,uint8_t num)
{
   uint8_t i;
   
   for(i=0;i<num;i++)
   { 
     I2C_start();                   //启动总线
     
     I2C_send_one_char(slave);      //发送器件地址(写)
     I2C_check_ack();
     I2C_send_one_char(reg);        //发送器件子地址
     I2C_check_ack();
     I2C_send_one_char(*s);         //发送数据 
     I2C_check_ack();
        
     I2C_stop();                    //结束总线  
     
	   delay_ms(1);                 //必须延时等待芯片内部自动处理数据完毕  
	   s++;
	   reg++;                       //寄存器地址无自增功能
   }
} */

void I2C_send_str(uint8_t slave,uint8_t reg,uint8_t *s,uint8_t num)
{
    int i;

    I2C_start();                    //起始信号

    I2C_send_one_char(slave);       //发送设备地址(写)
    I2C_check_ack();
    I2C_send_one_char(reg);         //内部寄存器地址
    I2C_check_ack();

    for (i=0;i<num;i++)
    {  
     I2C_send_one_char(s[i]);       //内部寄存器数据
     I2C_check_ack();               //寄存器地址有自增功能
    }

    I2C_stop();                     //发送停止信号
}  

//-----------------------------------------------------------------
//函数名称：I2C_recv_str
//功能概要：向有寄存器地址器件接收一个字符串。         
//          从启动总线到发送地址，寄存器地址,接收数据，结束总线的全过程。
//          从机地址slave，寄存器地址reg,接收的内容放入s指向的存储区，接收num个字节。
//函数返回：void
//参数说明：slave:从机地址
//          reg  :寄存器地址
//          s    :接收字符串首地址
//          num  :字节个数
//-----------------------------------------------------------------
void I2C_recv_str(uint8_t slave,uint8_t reg,uint8_t *s,uint8_t num)
{
     I2C_start();                    //起始信号

     I2C_send_one_char(slave);       //发送设备地址(写)
     I2C_check_ack();
     I2C_send_one_char(reg);         //发送存储单元地址，从0开始
     I2C_check_ack();
     	
     I2C_start();                    //起始信号，表示下面发的是从机地址，而不是第三个字节！！！
     
     I2C_send_one_char(slave+1);     //发送设备地址(读)
     I2C_check_ack();
     
     while(num) 
     {   
         I2C_recv_one_char(s);     //读出寄存器数据
         if (num == 1)
            I2C_no_ack();          //无应答，停止传输
         else
            I2C_ack();             //有应答，继续传输
         s++;
         num--;
     }
     
     I2C_stop();                     //停止信号
} 


//交换参数输入顺序：
int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	I2C_send_str(addr,reg,data,len);
	return 0;
}
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * buf)
{
	I2C_recv_str(addr,reg,buf,len);
	return 0;
}

