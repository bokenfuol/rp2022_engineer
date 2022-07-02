/**
 * @file        filter.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        13-November-2021
 * @brief       �˲�
 */
 
 #include "filter.h"
 
 
/**
 * @date        13-November-2021
 * @brief       �����˲�
 ** input: �����˲�����
 ** output: �����˲���� 
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
 * @brief       ��ͨ�˲�
** Descriptions: ��ͨ�˲�
** Input: ��һ����ѹ���ֵ���²���ֵ��ϵ��
** Output: ����ѹ���ֵ
*/
float Lowpass( float X_last, float X_new, float K)
{
	float new_value, add;
	add = (X_new - X_last) * K;			//����
	new_value = add + X_last;			//��һ����ѹ���ֵ
	return new_value;
}
