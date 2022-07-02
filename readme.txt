keyboard		主要编写键盘控制入口
rc_sensor		主要编写遥控器控制入口
auto_process	主要编写自动化流程
mode_switch	主要编写模式切换（自动化流程需要经过模式切换）
control		主要是复位入口、控制入口以及控制的重要数据
xx_control		主要是各模块的控制，负责模块的控制转为PID目标值以及各模块的复位
output		输出can、视觉、裁判系统数据
module		主要是存储各模块数据（PID、电机数据等）以及PID入口
device		作为模块以及通信协议的中间层负责如PID计算、初始数据(以及初始转化数据)保存等，其中也有部分通用函数如motor.c里的angle_constrain
protocol		通信协议，作为通信数据解读
driver		底层，与HAL库底层对接


控制逻辑: 
键盘：	control.c -> rc_sensor.c -> keyboard.c -> auto_process.c/mode_switch.c -> xx_control.c -> xx.c -> motor.c -> can_peotocol.c 
遥控器：   control.c -> rc_sensor.c ------------------------------------------------------> xx_control.c -> xx.c -> motor.c -> can_peotocol.c        
注：xx为clamp、tranlation、lifting、cabin、ground、save


             
接线原理： 舵轮底盘8电机:主控2can2 
	救援卡、下夹爪、抬升6电机：主控1can2
	矿仓、夹爪、横移5电机：主控1can1
	主控通信：主控1can1与主控2can1
	
主控1主要负责除底盘、视觉通信与裁判系统外的所有任务：两个电磁阀、三个光电门
主控2主要负责底盘、视觉通信与裁判系统通信


--------------------------------------------------上述为初代车代码介绍----------------------------------------------------------



                                  