


# 欢迎您使用本开源任务轮询调度系统
***
## 功能描述：
+ 开启/关闭某任务。
+ 不同的任务可以设置不同的间隔运行时间
+ 将某一任务暂停一段时间
+ 粗略统计单个任务上一次的运行花费的时间，在1秒内占用cpu的百分比
+ 粗略统计所有要运行的任务在某一时刻占用cpu的时间
+ 每当有任务开始/停止，暂停/恢复时，都会进入相应的回调函数
+ 支持紧急停机
***
## 开发环境：
+ MDK
+ STM32CUBEMX
+ HAL库
***
## 项目结构简介：
把
+ System_Update();
+ \#include "sys.h"

放入
+ stm32f4xx_it.c
```c
/* USER CODE BEGIN Includes */
	#include "sys.h"
/* USER CODE END Includes */
void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */
		System_Update();
	/* USER CODE END SysTick_IRQn 0 */
		HAL_IncTick();
	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}
```

把
+ System_Init();
+ System_RunTask();
+ \#include "sys.h"

放入
+ main.c
```c
/* USER CODE BEGIN Includes */
	#include "sys.h"
/* USER CODE END Includes */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	System_Init();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		System_RunTask();
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}
```

+ sys.c 和 sys.h 不建议大家私自修改
+ task.c 和 task.h 可以由使用者修改


***
## 作者列表：
+ whlphlg
***
## 下载链接：
+ gitee：<https://gitee.com/whlphlg/simple_task_scheduling_system>
+ github：<https://github.com/whlphlgcn/simple_task_scheduling_system>
***
## 视频连接：
+ bilibili：<https://www.bilibili.com/video/BV1Va4y1j7p8>
***
## 历史版本：
+ 2020.09.15 第一代版本
***






