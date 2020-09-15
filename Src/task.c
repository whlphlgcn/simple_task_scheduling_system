/**
  * @auther whlphlg
  * @brief task.c用来定义要运行的任务函数，该文件可随意更改，自由度较大
  * @changedate 2020.09.13
  */
	
//至少要include所有的main.c里面的
#include "main.h"
#include "gpio.h"
//下面的是作者写的，必须include！！！
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



