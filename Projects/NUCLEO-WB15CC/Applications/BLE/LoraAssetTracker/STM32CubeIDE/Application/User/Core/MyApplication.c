/*
 * MyApplication.c
 *
 *  Created on: Mar 30, 2022
 *      Author: HP
 */

#include "MyApplication.h"
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "p2p_server_app.h"
#include "stm32_seq.h"
#include "app_conf.h"



//__attribute__((section(".noinit"))) eLoraModuleSelector_t eLoraModuleSelector = LORA_MODULE_WAN_NETWORK;
//__attribute__((section(".noinit"))) uint32_t	loraMasterSelector = false;

eLoraModuleSelector_t eLoraModuleSelector = LORA_MODULE_PING_PONG;//LORA_MODULE_WAN_NETWORK;
uint32_t	loraMasterSelector = FALSE;

sBleData_t sBleRxData;
uBleAdvData_t g_uBleAdvData;

uint8_t u8ProcessFlasg = 0;
uint8_t u8BleConnectionStatus = 0;;

uint8_t u8LoraPingPongTimerId = 0;
uint8_t u8LoraWanTimerId = 0;

uint8_t u8LoraPingPongInProgress = 0;
uint8_t u8LoraWanInProgress = 0;
uint8_t u8LoraPingPongPacketCounter = 0;

int8_t i8LoraRssiValue = 0, i8LoraSnrValue = 0;

extern int lora_ping_pong( void );
extern int lora_wan( void );
extern void lorawan_process(void);
extern int lorawan_init(void);

extern void lorapingpong_process(void);
extern void lorapingpong_init(void);


static uint16_t u16NotifyData = 0;

void App_vNotifyDataTask(void)
{
	u16NotifyData++;

	if(u16NotifyData % 2)
	{
		P2PS_STM_App_Update_Char_With_Len(P2P_NOTIFY_CHAR_UUID, (uint16_t *)&u16NotifyData, 2);
	}
	else
	{
		P2PS_STM_App_Update_Char_With_Len(P2P_WRITE_CHAR_UUID, (uint16_t *)&u16NotifyData, 2);
	}
}


uint32_t u32LoraPingPongOneMinTimerCounter = 0;
void App_vLoraPingPongTimerHandler(void)
{
	u32LoraPingPongOneMinTimerCounter++;
	if(u32LoraPingPongOneMinTimerCounter > 10)
	{
		u32LoraPingPongOneMinTimerCounter = 0;


		App_vLoraWanInitTaskRun();
		App_vLoraWanTimerStart(MAX_LORA_WAN_TIMEOUT);

		App_vLoraPingPongTimerStop();
	}
}

void App_vLoraWanTimerHandler(void)
{
	if(App_eLoraModuleSelectorModeGet() == LORA_MODULE_PING_PONG)
	{
		App_vLoraPingPongInitTaskRun();

		App_vLoraPingPongTimerStart(MAX_LORA_PING_PONG_TIMEOUT);
	}
}

void App_vLoraTimeoutTimerCreate(void)
{
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(u8LoraPingPongTimerId), hw_ts_Repeated, App_vLoraPingPongTimerHandler);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(u8LoraWanTimerId), hw_ts_SingleShot, App_vLoraWanTimerHandler);
}

void App_vLoraPingPongTimerStart(uint32_t u32Timeout)
{
	u32LoraPingPongOneMinTimerCounter = 0;
	HW_TS_Start(u8LoraPingPongTimerId, u32Timeout);
}

void App_vLoraPingPongTimerStop(void)
{
	HW_TS_Stop(u8LoraPingPongTimerId);
}

void App_vLoraWanTimerStart(uint32_t u32Timeout)
{
	u32LoraPingPongOneMinTimerCounter = 0;
	HW_TS_Start(u8LoraWanTimerId, u32Timeout);
}

void App_vLoraWanTimerStop(void)
{
	HW_TS_Stop(u8LoraWanTimerId);
}

void App_vLoraWanInit(void)
{
	u8ProcessFlasg = 1;
	lorawan_init();

	printf("App_vLoraWanInit\r\n");

	App_vLoraWanProcessTaskRun();
}

void App_vLoraWanProcess(void)
{
	if(u8ProcessFlasg == 1)
	{
		lorawan_process();
		App_vLoraWanProcessTaskRun();
	}
	App_vReqLoraAppRun();
}

void App_vLoraPingPongInit(void)
{
	u8ProcessFlasg = 2;
	lorapingpong_init();

	printf("App_vLoraPingPongInit\r\n");

	App_vLoraPingPongProcessTaskRun();
}

void App_vLoraPingPongProcess(void)
{
	if(u8ProcessFlasg == 2)
	{
		lorapingpong_process();
		App_vLoraPingPongProcessTaskRun();
	}
	App_vReqLoraAppRun();
}

void App_vBleAdvReqTask(void)
{
	if(App_u8GetBleConnectionStatus() != TRUE)
	{
		printf("Adv the Ping Pong Data\r\n");
		Adv_Request(1);
	}
	else
	{
		printf("Notfy the Ping Pong Data\r\n");
		App_vPrepareBleAdvData();
		P2PS_STM_App_Update_Char_With_Len(P2P_NOTIFY_CHAR_UUID, (uint8_t *) &g_uBleAdvData, sizeof(uBleAdvData_t));
	}
}


void App_vNotifyCreateTask(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_MY_NOTIFY_DATA, UTIL_SEQ_RFU, App_vNotifyDataTask );
}

void App_vNotifyTaskRun(void)
{
	UTIL_SEQ_SetTask( 1<<CFG_TASK_MY_NOTIFY_DATA, CFG_SCH_PRIO_0);
}

void App_vBleAdvRequestCreateTask(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_MY_BLE_ADV_REQUEST_DATA, UTIL_SEQ_RFU, App_vBleAdvReqTask );
}

void App_vBleAdvRequestTaskRun(void)
{
	UTIL_SEQ_SetTask( 1<<CFG_TASK_MY_BLE_ADV_REQUEST_DATA, CFG_SCH_PRIO_0);
}

void App_vLoraWanInitTaskCreate(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_MY_LORA_WAN_INIT_TASK, UTIL_SEQ_RFU, App_vLoraWanInit );
}

void App_vLoraWanInitTaskRun(void)
{
	UTIL_SEQ_SetTask( 1<<CFG_TASK_MY_LORA_WAN_INIT_TASK, CFG_SCH_PRIO_0);
}

void App_vLoraWanProcessTaskCreate(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_MY_LORA_WAN_PROCESS_TASK, UTIL_SEQ_RFU, App_vLoraWanProcess );
}

void App_vLoraWanProcessTaskRun(void)
{
	UTIL_SEQ_SetTask( 1<<CFG_TASK_MY_LORA_WAN_PROCESS_TASK, CFG_SCH_PRIO_0);
}

void App_vLoraPingPongInitTaskCreate(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_MY_LORA_PING_PONG_INT_TASK, UTIL_SEQ_RFU, App_vLoraPingPongInit );
}

void App_vLoraPingPongInitTaskRun(void)
{
	UTIL_SEQ_SetTask( 1<<CFG_TASK_MY_LORA_PING_PONG_INT_TASK, CFG_SCH_PRIO_0);
}

void App_vLoraPingPongProcessTaskCreate(void)
{
	UTIL_SEQ_RegTask( 1<< CFG_TASK_MY_LORA_PING_PONG_PROCESS_TASK, UTIL_SEQ_RFU, App_vLoraPingPongProcess );
}

void App_vLoraPingPongProcessTaskRun(void)
{
	UTIL_SEQ_SetTask( 1<<CFG_TASK_MY_LORA_PING_PONG_PROCESS_TASK, CFG_SCH_PRIO_0);
}

void App_vLoraModuleSelectorUpdate(uint32_t u32LoraModuleSelector)
{
	if(u32LoraModuleSelector < LORA_MODULE_MAX)
	{
		eLoraModuleSelector = u32LoraModuleSelector;
	}
}

eLoraModuleSelector_t App_eLoraModuleSelectorModeGet(void)
{
	return eLoraModuleSelector;
}

void App_vLoraAppStateMachineRun(void)
{

    switch(eLoraModuleSelector)
    {
    case LORA_MODULE_WAN_NETWORK:
    {
    	printf("App_vLoraAppStateMachineRun :: LORA_MODULE_WAN_NETWORK\r\n");
    	App_vLoraWanInitTaskRun();
    }
    break;

    case LORA_MODULE_PING_PONG:
    {
    	printf("App_vLoraAppStateMachineRun :: LORA_MODULE_PING_PONG\r\n");
    	App_vLoraPingPongPacketCounterClear();

    	App_vLoraPingPongInitTaskRun();
    	App_vLoraPingPongTimerStart(MAX_LORA_PING_PONG_TIMEOUT);
    }
    break;

    default:
    	break;
    }
}

void App_vReqLoraAppRun(void)
{
	static uint32_t u32LoraModeSelector = LORA_MODULE_UNKOWN;

	if(u32LoraModeSelector != eLoraModuleSelector)
	{
		App_vLoraAppStateMachineRun();
		u32LoraModeSelector = eLoraModuleSelector;
	}
}

void App_vBleReceiveDataCopy(uint8_t * pu8Data, uint8_t u8Len)
{
	if(MAX_BLE_TXRX_DATA < u8Len)
		return;

	memset(&sBleRxData, 0x00, sizeof(sBleRxData));
	memcpy((uint8_t *) &sBleRxData.au8Payload, (uint8_t *) pu8Data, u8Len);
	sBleRxData.u8Length = u8Len;
}

void App_vBleReceiveProcessData(uint8_t * pu8Data, uint8_t u8Len)
{
	if(MAX_BLE_TXRX_DATA < u8Len)
		return;

	memset(&sBleRxData, 0x00, sizeof(sBleRxData));
	memcpy((uint8_t *) &sBleRxData.au8Payload, (uint8_t *) pu8Data, u8Len);
	sBleRxData.u8Length = u8Len;

	switch(sBleRxData.au8Payload[0])
	{
	case BLE_RX_FRAME_MSG_TYPE_LORA_MODE:
	{
		if(sBleRxData.au8Payload[1] == 0x55)
		{
			printf("BLE_RX_FRAME_MSG_TYPE_LORA_MODE :: LORA PING PONG\r\n");
			App_vLoraModuleSelectorUpdate(LORA_MODULE_PING_PONG);
		}
		else if(sBleRxData.au8Payload[1] == 0xAA)
		{
			printf("BLE_RX_FRAME_MSG_TYPE_LORA_MODE :: LORA WAN\r\n");
			App_vLoraModuleSelectorUpdate(LORA_MODULE_WAN_NETWORK);
		}
	}
	break;

	case BLE_RX_FRAME_MSG_TYPE_TIME_SYNC:
	{
		break;
	}

	case BLE_RX_FRAME_MSG_TYPE_SLAVE_MASTER_SELECTOR:
	{
		printf("BLE_RX_FRAME_MSG_TYPE_SLAVE_MASTER_SELECTOR\r\n");
		if(sBleRxData.au8Payload[1] == 0x01)
		{
			App_vSetDeviceIsMaster(FALSE);
		}
		else if(sBleRxData.au8Payload[1] == 0x02)
		{
			App_vSetDeviceIsMaster(TRUE);
		}

		break;
	}

	case 'N':
	{
		App_vNotifyTaskRun();
	}
	break;

	default:
		break;
	}
	App_vReqLoraAppRun();
}

void App_vPrepareBleAdvData(void)
{
	memset((uint8_t *)&g_uBleAdvData, 0x00, sizeof(g_uBleAdvData));

	g_uBleAdvData.u8MsgType = BLE_ADV_FRAME_MSG_TYPE_ADV;
	g_uBleAdvData.u8LoraMode = App_eLoraModuleSelectorModeGet();

	g_uBleAdvData.au8Data[0] = App_vLoraRssiGet();
	g_uBleAdvData.au8Data[1] = App_vLoraSnrGet();
	g_uBleAdvData.au8Data[2] = App_vLoraPingPongPacketCounterGet();

//	App_vLoraPingPongPacketCounterIncreament();

}

void App_vBleAdvDataGet(uint8_t * pu8Data, uint8_t len)
{
	if(len > MAX_BLE_ADV_DATA)
		return;

	memcpy((uint8_t *) pu8Data, (uint8_t *)&g_uBleAdvData.au8Payload, len);
	pu8Data[len - 2] = 0xBE;
	pu8Data[len - 1] = 0xEF;

}

void App_vLoraRssiSet(int8_t i8Rssi)
{
	i8LoraRssiValue = i8Rssi;
}

int8_t App_vLoraRssiGet(void)
{
	return (i8LoraRssiValue);
}

void App_vLoraSnrSet(int8_t i8Rssi)
{
	i8LoraSnrValue = i8Rssi;
}

int8_t App_vLoraSnrGet(void)
{
	return (i8LoraSnrValue);
}


void App_vLoraPingPongPacketCounterClear(void)
{
	u8LoraPingPongPacketCounter = 0;
}

void App_vLoraPingPongPacketCounterIncreament(void)
{
	u8LoraPingPongPacketCounter++;
}

uint8_t App_vLoraPingPongPacketCounterGet(void)
{
	return (u8LoraPingPongPacketCounter);
}

void App_vSetBleConnectionStatus(uint8_t u8Bool)
{
	u8BleConnectionStatus = u8Bool;
}

uint8_t App_u8GetBleConnectionStatus(void)
{
	return (u8BleConnectionStatus);
}


void App_ReqAdvUpdateData(void)
{
//	Adv_UpdateData();

	if(App_u8GetBleConnectionStatus() != TRUE)
	{
		printf("Adv the Ping Pong Data\r\n");
		Adv_UpdateData();
	}
	else
	{
		printf("Notfy the Ping Pong Data\r\n");
		App_vPrepareBleAdvData();
		P2PS_STM_App_Update_Char_With_Len(P2P_NOTIFY_CHAR_UUID, (uint8_t *) &g_uBleAdvData, sizeof(uBleAdvData_t));
	}
}












/* Lora Commu Time Interval */

uint16_t u16LoraCommTimeInterval = DEFAULT_LORA_WAN_TIME;

void App_SeLoraCommInterval(uint16_t u16TimeInMin)
{
	printf("App_SeLoraCommInterval: %d", u16TimeInMin);
	u16LoraCommTimeInterval = u16TimeInMin;
	printf("App_SeLoraCommInterval: %d", u16LoraCommTimeInterval);
}

uint16_t App_GetLoraCommInterval(void)
{
	return u16LoraCommTimeInterval;
}










/* Lora Master / Smave Selector */
void App_vSetDeviceIsMaster(uint8_t u8Bool)
{
	if(u8Bool)
	{
		loraMasterSelector = TRUE;
	}
	else
	{
		loraMasterSelector = FALSE;
	}
}

uint8_t App_u8GetDeviceIsMaster(void)
{
	return loraMasterSelector;
}





