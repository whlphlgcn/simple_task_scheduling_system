#ifndef __SYS_H
#define __SYS_H

#include "main.h"

#define TASK_NUMBER 3    //��������
#define TASK1 0    //���ӿɶ���
#define TASK2 1    //���ӿɶ���
#define TASK3 2    //���ӿɶ���

typedef struct
{
	uint8_t state;    //��ǰ״̬��1��ʾ������0��ʾ����
	uint32_t time_load;    //��װ��ֵ
	uint32_t time_cnt;    //�����������¼���
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

