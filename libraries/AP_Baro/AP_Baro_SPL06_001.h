/*
 * AP_Baro_SPL06_001.h
 *
 *  Created on: Jan 7, 2019
 *      Author: coco
 */

#ifndef ARDUPILOT_LIBRARIES_AP_BARO_AP_BARO_SPL06_001_H_
#define ARDUPILOT_LIBRARIES_AP_BARO_AP_BARO_SPL06_001_H_

#include "AP_Baro_Backend.h"

#include <AP_HAL/AP_HAL.h>
#include <AP_HAL/Semaphores.h>
#include <AP_HAL/Device.h>

#define  SPL06_001_PSR_B2     0x00
#define  SPL06_001_PSR_B1     0x01
#define  SPL06_001_PSR_B0     0x02
#define  SPL06_001_TMP_B2     0x03
#define  SPL06_001_TMP_B1     0x04
#define  SPL06_001_TMP_B0     0x05
#define  SPL06_001_PRS_CFG    0x06
#define  SPL06_001_TMP_CFG    0x07
#define  SPL06_001_MEAS_CFG   0x08
#define  SPL06_001_CFG_REG    0x09
#define  SPL06_001_INT_STS    0x0A
#define  SPL06_001_FIFO_STS   0x0B
#define  SPL06_001_RESET	  0x0C
#define  SPL06_001_ID         0x0D
#define  SPL06_001_COEF_SRCE  0x28
#define  SPL06_001_COEF		  0x10
#define  SPL06_001_ADDRESS	  0x02

//气压计测量速率(sample/sec),Background模式采用
#define  PM_RATE_1			(0<<4)		//1 measurements pr. sec.
#define  PM_RATE_2			(1<<4)		//2 measurements pr. sec.
#define  PM_RATE_4			(2<<4)		//4 measurements pr. sec.
#define  PM_RATE_8			(3<<4)		//8 measurements pr. sec.
#define  PM_RATE_16			(4<<4)		//16 measurements pr. sec.
#define  PM_RATE_32			(5<<4)		//32 measurements pr. sec.
#define  PM_RATE_64			(6<<4)		//64 measurements pr. sec.
#define  PM_RATE_128		(7<<4)		//128 measurements pr. sec.
//气压计重采样速率（times）,background模式采用
#define PM_PRC_1			0		//Sigle			kP=524288	,3.6ms
#define PM_PRC_2			1		//2 times		kP=1572864	,5.2ms
#define PM_PRC_4			2		//4 times		kP=3670016	,8.4ms
#define PM_PRC_8			3		//8 times		kP=7864320	,14.8ms
#define PM_PRC_16			4		//16 times		kP=253952	,27.6ms
#define PM_PRC_32			5		//32 times		kP=516096	,53.2ms
#define PM_PRC_64			6		//64 times		kP=1040384	,104.4ms
#define PM_PRC_128			7		//128 times		kP=2088960	,206.8ms

//温度测量速率(sample/sec),Background 模式使用
#define  TMP_RATE_1			(0<<4)		//1 measurements pr. sec.
#define  TMP_RATE_2			(1<<4)		//2 measurements pr. sec.
#define  TMP_RATE_4			(2<<4)		//4 measurements pr. sec.
#define  TMP_RATE_8			(3<<4)		//8 measurements pr. sec.
#define  TMP_RATE_16		(4<<4)		//16 measurements pr. sec.
#define  TMP_RATE_32		(5<<4)		//32 measurements pr. sec.
#define  TMP_RATE_64		(6<<4)		//64 measurements pr. sec.
#define  TMP_RATE_128		(7<<4)		//128 measurements pr. sec.
//温度重采样速率（times）,background模式采用
#define TMP_PRC_1			0		//Sigle
#define TMP_PRC_2			1		//2 times
#define TMP_PRC_4			2		//4 times
#define TMP_PRC_8			3		//8 times
#define TMP_PRC_16			4		//16 times
#define TMP_PRC_32			5		//32 times
#define TMP_PRC_64			6		//64 times
#define TMP_PRC_128			7		//128 times



//SPL06_MEAS_CFG
#define MEAS_COEF_RDY		0x80
#define MEAS_SENSOR_RDY		0x40		//传感器初始化完成
#define MEAS_TMP_RDY		0x20		//有新的温度数据
#define MEAS_PRS_RDY		0x10		//有新的气压数据

#define MEAS_CTRL_Standby				0x00		//空闲模式
#define MEAS_CTRL_PressMeasure			0x01	    //单次气压测量
#define MEAS_CTRL_TempMeasure			0x02	    //单次温度测量
#define MEAS_CTRL_ContinuousPress		0x05	    //连续气压测量
#define MEAS_CTRL_ContinuousTemp		0x06	    //连续温度测量
#define MEAS_CTRL_ContinuousPressTemp	0x07	    //连续气压温度测量

//CFG_REG
#define SPL06_CFG_T_SHIFT	0x08	//oversampling times>8时必须使用
#define SPL06_CFG_P_SHIFT	0x04
//FIFO_STS
#define SPL06_FIFO_FULL		0x02
#define SPL06_FIFO_EMPTY	0x01
//INT_STS
#define SPL06_INT_FIFO_FULL		0x04
#define SPL06_INT_TMP			0x02
#define SPL06_INT_PRS			0x01

class AP_BARO_SPL06_001 : public AP_Baro_Backend
{
public:

	AP_BARO_SPL06_001(AP_Baro &baro, AP_HAL::OwnPtr<AP_HAL::Device> dev);
	void update() override;
    bool _init();
    void _timer(void);
    void spl06_config_temperature(uint8_t rate,uint8_t oversampling);
    void spl06_config_pressure(uint8_t rate,uint8_t oversampling);
    void _calculate_spl06();
    int32_t spl06_get_pressure_adc(void);
    int32_t spl06_get_temperature_adc(void);
    static AP_Baro_Backend *probe(AP_Baro &baro, AP_HAL::OwnPtr<AP_HAL::Device> dev);
private:
    uint8_t _product_id;
    int32_t _raw_temp,_raw_press;
    float _kT,_kP;
    int16_t _C0,_C1,_C01,_C11,_C20,_C21,_C30;
    int32_t _C00,_C10;


	bool _updated;

	uint8_t _tmp_count, _prs_count;
	int64_t _tmp_sum,_prs_sum;

    float Spl06_Temp,Spl06_Press;
    uint8_t _state;
    uint8_t _instance;
	uint8_t _pressure_samples;

    AP_HAL::OwnPtr<AP_HAL::Device> _dev;
};

#endif /* ARDUPILOT_LIBRARIES_AP_BARO_AP_BARO_SPL06_001_H_ */
