/*
 * =====================================================================================
 *
 *       Filename:  io_manager.c
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

#include "io_manager.h"

io_state g_io_manager;

void io_manager_input(io_state * state)
{
	input_state* io_input = io_manager_get_input(state);
	unsigned short tmp_servo_rd_input;
	io_input->ad_input0 = AD_INPUT_RD0_REG & 0x0000ffff;
	io_input->ad_input1 = AD_INPUT_RD1_REG & 0x0000ffff;
	io_input->ad_input2 = AD_INPUT_RD2_REG & 0x0000ffff;
	io_input->ad_input3 = AD_INPUT_RD3_REG & 0x0000ffff;

	io_input->capacity_rd_input = CAP_RD_REG;
	io_input->encoder_rd_input = ENCODER_RD_REG;
	io_input->gpio_input = IO_INPUT_RD_REG & 0x000000ff;
	tmp_servo_rd_input = SRV_INPUT_RD_REG & 0x00000fff;
	if (tmp_servo_rd_input & 0x100)
	{
		tmp_servo_rd_input |= 0x2;
	}
	else
	{
		tmp_servo_rd_input &= (~0x2);
	}
	io_input->servo_rd_input = tmp_servo_rd_input & 0x000000ff;
}

void io_manager_output(io_state * state)
{
	output_state* io_output = io_manager_get_output(state);
	SERVER_DA_OUTPUT_REG = io_output->servo_da_output;
	SERVER_IO_OUTPUT_REG = io_output->servo_io_output;
	PWM_OUTPUT_REG = io_output->pwm_output;
	IO_OUTPUT_REG = io_output->gpio_output;
	DA_OUTPUT1_REG = io_output->da_output1;
	DA_OUTPUT2_REG = io_output->da_output2;
}

void io_manager_initialize(io_state * state)
{
	output_state* io_output = io_manager_get_output(state);
	
	io_manager_input(state);

	io_manager_disable_servo(state);
	io_manager_disable_alarm_clear(state);
	io_manager_enable_zero_clamp(state);
	io_output->servo_da_output = 0;
	io_output->pwm_output = 0;
	io_output->gpio_output = 0;
	io_output->da_output1 = 0;
	io_output->da_output2 = 0;

	io_manager_output(state);
}

#define constrain_limit(x,lower,upper)    ((x)<(lower)?(lower):((x)>(upper)?(upper):(x)))
#define constarin_up(x, upper) ((x) > (upper) ? (upper) : (x))
#define constarin_low(x, lower)  ((x) < (lower) ? (lower) : (x))
#define ZERO_ERROR 0.01f

void servo_manager_initialize(servo_state * state)
{
	io_state* io_manager = io_manager_get_manager();
	state->convert_unit_voltage = (float)(VOLTAGE_SUMMATION * PER_MINUTE_SECONDS * state->pulse_per_inc);
	state->convert_unit_voltage /= state->rpm_per_volage;
	state->convert_unit_voltage /= state->pulse_per_rpm;

	state->last_encoder_position = io_manager_read_encoder(io_manager);
	state->last_out_speed = 0.0f;
	state->last_out_acceleration = 0.0f;
}

void servo_manager_set_speed(servo_state* state, float target_speed)
{
	io_state* io_manager = io_manager_get_manager();
	int out_voltage = 0;
	float up_acceleration = 0.0f;
	float down_acceleration = 0.0f;

	target_speed = constrain_limit(target_speed, -state->max_speed, state->max_speed);
	up_acceleration = state->last_out_acceleration + state->max_jerk;
	up_acceleration = constarin_up(up_acceleration, state->max_acceleration);
	down_acceleration = state->last_out_acceleration - state->max_jerk;
	down_acceleration = constarin_low(down_acceleration, -state->max_acceleration);
	target_speed = constrain_limit(target_speed,
		state->last_out_speed + up_acceleration,
		state->last_out_speed + down_acceleration);

	state->last_out_acceleration = target_speed - state->last_out_speed;
	state->last_out_speed = target_speed;
	state->last_encoder_position = io_manager_read_encoder(io_manager);

	out_voltage = target_speed * state->convert_unit_voltage - state->zero_com_value;
	if (out_voltage < 0)
	{
		out_voltage = -out_voltage;
	}
	if (out_voltage > MAX_SPEED_VOLTAGE)
	{
		out_voltage = MAX_SPEED_VOLTAGE;
	}

	if (target_speed > ZERO_ERROR)
	{
		io_manager->output_cache.servo_da_output = ZERO_SPEED_VOLTAGE + out_voltage;
	}
	else if (target_speed < -ZERO_ERROR)
	{
		io_manager->output_cache.servo_da_output = ZERO_SPEED_VOLTAGE - out_voltage;
	}
	else
	{
		io_manager->output_cache.servo_da_output = ZERO_SPEED_VOLTAGE - state->zero_com_value;
		if (state->last_out_speed > -ZERO_ERROR && state->last_out_speed < ZERO_ERROR)
		{
			io_manager_enable_zero_clamp(io_manager);
		}
	}
}