/**
  * @auther whlphlg
  * @brief task.c��������Ҫ���е������������ļ���������ģ����ɶȽϴ�
  * @changedate 2020.09.13
  */
	
//����Ҫinclude���е�main.c�����
#include "main.h"
#include "gpio.h"
//�����������д�ģ�����include������
#include "task.h"
#include "sys.h"


void task1()
{
	//i=100+3;
	//HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
	//float j;
	for(int i=0;i<1000000;i++) 
	{
		float j=i/2333.f;
	}
}
void task2()
{
	System_TaskOut(TASK1,4000);
}
void task3()
{
	
}



