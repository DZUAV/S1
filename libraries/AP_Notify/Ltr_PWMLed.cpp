/***********************************************************************************************************************
*文件功能：通过三个io直接控制led
*修改日期：2018-11-23
*修改作者：
*备注信息：--驱动文件配置
*************************************************************************************************************************/

#include "Ltr_PWMLed.h"

#include "AP_Notify.h"
#include <AP_GPS/AP_GPS.h>
#include <AP_HAL/AP_HAL.h>

extern const AP_HAL::HAL& hal;


/***********************************************************************************************************************
*函数原型：bool Ltr_PWMLed::init(void)
*函数功能：初始化
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/

bool Ltr_PWMLed::init(void)
{
	    // setup the main LEDs as outputs
	    hal.gpio->pinMode(HAL_GPIO_NUM_GPIOH_10, HAL_GPIOH_OUTPUT);
	    hal.gpio->pinMode(HAL_GPIO_NUM_GPIOH_11, HAL_GPIOH_OUTPUT);
	    hal.gpio->pinMode(HAL_GPIO_NUM_GPIOH_12, HAL_GPIOH_OUTPUT);

	    //配置PA7，配置IMU1的恒温控制
	    hal.gpio->pinMode(HAL_GPIO_NUM_GPIOA_7, HAL_GPIOH_OUTPUT); //初始化
	    //配置PA5，配置IMU2的恒温控制
	    hal.gpio->pinMode(HAL_GPIO_NUM_GPIOA_5, HAL_GPIOA_OUTPUT);
	    // 关闭所有的RGBLED
	    hal.gpio->write(HAL_GPIO_NUM_GPIOH_10, HAL_GPIOH_LED_OFF);  //PH10----R
	    hal.gpio->write(HAL_GPIO_NUM_GPIOH_11, HAL_GPIOH_LED_OFF);  //PH11----G
	    hal.gpio->write(HAL_GPIO_NUM_GPIOH_12, HAL_GPIOH_LED_OFF);  //PH12----B


	    //配置IMU1的 PA7输出低电平
	    hal.gpio->write(HAL_GPIO_NUM_GPIOA_7, HAL_GPIOA_IMU_CONT_TEMP_OFF);  //PA7
	    //配置PA5输出低电平
	    hal.gpio->write(HAL_GPIO_NUM_GPIOA_5, HAL_GPIOA7_LED_OFF);  //PA5

	    _counter=0;
	    ltr_pwmled_init_completed_flag=1;
	    return 1;

}



/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::update(void)
*函数功能：更新
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/
void Ltr_PWMLed::update(void)
{
  if(ltr_pwmled_init_completed_flag)
  {
	   // reduce update rate from 50hz to 10hz
	    _counter++;
	    if (_counter < 5)
	    {
	        return;
	    }
	    _counter = 0;

	    // internal counter used to control step of armed and gps led
	    _counter2++;
	    _counter3++;
	    _counter4++;
	    if (_counter2 >= 10)
	    {
	        _counter2 = 0;
	    }

	    if (_counter3 >= 6)
	    {
	        _counter3 = 0;
	    }
	    if (_counter4 >= 12)
	    {
	        _counter4 = 0;
	    }

		if (AP_Notify::flags.initialising)
		{

	        // blink arming and gps leds at 5hz
	        switch(_counter2)
	        {
	            case 0:
	            case 2:
	            case 4:
	            case 6:
	            case 8:
	            	hw_set_rgb(Ltr_PWMLed_Color_GREEN);
	                break;
	            case 1:
	            case 3:
	            case 5:
	            case 7:
	            case 9:
	            	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
	                break;
	        }
	        return;

		}


		else if (AP_Notify::flags.save_trim || AP_Notify::flags.esc_calibration)
		{

	        // blink arming and gps leds at 5hz
	        switch(_counter3)
	        {
	            case 1:
	            	   hw_set_rgb(Ltr_PWMLed_Color_RED);
	            case 3:
	            	   hw_set_rgb(Ltr_PWMLed_Color_GREEN);
	            case 5:
	            	   hw_set_rgb(Ltr_PWMLed_Color_BLUE);

	                   break;
	            case 0:
	            case 2:
	            case 4:
	            	   hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);

	                break;
	        }

		}
		   // radio and battery failsafe patter: flash yellow
		    // gps failsafe pattern : flashing yellow and blue
		    // ekf_bad pattern : flashing yellow and red
		else  if (AP_Notify::flags.failsafe_radio ||
		        AP_Notify::flags.failsafe_battery ||
		        AP_Notify::flags.ekf_bad ||
		        AP_Notify::flags.gps_glitching ||
		        AP_Notify::flags.leak_detected)
		    {
		        if (AP_Notify::flags.leak_detected)
		        {
		            // purple if leak detected
		            // blink arming and gps leds at 5hz
		            switch(_counter2)
		            {
		                case 0:
		                case 2:
		                case 4:
		                case 6:
		                case 8:
		                	hw_set_rgb(Ltr_PWMLed_Color_PURPLE);
		                    break;
		                case 1:
		                case 3:
		                case 5:
		                case 7:
		                case 9:
		                	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
		                    break;
		            }

		        } else if (AP_Notify::flags.ekf_bad)
		        {
		            // red on if ekf bad
		            switch(_counter2)
			            {
			                case 0:
			                case 2:
			                case 4:
			                case 6:
			                case 8:
			                	hw_set_rgb(Ltr_PWMLed_Color_RED);
			                    break;
			                case 1:
			                case 3:
			                case 5:
			                case 7:
			                case 9:
			                	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
			                    break;
			            }

		        } else if (AP_Notify::flags.gps_glitching)
		        {
		            // blue on gps glitch
		            switch(_counter2)
			            {
			                case 0:
			                case 2:
			                case 4:
			                case 6:
			                case 8:
			                	hw_set_rgb(Ltr_PWMLed_Color_BLUE);
			                    break;
			                case 1:
			                case 3:
			                case 5:
			                case 7:
			                case 9:
			                	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
			                    break;
			            }

		        }
		        else
		        {

		        	 // all off for radio or battery failsafe
		        	hw_set_rgb(Ltr_PWMLed_Color_YELLOW);

		        }



		    }

		else  if (AP_Notify::flags.armed)
		    {
		        // solid green if armed with GPS 3d lock
		        if (AP_Notify::flags.gps_status >= AP_GPS::GPS_OK_FIX_3D)
		        {
		        	hw_set_rgb(Ltr_PWMLed_Color_GREEN);

		        }
		        // solid blue if armed with no GPS lock
		        else
		        {

		        	hw_set_rgb(Ltr_PWMLed_Color_BLUE);
		        }

		    }


		else   if (!AP_Notify::flags.pre_arm_check)
		    {
	            switch(_counter2)
			            {
			                case 0:
			                case 2:
			                case 4:
			                case 6:
			                case 8:
			                	hw_set_rgb(Ltr_PWMLed_Color_YELLOW);
			                    break;
			                case 1:
			                case 3:
			                case 5:
			                case 7:
			                case 9:
			                	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
			                    break;
			            }

		    }

		else   if (AP_Notify::flags.gps_status >= AP_GPS::GPS_OK_FIX_3D_DGPS && AP_Notify::flags.pre_arm_gps_check)
		    {
	            switch(_counter2)
			            {
			                case 0:
			                case 2:
			                case 4:
			                case 6:
			                case 8:
			                	hw_set_rgb(Ltr_PWMLed_Color_GREEN);
			                    break;
			                case 1:
			                case 3:
			                case 5:
			                case 7:
			                case 9:
			                	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
			                    break;
			            }

		    }


		else  if (AP_Notify::flags.gps_status >= AP_GPS::GPS_OK_FIX_3D && AP_Notify::flags.pre_arm_gps_check)
		    {
	            switch(_counter2)
			            {
			                case 1:
			                case 2:
			                case 5:
			                case 6:
			                case 9:
			                case 10:
			                	hw_set_rgb(Ltr_PWMLed_Color_GREEN);
			                    break;
			                case 3:
			                case 4:
			                case 7:
			                case 8:
			                case 11:
			                case 0:
			                	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
			                    break;
			            }

		    }

		else
		{

	        switch(_counter2)
		            {
		                case 1:
		                case 2:
		                case 5:
		                case 6:
		                case 9:
		                case 10:
		                	hw_set_rgb(Ltr_PWMLed_Color_BLUE);
		                    break;
		                case 3:
		                case 4:
		                case 7:
		                case 8:
		                case 11:
		                case 0:
		                	hw_set_rgb(Ltr_PWMLed_Color_ALLOFF);
		                    break;
		            }
		}


  }
  else
  {

	  return ;
  }

}


/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::hw_set_rgb(uint8_t rgb_color)
*函数功能：设定颜色
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/
void Ltr_PWMLed::hw_set_rgb(uint8_t rgb_color)
{
	 switch (rgb_color)
	 {
		 case Ltr_PWMLed_Color_RED:   //红色
			 set_rgbled_red_on();
			 set_rgbled_green_off();
			 set_rgbled_blue_off();
			   break;
		 case Ltr_PWMLed_Color_GREEN: //绿色
			 set_rgbled_red_off();
			 set_rgbled_green_on();
			 set_rgbled_blue_off();
		       break;
		 case Ltr_PWMLed_Color_BLUE:  //蓝色
			 set_rgbled_red_off();
			 set_rgbled_green_off();
			 set_rgbled_blue_on();
			   break;
		 case Ltr_PWMLed_Color_YELLOW://黄色 ：红+绿
			 set_rgbled_red_on();
			 set_rgbled_green_on();
			 set_rgbled_blue_off();
		       break;
		 case Ltr_PWMLed_Color_PURPLE://紫色 ：红+蓝
			 set_rgbled_red_on();
			 set_rgbled_green_off();
			 set_rgbled_blue_on();
			   break;
		 case Ltr_PWMLed_Color_CYAN: //青色 ：蓝+绿
			 set_rgbled_red_off();
			 set_rgbled_green_on();
			 set_rgbled_blue_on();
		       break;
		 case Ltr_PWMLed_Color_ALLOFF: //青色 ：蓝+绿
				 set_rgbled_red_off();
				 set_rgbled_green_off();
				 set_rgbled_blue_off();
			       break;
		 default:
			 break;
	 }


}

/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::set_rgbled_red_on(void)
*函数功能：打开红色
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/

void Ltr_PWMLed::set_rgbled_red_on(void)
{
	hal.gpio->write(HAL_GPIO_NUM_GPIOH_10, HAL_GPIOH_LED_ON);

}
/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::set_rgbled_red_off(void)
*函数功能：关闭红色
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/
void Ltr_PWMLed::set_rgbled_red_off(void)
{
	hal.gpio->write(HAL_GPIO_NUM_GPIOH_10, HAL_GPIOH_LED_OFF);

}

/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::set_rgbled_green_on(void)
*函数功能：打开绿色
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/
void Ltr_PWMLed::set_rgbled_green_on(void)
{
	hal.gpio->write(HAL_GPIO_NUM_GPIOH_11, HAL_GPIOH_LED_ON);
}

/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::set_rgbled_green_off(void)
*函数功能：关闭绿色
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/
void Ltr_PWMLed::set_rgbled_green_off(void)
{

	hal.gpio->write(HAL_GPIO_NUM_GPIOH_11, HAL_GPIOH_LED_OFF);
}

/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::set_rgbled_blue_on(void)
*函数功能：打开蓝色
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/
void Ltr_PWMLed::set_rgbled_blue_on(void)
{
	hal.gpio->write(HAL_GPIO_NUM_GPIOH_12, HAL_GPIOH_LED_ON);

}

/***********************************************************************************************************************
*函数原型：void Ltr_PWMLed::set_rgbled_blue_off(void)
*函数功能：关闭蓝色led
*修改日期：2018-11-23
*修改作者：
*备注信息：
*************************************************************************************************************************/
void Ltr_PWMLed::set_rgbled_blue_off(void)
{
	hal.gpio->write(HAL_GPIO_NUM_GPIOH_12, HAL_GPIOH_LED_OFF);
}


/**********************************************************************************************************************
*                             file-end
***********************************************************************************************************************/

