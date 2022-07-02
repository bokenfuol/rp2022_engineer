/**
 * @file        filter.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        13-November-2021
 * @brief       滤波
 */
 
 #include "filter.h"
 
 
/**
 * @date        13-November-2021
 * @brief       滑动滤波
 ** input: 滑动滤波输入
 ** output: 滑动滤波输出 
*/
void moving_filter(int16_t *input,float *output)
{
	int64_t sum=0,size=0;
	size = LEN_FILTER;
	for(uint16_t i = 0 ; i <size ; i ++)
		sum += input[i];
	*output = (float)sum/size;
}


/**
 * @date        13-November-2021
 * @brief       低通滤波
** Descriptions: 低通滤波
** Input: 上一次最佳估计值，新测量值，系数
** Output: 新最佳估计值
*/
float Lowpass( float X_last, float X_new, float K)
{
	float new_value, add;
	add = (X_new - X_last) * K;			//增量
	new_value = add + X_last;			//下一个最佳估计值
	return new_value;
}
