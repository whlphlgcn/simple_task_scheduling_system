#ifndef __TASK_H
#define __TASK_H

#include "main.h"

void task1(void);
void task2(void);
void task3(void);

__weak void Task_DisableCallback(uint8_t task_number);
__weak void Task_EnableCallback(uint8_t task_number);
__weak void Task_OutCallback(uint8_t task_number);
__weak void Task_InCallback(uint8_t task_number);

#endif /* __TASK_H */
