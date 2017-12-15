/*
 * =====================================================================================
 *
 *       Filename:  io_manager.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017/12/11
 *
 *         Author:  cking ((kanghaitao)), cking616@mail.ustc.edu.cn
 *        License:  Apache Licence 
 *
 * =====================================================================================
 */

#ifndef _IO_MANAGER_H
#define _IO_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef HAL_DEF
#include "hal_def.h"
#else
#define 	MASK_00 			0x00000001	//
#define 	MASK_01 			0x00000002	// Mask for SRV-ON
#define 	MASK_02 			0x00000004	// Mask for SRV-ALARM-CLEAR
#define 	MASK_03 			0x00000008	//
#define 	MASK_04 			0x00000010	// Mask for SRV-ZERO-CONTROL
#define 	MASK_08 			0x00000100  //
#define 	MASK_16 			0x00010000  //

static unsigned int XPAR_USER_UART_0_BASEADDR[0xFF];

#define 	SERVER_DA_OUTPUT_REG	*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x44)
#define 	SERVER_IO_OUTPUT_REG	*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x48)
#define 	PWM_OUTPUT_REG			*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x78)
#define 	IO_OUTPUT_REG			*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x4c)
#define 	DA_OUTPUT1_REG			*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x70)
#define 	DA_OUTPUT2_REG			*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x74)

#define 	ENCODER_RD_REG			*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x60)
#define 	IO_INPUT_RD_REG			*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x68)
#define 	AD_INPUT_RD0_REG		*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x74)
#define 	AD_INPUT_RD1_REG		*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x6c)
#define 	AD_INPUT_RD2_REG		*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x24)
#define 	AD_INPUT_RD3_REG		*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x20)

#define 	CAP_RD_REG				*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0xE8)
#define  	SRV_INPUT_RD_REG 		*(unsigned int*)(XPAR_USER_UART_0_BASEADDR + 0x64)
#endif

/*
 * =====================================================================================
 *
 *									IO_STATE
 *
 * =====================================================================================
 */

typedef struct
{
	unsigned int encoder_rd_input;
	unsigned int capacity_rd_input;
	unsigned short ad_input0;
	unsigned short ad_input1;
	unsigned short ad_input2;
	unsigned short ad_input3;
	unsigned char servo_rd_input;
	unsigned char gpio_input;
} input_state;

typedef struct
{
	unsigned int servo_da_output;
	unsigned int servo_io_output;
	unsigned int gpio_output;
	unsigned int pwm_output;
	unsigned int da_output1;
	unsigned int da_output2;
} output_state;

typedef struct
{
	input_state		input_cache;
	output_state	output_cache;
} io_state;

void io_manager_initialize(io_state* state);

void io_manager_input(io_state* state);

void io_manager_output(io_state* state);

#define io_manager_get_input(state) (&((state)->input_cache))

#define io_manager_get_output(state) (&((state)->output_cache))

#define io_manager_read_cap(state) ((state)->input_cache.capacity_rd_input)

#define io_manager_read_encoder(state) ((state)->input_cache.encoder_rd_input)

#define io_manager_enable_servo(state) ((state)->output_cache.servo_io_output |= MASK_01)

#define io_manager_disenable_servo(state) ((state)->output_cache.servo_io_output &= (~MASK_01))

#define io_manager_enable_zero_clamp(state)  ((state)->output_cache.servo_io_output |= MASK_04)

#define io_manager_disable_zero_clamp(state) ((state)->output_cache.servo_io_output &= (~MASK_04))

#define io_manager_enable_alarm_clear(state) ((state)->output_cache.servo_io_output |= MASK_02)

#define io_manager_disable_alarm_clear(state) ((state)->output_cache.servo_io_output &= (~MASK_02))

#define io_manager_is_alarmed(state) (((state)->input_cache.servo_rd_input & MASK_01) >> 1)

#define io_manager_set_servo_output(state, output) ((state)->output_cache.servo_da_output = (output))

extern io_state g_io_manager;
#define io_manager_get_manager() (&g_io_manager)

/*
* =====================================================================================
*
*									SERVO_STATE
*
* =====================================================================================
*/
#define __postive
#define ZERO_SPEED_VOLTAGE 0x00008000
#define VOLTAGE_SUMMATION  0x00008000	
#define MAX_SPEED_VOLTAGE  0x00007fff
#define PER_MINUTE_SECONDS 60

typedef struct
{
	int zero_init_result;
	int zero_result_decade[16];
	int zero_result_unit[16];
	unsigned short zero_num_decade;
	unsigned short zero_num_unit;
} servo_zero_test_state;

typedef struct
{
	int limit_encoder_position;
	unsigned short rpm_per_volage;
	unsigned short pulse_per_inc;
	unsigned int   pulse_per_rpm;
	short		   pg_ratio;
	short		   last_pg_ratio;
	short		   direction;
	short		   zero_com_value;

	__postive float max_acceleration;
	__postive float max_speed;
	__postive float max_jerk;

	float last_out_speed;
	float last_out_acceleration;
	int	  last_encoder_position;

	float convert_unit_voltage;
} servo_state;

void servo_manager_initialize(servo_state* state);

void servo_manager_set_speed(servo_state* state, float target_speed);

#ifdef __cplusplus
}

#endif

#endif