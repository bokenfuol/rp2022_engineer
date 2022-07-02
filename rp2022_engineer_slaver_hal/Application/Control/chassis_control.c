/**
 * @file        chassis_control.c
 * @author      RobotPilots@2022
 * @Version     V1.5
 * @date        3-November-2021
 * @brief       ���̿���
 */
 
 
/*-----���ֹ��ܻ�����ɣ��������Ż�����-----*/
/*-------------12-November-2021-------------*/
/*----------���Ʋ��ٴ���Ŀ�����----------*/
/*-------------15-November-2021-------------*/
/*-�����񵴷�ת�������̶��ֵ�����ɣ�����-*/
/*-------------16-November-2021-------------*/
/*---------------���Ӷ��ֳ�ʼ��-------------*/
/*-------------14-January-2022--------------*/
/*------------�������鸴λǿ��ͨ��----------*/
/*---------------11-May-2022----------------*/

#include "control.h"
#include "chassis_control.h"
#include "arm_math.h"
#include "cmsis_os.h"


#define CONSTRAIN 20


chassis_control_t chassis_control = {
	.move = 0,
	.tranlation = 0,
	.turn = 0,
	.reset = RESET_NOT_OK,
	.barke_angle = NO_BARKE,
};
 
/**
 * @date        29-November-2021
 * @brief       ��������
*/
void Chassis_Task(void const * argument)
{
  for(;;)
  {
		if(IS_SYSTEM_RESET)
			general_chassis_control();
		chassis_check();
    osDelay(2);
  }
}

/**
 * @date        3-November-2021
 * @brief       ���̸�λ
*/
uint32_t chassis_reset_time;
reset_state_t chassis_reset()
{
	if(!chassis_control.reset)
	{
			if(chassis_reset_time>3000)
			{
				chassis_reset_time = 0;
				return RESET_OK;
			}

			chassis_motor[CHAS_FL].angle_target = chassis_motor[CHAS_FL].mid_angle;
			chassis_motor[CHAS_FR].angle_target = chassis_motor[CHAS_FR].mid_angle;
			chassis_motor[CHAS_BL].angle_target = chassis_motor[CHAS_BL].mid_angle;
			chassis_motor[CHAS_BR].angle_target = chassis_motor[CHAS_BR].mid_angle;
			/*������λ*/
			if(distance(chassis_motor[CHAS_FL].rudder_motor.info.angle,LF_MidAngle)<115&&\
			distance(chassis_motor[CHAS_FR].rudder_motor.info.angle,RF_MidAngle)<115&&\
			distance(chassis_motor[CHAS_BL].rudder_motor.info.angle,LB_MidAngle)<115&&\
			distance(chassis_motor[CHAS_BR].rudder_motor.info.angle,RB_MidAngle)<115)
			{
				chassis_reset_time = 0;
				return RESET_OK;
			}
			else
				chassis_reset_time++;
			
			return RESET_NOT_OK;		
	}
	return chassis_control.reset;
}

 /**
 * @date        14-January-2021
 * @brief       ���̳�ʼ��
 */
void chassis_init()
{
	chassis_motor[CHAS_FL].power_motor.speed_pid.target = 0;
	chassis_motor[CHAS_FR].power_motor.speed_pid.target = 0;
	chassis_motor[CHAS_BL].power_motor.speed_pid.target = 0;
	chassis_motor[CHAS_BR].power_motor.speed_pid.target = 0;
	chassis_control.real_speed[0] = 0;
	chassis_control.real_speed[1] = 0;
	chassis_control.real_speed[2] = 0;
	chassis_control.real_speed[3] = 0;
}


/**
 * @date        3-November-2021
 * @brief       �����ܿ�
*/
void general_chassis_control()
{
	/*��������*/
	slow_dwon(&chassis_control);
	/*��XY�ٶ�����ת�ٶ�*/	
	calculate_speed(&chassis_control);
	/*���XYƫ���Ƕ�����ת�Ƕ�*/
	calculate_angle(&chassis_control);
	
	/*��ɲ�׶�ң������ֹ�ٿ�*/
	if(chassis_control.barke_angle == NO_BARKE)
	{
		/*��ʵ�ʿ��Ʋ���(���ƶ����)*/
		calculate_real(&chassis_motor[CHAS_FL],&chassis_control);
		calculate_real(&chassis_motor[CHAS_FR],&chassis_control);
		calculate_real(&chassis_motor[CHAS_BL],&chassis_control);
		calculate_real(&chassis_motor[CHAS_BR],&chassis_control);
	}
	/*ɲ���ж�*/
	motor_barke_check();
}


/**
 * @date        10-November-2021
 * @brief       ��������
*/
float last_move=0,last_tranlation=0,last_turn=0;
void slow_dwon(chassis_control_t *control)
{
	if(chassis_control.barke_angle == NO_BARKE)
	{
		/*�仯����ʱ����Ϊ�����仯����ֹ����ת��ͻ��*/
		if(control->move - last_move > CONSTRAIN)
			control->move = last_move + CONSTRAIN;
		else if(control->move - last_move< -CONSTRAIN)
			control->move =  last_move - CONSTRAIN;
		if(control->tranlation - last_tranlation > CONSTRAIN)
			control->tranlation = last_tranlation + CONSTRAIN;
		else if(control->tranlation - last_tranlation < -CONSTRAIN)
			control->tranlation = last_tranlation - CONSTRAIN;
		if(control->turn -last_turn > CONSTRAIN)
			control->turn = last_turn + CONSTRAIN;
		else if(control->turn - last_turn < -CONSTRAIN)
			control->turn = last_turn - CONSTRAIN;
	}
	/*Ϊ�˱���ɲ��ʱ���ӽǶȲ��䣬��Ҫ�̶��Ƕ�*/
	else
	{
		control->move =0;
		control->tranlation = 0;
		control->turn = 0;
	}
	last_move = control->move;
	last_tranlation = control->tranlation;
	last_turn = control->turn;
}


/**
 * @date        3-November-2021
 * @brief       ��XY�ٶ�����ת�ٶ�
 */
void calculate_speed(chassis_control_t *control)
{	
	control->XY_speed = max(abs(control->move),abs(control->tranlation));
	control->XY_speed = constrain(control->XY_speed,0,8000);
	control->turn_speed = abs(control->turn);
}


/**
 * @date        3-November-2021
 * @brief       ���XYƫ���Ƕ�����ת�Ƕ�
 */
void calculate_angle(chassis_control_t *control)
{
	if(abs(control->XY_speed) > 5)
		control->XY_angle = atan2(control->tranlation,control->move)/3.14159*180.f;
	/*angle��ΧΪ-180-180*/
	angle_constrain(&control->XY_angle);
	
	if(control->turn>0)
	{
		control->turn_angle[0] = 45;
		control->turn_angle[1] = 135;
		control->turn_angle[2] = -45;
		control->turn_angle[3] = -135;
	}
	else if(control->turn<0)
	{
		control->turn_angle[0] = -135;
		control->turn_angle[1] = -45;
		control->turn_angle[2] = 135;
		control->turn_angle[3] = 45;		
	}
}


/**
 * @date        4-November-2021
 * @brief       ��ʵ�ʿ��Ʋ���
 */
void calculate_real(chassis_motor_t *chas,chassis_control_t *control)
{
	/*�����ʵ�Ƕ�*/
	calculate_real_angle(control,chas->id);
	/*�����ʵ�ٶ�*/
	calculate_real_speed(control,chas->id);	
	/*�����ٶ�*/
	speed_constrain(control);
	/*�ͽ�ѡ�����Ƕ�(���ƶ����)*/
	angle_nearby(chas,control);
}


/**
 * @date        3-November-2021
 * @brief       ����ʵ�Ƕ�
 */
void calculate_real_angle(chassis_control_t *control,chassis_motor_cnt_t num)
{
	float err_angle;
	float x,y;
	err_angle = (control->turn_angle[num] - control->XY_angle)/180.f*3.14159f;
	x = control->turn_speed*sin(err_angle);
	y = control->turn_speed*cos(err_angle) + control->XY_speed;
	if(abs(x) > 1 || abs(y) > 1)
		control->real_angle[num] = control->XY_angle + atan2(x,y)*180.f/3.14159f;
	/*��ʵ�Ƕȷ�ΧΪ-180-180*/
	angle_constrain(&control->real_angle[num]);
}


/**
 * @date        3-November-2021
 * @brief       ����ʵ�ٶ�
 */
void calculate_real_speed(chassis_control_t *control,chassis_motor_cnt_t num)
{
	control->real_speed[num] = control->turn_speed * abs(cos((control->turn_angle[num] - control->real_angle[num])/180.f*3.14159f))\
	+ control->XY_speed * abs(cos((control->real_angle[num] - control->XY_angle)/180.f*3.14159f));
}


/**
 * @date        13-November-2021
 * @brief       ���ٴ���
 */
void speed_constrain(chassis_control_t *control)
{
	float max_real_speed = 0;
	for(uint8_t i = 0 ; i < 4 ; i ++)
	{
		if(max_real_speed < control->real_speed[i])
			max_real_speed = control->real_speed[i];
	}
	if(max_real_speed>8000)
	{
		/*���ڲ��٣����ж���ҪȨ��*/
		control->real_speed[CHAS_FL] = control->real_speed[CHAS_FL]/max_real_speed*8000.f;
		control->real_speed[CHAS_FR] = control->real_speed[CHAS_FR]/max_real_speed*8000.f;
		control->real_speed[CHAS_BL] = control->real_speed[CHAS_BL]/max_real_speed*8000.f;
		control->real_speed[CHAS_BR] = control->real_speed[CHAS_BR]/max_real_speed*8000.f;				
	}	
}


/**
 * @date        3-November-2021
 * @brief       �ͽ�ѡ�����Ƕ�
 */
void angle_nearby(chassis_motor_t *chas,chassis_control_t *control)
{
	/*Ŀ���뵱ǰ�Ƕȳ���95�ȶ��������䣬���������ת*/
	float err_angle;
	err_angle = control->real_angle[chas->id] + chas->mid_angle - chas->rudder_motor.info.angle/8192.f*360.f;
	angle_constrain(&err_angle);
	/*��ת*/
	if(abs(err_angle)>95)
	{
		chas->veer_state = REVERSAL;
		chas->angle_target = chas->mid_angle + control->real_angle[chas->id] + 180;
		switch(chas->id)
		{
			case CHAS_FR:
			case CHAS_BR:
				chas->speed_target = control->real_speed[chas->id];				
			break;
			case CHAS_FL:			
			case CHAS_BL:
				chas->speed_target = - control->real_speed[chas->id];
			break;
		}
	}
	/*��ת*/
	else
	{
		chas->veer_state = COROTATION;
		chas->angle_target = chas->mid_angle + control->real_angle[chas->id];
		switch(chas->id)
		{
			case CHAS_FR:	
			case CHAS_BR:
				chas->speed_target = - control->real_speed[chas->id];				
			break;
			case CHAS_FL:			
			case CHAS_BL:
				chas->speed_target = control->real_speed[chas->id];
			break;
		}
	}
	if(chas->angle_target>=360)
		chas->angle_target -= 360;
	else if(chas->angle_target>360)
		chas->angle_target += 360;
}


/**
 * @date        8-November-2021
 * @brief       ɲ��
 */
float car_angle,car_speed = 0,car_last_speed = 0;
uint32_t barke_time=0,judge_barke_time=0;
void motor_barke_check()
{
	/*������ٶ�*/
	car_last_speed = car_speed;
	car_speed = chassis_control.XY_speed;
	/*������Ƕ�*/
	if(chassis_control.turn > 0)
		car_angle = chassis_control.XY_angle + atan2(chassis_control.turn_speed,chassis_control.XY_speed);
	else
		car_angle = chassis_control.XY_angle - atan2(chassis_control.turn_speed,chassis_control.XY_speed);
	angle_constrain(&car_angle);


	/*-----------��һ�׶Σ��������ٽ���,ң�������Բٿ�-----------*/
	if((car_last_speed - car_speed >= CONSTRAIN)&&chassis_control.barke_angle==NO_BARKE)
		judge_barke_time ++;
	else if(chassis_control.barke_angle==NO_BARKE)
		judge_barke_time = 0;	
	
	
	/*-----------�ڶ��׶Σ����ٽ��ٽ�300ms���ȡ��ɲ��ң���������Բٿ�-----------*/
	if(judge_barke_time>300)
	{
		judge_barke_time = 0;
		barke_time = 0;
		chassis_motor[CHAS_FL].speed_target  = 0;
		chassis_motor[CHAS_FR].speed_target  = 0;
		chassis_motor[CHAS_BL].speed_target  = 0;
		chassis_motor[CHAS_BR].speed_target  = 0;
		/*�ְ˶�*/		
		if(car_angle<22.5f&&car_angle>-22.5f)
			chassis_control.barke_angle = F;
		else if(car_angle<67.5f&&car_angle>22.5f)
			chassis_control.barke_angle = RF;	
		else if(car_angle<112.5f&&car_angle>67.5f)
			chassis_control.barke_angle = R;
		else if(car_angle<157.5f&&car_angle>112.5f)
			chassis_control.barke_angle = RB;
		else if(car_angle<-157.5f||car_angle>157.5f)
			chassis_control.barke_angle = B;
		else if(car_angle<-112.5f&&car_angle>-157.5f)
			chassis_control.barke_angle = LB;
		else if(car_angle<-67.5f&&car_angle>-112.5f)
			chassis_control.barke_angle = L;
		else if(car_angle<-22.5f&&car_angle>-67.5f)
			chassis_control.barke_angle = LF;		
	}
	/*����ɲ���ֶ�ѡ�����Ƿ�ж��*/
	if(chassis_control.barke_angle == F || chassis_control.barke_angle == L || chassis_control.barke_angle == LF)
		chassis_motor[CHAS_FL].motor_barke= DISCHARGE;
	else
		chassis_motor[CHAS_FL].motor_barke = NORMAL;
	if(chassis_control.barke_angle == F || chassis_control.barke_angle == R || chassis_control.barke_angle == RF)
		chassis_motor[CHAS_FR].motor_barke = DISCHARGE;
	else
		chassis_motor[CHAS_FR].motor_barke = NORMAL;
	if(chassis_control.barke_angle == B || chassis_control.barke_angle == L || chassis_control.barke_angle == LB)
		chassis_motor[CHAS_BL].motor_barke = DISCHARGE;
	else
		chassis_motor[CHAS_BL].motor_barke = NORMAL;
	if(chassis_control.barke_angle == B || chassis_control.barke_angle == R || chassis_control.barke_angle == RB)
		chassis_motor[CHAS_BR].motor_barke = DISCHARGE;
	else
		chassis_motor[CHAS_BR].motor_barke = NORMAL;
	
	
	/*-----------�����׶Σ������ٶ�С��3000ת��ȡȫ��ɲ����ң���������Բٿ�-----------*/
	if(abs(chassis_motor[CHAS_FL].power_motor.info.speed)<3000&&abs(chassis_motor[CHAS_FR].power_motor.info.speed)<3000\
	&&abs(chassis_motor[CHAS_BL].power_motor.info.speed)<3000&&abs(chassis_motor[CHAS_BR].power_motor.info.speed)<3000)
	{
		barke_time ++;
		chassis_motor[CHAS_FL].motor_barke = NORMAL;
		chassis_motor[CHAS_FR].motor_barke = NORMAL;
		chassis_motor[CHAS_BL].motor_barke = NORMAL;
		chassis_motor[CHAS_BR].motor_barke = NORMAL;
	}
	
	
	/*----------���Ľ׶Σ������׶�200ms��ɲ����ɣ��˳�ɲ���׶Σ�ң�������²ٿ�-------*/
	if(barke_time>200&&chassis_control.barke_angle != NO_BARKE)
		chassis_control.barke_angle = NO_BARKE;
}

