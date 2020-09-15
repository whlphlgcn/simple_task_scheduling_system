#include "main.h"

#define TASK_NUMBER 3    //任务数量
#define TASK1 0    //增加可读性
#define TASK2 1    //增加可读性
#define TASK3 2    //增加可读性

typedef struct
{
	uint8_t enable_flag;       // 任务是否被开启，任务不被开启无法运行 (非0为任务已经开启)
	uint16_t interval_time;    // 任务运行间隔时间 (单位ms)
	uint32_t last_time;        // 任务上一次运行时间 (单位ms)
	uint8_t ready_flag;        // 任务是否准备好运行 (非0为任务已经开启)
	uint32_t out_time;         // 任务暂时停止运行多少时间 (单位ms)
	int32_t usage_tick;        // 任务函数从开始到结束时，滴答定时器内VAL(CNT)自减次数 (详见sys.c里面的 System_RunTask())
	float usage_time;          // 任务上一次运行时所需的时间，单位为us！单位为us！单位为us！ (计算方法见sys.c里面的 System_RunTask())
	float usage_percent;       // 任务在1秒内占用cpu的百分比 (计算方法见sys.c里面的 System_RunTask())
	void (*f)(void);           // 指向该任务需要运行的函数 (不知道怎么指见sys.c里面的 System_Init())
}TaskStruct;


void System_Init(void);
void System_Update(void);
void System_RunTask(void);
void System_TaskOut(uint8_t task_number,uint32_t stop_time);
void System_TaskDisable(uint8_t task_number);
void System_TaskEnable(uint8_t task_number);



