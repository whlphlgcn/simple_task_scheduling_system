#ifndef __SYS_H
#define __SYS_H

#include "main.h"

#define TASK_NUMBER 3    //任务数量
#define TASK1 0    //增加可读性
#define TASK2 1    //增加可读性
#define TASK3 2    //增加可读性

typedef struct
{
	uint8_t state;    //当前状态，1表示正常，0表示离线
	uint32_t time_load;    //重装载值
	uint32_t time_cnt;    //计数器，向下计数
}SoftWatchDogStruct;

void System_TaskInit(void);
void System_TaskUpdate(void);
void System_TaskRun(void);
void System_TaskOut(uint8_t task_number,uint32_t stop_time);
void System_TaskDisable(uint8_t task_number);
void System_TaskEnable(uint8_t task_number);

void System_SoftWatchDogInit(SoftWatchDogStruct *dog,uint32_t time_load,uint8_t default_state);
void System_SoftWatchDogFeed(SoftWatchDogStruct *dog);
void System_SoftWatchDogUpdate(SoftWatchDogStruct *dog);
uint8_t System_GetSoftWatchDogState(SoftWatchDogStruct *dog);

#endif /* __SYS_H */

