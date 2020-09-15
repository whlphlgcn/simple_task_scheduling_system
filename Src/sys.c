/**
  * @auther whlphlg
  * @brief sys.c用来定义有关控制系统，运行任务的函数
	*        uwTick 变量可以通过 HAL_GetTick() 函数来获取，这里为了优化，直接用uwTick(只读)，不要随意改变uwTick的值！！！
	*        uwTick 每隔1ms自增一次，只要#include "main.h"就能直接用uwTick，是hal库自带的！！！不是我声明和定义的！！！
  * @changedate 2020.09.13
  */

//下面的是cubemx生成的，必须include！！！
#include "main.h"
//下面的是作者写的，必须include！！！
#include "sys.h"
#include "task.h"

TaskStruct task_structs[TASK_NUMBER];    //声明任务的结构体数组

/**
  * @auther whlphlg
  * @brief 系统初始化，给每一个任务结构体里面的enable_flag,interval_time,task_priority,f赋初值，其他的元素单片机会默认是0
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
  * @brief 运行任务并统计单个任务运行的 usage_time,usage_percent,和此函数运行一次的total_usage_time
  * @changedate 2020.09.13
  */
int32_t total_usage_tick;    // 这个是此函数运行一次的总tick，这里声明成全局变量，方便debug
float total_usage_time;    // 这个是此函数运行一次的总时间(单位us)，这里声明成全局变量，方便debug
int32_t total_usage_tick_max_in1s;    // total_usage_tick 在某1秒内的最大值
float total_usage_time_max_in1s;    // total_usage_time 在某1秒内的最大值

void System_RunTask()
{
	int64_t total_begin_tick = SysTick->VAL - uwTick*(SysTick->LOAD+1);    // 思路同计算 begin_tick
	
	for(uint8_t i=0;i<TASK_NUMBER;i++)
	{
		if(task_structs[i].ready_flag)
		{
			task_structs[i].ready_flag--;
			// 计算 begin_tick 的公式 = ↓ >0的整数 ↓ - ↓------ <0的整数-------↓ (begin_tick < 0)
			int64_t begin_tick        = SysTick->VAL - uwTick*(SysTick->LOAD+1);
			
			task_structs[i].f();    // 运行任务结构体对应的任务函数
			
			//  计算 usage_tick 的公式 = 开始的tick - ↓---------------现在的tick--------------↓(两个tick都是负整数，且 开始的tick > 现在的tick)
			task_structs[i].usage_tick = begin_tick - (SysTick->VAL - uwTick*(SysTick->LOAD+1));
			//  计算 usage_time 的公式 = ↓-----------usage_tick----------↓ / ↓1ms内tick总数--↓ * 1000(单位换算，1ms换算成1000us)
			task_structs[i].usage_time = (float)task_structs[i].usage_tick / (SysTick->LOAD+1) * 1000.f;
			//  计算 usage_percent 的公式 = ↓-----任务每秒运行次数(单位Hz)-----↓ * ↓---任务单次运行所需时间(单位s)----↓ * 100(最后要变成%,所以*100)
			task_structs[i].usage_percent = 1000.f/task_structs[i].interval_time * task_structs[i].usage_time/1000000.f * 100;
		}
	}
	
	total_usage_tick = total_begin_tick - (SysTick->VAL - uwTick*(SysTick->LOAD+1));    // 思路同计算 usage_tick
	total_usage_time = (float)total_usage_tick / (SysTick->LOAD+1) * 1000.f;    // 思路同计算 usage_time
	
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
  * @brief 系统更新，每隔1ms从滴答定时器中断进入一次 (在stm32f4xx_it.c的 System_Update()里被调用)
	*        通过uwTick,enable_flag,stop_time的值来确定ready_flag并更新last_time,stop_time
	*        每隔1000ms重置一次 total_usage_tick_max_in1s 和 total_usage_time_max_in1s
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
				Task_InCallback(i);    // 有任务从暂停中恢复，恢复前触发对应的回调函数
			}
			task_structs[i].out_time--;    //更新 out_time
		}
		// 只有下面3个都达到条件才确定 ready_flag 并更新 last_time
		if(task_structs[i].enable_flag &&
			task_structs[i].out_time == 0 &&
			task_structs[i].interval_time<=(uwTick-task_structs[i].last_time))
		{
			task_structs[i].ready_flag++;
			task_structs[i].last_time=uwTick;
		}
	}
	
	if(uwTick%1000 == 0)    // 每隔1000ms重置一次 total_usage_tick_max_in1s 和 total_usage_time_max_in1s
	{
		total_usage_tick_max_in1s = total_usage_tick;
		total_usage_time_max_in1s = total_usage_time;
	}
}

/**
  * @auther whlphlg
  * @brief 让指定任务暂停一段时间，并触发对应的回调函数
  * @changedate 2020.09.13
  */
void System_TaskOut(uint8_t task_number,uint32_t stop_time)
{
	task_structs[task_number].out_time = stop_time;
	Task_OutCallback(task_number);
}

/**
  * @auther whlphlg
  * @brief 让指定任务关闭，并触发对应的回调函数
  * @changedate 2020.09.13
  */
void System_TaskDisable(uint8_t task_number)
{
	task_structs[task_number].enable_flag = 0;
	Task_DisableCallback(task_number);
}

/**
  * @auther whlphlg
  * @brief 让指定任务开启，并触发对应的回调函数
  * @changedate 2020.09.13
  */
void System_TaskEnable(uint8_t task_number)
{
	task_structs[task_number].enable_flag = 1;
	Task_EnableCallback(task_number);
}

/**
  * @auther whlphlg
  * @brief 系统强制停机
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





