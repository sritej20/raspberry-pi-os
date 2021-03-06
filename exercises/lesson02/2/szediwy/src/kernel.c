#include "custom_printf.h"
#include "timer.h"
#include "utils.h"
#include "mini_uart.h"
#include "irq.h"

void kernel_main(unsigned long processor_index)
{
	static unsigned int current_processor_index = 0;

	if (processor_index == 0)
	{
		uart_init();
		init_printf(0, putc);
		irq_vector_init();
		timer_init();
		enable_interrupt_controller();
		enable_irq();
	}

	while (processor_index != current_processor_index)
		;

	int exception_level = get_el();
	printf("{CPU: %d, Exception level: %d} \r\n", processor_index, exception_level);

	current_processor_index++;

	if (processor_index == 0)
	{
		// if current_processor_index == 4 then all processors send message
		while (current_processor_index != 4)
			;
		for (;;)
		{
			uart_send(uart_recv());
		}
	}
}