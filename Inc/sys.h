#include "main.h"

#define TASK_NUMBER 3    //��������
#define TASK1 0    //���ӿɶ���
#define TASK2 1    //���ӿɶ���
#define TASK3 2    //���ӿɶ���

typedef struct
{
	uint8_t enable_flag;       // �����Ƿ񱻿��������񲻱������޷����� (��0Ϊ�����Ѿ�����)
	uint16_t interval_time;    // �������м��ʱ�� (��λms)
	uint32_t last_time;        // ������һ������ʱ�� (��λms)
	uint8_t ready_flag;        // �����Ƿ�׼�������� (��0Ϊ�����Ѿ�����)
	uint32_t out_time;         // ������ʱֹͣ���ж���ʱ�� (��λms)
	int32_t usage_tick;        // �������ӿ�ʼ������ʱ���δ�ʱ����VAL(CNT)�Լ����� (���sys.c����� System_RunTask())
	float usage_time;          // ������һ������ʱ�����ʱ�䣬��λΪus����λΪus����λΪus�� (���㷽����sys.c����� System_RunTask())
	float usage_percent;       // ������1����ռ��cpu�İٷֱ� (���㷽����sys.c����� System_RunTask())
	void (*f)(void);           // ָ���������Ҫ���еĺ��� (��֪����ôָ��sys.c����� System_Init())
}TaskStruct;


void System_Init(void);
void System_Update(void);
void System_RunTask(void);
void System_TaskOut(uint8_t task_number,uint32_t stop_time);
void System_TaskDisable(uint8_t task_number);
void System_TaskEnable(uint8_t task_number);



