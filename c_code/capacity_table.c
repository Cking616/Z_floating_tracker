/*
 * =====================================================================================
 *
 *       Filename:  capacity_table.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017/12/20
 *
 *         Author:  cking ((kanghaitao)), cking616@mail.ustc.edu.cn
 *        License:  Apache Licence 
 *
 * =====================================================================================
 */

#include "capacity_table.h"

inline int get_step_dir(
	opp_cap_t cur_opp_cap, 
	opp_cap_t* cap_table, 
	int index) 
{
	return (cur_cap  IS_ORDER_TO cap_table[index])
			? ( (cur_cap IS_INVERTED_TO cap_table[index + 1])
				? 0 : 1)
			: -1;
}

err_t cap_table_initialize(
	cap_state_t* state)
{
	return 0;
}

len_t cap_table_get_length(
	cap_state_t* state, 
	rel_cap_t cur_cap)
{
	if(cur_cap >= state->base_cap_val)
	{
		state->last_step = state->last_index;
		state->last_index = 0;
		return 0;
	}

	opp_cap_t _opp_cap = state->base_cap_val - cur_cap;
	opp_cap_t* _table = state->table;
	int _num_of_point = state->num_of_point;
	int _cur_index = (state->last_index == INVALID_POINT)
						? _num_of_point
						: state->last_index;

	int _cur_step = state->last_step;
	int _init_dir = get_step_dir(_opp_cap, _table, _cur_index);
	int _dir,_last_dir = _init_dir;
	int _acc = state->max_acc;
	
	while(_dir != 0){
		_cur_index = _cur_index + _dir * _cur_step;

		if(_cur_index > _num_of_point)
		{
			_cur_index = INVALID_POINT;
			break;
		}
		_dir = get_step_dir(_opp_cap, _table, _cur_index);

		_cur_step = _acc;
		if(_last_dir * _dir < 0)
		{
			_acc >>= 1;
		}	
		_last_dir = _dir;
	}

	_tmp_index = (_cur_index == INVALID_POINT) ? _num_of_point : _cur_index;
	state->last_step = (_tmp_index - state->last_index) * _init_dir;
	state->last_index = _cur_index;
	
	return _cur_index;
}