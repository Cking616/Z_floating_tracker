/*
 * =====================================================================================
 *
 *       Filename:  capacity_table.h
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

#ifndef _CAPACITY_TABLE_H
#define _CAPACITY_TABLE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef err_t unsigned int
typedef	opp_cap_t unsigned short
typedef rel_cap_t unsigned int
typedef len_t int

#define IS_ORDER_TO <
#define IS_INVERTED_TO >

#define TABLE_BUFFER_SIZE 10000
#define INVALID_POINT 0xffff

/* μm*/
typedef struct capacity_table
{
	opp_cap_t 	table[TABLE_BUFFER_SIZE];	
	rel_cap_t 	base_cap_val;				/* */
	int 		num_of_point;
	int 		last_index;					/* */
	int 		last_step;					/* */
	int 		max_acc;					/* */
}cap_state_t;

err_t cap_table_initialize(cap_state_t* state);

len_t cap_table_get_length(cap_state_t* state, rel_cap_t cur_cap);

#ifdef __cplusplus
}
#endif

#endif