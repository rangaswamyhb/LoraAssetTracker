/*
 * MyApplication.h
 *
 *  Created on: Mar 30, 2022
 *      Author: HP
 */

#ifndef APPLICATION_USER_CORE_MYAPPLICATION_H_
#define APPLICATION_USER_CORE_MYAPPLICATION_H_


#include "stdint.h"

#define MAX_BLE_TXRX_DATA   		(255u)
#define MAX_BLE_ADV_DATA    		(10u)

#define MAX_LORA_WAN_TIMEOUT 		(180*1000/CFG_TS_TICK_VAL) /**< 100s */
#define MAX_LORA_PING_PONG_TIMEOUT 	(60*1000/CFG_TS_TICK_VAL) /**< 60s */

#define DEFAULT_LORA_WAN_TIME		(1) /**< In Min */



typedef enum
{
	LORA_MODULE_UNKOWN,
	LORA_MODULE_WAN_NETWORK,
	LORA_MODULE_PING_PONG,
	LORA_MODULE_MAX,
}eLoraModuleSelector_t;

typedef enum
{
	BLE_RX_FRAME_MSG_TYPE_UNKOWN,
	BLE_RX_FRAME_MSG_TYPE_LORA_MODE = 0x01,
	BLE_RX_FRAME_MSG_TYPE_TIME_SYNC,
	BLE_RX_FRAME_MSG_TYPE_SLAVE_MASTER_SELECTOR,
	BLE_RX_FRAME_MSG_TYPE_MAX
}eBleRxFrameMsgType_t;


typedef enum
{
	BLE_ADV_FRAME_MSG_TYPE_UNKOWN,
	BLE_ADV_FRAME_MSG_TYPE_ADV = 0x01,
	BLEADVX_FRAME_MSG_TYPE_MAX
}eBleAdvFrameMsgType_t;


typedef struct __attribute__((packed))
{
	uint8_t au8Payload[MAX_BLE_TXRX_DATA];
	uint8_t u8Length;
}sBleData_t;

typedef union __attribute__((packed))
{
	struct __attribute__((packed))
	{
		uint8_t u8MsgType;
		uint8_t u8LoraMode;
		uint8_t au8Data[MAX_BLE_ADV_DATA - 2];
	};

	uint8_t au8Payload[MAX_BLE_ADV_DATA];

}uBleAdvData_t;


typedef struct __attribute__((packed))
{
	uint8_t u8NoOfWifiMacId;

	uint8_t au8MacId1[6];
	int8_t  i8Rssi1;

	uint8_t au8MacId2[6];
	int8_t  i8Rssi2;

	uint8_t au8MacId3[6];
	int8_t  i8Rssi3;

	uint8_t au8MacId4[6];
	int8_t  i8Rssi4;

	uint8_t au8MacId5[6];
	int8_t  i8Rssi5;

}sWifiMacIdFrameFormat_t;


typedef struct __attribute__((packed))
{
	uint8_t u8MsgType;
	uint8_t u8PacktCount;
	uint8_t u8BatteryVtg;

	sWifiMacIdFrameFormat_t sWifiMacId;
//
//	typedef union __attribute__((packed))
//	{
//		sWifiMacIdFrameFormat_t sMacId;
//		uint8_t	au8MacIdDataBuffer[sizeof(sWifiMacIdFrameFormat_t)];
//	}uWifiData;

}sLoraWifiMacIdFrameFormat_t;


extern eLoraModuleSelector_t eLoraModuleSelector;

void App_vLoraModuleSelectorUpdate(uint32_t u32LoraModuleSelector);
eLoraModuleSelector_t App_eLoraModuleSelectorModeGet(void);
void App_vReqLoraAppRun(void);


void App_vLoraTimeoutTimerCreate(void);

void App_vNotifyDataTask(void);
void App_vNotifyCreateTask(void);
void App_vNotifyTaskRun(void);

void App_vBleAdvRequestCreateTask(void);
void App_vBleAdvRequestTaskRun(void);

void App_vLoraWanInitTaskCreate(void);
void App_vLoraWanInitTaskRun(void);

void App_vLoraWanProcessTaskCreate(void);
void App_vLoraWanProcessTaskRun(void);

void App_vLoraPingPongInitTaskCreate(void);
void App_vLoraPingPongInitTaskRun(void);

void App_vLoraPingPongProcessTaskCreate(void);
void App_vLoraPingPongProcessTaskRun(void);

void App_vBleReceiveDataCopy(uint8_t * pu8Data, uint8_t u8Len);
void App_vBleReceiveProcessData(uint8_t * pu8Data, uint8_t u8Len);

void App_vPrepareBleAdvData(void);
void App_vBleAdvDataGet(uint8_t * pu8Data, uint8_t len);
void App_vLoraRssiSet(int8_t i8Rssi);
int8_t App_vLoraRssiGet(void);

void App_vLoraSnrSet(int8_t i8Rssi);
int8_t App_vLoraSnrGet(void);

void App_vLoraPingPongPacketCounterClear(void);
void App_vLoraPingPongPacketCounterIncreament(void);
uint8_t App_vLoraPingPongPacketCounterGet(void);

void App_vLoraTimeoutTimerCreate(void);
void App_vLoraPingPongTimerStart(uint32_t u32Timeout);
void App_vLoraPingPongTimerStop(void);
void App_vLoraWanTimerStart(uint32_t u32Timeout);
void App_vLoraWanTimerStop(void);


void App_vSetBleConnectionStatus(uint8_t u8Bool);
uint8_t App_u8GetBleConnectionStatus(void);

void App_ReqAdvUpdateData(void);



void App_SeLoraCommInterval(uint16_t u16TimeInMin);
uint16_t App_GetLoraCommInterval(void);

void App_vSetDeviceIsMaster(uint8_t u8Bool);
uint8_t App_u8GetDeviceIsMaster(void);


#endif /* APPLICATION_USER_CORE_MYAPPLICATION_H_ */
