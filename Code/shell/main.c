#include <REGX51.H>
#include "./uart.h"
#include "./cli.h"
#include "./gpio.h"
#include "./timer.h"

#define    NUM_CLI 		2

cli_t list_cmd[NUM_CLI] =	{
								{
									"led", /* name of cmd */
									2, /* num_input_par */
									led_callback,
									NULL
								},
								{
									"test",
									0,
									test_callback,
									NULL
								}
							};

void main()
{
	timer0_init();
	uart_init();
	P1 = 0x00;

	cli_init();
#if 1
	uart_send_str("\n\r3main :: cli_init()\n\rhead_cmd: ");
	uart_send_str(head_cmd->cmd);
	uart_send_str("\n\r\n\r");
#endif

	cli_add(list_cmd,NUM_CLI);

	while(1)
	{		
		if(_data != 0)
		{
			if(prepare_cli(_data))
			{
				uart_send_str("\n\r");
			}	
			_data = 0;
		}
	}				
}
/****************** INTERRUPT  ******************************/
void timer0_ISR(void) interrupt 1
{
	 tick++;
	 TH0 = 0xFC;
	 TL0 = 0x66;
}

void uart_ISR(void) interrupt 4
{
	if(RI == 1)
	{
		_data = SBUF;
		uart_send_byte(_data);
		RI = 0;
	}
}