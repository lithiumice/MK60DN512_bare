/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_7725.c
 * @brief      		小钻风(二值化摄像头)函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-09-25
 * @note
					OV7725接线定义：
					------------------------------------
						模块管脚            单片机管脚
						SDA(51的RX)         查看MK60DN1_port_cfg.h文件内的UART3_TX_PIN	宏定义
						SCL(51的TX)         查看MK60DN1_port_cfg.h文件内的UART3_RX_PIN  宏定义
						场中断              查看SEEKFREE_7725_UART.h中的OV7725_UART_VSYNC 	宏定义
						行中断				未使用，因此不接
						像素中断            查看SEEKFREE_7725_UART.h中的OV7725_UART_PCLK  	宏定义
						数据口              查看SEEKFREE_7725_UART.h中的OV7725_UART_DATAPORT 	宏定义
						默认分辨率        	160*120
						默认FPS             50帧
					------------------------------------
 ********************************************************************************************************************/



#include "SEEKFREE_7725_UART.h"
extern int ov_choose;
uint8 ov7725_uart_image_bin[OV7725_UART_SIZE];      	//定义存储接收图像的数组
uint8 ov7725_uart_image_dec[OV7725_UART_H][OV7725_UART_W];
int8 buf01[10] = {0x03, 0xfc,
                   0,0,0,0,0,0,0,0
                 };
int8 buf12[36] = { 0x03, 0xfc,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0xfc, 0x03
                 };
int buf12_num = 0;
//需要配置到摄像头的数据
uint16 OV7725_CFG[OV7725_CONFIG_FINISH][2] =
{
    {OV7725_RESERVE,           0},
    {OV7725_CONTRAST,       0x50},    	//对比度0x00-0xff(改变这个值 间接实现阈值修改的效果)
    {OV7725_FPS,             150},    	//帧率设置 只支持50 75 100 112 150
    {OV7725_COL,   OV7725_UART_W},    	//只能是80 160 240 320 宽度和高度务必一一对应，例如宽度设置为80，高度只能设置为60。
    {OV7725_ROW,   OV7725_UART_H},    	//只能是60 120 180 240

    {OV7725_INIT,              0}     	//摄像头开始初始化
};


//从摄像头内部获取到的配置数据
uint16 OV7725_GET_CFG[OV7725_CONFIG_FINISH - 1][2] =
{
    {OV7725_RESERVE,    	   0},
    {OV7725_CONTRAST,   	   0},
    {OV7725_FPS,        	   0},
    {OV7725_COL,   			   0},
    {OV7725_ROW,   			   0}
};


uint8   ov7725_uart_receive[3];
uint8   ov7725_uart_receive_num = 0;
uint8   ov7725_uart_receive_flag = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头串口中断函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       该函数在ISR文件 串口3中断程序被调用
//-------------------------------------------------------------------------------------------------------------------
void ov7725_cof_uart_interrupt(void)
{
    uart_getchar(OV7725_COF_UART, &ov7725_uart_receive[ov7725_uart_receive_num]);;
    ov7725_uart_receive_num++;
    if(1 == ov7725_uart_receive_num && 0XA5 != ov7725_uart_receive[0])	ov7725_uart_receive_num = 0;
    if(3 == ov7725_uart_receive_num)
    {
        ov7725_uart_receive_num = 0;
        ov7725_uart_receive_flag = 1;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风对摄像头内部寄存器进行写操作
//  @param      uartn   摄像头串口
//  @param      buff    需要写入的数据
//  @return     void
//  @since      v1.0
//  Sample usage:		调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
void ov7725_set_all_config(UARTn uartn, uint16 buff[OV7725_CONFIG_FINISH - 1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];
    ov7725_uart_receive_flag = 0;
    //设置参数  具体请参看问题锦集手册
    //开始配置摄像头并重新初始化
    for(i = 0; i < OV7725_CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = buff[i][0];
        temp           = buff[i][1];
        send_buffer[2] = temp >> 8;
        send_buffer[3] = (uint8)temp;
        uart_putbuff(uartn, send_buffer, 4);
        systick_delay_ms(2);
    }
    //等待摄像头初始化成功
    while(!ov7725_uart_receive_flag);
    ov7725_uart_receive_flag = 0;
    while((0xff != ov7725_uart_receive[1]) || (0xff != ov7725_uart_receive[2]));
    //以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风获取摄像头内部配置信息
//  @param      uartn   摄像头串口
//  @param      buff    需要读取的数据
//  @return     void
//  @since      v1.0
//  Sample usage:		调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
void ov7725_get_all_config(UARTn uartn, uint16 buff[OV7725_CONFIG_FINISH - 1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];
    for(i = 0; i < OV7725_CONFIG_FINISH - 1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = OV7725_GET_STATUS;
        temp           = buff[i][0];
        send_buffer[2] = temp >> 8;
        send_buffer[3] = (uint8)temp;
        uart_putbuff(uartn, send_buffer, 4);
        //等待接受回传数据
        while(!ov7725_uart_receive_flag);
        ov7725_uart_receive_flag = 0;
        buff[i][1] = ov7725_uart_receive[1] << 8 | ov7725_uart_receive[2];
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风获取摄像头固件版本
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:		调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint16 ov7725_get_id(UARTn uartn)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = OV7725_GET_WHO_AM_I;
    temp           = 0;
    send_buffer[2] = temp >> 8;
    send_buffer[3] = (uint8)temp;
    uart_putbuff(uartn, send_buffer, 4);
    //等待接受回传数据
    while(!ov7725_uart_receive_flag);
    ov7725_uart_receive_flag = 0;
    return ((uint16)(ov7725_uart_receive[1] << 8) | ov7725_uart_receive[2]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风获取摄像头固件版本
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart3
//-------------------------------------------------------------------------------------------------------------------
uint16 ov7725_get_version(UARTn uartn)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = OV7725_GET_STATUS;
    temp           = OV7725_GET_VERSION;
    send_buffer[2] = temp >> 8;
    send_buffer[3] = (uint8)temp;
    uart_putbuff(uartn, send_buffer, 4);
    //等待接受回传数据
    while(!ov7725_uart_receive_flag);
    ov7725_uart_receive_flag = 0;
    return ((uint16)(ov7725_uart_receive[1] << 8) | ov7725_uart_receive[2]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头初始化(调用之后设置好相关中断函数即可采集图像)
//  @param      NULL
//  @return     0
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_init(void)
{
    uart_init (OV7725_COF_UART, 9600);	//初始换串口 配置摄像头
    uart_rx_irq_en(OV7725_COF_UART);
    EnableInterrupts;
    ov7725_uart_receive_flag = 0;
    //等待摄像头上电初始化成功 方式有两种：延时或者通过获取配置的方式 二选一
    //systick_delay_ms(1000);//延时方式
    ov7725_get_all_config(OV7725_COF_UART, OV7725_GET_CFG);//获取配置的方式
    ov7725_uart_receive_flag = 0;
    ov7725_set_all_config(OV7725_COF_UART, OV7725_CFG);
    //获取配置便于查看配置是否正确
    ov7725_get_all_config(OV7725_COF_UART, OV7725_GET_CFG);
    //摄像头采集初始化
    DisableInterrupts;
    //DMA通道0初始化，OV7725_PCLK触发源(默认上升沿)，源地址为OV7725_DATAPORT，目的地址为：image_bin，每次传输1Byte
    dma_portx2buff_init(OV7725_UART_DMA_CH, (void *)&OV7725_UART_DATAPORT, (void *)ov7725_uart_image_bin, OV7725_UART_PCLK, DMA_BYTE1, OV7725_UART_DMA_NUM, DADDR_KEEPON);
    DMA_DIS(OV7725_UART_DMA_CH);
    disable_irq(OV7725_UART_INTERRUPT);                              //关闭中断
    DMA_IRQ_CLEAN(OV7725_UART_DMA_CH);                               //清除通道传输中断标志位
    DMA_IRQ_EN(OV7725_UART_DMA_CH);
    port_init(OV7725_UART_PCLK, ALT1 | DMA_FALLING | PULLDOWN );     //PCLK 下降沿触发DMA
    port_init(OV7725_UART_VSYNC, ALT1 | IRQ_RISING | PULLDOWN | PF);//场中断，上拉，上降沿触发中断，带滤波
    enable_irq(OV7725_UART_INTERRUPT);
    EnableInterrupts;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_vsync(void)
{
    PORTC_FLAG_CLR(OV7725_UART_VSYNC);
    PORTC_FLAG_CLR(OV7725_UART_PCLK);
    DMA0->TCD[OV7725_UART_DMA_CH].DADDR = (uint32)ov7725_uart_image_bin;     	//恢复DMA地址
    DMA_EN(OV7725_UART_DMA_CH);                                  				//使能DMA通道CHn 硬件请求
}


uint8 ov7725_uart_finish_flag = 0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头DMA完成中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_dma(void)
{
    DMA_IRQ_CLEAN(OV7725_UART_DMA_CH);                               			//清除通道传输中断标志位
    ov7725_uart_finish_flag = 1;												//采集完成标志位
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头数据解压函数
//  @param      *data1				源地址
//  @param      *data2				目的地址
//  @return     void
//  @since      v1.0
//  Sample usage:					Image_Decompression(da1,dat2[0]);//将一维数组dat1的内容解压到二维数组dat2里.
//-------------------------------------------------------------------------------------------------------------------
void Image_Decompression(uint8 *data1, uint8 *data2)
{
    uint8  temp[2] = {255, 0};
    uint8 temp2[2] = {0, 255};
    uint16 lenth = OV7725_UART_SIZE;
    uint8  i = 8;
    while(lenth--)
    {
        i = 8;
        while(i--)
        {
            if(ov_choose == OV_PIN20)
            {
                *data2++ = temp[(*data1 >> i) & 0x01];
            }
            else if(ov_choose == OV_PIN16)
            {
                *data2++ = temp2[(*data1 >> i) & 0x01];
            }
        }
        data1++;
    }
}

void img_extract(void)
{
    if (ov_choose == OV_PIN20)
    {
        Image_Decompression(image_bin, ov7725_uart_image_dec[0]);
    }
    if(ov_choose == OV_PIN16)
    {
        Image_Decompression(ov7725_uart_image_bin, ov7725_uart_image_dec[0]);
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头未解压图像发送至上位机查看图像
//  @param      *imgaddr			压缩图像数据地址
//  @param      *imgsize			图像大小(直接填写OV7725_UART_SIZE)
//  @return     void
//  @since      v1.0
//  Sample usage:					调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_7725(void *imgaddr, uint32_t imgsize)
{
    uart_putchar(uart0, 0x00);
    uart_putchar(uart0, 0xff);
    uart_putchar(uart0, 0x01);
    uart_putchar(uart0, 0x01);//发送四个字节命令
    uart_putbuff(uart0, (uint8_t *)imgaddr, imgsize);   //再发送图像
}

void uint2Byte(float *target, int8 *buf, int8 beg4) //类型转换函数 将uint16转成字节发送
{
    int8 *point;
    point = (int8 *)target;
    buf[beg4] = point[0];
    buf[beg4 + 1] = point[1];
    buf[beg4 + 2] = point[2];
    buf[beg4 + 3] = point[3];
}

void virtual_osc (void)
{
//    static float float_out = 0;
//    float_out = (float)rightturncounter;    
//    uint2Byte(&float_out, buf12, 2);
//    float_out = (float)Right_count;
//    uint2Byte(&float_out, buf12, 6);
//    float_out = (float)Findline.err[0];
//    uint2Byte(&float_out, buf12, 10);
//    float_out = (float)leftturncounter;
//    uint2Byte(&float_out, buf12, 14);
//    float_out = (float)Left_count;
//    uint2Byte(&float_out, buf12, 18);
//    float_out = (float)turncounter;
//    uint2Byte(&float_out, buf12, 22);
//    float_out = (float)0;
//    uint2Byte(&float_out, buf12, 26);
//    float_out = (float)0;
//    uint2Byte(&float_out, buf12, 30);
    for(buf12_num = 0; buf12_num < 36; buf12_num++)
    {
        uart_putchar(uart0, buf12[buf12_num]);
    }
}

void sendError(void)
{
//  	static float floatOut1 = 0,floatOut2=0;
//	floatOut1 = l_sendspeed;
//        floatOut2 = r_sendspeed;
//	uint2Byte(&floatOut1, buf01, 2);
//        uint2Byte(&floatOut2,buf01,6);
//	for(int i = 0; i < 10; i++)
//    {
//        uart_putchar(uart5, buf01[i]);
//    }
      
}

void byteToUint(float *target, uint8 *buf, uint8 beg4)
{
    int8 *point = (int8 *)target;
    point[0] = buf[beg4];
    point[1] = buf[beg4 + 1];
    point[2] = buf[beg4 + 2];
    point[3] = buf[beg4 + 3];
}



