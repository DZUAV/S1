/*
 * Ltr_PWMLed.h
 *
 *  Created on: Nov 22, 2018
 *      Author: coco
 */

#ifndef ARDUPILOT_LIBRARIES_AP_NOTIFY_LTR_PWMLED_H_
#define ARDUPILOT_LIBRARIES_AP_NOTIFY_LTR_PWMLED_H_


#include <AP_HAL/AP_HAL.h>
#include "NotifyDevice.h"

#define HAL_GPIO_NUM_GPIOH_10  81    //定义引脚PH10
#define HAL_GPIO_NUM_GPIOH_11  82    //定义引脚PH11
#define HAL_GPIO_NUM_GPIOH_12  83    //定义引脚PH12
#define HAL_GPIO_NUM_GPIOA_7   84     //定义引脚PA7


#define HAL_GPIO_NUM_GPIOA_5   58   //定义引脚PA5

#define HAL_GPIOH_INPUT  0           //定义GPIO是输入模式
#define HAL_GPIOH_OUTPUT 1           //定义GPIO是输出模式

#define HAL_GPIOA_INPUT  0           //定义GPIO是输入模式
#define HAL_GPIOA_OUTPUT 1           //定义GPIO是输出模式


#define HAL_GPIOH_LED_ON       0     //点亮LED
#define HAL_GPIOH_LED_OFF      1    //关闭LED

#define HAL_GPIOA7_LED_ON       1
#define HAL_GPIOA7_LED_OFF      0

#define HAL_GPIOA_IMU_CONT_TEMP_ON       1     //关闭
#define HAL_GPIOA_IMU_CONT_TEMP_OFF      0    //关闭LED

#define LOW 0

enum
{
	 Ltr_PWMLed_Color_RED,     //红色
	 Ltr_PWMLed_Color_GREEN,   //绿色
	 Ltr_PWMLed_Color_BLUE,    //蓝色
	 Ltr_PWMLed_Color_YELLOW,  //黄色 ：红+绿
	 Ltr_PWMLed_Color_PURPLE,  //紫色 ：红+蓝
	 Ltr_PWMLed_Color_CYAN,    //青色 ：蓝+绿
	 Ltr_PWMLed_Color_ALLOFF,    //青色 ：蓝+绿
};
enum
{
	 Ltr_PWMLed_Color_Update_Frequency_20MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_40MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_60MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_100MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_200MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_300MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_400MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_500MS,     //红色
	 Ltr_PWMLed_Color_Update_Frequency_1000MS,    //红色

};

class Ltr_PWMLed: public NotifyDevice
{
public:
    // initialise the LED driver
    bool init(void) override;

    // shoul  be called at 50Hz
    void update(void) override;
    void set_rgbled_red_on(void);
    void set_rgbled_red_off(void);
    void set_rgbled_green_on(void);
    void set_rgbled_green_off(void);
    void set_rgbled_blue_on(void);
    void set_rgbled_blue_off(void);
    void hw_set_rgb(uint8_t rgb_color);
private:
    // counter incremented at 50Hz
    uint8_t _counter;
    uint8_t ltr_pwmled_init_completed_flag;
    uint8_t _counter2;
    uint8_t _counter3;
    uint8_t _counter4;

};



#endif /* ARDUPILOT_LIBRARIES_AP_NOTIFY_LTR_PWMLED_H_ */
