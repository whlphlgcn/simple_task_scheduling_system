/**
  * @auther whlphlg
  * @brief sys.c���������йؿ���ϵͳ����������ĺ���
	*        uwTick ��������ͨ�� HAL_GetTick() ��������ȡ������Ϊ���Ż���ֱ����uwTick(ֻ��)����Ҫ����ı�uwTick��ֵ������
	*        uwTick ÿ��1ms����һ�Σ�ֻҪ#include "main.h"����ֱ����uwTick����hal���Դ��ģ����������������Ͷ���ģ�����
  * @changedate 2020.09.13
  */

//�������cubemx���ɵģ�����include������
#include "main.h"
//�����������д�ģ�����include������
#include "sys.h"
#include "task.h"

TaskStruct task_structs[TASK_NUMBER];    //��������Ľṹ������

/**
  * @auther whlphlg
  * @brief ϵͳ��ʼ������ÿһ������ṹ�������enable_flag,interval_time,task_priority,f����ֵ��������Ԫ�ص�Ƭ����Ĭ����0
  * @changedate 2020.09.13
  */
void System_Init()
{
	task_structs[TASK1].enable_flag=1;
	task_structs[TASK1].interval_time=500;
	task_structs[TASK1].f=task1;
	
	task_structs[TASK2].enable_flag=1;
	task_structs[TASK2].interval_time=10000;
	task_structs[TASK2].f=task2;
	
	task_structs[TASK3].enable_flag=1;
	task_structs[TASK3].interval_time=5;
	task_structs[TASK3].f=task3;
}

/**
  * @auther whlphlg
  * @brief ��������ͳ�Ƶ����������е� usage_time,usage_percent,�ʹ˺�������һ�ε�total_usage_time
  * @changedate 2020.09.13
  */
int32_t total_usage_tick;    // ����Ǵ˺�������һ�ε���tick������������ȫ�ֱ���������debug
float total_usage_time;    // ����Ǵ˺�������һ�ε���ʱ��(��λus)������������ȫ�ֱ���������debug
int32_t total_usage_tick_max_in1s;    // total_usage_tick ��ĳ1���ڵ����ֵ
float total_usage_time_max_in1s;    // total_usage_time ��ĳ1���ڵ����ֵ

void System_RunTask()
{
	int64_t total_begin_tick = SysTick->VAL - uwTick*(SysTick->LOAD+1);    // ˼·ͬ���� begin_tick
	
	for(uint8_t i=0;i<TASK_NUMBER;i++)
	{
		if(task_structs[i].ready_flag)
		{
			task_structs[i].ready_flag--;
			// ���� begin_tick �Ĺ�ʽ = �� >0������ �� - ��------ <0������-------�� (begin_tick < 0)
			int64_t begin_tick        = SysTick->VAL - uwTick*(SysTick->LOAD+1);
			
			task_structs[i].f();    // ��������ṹ���Ӧ��������
			
			//  ���� usage_tick �Ĺ�ʽ = ��ʼ��tick - ��---------------���ڵ�tick--------------��(����tick���Ǹ��������� ��ʼ��tick > ���ڵ�tick)
			task_structs[i].usage_tick = begin_tick - (SysTick->VAL - uwTick*(SysTick->LOAD+1));
			//  ���� usage_time �Ĺ�ʽ = ��-----------usage_tick----------�� / ��1ms��tick����--�� * 1000(��λ���㣬1ms�����1000us)
			task_structs[i].usage_time = (float)task_structs[i].usage_tick / (SysTick->LOAD+1) * 1000.f;
			//  ���� usage_percent �Ĺ�ʽ = ��-----����ÿ�����д���(��λHz)-----�� * ��---���񵥴���������ʱ��(��λs)----�� * 100(���Ҫ���%,����*100)
			task_structs[i].usage_percent = 1000.f/task_structs[i].interval_time * task_structs[i].usage_time/1000000.f * 100;
		}
	}
	
	total_usage_tick = total_begin_tick - (SysTick->VAL - uwTick*(SysTick->LOAD+1));    // ˼·ͬ���� usage_tick
	total_usage_time = (float)total_usage_tick / (SysTick->LOAD+1) * 1000.f;    // ˼·ͬ���� usage_time
	
	if(total_usage_tick_max_in1s < total_usage_tick)
	{
		total_usage_tick_max_in1s = total_usage_tick;
	}
	if(total_usage_time_max_in1s < total_usage_time)
	{
		total_usage_time_max_in1s = total_usage_time;
	}
}

/**
  * @auther whlphlg
  * @brief ϵͳ���£�ÿ��1ms�ӵδ�ʱ���жϽ���һ�� (��stm32f4xx_it.c�� System_Update()�ﱻ����)
	*        ͨ��uwTick,enable_flag,stop_time��ֵ��ȷ��ready_flag������last_time,stop_time
	*        ÿ��1000ms����һ�� total_usage_tick_max_in1s �� total_usage_time_max_in1s
  * @changedate 2020.09.15
  */
void System_Update()
{
	for(uint8_t i=0;i<TASK_NUMBER;i++)
	{
		if(task_structs[i].out_time != 0)
		{
			if(task_structs[i].out_time == 1)
			{
				Task_InCallback(i);    // ���������ͣ�лָ����ָ�ǰ������Ӧ�Ļص�����
			}
			task_structs[i].out_time--;    //���� out_time
		}
		// ֻ������3�����ﵽ������ȷ�� ready_flag ������ last_time
		if(task_structs[i].enable_flag &&
			task_structs[i].out_time == 0 &&
			task_structs[i].interval_time<=(uwTick-task_structs[i].last_time))
		{
			task_structs[i].ready_flag++;
			task_structs[i].last_time=uwTick;
		}
	}
	
	if(uwTick%1000 == 0)    // ÿ��1000ms����һ�� total_usage_tick_max_in1s �� total_usage_time_max_in1s
	{
		total_usage_tick_max_in1s = total_usage_tick;
		total_usage_time_max_in1s = total_usage_time;
	}
}

/**
  * @auther whlphlg
  * @brief ��ָ��������ͣһ��ʱ�䣬��������Ӧ�Ļص�����
  * @changedate 2020.09.13
  */
void System_TaskOut(uint8_t task_number,uint32_t stop_time)
{
	task_structs[task_number].out_time = stop_time;
	Task_OutCallback(task_number);
}

/**
  * @auther whlphlg
  * @brief ��ָ������رգ���������Ӧ�Ļص�����
  * @changedate 2020.09.13
  */
void System_TaskDisable(uint8_t task_number)
{
	task_structs[task_number].enable_flag = 0;
	Task_DisableCallback(task_number);
}

/**
  * @auther whlphlg
  * @brief ��ָ������������������Ӧ�Ļص�����
  * @changedate 2020.09.13
  */
void System_TaskEnable(uint8_t task_number)
{
	task_structs[task_number].enable_flag = 1;
	Task_EnableCallback(task_number);
}

/**
  * @auther whlphlg
  * @brief ϵͳǿ��ͣ��
  * @changedate 2020.09.14
  */
void System_AllTaskDisable()
{
	while(1)
	{
		for(uint8_t i=0;i<TASK_NUMBER;i++)
		{
			task_structs[i].enable_flag = 0;
		}
	}
}





