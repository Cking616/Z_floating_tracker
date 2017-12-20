电容查表法
======
# 问题描述
控制方面而言，可以提高控制精度要同时能满足两点：
1) 控制周期短
2) 反馈精度高

**但这两者是矛盾的，要反馈精度高就需要存于表内的数据多，而数据越多不仅会造成空间上成十倍增长的开销，
而且会使得查表数据增多，查表时间几何倍数增长，采样时间增加控制周期也难以缩短。**

这里讨论的问题就是如何能使得在电容查表这个特定问题时，利用已知条件让查表算法尽可能运算快速使得控制周期能尽可能缩短。
# 解法
单就查找算法而言，较为通用的是[二分查找][binary_chop]（查表时间复杂度o(lgn)），如果对内存空间不在乎且设计了较好的数据压缩算法的话可用[hash表法][hash_chop]来存储(查表时间复杂度为o(1))。

这里就这个问题而言，想利用二分思想，利用已知的现实特征来最优化时间，就这个问题而言有
1) 表的坐标对应的是真实物理中的位置，真实物体的运动速度是有限的，特别但了100μm级后运动量更小。
2) 真实物体的运动加速度在100μm级的值很小。

查找算法如下:
~~~ c
/* 先定义好查找终止条件 */
...
#define IS_ORDER_TO <
#define IS_INVERTED_TO >
...

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

len_t cap_table_get_length(
	cap_state_t* state, 
	rel_cap_t cur_cap)
{	
...
/* 起始查找位置定义到上次查找位置 */
/* 查找增量定义到上次查找增量(真实物体速度变化范围有限) */
...
	_cur_index = (state->last_index == INVALID_POINT)
					? _num_of_point
					: state->last_index;

	_cur_step = state->last_step;
	_acc = state->max_acc;
...

/* 查找算法核心,选择最合适现实的acc量可以使得在真实环境运行最快 */
...
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
...
}
...
~~~

   [binary_chop]: https://www.zhihu.com/question/36132386
   [hash_chop]: http://blog.csdn.net/xiaoping8411/article/details/7706376