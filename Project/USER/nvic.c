#include "nvic.h"

uint8 NVIC_Init(NVIC_InitTypeDef NVIC_InitStructure)
{
	uint8 request = 1;
	IRQn_Type int_id = NVIC_InitStructure.NVIC_IRQChannel;
	uint32 nvic_priority_group = NVIC_InitStructure.NVIC_IRQChannelGroupPriority;
	uint32 nvic_preemption_priority = NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority;
	uint32 nvic_sub_priority = NVIC_InitStructure.NVIC_IRQChannelSubPriority;
	uint8 status = NVIC_InitStructure.NVIC_IRQChannelEnable;
	uint32 temp;

	switch (nvic_priority_group)
	{
	case NVIC_PriorityGroup_0:
		NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
		if (nvic_preemption_priority > 0 || nvic_sub_priority > 15)
			request = 0;
		break;
	case NVIC_PriorityGroup_1:
		NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
		if (nvic_preemption_priority > 1 || nvic_sub_priority > 7)
			request = 0;
		break;
	case NVIC_PriorityGroup_2:
		NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
		if (nvic_preemption_priority > 3 || nvic_sub_priority > 3)
			request = 0;
		break;
	case NVIC_PriorityGroup_3:
		NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
		if (nvic_preemption_priority > 7 || nvic_sub_priority > 1)
			request = 0;
		break;
	case NVIC_PriorityGroup_4:
		NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
		if (nvic_preemption_priority > 15 || nvic_sub_priority > 0)
			request = 0;
		break;
	default:
		return 0;
	}

	temp = NVIC_EncodePriority(nvic_priority_group, nvic_preemption_priority, nvic_sub_priority);
	NVIC_SetPriority(int_id, temp);
	if (status == 1)
	{
		NVIC_EnableIRQ(int_id);
	}
	else
	{
		NVIC_DisableIRQ(int_id);
	}
	return request;
}

void Priority_Init(void)
{
	static NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = PIT0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = PORTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelEnable = 1;
	NVIC_Init(NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelEnable = 1;
	NVIC_Init(NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = PIT1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART3_RX_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART5_RX_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelEnable = 1;
	NVIC_Init(NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = PIT2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(NVIC_InitStructure);

	gpio_init(E12, GPI, 0);
	port_init(E12, IRQ_RISING | PF | ALT1 | PULLDOWN);
	gpio_init(E7, GPI, 0);
	port_init(E7, IRQ_RISING | PF | ALT1 | PULLDOWN);

	set_irq_priority(PORTE_IRQn, 4);
	gpio_init(E11, GPO, 0);
	enable_irq(PORTE_IRQn);

	pit_init_ms(pit0, 1);
	// pit_time_start(pit1);
	pit_init_us(pit2, 500);
	// pit_time_start(pit3);

	enable_irq(PIT0_IRQn);
	// enable_irq(PIT1_IRQn);
	enable_irq(PIT2_IRQn);
	// enable_irq(PIT3_IRQn);
	uart_rx_irq_en(uart5);
}
