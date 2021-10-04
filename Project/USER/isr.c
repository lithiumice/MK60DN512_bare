#include "isr.h"
#include "nvic.h"
#include "common_macro.h"

unsigned int taskNum[5] = {0};
uint32 sys_tick_500us;
uint32 sys_tick_ms;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTA�ж�ִ�к���
//  @return     void
//  @since      v1.0
//  Sample usage:               ��A�������жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú���
//-------------------------------------------------------------------------------------------------------------------
void PORTA_IRQHandler(void)
{
    PORTA->ISFR = 0xffffffff;
}

void PORTC_IRQHandler(void)
{
    PORTC->ISFR = 0xffffffff;
    ov7725_uart_vsync();
}

void PORTE_IRQHandler(void)
{
    PORTE->ISFR = 0xffffffff;
}

void PORTD_IRQHandler(void)
{
    key_scan();
    PORTD->ISFR = 0xffffffff;
}

void DMA0_IRQHandler(void)
{
    ov7725_uart_dma();
}

void PIT0_IRQHandler(void)
{
    //sys tick inc
    task_handle();
    sys_tick_ms++;

    PIT_FlAG_CLR(pit0);
}

void PIT1_IRQHandler(void)
{
    PIT_FlAG_CLR(pit1);
}

void PIT2_IRQHandler(void)
{
    //sys timer thread
    sys_tick_500us++;
    if (sys_tick_500us % 2 == 0)
    {
        imu_update();
    }
    PIT_FlAG_CLR(pit2);
}

void PIT3_IRQHandler(void)
{
    PIT_FlAG_CLR(pit3);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART3�ж�ִ�к���
//  @return     void
//  @since      v1.0
//  Sample usage:               ��UART3�����жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú���
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
    if (UART3->S1 & UART_S1_RDRF_MASK) //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        ov7725_cof_uart_interrupt(); //V2.0 �汾С��紮��ͨѶ����
    }
    if (UART3->S1 & UART_S1_TDRE_MASK) //�������ݼĴ�����
    {
        //�û���Ҫ����������
    }
}

void UART0_RX_TX_IRQHandler(void)
{
    if (UART0->S1 & UART_S1_RDRF_MASK) //�������ݼĴ�����
    {
        //�û���Ҫ�����������
    }
    if (UART0->S1 & UART_S1_TDRE_MASK) //�������ݼĴ�����
    {
        //�û���Ҫ����������
    }
}

void UART5_RX_TX_IRQHandler(void)
{
    if (UART5->S1 & UART_S1_RDRF_MASK) //�������ݼĴ�����
    {
        //�û���Ҫ�����������
    }

    if (UART5->S1 & UART_S1_TDRE_MASK) //�������ݼĴ�����
    {
        //�û���Ҫ����������
    }
}

/*
�жϺ������ƣ��������ö�Ӧ���ܵ��жϺ���
Sample usage:��ǰ������DMA0�жϣ�Ȼ��͵�����ȥ���ĸ���DMA0���жϺ������ƣ��ҵ���дһ�������Ƶĺ�������
void DMA0_IRQHandler(void)
{
    ;
}
�ǵý����жϺ������־λ


DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
DMA13_IRQHandler
DMA14_IRQHandler
DMA15_IRQHandler
DMA_Error_IRQHandler
MCM_IRQHandler
FTFL_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
LLW_IRQHandler
Watchdog_IRQHandler
RNG_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
CAN0_ORed_Message_buffer_IRQHandler
CAN0_Bus_Off_IRQHandler
CAN0_Error_IRQHandler
CAN0_Tx_Warning_IRQHandler
CAN0_Rx_Warning_IRQHandler
CAN0_Wake_Up_IRQHandler
I2S0_Tx_IRQHandler
I2S0_Rx_IRQHandler
CAN1_ORed_Message_buffer_IRQHandler
CAN1_Bus_Off_IRQHandler
CAN1_Error_IRQHandler
CAN1_Tx_Warning_IRQHandler
CAN1_Rx_Warning_IRQHandler
CAN1_Wake_Up_IRQHandler
Reserved59_IRQHandler
UART0_LON_IRQHandler
UART0_RX_TX_IRQHandler
UART0_ERR_IRQHandler
UART1_RX_TX_IRQHandler
UART1_ERR_IRQHandler
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
USB0_IRQHandler
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler
ENET_Receive_IRQHandler
ENET_Error_IRQHandler
Reserved95_IRQHandler
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler
LPTimer_IRQHandler
Reserved102_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler
*/
