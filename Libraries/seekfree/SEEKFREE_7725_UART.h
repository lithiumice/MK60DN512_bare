/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file        	SEEKFREE_7725.h
 * @brief       	小钻风(二值化摄像头)函数库
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
						默认分辨率是        160*120
						默认FPS             50帧
					------------------------------------
 ********************************************************************************************************************/



#ifndef _SEEKFREE_7725_UART_h
#define _SEEKFREE_7725_UART_h


#include "headfile.h"


#define OV7725_UART_W    			160	//只能是80 160 240 320 宽度和高度务必一一对应，例如宽度设置为80，高度只能设置为60。
#define OV7725_UART_H    			120	//只能是60 120 180 240


//配置摄像头参数
#define OV7725_UART_DMA_CH      	DMA_CH0                         	//定义摄像头的DMA采集通道
#define OV7725_UART_SIZE        	(OV7725_UART_W * OV7725_UART_H/8 )	//图像占用空间大小
#define OV7725_UART_DMA_NUM     	(OV7725_UART_SIZE )               	//DMA采集次数


#define OV7725_COF_UART    			uart3								//摄像头通信串口定义


//--------------------------------------------------------------------------------------------------
//        建议OV7725_UART_PCLK与OV7725_UART_VSYNC 都使用同一组管脚，比如现在使用的PTE0与PTE1就是同一组的管脚。
//        一共有五组分别是PTA、PTB、PTC、PTD、PTE。
//        如果修改了这两个引脚那么注意修改PORTE_IRQn。
//        比如使用的是PTA这组的引脚那么将这三个修改为PORTA_IRQn即可
//--------------------------------------------------------------------------------------------------
#define OV7725_UART_PCLK           	C18                            		//摄像头像素时钟
#define OV7725_UART_VSYNC          	C19                            		//摄像头场同步信号
#define OV7725_UART_INTERRUPT		PORTC_IRQn                      	//中断编号
#define OV7725_UART_PLCK_PORT		PORTC								//PCLK端口


//--------------------------------------------------------------------------------------------------
//        摄像头数据接口
//        C_IN_DATA(1)
//        第一个字母C代表着使用C口，可以选择A、B、C、D、E.分别对应A_IN_DATA，B_IN_DATA，C_IN_DATA，D_IN_DATA，E_IN_DATA
//        第二个括号里面的参数1代表着使用的是C8 - C15口,
//		  可以选择0、1、2、3，分别对应的是	该端口的X0-X7，X8-X15，X16-X23，X24-X31(这里x代表的是第一个字母，比如现在的就是C8-C15)
//--------------------------------------------------------------------------------------------------
#define OV7725_UART_DATAPORT 		C_IN_DATA(1)	                	//DMA数据口


typedef enum
{
    OV7725_INIT 	= 0x00,			//初始化命令
    OV7725_RESERVE,					//保留位
    OV7725_CONTRAST,				//对比度调节 实现阈值的调节
    OV7725_FPS,						//帧率设置 只支持50 75 100 112 150
    OV7725_COL,						//分辨率设置 只能是80 160 240 320
    OV7725_ROW,						//分辨率设置 只能是60 120 180 240
    OV7725_CONFIG_FINISH,			//非命令位，主要用来占位计数

    OV7725_GET_WHO_AM_I = 0xEF,
    OV7725_GET_STATUS  = 0xF1,
    OV7725_GET_VERSION = 0xF2,

    OV7725_SET_ADDR    = 0xFE,
    OV7725_SET_DATA    = 0xFF,
} OV7725_CMD;

extern uint8 ov7725_uart_image_bin[OV7725_UART_SIZE];                	//定义存储接收图像的数组
extern uint8 ov7725_uart_image_dec[OV7725_UART_H][OV7725_UART_W];    	//图像解压后的数组
extern uint8 ov7725_uart_finish_flag;									//图像采集完成标志位
extern int8 buf01[10];

//函数声明
void ov7725_cof_uart_interrupt(void);
void ov7725_uart_init(void);
void ov7725_uart_vsync(void);
void ov7725_uart_dma(void);
void Image_Decompression(uint8 *data1, uint8 *data2);
void seekfree_sendimg_7725(void *imgaddr, uint32_t imgsize);
void img_extract(void);
void uint2Byte(float *target, int8 *buf, int8 beg4);
void virtual_osc (void);
void sendError(void);
void byteToUint(float *target, uint8 *buf, uint8 beg4);
#endif



