/*
 * AP_Baro_SPL06_001.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: coco
 */


#include "AP_Baro_SPL06_001.h"

#include <utility>
#include <stdio.h>

#include <AP_Math/AP_Math.h>

extern const AP_HAL::HAL &hal;
static uint8_t _count=0;
/****************************************************************************************************************************
*函数原型：AP_BARO_SPL06_001::AP_BARO_SPL06_001(AP_Baro &baro, AP_HAL::OwnPtr<AP_HAL::Device> dev)
*函数功能：构造函数
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
AP_BARO_SPL06_001::AP_BARO_SPL06_001(AP_Baro &baro, AP_HAL::OwnPtr<AP_HAL::Device> dev)
    : AP_Baro_Backend(baro)
    , _dev(std::move(dev))
{

}

/****************************************************************************************************************************
*函数原型：AP_Baro_Backend *AP_BARO_SPL06_001::probe(AP_Baro &baro,AP_HAL::OwnPtr<AP_HAL::Device> dev)
*函数功能：识别传感器
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
AP_Baro_Backend *AP_BARO_SPL06_001::probe(AP_Baro &baro,AP_HAL::OwnPtr<AP_HAL::Device> dev)
{
    if (!dev)
    {
        return nullptr;
    }
    dev->set_read_flag(0x80); //这个必须要设置，不然读取ID会错误

    AP_BARO_SPL06_001 *sensor = new AP_BARO_SPL06_001(baro, std::move(dev));
    if (!sensor || !sensor->_init())
    {
        delete sensor;
        return nullptr;
    }
    return sensor;
}
/****************************************************************************************************************************
*函数原型：AP_Baro_Backend *AP_BARO_SPL06_001::probe(AP_Baro &baro,AP_HAL::OwnPtr<AP_HAL::Device> dev)
*函数功能：识别传感器
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/

bool AP_BARO_SPL06_001::_init()
{
	uint8_t whoami=0;

	uint8_t coef[18];
//	const char *name = "SPL06_001";
	  _state = 0;
	  _count=0;
	  Spl06_Temp = 0;
	  Spl06_Press = 0;
	  _raw_temp=0;
	  _raw_press=0;
	  _tmp_sum = 0;
	  _prs_sum = 0;
	  _tmp_count = 0;
	  _prs_count = 0;
	  _pressure_samples=0;
	  _updated=0;
    if (!_dev)
    {
    	hal.console->printf("SPL06_001 no device\r\n");
        return false;
    }

    if (!_dev->get_semaphore()->take(HAL_SEMAPHORE_BLOCK_FOREVER))
    {
        AP_HAL::panic("PANIC: AP_BARO_SPL06_001: failed to take serial semaphore for init");
    }
//    _dev->write_register(SPL06_001_RESET, 0x89); //开始进行温度转换
//    hal.scheduler->delay(100);

    _dev->read_registers(SPL06_001_ID, &whoami, 1);    //读取ID上面的值，这里是0x10=16
    //_dev->read_registers(0X00, &whoami, 1);    //读取ID上面的值，这里是0x10=16
    hal.console->printf("whoami=%d\r\n",whoami);
    if(whoami !=16)
    {
    	 hal.console->printf("读取SPL06_001设备的ID错误\r\n");
    	return false;

    }
    else
    {
//    	hal.uartC->printf("whoami=%d\r\n",whoami);
//    	hal.uartC->printf("%s found on bus %u address 0x%02x\n", name, _dev->bus_num(), _dev->get_bus_address());
    	//开始读取ROM值
    	_dev->read_registers(SPL06_001_COEF, coef, 18); //读取ID上面的值
        _C0 = ((int16_t)coef[0] << 4) + ((coef[1] & 0xF0) >> 4);
    	_C0 = (_C0 & 0x0800) ? (0xF000 | _C0) : _C0;

		_C1 = ((int16_t)(coef[1] & 0x0F) << 8) + coef[2];
		_C1 = (_C1 & 0x0800) ? (0xF000 | _C1) : _C1;

		_C00 = ((int32_t)coef[3] << 12) + ((uint32_t)coef[4] << 4) + (coef[5] >> 4);
		_C00 = (_C00 & 0x080000) ? (0xFFF00000 | _C00) : _C00;

		_C10 = ((int32_t)(coef[5] & 0x0F) << 16) + ((uint32_t)coef[6] << 8) + coef[7];
		_C10 = (_C10 & 0x080000) ? (0xFFF00000 | _C10) : _C10;

		_C01 = ((int16_t)coef[8] << 8) + coef[9];

		_C11 = ((int16_t)coef[10] << 8) + coef[11];
		_C11 = (_C11 & 0x0800) ? (0xF000 | _C11) : _C11;

		_C20 = ((int16_t)coef[12] << 8) + coef[13];
		_C20 = (_C20 & 0x0800) ? (0xF000 | _C20) : _C20;

		_C21 = ((int16_t)coef[14] << 8) + coef[15];
		_C21 = (_C21 & 0x0800) ? (0xF000 | _C21) : _C21;

		_C30 = ((int16_t)coef[16] << 8) + coef[17];
		_C30 = (_C30 & 0x0800) ? (0xF000 | _C30) : _C30;


//		hal.uartC->printf("_C0=%d\r\n",_C0);
//		hal.uartC->printf("_C1=%d\r\n",_C1);
//		hal.uartC->printf("_C00=%d\r\n",_C00);
//		hal.uartC->printf("_C10=%d\r\n",_C10);
//		hal.uartC->printf("_C01=%d\r\n",_C01);
//		hal.uartC->printf("_C11=%d\r\n",_C11);
//		hal.uartC->printf("_C20=%d\r\n",_C20);
//		hal.uartC->printf("_C21=%d\r\n",_C21);
//		hal.uartC->printf("_C30=%d\r\n",_C30);
//		hal.uartC->printf("spl06_001: startup OK\n");
//		hal.uartC->printf("_dev->get_bus_id()=%d\r\n",_dev->get_bus_id());

		  //配置气压计的压力和温度
		  spl06_config_pressure(PM_RATE_128,PM_PRC_64);
		  spl06_config_temperature(TMP_RATE_32,TMP_PRC_8);
		  _dev->write_register(SPL06_001_MEAS_CFG, MEAS_CTRL_ContinuousPressTemp); //开始进行温度转换
		  hal.scheduler->delay(40);

		  // lower retries for run
		  _dev->set_retries(3);
		  _instance = _frontend.register_sensor();
		  _dev->get_semaphore()->give();
		// use 10ms to ensure we don't lose samples, with max lag of 10ms
		_dev->register_periodic_callback(10000, FUNCTOR_BIND_MEMBER(&AP_BARO_SPL06_001::_timer, void));

		return true;

    }



}

/****************************************************************************************************************************
*函数原型：void AP_BARO_SPL06_001::update()
*函数功能：配置温度
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
void AP_BARO_SPL06_001::update()
{

	if(_updated)
	{
		_updated = false;

		_calculate_spl06();

	}


}

/****************************************************************************************************************************
*函数原型：void AP_BARO_SPL06_001::_timer(void)
*函数功能：配置气压计定时器
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/

void AP_BARO_SPL06_001::_timer(void)
{

	_count++;
	hal.uartC->printf("_count=%d\r\n",_count);
	if (_state == 0)
	{
		_state++;
		_raw_temp = spl06_get_temperature_adc(); //获取温度
		_tmp_sum=+_raw_temp;
		_tmp_count++;

//		hal.uartC->printf("_raw_temp=%8.8f\r\n",_raw_temp);
		_dev->write_register(SPL06_001_MEAS_CFG, MEAS_CTRL_PressMeasure); //开始进行压力转换;
	}
	else
	{

		_raw_press = spl06_get_pressure_adc(); //获取压力
//		_prs_sum=+_raw_press;
//		_prs_count++;
	    _dev->write_register(SPL06_001_MEAS_CFG, MEAS_CTRL_TempMeasure); //开始进行温度转换
		_state = 0;



	}
	if(_count>=10)
	{
//		 WITH_SEMAPHORE(_sem);
		 _updated = true;
		 _raw_press=(int32_t)(_prs_sum/_prs_count);
//		 _raw_press=(int32_t)(_tmp_sum/_tmp_count);
		 _tmp_count=0;
		 _prs_count=0;
		 _tmp_sum=0;
		 _prs_sum=0;
		 hal.uartC->printf("_count=%d\r\n",_count);
		 _count=0;

	}


//	if(_count>=10)
//	{
//		 _count=0;
//		 _raw_press=(int32_t)(_prs_sum/_prs_count);
//		 _raw_press=(int32_t)(_tmp_sum/_tmp_count);
//		 _tmp_count=0;
//		 _prs_count=0;
//		 _tmp_sum=0;
//		 _prs_sum=0;
//		 _updated = true;
//
//	}
//	else
//	{
//
//		_updated = false;
//	}

}
/****************************************************************************************************************************
*函数原型：void AP_BARO_SPL06_001::_calculate_spl06()
*函数功能：配置温度
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
void AP_BARO_SPL06_001::_calculate_spl06()
{
	float Traw_src, Praw_src;
	float qua2, qua3;
//	hal.uartC->printf("$$$$$$$$$$$$$$$$\r\n");
//	hal.uartC->printf("_raw_temp=%8.8f\r\n",_raw_temp);
//	hal.uartC->printf("_raw_press=%8.8f\r\n",_raw_press);
//	hal.uartC->printf("_kT=%8.8f\r\n",_kT);
//	hal.uartC->printf("_kP=%8.8f\r\n",_kP);


	Traw_src =  _raw_temp/_kT;
	Praw_src =  _raw_press/_kP;
//	hal.uartC->printf("Traw_src=%8.8f\r\n",Traw_src);
//	hal.uartC->printf("Praw_src=%8.8f\r\n",Praw_src);
	//计算温度
	Spl06_Temp = (0.5f*_C0 + Traw_src * _C1);

	//计算气压
	qua2 = _C10 + Praw_src * (_C20 + Praw_src* _C30);
	qua3 = Traw_src * Praw_src * (_C11 + Praw_src * _C21);

	Spl06_Press = _C00 + Praw_src * qua2 + Traw_src * _C01 + qua3;
//	WITH_SEMAPHORE(_sem);
//	hal.uartC->printf("Spl06_Temp=%8.8f\r\n",Spl06_Temp);
//	hal.uartC->printf("Spl06_Press=%8.8f\r\n",Spl06_Press);
//	hal.uartC->printf("$$$$$$$$$$$$$$$$\r\n");
//	hal.console->printf("^^^^^^^^^^^^^^^^\r\n");
//	hal.console->printf("_instance=%d\r\n",_instance);
//	hal.console->printf("++++++++++++++++\r\n");
//	hal.console->printf("++++++++------\r\n");
//	hal.console->printf("Spl06_Press=%d\r\n",Spl06_Press);
	_copy_to_frontend(_instance, Spl06_Press, Spl06_Temp);

}

/****************************************************************************************************************************
*函数原型：int32_t AP_BARO_SPL06_001::spl06_get_pressure_adc()
*函数功能：配置压力ADC
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
int32_t AP_BARO_SPL06_001::spl06_get_pressure_adc()
{
	uint8_t buf[3]={0};
	int32_t adc;
	_dev->read_registers(SPL06_001_PSR_B2,buf,3);  //
	adc = (int32_t)(buf[0]<<16) + (buf[1]<<8) + buf[2];
	//adc = ((int32_t)(buf[0]<<16))+((int32_t)(buf[1]<<8))+((int32_t) (buf[2]));
	adc = (adc&0x800000)?(0xFF000000|adc):adc;
	return adc;
}

/****************************************************************************************************************************
*函数原型：int32_t AP_BARO_SPL06_001::spl06_get_temperature_adc()
*函数功能：获取温度ADC
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
int32_t AP_BARO_SPL06_001::spl06_get_temperature_adc()
{
	uint8_t buf[3];
	int32_t adc;
	_dev->read_registers(SPL06_001_TMP_B2,buf,3);  //
	adc = (int32_t)(buf[0]<<16) + (buf[1]<<8) + buf[2];
	//adc = ((int32_t)(buf[0]<<16))+((int32_t)(buf[1]<<8))+((int32_t) (buf[2]));
	adc = (adc&0x800000)?(0xFF000000|adc):adc;
	return adc;
}


/****************************************************************************************************************************
*函数原型：void AP_BARO_SPL06_001::spl06_config_temperature(uint8_t rate,uint8_t oversampling)
*函数功能：配置温度
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
void AP_BARO_SPL06_001::spl06_config_temperature(uint8_t rate,uint8_t oversampling)
{
	 uint8_t temp[1] ;
	  switch(oversampling)
		{
			case TMP_PRC_1:
				_kT = 524288;
				break;
			case TMP_PRC_2:
				_kT = 1572864;
				break;
			case TMP_PRC_4:
				_kT = 3670016;
				break;
			case TMP_PRC_8:
				_kT = 7864320;
				break;
			case TMP_PRC_16:
				_kT = 253952;
				break;
			case TMP_PRC_32:
				_kT = 516096;
				break;
			case TMP_PRC_64:
				_kT = 1040384;
				break;
			case TMP_PRC_128:
				_kT = 2088960;
				break;
		}

		_dev->write_register(SPL06_001_TMP_CFG, rate|oversampling|0x80);
		if(oversampling > TMP_PRC_8)
		{

			_dev->read_registers(SPL06_001_CFG_REG,temp,1);
			_dev->write_register(SPL06_001_CFG_REG, (temp[0])|SPL06_CFG_P_SHIFT);
		}



}

/****************************************************************************************************************************
*函数原型：void AP_BARO_SPL06_001::spl06_config_pressure(uint8_t rate,uint8_t oversampling)
*函数功能：配置气压计
*修改日期：2019-1-8
*备   注：
*****************************************************************************************************************************/
void AP_BARO_SPL06_001::spl06_config_pressure(uint8_t rate,uint8_t oversampling)
{
	uint8_t temp[1];
	switch(oversampling)
	{
		case PM_PRC_1:
			_kP = 524288;
			break;
		case PM_PRC_2:
			_kP = 1572864;
			break;
		case PM_PRC_4:
			_kP = 3670016;
			break;
		case PM_PRC_8:
			_kP = 7864320;
			break;
		case PM_PRC_16:
			_kP = 253952;
			break;
		case PM_PRC_32:
			_kP = 516096;
			break;
		case PM_PRC_64:
			_kP = 1040384;
			break;
		case PM_PRC_128:
			_kP = 2088960;
			break;
	}

	_dev->write_register(SPL06_001_PRS_CFG, rate|oversampling);

	if(oversampling > PM_PRC_8)
	{
		_dev->read_registers(SPL06_001_CFG_REG,temp,1);
		_dev->write_register(SPL06_001_CFG_REG, (temp[0])|SPL06_CFG_P_SHIFT);

	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


