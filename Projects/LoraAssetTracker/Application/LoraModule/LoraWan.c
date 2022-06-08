/*
 * LoraWan.c
 *
 *  Created on: 08-Jun-2022
 *      Author: Ranga
 */
/*!
 * \file      main.c
 *
 * \brief     LoRaMac classA device implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */

/*! \file classA/NucleoL073/main.c */

#include <stdio.h>
#include "utilities.h"
#include "board.h"
#include "lr1110_gpio.h"
#include "LoRaMac.h"
#include "Commissioning.h"
#include "radio.h"
#include "MyApplication.h"
#include "gnss/gnss_scan.h"


#ifndef ACTIVE_REGION

#define ACTIVE_REGION 				    LORAMAC_REGION_IN865

#endif

#define REGION_IN865				    1


/*!
 * Defines the application data transmission duty cycle. 5s, value in [ms].
 */
#define APP_TX_DUTYCYCLE                            60000

/*!
 * Defines a random delay for application data transmission duty cycle. 1s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        1000

/*!
 * Default datarate
 */
#define LORAWAN_DEFAULT_DATARATE                    DR_1

/*!
 * LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON                    false

/*!
 * LoRaWAN Adaptive Data Rate
 *
 * \remark Please note that when ADR is enabled the end-device should be static
 */
#define LORAWAN_ADR_ON                              0

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )

#include "./mac/LoRaMacTest.h"

/*!
 * LoRaWAN ETSI duty cycle control enable/disable
 *
 * \remark Please note that ETSI mandates duty cycled transmissions. Use only for test purposes
 */
#define LORAWAN_DUTYCYCLE_ON                        false

#endif

/*!
 * LoRaWAN application port
 */
#define LORAWAN_APP_PORT                            1

#if( ABP_ACTIVATION_LRWAN_VERSION == ABP_ACTIVATION_LRWAN_VERSION_V10x )
static uint8_t GenAppKey[] = LORAWAN_GEN_APP_KEY;
#else
static uint8_t AppKey[] = LORAWAN_APP_KEY;
#endif
static uint8_t NwkKey[] = LORAWAN_NWK_KEY;

#if( OVER_THE_AIR_ACTIVATION == 0 )

static uint8_t FNwkSIntKey[] = LORAWAN_F_NWK_S_INT_KEY;
static uint8_t SNwkSIntKey[] = LORAWAN_S_NWK_S_INT_KEY;
static uint8_t NwkSEncKey[] = LORAWAN_NWK_S_ENC_KEY;
static uint8_t AppSKey[] = LORAWAN_APP_S_KEY;

/*!
 * Device address
 */
static uint32_t DevAddr = LORAWAN_DEVICE_ADDRESS;

#endif



extern uint32_t u32LoraWanSelector;


/*!
 * Application port
 */
static uint8_t AppPort = LORAWAN_APP_PORT;

/*!
 * User application data size
 */
static uint8_t AppDataSize = 1;
static uint8_t AppDataSizeBackup = 1;

/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE                           242

/*!
 * User application data
 */
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_MAX_SIZE];

/*!
 * Indicates if the node is sending confirmed or unconfirmed messages
 */
static uint8_t IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;

/*!
 * Defines the application data transmission duty cycle
 */
static uint32_t TxDutyCycleTime;

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t TxNextPacketTimer;

/*!
 * Specifies the state of the application LED
 */
static bool AppLedStateOn = false;

/*!
 * Indicates if a new packet can be sent
 */
static bool NextTx = true;

/*!
 * Indicates if LoRaMacProcess call is pending.
 *
 * \warning If variable is equal to 0 then the MCU can be set in low power mode
 */
static uint8_t IsMacProcessPending = 0;

/*!
 * Device states
 */
typedef enum eDeviceState
{
    LORA_WAN_STATE_RESTORE,
	LORA_WAN_STATE_INIT,
    LORA_WAN_STATE_START,
    LORA_WAN_STATE_JOIN,
    LORA_WAN_STATE_SEND,
    LORA_WAN_STATE_CYCLE,
	LORA_WAN_STATE_TIMER,
    LORA_WAN_STATE_SLEEP,
	LORA_WAN_STATE_MAX
}eLoraWanStateMachine_t;

/*!
 * LoRaWAN compliance tests support data
 */
struct ComplianceTest_s
{
    bool Running;
    uint8_t State;
    bool IsTxConfirmed;
    uint8_t AppPort;
    uint8_t AppDataSize;
    uint8_t *AppDataBuffer;
    uint16_t DownLinkCounter;
    bool LinkCheck;
    uint8_t DemodMargin;
    uint8_t NbGateways;
}ComplianceTest;

/*!
 *
 */
typedef enum
{
    LORAMAC_HANDLER_UNCONFIRMED_MSG = 0,
    LORAMAC_HANDLER_CONFIRMED_MSG = !LORAMAC_HANDLER_UNCONFIRMED_MSG
}LoRaMacHandlerMsgTypes_t;

/*!
 * Application data structure
 */
typedef struct LoRaMacHandlerAppData_s
{
    LoRaMacHandlerMsgTypes_t MsgType;
    uint8_t Port;
    uint8_t BufferSize;
    uint8_t *Buffer;
}LoRaMacHandlerAppData_t;

LoRaMacHandlerAppData_t AppData =
{
    .MsgType = LORAMAC_HANDLER_UNCONFIRMED_MSG,
    .Buffer = NULL,
    .BufferSize = 0,
    .Port = 0
};


/*!
 * MAC status strings
 */
const char* myMacStatusStrings[] =
{
    "OK",                            // LORAMAC_STATUS_OK
    "Busy",                          // LORAMAC_STATUS_BUSY
    "Service unknown",               // LORAMAC_STATUS_SERVICE_UNKNOWN
    "Parameter invalid",             // LORAMAC_STATUS_PARAMETER_INVALID
    "Frequency invalid",             // LORAMAC_STATUS_FREQUENCY_INVALID
    "Datarate invalid",              // LORAMAC_STATUS_DATARATE_INVALID
    "Frequency or datarate invalid", // LORAMAC_STATUS_FREQ_AND_DR_INVALID
    "No network joined",             // LORAMAC_STATUS_NO_NETWORK_JOINED
    "Length error",                  // LORAMAC_STATUS_LENGTH_ERROR
    "Region not supported",          // LORAMAC_STATUS_REGION_NOT_SUPPORTED
    "Skipped APP data",              // LORAMAC_STATUS_SKIPPED_APP_DATA
    "Duty-cycle restricted",         // LORAMAC_STATUS_DUTYCYCLE_RESTRICTED
    "No channel found",              // LORAMAC_STATUS_NO_CHANNEL_FOUND
    "No free channel found",         // LORAMAC_STATUS_NO_FREE_CHANNEL_FOUND
    "Busy beacon reserved time",     // LORAMAC_STATUS_BUSY_BEACON_RESERVED_TIME
    "Busy ping-slot window time",    // LORAMAC_STATUS_BUSY_PING_SLOT_WINDOW_TIME
    "Busy uplink collision",         // LORAMAC_STATUS_BUSY_UPLINK_COLLISION
    "Crypto error",                  // LORAMAC_STATUS_CRYPTO_ERROR
    "FCnt handler error",            // LORAMAC_STATUS_FCNT_HANDLER_ERROR
    "MAC command error",             // LORAMAC_STATUS_MAC_COMMAD_ERROR
    "ClassB error",                  // LORAMAC_STATUS_CLASS_B_ERROR
    "Confirm queue error",           // LORAMAC_STATUS_CONFIRM_QUEUE_ERROR
    "Multicast group undefined",     // LORAMAC_STATUS_MC_GROUP_UNDEFINED
    "Unknown error",                 // LORAMAC_STATUS_ERROR
};

/*!
 * MAC event info status strings.
 */
const char* myEventInfoStatusStrings[] =
{
    "OK",                            // LORAMAC_EVENT_INFO_STATUS_OK
    "Error",                         // LORAMAC_EVENT_INFO_STATUS_ERROR
    "Tx timeout",                    // LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT
    "Rx 1 timeout",                  // LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT
    "Rx 2 timeout",                  // LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT
    "Rx1 error",                     // LORAMAC_EVENT_INFO_STATUS_RX1_ERROR
    "Rx2 error",                     // LORAMAC_EVENT_INFO_STATUS_RX2_ERROR
    "Join failed",                   // LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL
    "Downlink repeated",             // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED
    "Tx DR payload size error",      // LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR
    "Downlink too many frames loss", // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS
    "Address fail",                  // LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL
    "MIC fail",                      // LORAMAC_EVENT_INFO_STATUS_MIC_FAIL
    "Multicast fail",                // LORAMAC_EVENT_INFO_STATUS_MULTICAST_FAIL
    "Beacon locked",                 // LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED
    "Beacon lost",                   // LORAMAC_EVENT_INFO_STATUS_BEACON_LOST
    "Beacon not found"               // LORAMAC_EVENT_INFO_STATUS_BEACON_NOT_FOUND
};




eLoraWanStateMachine_t 		eLoraWanState = LORA_WAN_STATE_SLEEP;

LoRaMacPrimitives_t 		g_macPrimitives;
LoRaMacCallback_t 			g_macCallbacks;
MibRequestConfirm_t 		g_mibReq;
sLoraWifiMacIdFrameFormat_t sLoraWifiMacFrame;

static uint32_t 			u32JoinCount = 0;
volatile static uint8_t 	u8PacketCounter = 0;
volatile static uint32_t 	s_u32LorawanTimerCounter = 0;

/*!
 * Prints the provided buffer in HEX
 *
 * \param buffer Buffer to be printed
 * \param size   Buffer size to be printed
 */
void mymyPrintHexBuffer( uint8_t *buffer, uint8_t size )
{
    uint8_t newline = 0;

    for( uint8_t i = 0; i < size; i++ )
    {
        if( newline != 0 )
        {
            printf( "\r\n" );
            newline = 0;
        }

        printf( "0x%x ,", buffer[i] );

        if( ( ( i + 1 ) % 16 ) == 0 )
        {
            newline = 1;
        }
    }
    printf( "\r\n" );
}


/*!
 * Executes the network Join request
 */
static void JoinNetwork( void )
{
    LoRaMacStatus_t status;
    MlmeReq_t mlmeReq;
    mlmeReq.Type = MLME_JOIN;
    mlmeReq.Req.Join.Datarate = LORAWAN_DEFAULT_DATARATE;
    mlmeReq.Req.Join.NetworkActivation = ACTIVATION_TYPE_OTAA;

    // Starts the join procedure
    status = LoRaMacMlmeRequest( &mlmeReq );
    printf( "\r\n## MLME-Request - MLME_JOIN ##\r\n" );
    printf( "STATUS      : %s\r\n", myMacStatusStrings[status] );

    if( status == LORAMAC_STATUS_OK )
    {
        printf( "## JOINING ##\r\n" );
        u32JoinCount  = 0;
        eLoraWanState = LORA_WAN_STATE_SLEEP;
    }
    else
    {
        MibRequestConfirm_t mibReq;
        DeviceClass_t currentClass;

        mibReq.Type = MIB_DEVICE_CLASS;
        LoRaMacMibGetRequestConfirm( &mibReq );
        currentClass = mibReq.Param.Class;

        if( currentClass != CLASS_A )
        {
        	mibReq.Param.Class = CLASS_A;
			LoRaMacMibSetRequestConfirm( &mibReq );
        }

        eLoraWanState = LORA_WAN_STATE_START;
    }
}

/*!
 * \brief   Prepares the payload of the frame
 */
static void PrepareTxFrame( uint8_t port )
{
    switch( port )
    {
    case 1:
	{
//          AppDataSizeBackup = AppDataSize = 6;
//          AppDataBuffer[0] = 0x11;
//          AppDataBuffer[1] = 0x22;
//          AppDataBuffer[2] = 0x33;
//          AppDataBuffer[3] = 0x44;
//          AppDataBuffer[4] = 0x55;
//          AppDataBuffer[5] = 0x66;

	  uint16_t i = 0;
	  for(i = 0; i < 10; i++)
	  {
	    AppDataBuffer[i] = i+30;
	  }
	  AppDataSizeBackup = AppDataSize = i;

	  memset((uint8_t *) &sLoraWifiMacFrame, 0x00, sizeof(sLoraWifiMacFrame));

	  sLoraWifiMacFrame.u8MsgType = 0x01;
	  sLoraWifiMacFrame.u8PacktCount = u8PacketCounter++;

	  app_wifi_scan((uint8_t *) &sLoraWifiMacFrame.sWifiMacId);

	  memcpy((uint8_t *)AppDataBuffer, (uint8_t *) &sLoraWifiMacFrame, sizeof(sLoraWifiMacFrame));
	  AppDataSizeBackup = AppDataSize = sizeof(sLoraWifiMacFrame);

	}
	break;
    case 2:
        {
            AppDataSizeBackup = AppDataSize = 5;
            AppDataBuffer[0] = 0x11;
            AppDataBuffer[1] = 0x22;
            AppDataBuffer[2] = 0x33;
            AppDataBuffer[3] = 0x44;
            AppDataBuffer[4] = 0x55;
            AppDataBuffer[5] = 0x66;
        }
        break;
    case 224:
        if( ComplianceTest.LinkCheck == true )
        {
            ComplianceTest.LinkCheck = false;
            AppDataSize = 3;
            AppDataBuffer[0] = 5;
            AppDataBuffer[1] = ComplianceTest.DemodMargin;
            AppDataBuffer[2] = ComplianceTest.NbGateways;
            ComplianceTest.State = 1;
        }
        else
        {
            switch( ComplianceTest.State )
            {
            case 4:
                ComplianceTest.State = 1;
                break;
            case 1:
                AppDataSize = 2;
                AppDataBuffer[0] = ComplianceTest.DownLinkCounter >> 8;
                AppDataBuffer[1] = ComplianceTest.DownLinkCounter;
                break;
            }
        }
        break;
    default:
        break;
    }
}

/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
static bool SendFrame( void )
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;

    if( LoRaMacQueryTxPossible( AppDataSize, &txInfo ) != LORAMAC_STATUS_OK )
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
    }
    else
    {
        if( IsTxConfirmed == false )
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = AppPort;
            mcpsReq.Req.Unconfirmed.fBuffer = AppDataBuffer;
            mcpsReq.Req.Unconfirmed.fBufferSize = AppDataSize;
            mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
        else
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = AppPort;
            mcpsReq.Req.Confirmed.fBuffer = AppDataBuffer;
            mcpsReq.Req.Confirmed.fBufferSize = AppDataSize;
//            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
    }

    // Update global variable
    AppData.MsgType = ( mcpsReq.Type == MCPS_CONFIRMED ) ? LORAMAC_HANDLER_CONFIRMED_MSG : LORAMAC_HANDLER_UNCONFIRMED_MSG;
    AppData.Port = mcpsReq.Req.Unconfirmed.fPort;
    AppData.Buffer = mcpsReq.Req.Unconfirmed.fBuffer;
    AppData.BufferSize = mcpsReq.Req.Unconfirmed.fBufferSize;

    printf("Lora Send: %d\r\n", AppDataSize);
    uint8_t ii = 0;
    for(ii = 0; ii < AppDataSize; ii++)
    {
    	printf("0x%02X, ", AppDataBuffer[ii]);
    }
    printf("\r\n\r\n");

    LoRaMacStatus_t status;
    status = LoRaMacMcpsRequest( &mcpsReq );
    printf( "\r\n###### ===== MCPS-Request ==== ######\r\n" );
    printf( "STATUS      : %s\r\n", myMacStatusStrings[status] );

    if( status == LORAMAC_STATUS_OK )
    {
        return false;
    }
    return true;
}


static void LorawanTimeCounterClear(void)
{
	s_u32LorawanTimerCounter = 0;
}

/*!
 * \brief Function executed on TxNextPacket Timeout event
 */
static void OnTxNextPacketTimerEvent( void* context )
{
	s_u32LorawanTimerCounter++;

	if(s_u32LorawanTimerCounter >= App_GetLoraCommInterval())
	{
		s_u32LorawanTimerCounter = 0;

		TimerStop( &TxNextPacketTimer );

		MibRequestConfirm_t mibReq;
		LoRaMacStatus_t status;

		mibReq.Type = MIB_NETWORK_ACTIVATION;
		status = LoRaMacMibGetRequestConfirm( &mibReq );

		if( status == LORAMAC_STATUS_OK )
		{
			if( mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE )
			{
				// Network not joined yet. Try to join again
				JoinNetwork( );
			}
			else
			{
				eLoraWanState = LORA_WAN_STATE_SEND;
				NextTx = true;
			}
		}
	}
	else
	{
		eLoraWanState = LORA_WAN_STATE_TIMER;

		TimerStop( &TxNextPacketTimer );
		TimerSetValue( &TxNextPacketTimer, APP_TX_DUTYCYCLE );

		TimerStart( &TxNextPacketTimer );
	}
}

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] mcpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void McpsConfirm( McpsConfirm_t *mcpsConfirm )
{
    printf( "\r\n###### ===== MCPS-Confirm ==== ######\r\n" );
    printf( "STATUS      : %s\r\n", myEventInfoStatusStrings[mcpsConfirm->Status] );
    if( mcpsConfirm->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
    }
    else
    {
        switch( mcpsConfirm->McpsRequest )
        {
            case MCPS_UNCONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                break;
            }
            case MCPS_CONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                // Check AckReceived
                // Check NbTrials
                break;
            }
            case MCPS_PROPRIETARY:
            {
                break;
            }
            default:
                break;
        }
    }
    MibRequestConfirm_t mibGet;
    MibRequestConfirm_t mibReq;

    mibReq.Type = MIB_DEVICE_CLASS;
    LoRaMacMibGetRequestConfirm( &mibReq );

    printf( "\r\n###### ===== UPLINK FRAME %lu ==== ######\r\n", mcpsConfirm->UpLinkCounter );
    printf( "\r\n" );

    printf( "CLASS       : %c\r\n", "ABC"[mibReq.Param.Class] );
    printf( "\r\n" );
    printf( "TX PORT     : %d\r\n", AppData.Port );

    printf( "TX DATA SIZE: %d\r\n", AppData.BufferSize);

    if( AppData.BufferSize != 0 )
    {
        printf( "TX DATA     : " );
        if( AppData.MsgType == LORAMAC_HANDLER_CONFIRMED_MSG )
        {
            printf( "CONFIRMED - %s\r\n", ( mcpsConfirm->AckReceived != 0 ) ? "ACK" : "NACK" );
        }
        else
        {
            printf( "UNCONFIRMED\r\n" );
        }
        mymyPrintHexBuffer( AppData.Buffer, AppData.BufferSize );
    }

    printf( "\r\n" );
    printf( "DATA RATE   : DR_%d\r\n", mcpsConfirm->Datarate );

    mibGet.Type  = MIB_CHANNELS;
    if( LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK )
    {
        printf( "U/L FREQ    : %lu\r\n", mibGet.Param.ChannelList[mcpsConfirm->Channel].Frequency );
    }

    printf( "TX POWER    : %d\r\n", mcpsConfirm->TxPower );

    mibGet.Type  = MIB_CHANNELS_MASK;
    if( LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK )
    {
        printf("CHANNEL MASK: ");
#if defined( REGION_AS923 ) || defined( REGION_CN779 ) || \
    defined( REGION_EU868 ) || defined( REGION_IN865 ) || \
    defined( REGION_KR920 ) || defined( REGION_EU433 ) || \
    defined( REGION_RU864 )

        for( uint8_t i = 0; i < 1; i++)

#elif defined( REGION_AU915 ) || defined( REGION_US915 ) || defined( REGION_CN470 )

        for( uint8_t i = 0; i < 5; i++)
#else

//#error "Please define a region in the compiler options."

#endif
        {
            printf("%04X ", mibGet.Param.ChannelsMask[i] );
        }
        printf("\r\n");
    }

    printf( "\r\n" );
}

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */

/*
 * This Callback event call when Down-link happened
 */
static void McpsIndication( McpsIndication_t *mcpsIndication )
{
    printf( "\r\n### ===== MCPS-Indication ==== ###\r\n" );
    printf( "STATUS      : %s\r\n", myEventInfoStatusStrings[mcpsIndication->Status] );
    if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
        return;
    }

    switch( mcpsIndication->McpsIndication )
    {
        case MCPS_UNCONFIRMED:
        {
            break;
        }
        case MCPS_CONFIRMED:
        {
            break;
        }
        case MCPS_PROPRIETARY:
        {
            break;
        }
        case MCPS_MULTICAST:
        {
            break;
        }
        default:
            break;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    if( mcpsIndication->IsUplinkTxPending == true )
    {
        // The server signals that it has pending data to be sent.
        // We schedule an uplink as soon as possible to flush the server.
        OnTxNextPacketTimerEvent( NULL );
    }
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot

    if( ComplianceTest.Running == true )
    {
        ComplianceTest.DownLinkCounter++;
    }

    if( mcpsIndication->RxData == true )
    {
        switch( mcpsIndication->Port )
        {
        case 1: // The application LED can be controlled on port 1 or 2
        {
          if( mcpsIndication->BufferSize )
          {
            printf("Lora RX DATA: ");
            uint16_t i = 0;
            for( i = 0; i < mcpsIndication->BufferSize; i++ )
            {
              printf("0x%x, ", mcpsIndication->Buffer[i]);
            }
            printf("\r\n");

            switch(mcpsIndication->Buffer[0])
            {
            case 0x01:
            {
            	if(mcpsIndication->Buffer[1] == 0x55)
            	{
                	u32LoraWanSelector = 0xAA;
                	App_vLoraModuleSelectorUpdate(LORA_MODULE_PING_PONG);
            	}
            	break;
            }

            case 0x02:
            {
            	uint16_t u16Interval = ((mcpsIndication->Buffer[2]) << 8) | (mcpsIndication->Buffer[1]);
//            	printf("Rx : %d, ((mcpsIndication->Buffer[2]) << 8): %d,   %d\r\n", u16Interval, ((mcpsIndication->Buffer[2]) << 8),
//            			(mcpsIndication->Buffer[1]));

            	App_SeLoraCommInterval(u16Interval);

            	break;
            }

            default:
            	break;
            }

//            if((mcpsIndication->Buffer[0] == 0x01) && (mcpsIndication->Buffer[1] == 0x55))
//            {
//            	u32LoraWanSelector = 0xAA;
//            	App_vLoraModuleSelectorUpdate(LORA_MODULE_PING_PONG);
//            }
          }
        }
        case 2:
            if( mcpsIndication->BufferSize == 1 )
            {
                AppLedStateOn = mcpsIndication->Buffer[0] & 0x01;
            }
            break;
        case 224:
            if( ComplianceTest.Running == false )
            {
                // Check compliance test enable command (i)
                if( ( mcpsIndication->BufferSize == 4 ) &&
                    ( mcpsIndication->Buffer[0] == 0x01 ) &&
                    ( mcpsIndication->Buffer[1] == 0x01 ) &&
                    ( mcpsIndication->Buffer[2] == 0x01 ) &&
                    ( mcpsIndication->Buffer[3] == 0x01 ) )
                {
                    IsTxConfirmed = false;
                    AppPort = 224;
                    AppDataSizeBackup = AppDataSize;
                    AppDataSize = 2;
                    ComplianceTest.DownLinkCounter = 0;
                    ComplianceTest.LinkCheck = false;
                    ComplianceTest.DemodMargin = 0;
                    ComplianceTest.NbGateways = 0;
                    ComplianceTest.Running = true;
                    ComplianceTest.State = 1;

                    MibRequestConfirm_t mibReq;
                    mibReq.Type = MIB_ADR;
                    mibReq.Param.AdrEnable = true;
                    LoRaMacMibSetRequestConfirm( &mibReq );

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
                    LoRaMacTestSetDutyCycleOn( false );
#endif
                }
            }
            else
            {
                ComplianceTest.State = mcpsIndication->Buffer[0];
                switch( ComplianceTest.State )
                {
                case 0: // Check compliance test disable command (ii)
                    IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
                    AppPort = LORAWAN_APP_PORT;
                    AppDataSize = AppDataSizeBackup;
                    ComplianceTest.DownLinkCounter = 0;
                    ComplianceTest.Running = false;

                    MibRequestConfirm_t mibReq;
                    mibReq.Type = MIB_ADR;
                    mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                    LoRaMacMibSetRequestConfirm( &mibReq );
#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
                    LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
#endif
                    break;
                case 1: // (iii, iv)
                    AppDataSize = 2;
                    break;
                case 2: // Enable confirmed messages (v)
                    IsTxConfirmed = true;
                    ComplianceTest.State = 1;
                    break;
                case 3:  // Disable confirmed messages (vi)
                    IsTxConfirmed = false;
                    ComplianceTest.State = 1;
                    break;
                case 4: // (vii)
                    AppDataSize = mcpsIndication->BufferSize;

                    AppDataBuffer[0] = 4;
                    for( uint8_t i = 1; i < MIN( AppDataSize, LORAWAN_APP_DATA_MAX_SIZE ); i++ )
                    {
                        AppDataBuffer[i] = mcpsIndication->Buffer[i] + 1;
                    }
                    break;
                case 5: // (viii)
                    {
                        MlmeReq_t mlmeReq;
                        mlmeReq.Type = MLME_LINK_CHECK;
                        LoRaMacStatus_t status = LoRaMacMlmeRequest( &mlmeReq );
                        printf( "\r\n###### ===== MLME-Request - MLME_LINK_CHECK ==== ######\r\n" );
                        printf( "STATUS      : %s\r\n", myMacStatusStrings[status] );
                    }
                    break;
                case 6: // (ix)
                    {
                        // Disable TestMode and revert back to normal operation
                        IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
                        AppPort = LORAWAN_APP_PORT;
                        AppDataSize = AppDataSizeBackup;
                        ComplianceTest.DownLinkCounter = 0;
                        ComplianceTest.Running = false;

                        MibRequestConfirm_t mibReq;
                        mibReq.Type = MIB_ADR;
                        mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                        LoRaMacMibSetRequestConfirm( &mibReq );
#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
                        LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
#endif

                        JoinNetwork( );
                    }
                    break;
                case 7: // (x)
                    {
                        if( mcpsIndication->BufferSize == 3 )
                        {
                            MlmeReq_t mlmeReq;
                            mlmeReq.Type = MLME_TXCW;
                            mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                            LoRaMacStatus_t status = LoRaMacMlmeRequest( &mlmeReq );
                            printf( "\r\n###### ===== MLME-Request - MLME_TXCW ==== ######\r\n" );
                            printf( "STATUS      : %s\r\n", myMacStatusStrings[status] );
                        }
                        else if( mcpsIndication->BufferSize == 7 )
                        {
                            MlmeReq_t mlmeReq;
                            mlmeReq.Type = MLME_TXCW;
                            mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                            mlmeReq.Req.TxCw.Frequency = ( uint32_t )( ( mcpsIndication->Buffer[3] << 16 ) | ( mcpsIndication->Buffer[4] << 8 ) | mcpsIndication->Buffer[5] ) * 100;
                            mlmeReq.Req.TxCw.Power = mcpsIndication->Buffer[6];
                            LoRaMacStatus_t status = LoRaMacMlmeRequest( &mlmeReq );
                            printf( "\r\n###### ===== MLME-Request - MLME_TXCW1 ==== ######\r\n" );
                            printf( "STATUS      : %s\r\n", myMacStatusStrings[status] );
                        }
                        ComplianceTest.State = 1;
                    }
                    break;
                case 8: // Send DeviceTimeReq
                    {
                        MlmeReq_t mlmeReq;

                        mlmeReq.Type = MLME_DEVICE_TIME;

                        LoRaMacStatus_t status = LoRaMacMlmeRequest( &mlmeReq );
                        printf( "\r\n###### ===== MLME-Request - MLME_DEVICE_TIME ==== ######\r\n" );
                        printf( "STATUS      : %s\r\n", myMacStatusStrings[status] );
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }

    // Switch LED 2 ON for each received downlink
 //   GpioWrite( &Led2, 1 );
   // TimerStart( &Led2Timer );

    const char *slotStrings[] = { "1", "2", "C", "C Multicast", "B Ping-Slot", "B Multicast Ping-Slot" };

    printf( "\r\n###### ===== DOWNLINK FRAME %lu ==== ######\r\n", mcpsIndication->DownLinkCounter );

    printf( "RX WINDOW   : %s\r\n", slotStrings[mcpsIndication->RxSlot] );

    printf( "RX PORT     : %d\r\n", mcpsIndication->Port );

    printf( "RX DATA SIZE: %d\r\n", mcpsIndication->BufferSize);

    if( mcpsIndication->BufferSize != 0 )
    {
        printf( "RX DATA     : \r\n" );
        mymyPrintHexBuffer( mcpsIndication->Buffer, mcpsIndication->BufferSize );
    }

//    printf( "\r\n" );
    printf( "DATA RATE   : DR_%d\r\n", mcpsIndication->RxDatarate );
    printf( "RX RSSI     : %d\r\n", mcpsIndication->Rssi );
    printf( "RX SNR      : %d\r\n", mcpsIndication->Snr );

    App_vLoraRssiSet(mcpsIndication->Rssi);
    App_vLoraSnrSet(mcpsIndication->Snr);

    printf( "\r\n" );
}

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] mlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
    printf( "\r\n## MLME-Confirm ##\r\n" );
    printf( "STATUS      : %s\r\n", myEventInfoStatusStrings[mlmeConfirm->Status] );
    if( mlmeConfirm->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
    }
    switch( mlmeConfirm->MlmeRequest )
    {
        case MLME_JOIN:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                MibRequestConfirm_t mibGet;
                printf( "###### ===== JOINED ==== ######\r\n" );
                printf( "\r\nOTAA\r\n\r\n" );

                mibGet.Type = MIB_DEV_ADDR;
                LoRaMacMibGetRequestConfirm( &mibGet );
                printf( "DevAddr     : %08lX\r\n", mibGet.Param.DevAddr );

                printf( "\n\r\n" );
                mibGet.Type = MIB_CHANNELS_DATARATE;
                LoRaMacMibGetRequestConfirm( &mibGet );
                printf( "DATA RATE   : DR_%d\r\n", mibGet.Param.ChannelsDatarate );
                printf( "\r\n" );
                // Status is OK, node has joined the network
                eLoraWanState = LORA_WAN_STATE_SEND;
            }
            else
            {
                // Join was not successful. Try to join again
                JoinNetwork( );
            }
            break;
        }
        case MLME_LINK_CHECK:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Check DemodMargin
                // Check NbGateways
                if( ComplianceTest.Running == true )
                {
                    ComplianceTest.LinkCheck = true;
                    ComplianceTest.DemodMargin = mlmeConfirm->DemodMargin;
                    ComplianceTest.NbGateways = mlmeConfirm->NbGateways;
                }
            }
            break;
        }
        default:
            break;
    }
}

/*!
 * \brief   MLME-Indication event function
 *
 * \param   [IN] mlmeIndication - Pointer to the indication structure.
 */
static void MlmeIndication( MlmeIndication_t *mlmeIndication )
{
    if( mlmeIndication->Status != LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED )
    {
        printf( "\r\n###### ===== MLME-Indication ==== ######\r\n" );
        printf( "STATUS      : %s\r\n", myEventInfoStatusStrings[mlmeIndication->Status] );
    }
    if( mlmeIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
    }
    switch( mlmeIndication->MlmeIndication )
    {
        case MLME_REVERT_JOIN:
        {// The MAC signals that we shall provide an uplink as soon as possible
            OnTxNextPacketTimerEvent( NULL );
            break;
        }
        default:
            break;
    }
}

void OnMacProcessNotify( void )
{
    IsMacProcessPending = 1;
    App_vLoraWanProcessTaskRun();
}

uint32_t LoraWan_u32Init(void)
{
    LoRaMacStatus_t status;

    printf("#========== Lorawan Init ==============#\r\n");

    BoardInitMcu( );
    BoardInitPeriph( );

    g_macPrimitives.MacMcpsConfirm = McpsConfirm;
    g_macPrimitives.MacMcpsIndication = McpsIndication;
    g_macPrimitives.MacMlmeConfirm = MlmeConfirm;
    g_macPrimitives.MacMlmeIndication = MlmeIndication;
    g_macCallbacks.GetBatteryLevel = NULL;
    g_macCallbacks.GetTemperatureLevel = NULL;
    g_macCallbacks.NvmDataChange = NULL;
    g_macCallbacks.MacProcessNotify = OnMacProcessNotify;

    printf("#========== LORA WAN Start ==============#\r\n");

    status = LoRaMacInitialization( &g_macPrimitives, &g_macCallbacks, ACTIVE_REGION );
    if ( status != LORAMAC_STATUS_OK )
    {
        printf( "LoRaMac wasn't properly initialized, error: %s", myMacStatusStrings[status] );
        // Fatal error, endless loop.

        /* TODO Put error handling condition */

        while ( 1 )
        {
        }
    }

    eLoraWanState = LORA_WAN_STATE_RESTORE;
    printf("LoRaMacInitialization --> Done\r\n");

    LorawanTimeCounterClear();

//    app_wifi_scan();

//    spdt_2g4_on();
//    set_ble_antenna();

//    HAL_Delay(2000);
//    app_gnss_scan();

    printf("### Lora wan Init ###\r\n");

    return ((status != LORAMAC_STATUS_OK)? false : true);
}

void LoraWan_vRunStateMachine(void)
{
    LoRaMacStatus_t status;
    uint8_t au8DevEui[] = LORAWAN_DEVICE_EUI;
    uint8_t au8JoinEui[] = LORAWAN_JOIN_EUI;

    // Process Radio IRQ
    if( Radio.IrqProcess != NULL )
    {
        Radio.IrqProcess( );
    }
    // Processes the LoRaMac events
    LoRaMacProcess( );


    switch( eLoraWanState )
    {
		case LORA_WAN_STATE_INIT:
		{
			printf("#======== LORA_WAN_STATE_INIT ==========#\r\n");
			LoraWan_u32Init();
			break;
		}

        case LORA_WAN_STATE_RESTORE:
        {
            // Try to restore from NVM and query the mac if possible.
            if( 1 == 0 )
            {
                printf( "\r\n###### ===== CTXS RESTORED ==== ######\r\n\r\n" );
            }
            else
            {
#if( OVER_THE_AIR_ACTIVATION == 0 )
                // Tell the MAC layer which network server version are we connecting too.
                mibReq.Type = MIB_ABP_LORAWAN_VERSION;
                mibReq.Param.AbpLrWanVersion.Value = ABP_ACTIVATION_LRWAN_VERSION;
                LoRaMacMibSetRequestConfirm( &mibReq );
#endif

#if( ABP_ACTIVATION_LRWAN_VERSION == ABP_ACTIVATION_LRWAN_VERSION_V10x )
                mibReq.Type = MIB_GEN_APP_KEY;
                mibReq.Param.GenAppKey = GenAppKey;
                LoRaMacMibSetRequestConfirm( &mibReq );
#else
                printf("mibReq.Type = MIB_APP_KEY;\r\n");
                g_mibReq.Type = MIB_APP_KEY;
                g_mibReq.Param.AppKey = AppKey;
                LoRaMacMibSetRequestConfirm( &g_mibReq );
#endif

                g_mibReq.Type = MIB_NWK_KEY;
                g_mibReq.Param.NwkKey = NwkKey;
                LoRaMacMibSetRequestConfirm( &g_mibReq );

                // Initialize LoRaMac device unique ID if not already defined in Commissioning.h
                if( ( au8DevEui[0] == 0 ) && ( au8DevEui[1] == 0 ) &&
                    ( au8DevEui[2] == 0 ) && ( au8DevEui[3] == 0 ) &&
                    ( au8DevEui[4] == 0 ) && ( au8DevEui[5] == 0 ) &&
                    ( au8DevEui[6] == 0 ) && ( au8DevEui[7] == 0 ) )
                {
                    BoardGetUniqueId( au8DevEui );
                }

                g_mibReq.Type = MIB_DEV_EUI;
                g_mibReq.Param.DevEui = au8DevEui;
                LoRaMacMibSetRequestConfirm( &g_mibReq );

                g_mibReq.Type = MIB_JOIN_EUI;
                g_mibReq.Param.JoinEui = au8JoinEui;
                LoRaMacMibSetRequestConfirm( &g_mibReq );


#if( OVER_THE_AIR_ACTIVATION == 0 )
                // Choose a random device address if not already defined in Commissioning.h
                if( DevAddr == 0 )
                {
                    // Random seed initialization
                    srand1( BoardGetRandomSeed( ) );

                    // Choose a random device address
                    DevAddr = randr( 0, 0x01FFFFFF );
                }

                g_mibReq.Type = MIB_NET_ID;
                g_mibReq.Param.NetID = LORAWAN_NETWORK_ID;
                LoRaMacMibSetRequestConfirm( &g_mibReq );

                g_mibReq.Type = MIB_DEV_ADDR;
                g_mibReq.Param.DevAddr = DevAddr;
                LoRaMacMibSetRequestConfirm( &g_mibReq );

                g_mibReq.Type = MIB_F_NWK_S_INT_KEY;
                g_mibReq.Param.FNwkSIntKey = FNwkSIntKey;
                LoRaMacMibSetRequestConfirm( &g_mibReq );

                g_mibReq.Type = MIB_S_NWK_S_INT_KEY;
                g_mibReq.Param.SNwkSIntKey = SNwkSIntKey;
                LoRaMacMibSetRequestConfirm( &g_mibReq );

                g_mibReq.Type = MIB_NWK_S_ENC_KEY;
                g_mibReq.Param.NwkSEncKey = NwkSEncKey;
                LoRaMacMibSetRequestConfirm( &g_mibReq );

                g_mibReq.Type = MIB_APP_S_KEY;
                g_mibReq.Param.AppSKey = AppSKey;
                LoRaMacMibSetRequestConfirm( &g_mibReq );
#endif
            }
            eLoraWanState = LORA_WAN_STATE_START;
            break;
        }

        case LORA_WAN_STATE_START:
        {
        	LorawanTimeCounterClear();

        	printf("State: LORA_WAN_STATE_START\r\n");
            TimerInit( &TxNextPacketTimer, OnTxNextPacketTimerEvent );

            g_mibReq.Type = MIB_PUBLIC_NETWORK;
            g_mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
            LoRaMacMibSetRequestConfirm( &g_mibReq );

            g_mibReq.Type = MIB_ADR;
            g_mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
            LoRaMacMibSetRequestConfirm( &g_mibReq );

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
            LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
#endif
            g_mibReq.Type = MIB_SYSTEM_MAX_RX_ERROR;
            g_mibReq.Param.SystemMaxRxError = 20;
            LoRaMacMibSetRequestConfirm( &g_mibReq );

            LoRaMacStart( );

            g_mibReq.Type = MIB_NETWORK_ACTIVATION;
            status = LoRaMacMibGetRequestConfirm( &g_mibReq );

            if( status == LORAMAC_STATUS_OK )
            {
                if( g_mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE )
                {
                    eLoraWanState = LORA_WAN_STATE_JOIN;
                }
                else
                {
                    eLoraWanState = LORA_WAN_STATE_SEND;
                    NextTx = true;
                }
            }
            break;
        }
        case LORA_WAN_STATE_JOIN:
        {
        	printf("State: LORA_WAN_STATE_JOIN\r\n");
            g_mibReq.Type = MIB_DEV_EUI;
            LoRaMacMibGetRequestConfirm( &g_mibReq );
            printf( "DevEui      : %02X", g_mibReq.Param.DevEui[0] );
            for( int i = 1; i < 8; i++ )
            {
                printf( "-%02X", g_mibReq.Param.DevEui[i] );
            }
            printf( "\r\n" );
            g_mibReq.Type = MIB_JOIN_EUI;
            LoRaMacMibGetRequestConfirm( &g_mibReq );
            printf( "AppEui      : %02X", g_mibReq.Param.JoinEui[0] );
            for( int i = 1; i < 8; i++ )
            {
                printf( "-%02X", g_mibReq.Param.JoinEui[i] );
            }
            printf( "\r\n" );
            printf( "AppKey      : %02X", NwkKey[0] );
            for( int i = 1; i < 16; i++ )
            {
                printf( " %02X", NwkKey[i] );
            }
            printf( "\n\r\n" );
#if( OVER_THE_AIR_ACTIVATION == 0 )
            printf( "###### ===== JOINED ==== ######\r\n" );
            printf( "\r\nABP\r\n\r\n" );
            printf( "DevAddr     : %08lX\r\n", DevAddr );
            printf( "NwkSKey     : %02X", FNwkSIntKey[0] );
            for( int i = 1; i < 16; i++ )
            {
                printf( " %02X", FNwkSIntKey[i] );
            }
            printf( "\r\n" );
            printf( "AppSKey     : %02X", AppSKey[0] );
            for( int i = 1; i < 16; i++ )
            {
                printf( " %02X", AppSKey[i] );
            }
            printf( "\n\r\n" );

            g_mibReq.Type = MIB_NETWORK_ACTIVATION;
            g_mibReq.Param.NetworkActivation = ACTIVATION_TYPE_ABP;
            LoRaMacMibSetRequestConfirm( &g_mibReq );

            eLoraWanState = LORA_WAN_STATE_SEND;
#else

            JoinNetwork( );
#endif
            break;
        }
        case LORA_WAN_STATE_SEND:
        {
          /*
           *
           * for send the data
           */
          printf( "\r\n## STATE_SEND ##\r\n" );
            if( NextTx == true )
            {
                PrepareTxFrame( AppPort );

                NextTx = SendFrame( );
            }
            eLoraWanState = LORA_WAN_STATE_CYCLE;
            break;
        }

        case LORA_WAN_STATE_CYCLE:
        {
          printf( "\r\n## STATE_CYCLE ##\r\n" );
            eLoraWanState = LORA_WAN_STATE_SLEEP;
            if( ComplianceTest.Running == true )
            {
                // Schedule next packet transmission
                TxDutyCycleTime = 5000; // 5000 ms
            }
            else
            {
                // Schedule next packet transmission
                //TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
            	TxDutyCycleTime = APP_TX_DUTYCYCLE;
            }

            // Schedule next packet transmission
            TimerSetValue( &TxNextPacketTimer, TxDutyCycleTime );
            TimerStart( &TxNextPacketTimer );
            break;
        }

        case LORA_WAN_STATE_TIMER:
        {
        	printf("Lora WAN Device Timer: %d\r\n", s_u32LorawanTimerCounter);
        	eLoraWanState = LORA_WAN_STATE_SLEEP;
        	break;
        }

        case LORA_WAN_STATE_SLEEP:
        {
//              printf( "\r\n## STATE_SLEEP ##\r\n" );
            if( 1 == 0 )
            {
                printf( "\r\n###### ===== CTXS STORED ==== ######\r\n" );
            }

            CRITICAL_SECTION_BEGIN( );
            if( IsMacProcessPending == 1 )
            {
                // Clear flag and prevent MCU to go into low power modes.
                IsMacProcessPending = 0;
            }
            else
            {
                // The MCU wakes up through events
//                BoardLowPowerHandler( );
            }
            CRITICAL_SECTION_END( );
            break;
        }
        default:
        {
            eLoraWanState = LORA_WAN_STATE_START;
            break;
        }
    }
}

/**
 * Main application entry point.
 */
int lora_wan( void )
{
    LoRaMacPrimitives_t macPrimitives;
    LoRaMacCallback_t macCallbacks;
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;
    uint8_t au8DevEui[] = LORAWAN_DEVICE_EUI;
    uint8_t au8JoinEui[] = LORAWAN_JOIN_EUI;

    printf("#========== Jump into Lora app ==============#\r\n");

    BoardInitMcu( );
    BoardInitPeriph( );

    RtcInit();

    macPrimitives.MacMcpsConfirm = McpsConfirm;
    macPrimitives.MacMcpsIndication = McpsIndication;
    macPrimitives.MacMlmeConfirm = MlmeConfirm;
    macPrimitives.MacMlmeIndication = MlmeIndication;
    macCallbacks.GetBatteryLevel = NULL;
    macCallbacks.GetTemperatureLevel = NULL;
    macCallbacks.NvmDataChange = NULL;
    macCallbacks.MacProcessNotify = OnMacProcessNotify;

    printf("#========== LORA WAN Start ==============#\r\n");

    status = LoRaMacInitialization( &macPrimitives, &macCallbacks, ACTIVE_REGION );
    if ( status != LORAMAC_STATUS_OK )
    {
        printf( "LoRaMac wasn't properly initialized, error: %s", myMacStatusStrings[status] );
        // Fatal error, endless loop.
        while ( 1 )
        {
        }
    }

    printf("LoRaMacInitialization\r\n");


    app_wifi_scan();


    eLoraWanState = LORA_WAN_STATE_RESTORE;

    printf( "###### ===== ClassA demo application v1.0.RC1 ==== ######\r\n\r\n" );

    while( 1 )
    {
        // Process Radio IRQ
        if( Radio.IrqProcess != NULL )
        {
            Radio.IrqProcess( );
        }
        // Processes the LoRaMac events
        LoRaMacProcess( );


        switch( eLoraWanState )
        {
            case LORA_WAN_STATE_RESTORE:
            {
                // Try to restore from NVM and query the mac if possible.
                if( 1 == 0 )
                {
                    printf( "\r\n###### ===== CTXS RESTORED ==== ######\r\n\r\n" );
                }
                else
                {
#if( OVER_THE_AIR_ACTIVATION == 0 )
                    // Tell the MAC layer which network server version are we connecting too.
                    mibReq.Type = MIB_ABP_LORAWAN_VERSION;
                    mibReq.Param.AbpLrWanVersion.Value = ABP_ACTIVATION_LRWAN_VERSION;
                    LoRaMacMibSetRequestConfirm( &mibReq );
#endif

#if( ABP_ACTIVATION_LRWAN_VERSION == ABP_ACTIVATION_LRWAN_VERSION_V10x )
                    mibReq.Type = MIB_GEN_APP_KEY;
                    mibReq.Param.GenAppKey = GenAppKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );
#else
                    printf("mibReq.Type = MIB_APP_KEY;\r\n");
                    mibReq.Type = MIB_APP_KEY;
                    mibReq.Param.AppKey = AppKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );
#endif

                    mibReq.Type = MIB_NWK_KEY;
                    mibReq.Param.NwkKey = NwkKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    // Initialize LoRaMac device unique ID if not already defined in Commissioning.h
                    if( ( au8DevEui[0] == 0 ) && ( au8DevEui[1] == 0 ) &&
                        ( au8DevEui[2] == 0 ) && ( au8DevEui[3] == 0 ) &&
                        ( au8DevEui[4] == 0 ) && ( au8DevEui[5] == 0 ) &&
                        ( au8DevEui[6] == 0 ) && ( au8DevEui[7] == 0 ) )
                    {
                        BoardGetUniqueId( au8DevEui );
                    }

                    mibReq.Type = MIB_DEV_EUI;
                    mibReq.Param.DevEui = au8DevEui;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    mibReq.Type = MIB_JOIN_EUI;
                    mibReq.Param.JoinEui = au8JoinEui;
                    LoRaMacMibSetRequestConfirm( &mibReq );


#if( OVER_THE_AIR_ACTIVATION == 0 )
                    // Choose a random device address if not already defined in Commissioning.h
                    if( DevAddr == 0 )
                    {
                        // Random seed initialization
                        srand1( BoardGetRandomSeed( ) );

                        // Choose a random device address
                        DevAddr = randr( 0, 0x01FFFFFF );
                    }

                    mibReq.Type = MIB_NET_ID;
                    mibReq.Param.NetID = LORAWAN_NETWORK_ID;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    mibReq.Type = MIB_DEV_ADDR;
                    mibReq.Param.DevAddr = DevAddr;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    mibReq.Type = MIB_F_NWK_S_INT_KEY;
                    mibReq.Param.FNwkSIntKey = FNwkSIntKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    mibReq.Type = MIB_S_NWK_S_INT_KEY;
                    mibReq.Param.SNwkSIntKey = SNwkSIntKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    mibReq.Type = MIB_NWK_S_ENC_KEY;
                    mibReq.Param.NwkSEncKey = NwkSEncKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    mibReq.Type = MIB_APP_S_KEY;
                    mibReq.Param.AppSKey = AppSKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );
#endif
                }
                eLoraWanState = LORA_WAN_STATE_START;
                break;
            }

            case LORA_WAN_STATE_START:
            {
            	printf("State: LORA_WAN_STATE_START\r\n");
                TimerInit( &TxNextPacketTimer, OnTxNextPacketTimerEvent );

                mibReq.Type = MIB_PUBLIC_NETWORK;
                mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
                LoRaMacMibSetRequestConfirm( &mibReq );

                mibReq.Type = MIB_ADR;
                mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                LoRaMacMibSetRequestConfirm( &mibReq );

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
                LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
#endif
                mibReq.Type = MIB_SYSTEM_MAX_RX_ERROR;
                mibReq.Param.SystemMaxRxError = 20;
                LoRaMacMibSetRequestConfirm( &mibReq );

                LoRaMacStart( );

                mibReq.Type = MIB_NETWORK_ACTIVATION;
                status = LoRaMacMibGetRequestConfirm( &mibReq );

                if( status == LORAMAC_STATUS_OK )
                {
                    if( mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE )
                    {
                        eLoraWanState = LORA_WAN_STATE_JOIN;
                    }
                    else
                    {
                        eLoraWanState = LORA_WAN_STATE_SEND;
                        NextTx = true;
                    }
                }
                break;
            }
            case LORA_WAN_STATE_JOIN:
            {
            	printf("State: LORA_WAN_STATE_JOIN\r\n");
                mibReq.Type = MIB_DEV_EUI;
                LoRaMacMibGetRequestConfirm( &mibReq );
                printf( "DevEui      : %02X", mibReq.Param.DevEui[0] );
                for( int i = 1; i < 8; i++ )
                {
                    printf( "-%02X", mibReq.Param.DevEui[i] );
                }
                printf( "\r\n" );
                mibReq.Type = MIB_JOIN_EUI;
                LoRaMacMibGetRequestConfirm( &mibReq );
                printf( "AppEui      : %02X", mibReq.Param.JoinEui[0] );
                for( int i = 1; i < 8; i++ )
                {
                    printf( "-%02X", mibReq.Param.JoinEui[i] );
                }
                printf( "\r\n" );
                printf( "AppKey      : %02X", NwkKey[0] );
                for( int i = 1; i < 16; i++ )
                {
                    printf( " %02X", NwkKey[i] );
                }
                printf( "\n\r\n" );
#if( OVER_THE_AIR_ACTIVATION == 0 )
                printf( "###### ===== JOINED ==== ######\r\n" );
                printf( "\r\nABP\r\n\r\n" );
                printf( "DevAddr     : %08lX\r\n", DevAddr );
                printf( "NwkSKey     : %02X", FNwkSIntKey[0] );
                for( int i = 1; i < 16; i++ )
                {
                    printf( " %02X", FNwkSIntKey[i] );
                }
                printf( "\r\n" );
                printf( "AppSKey     : %02X", AppSKey[0] );
                for( int i = 1; i < 16; i++ )
                {
                    printf( " %02X", AppSKey[i] );
                }
                printf( "\n\r\n" );

                mibReq.Type = MIB_NETWORK_ACTIVATION;
                mibReq.Param.NetworkActivation = ACTIVATION_TYPE_ABP;
                LoRaMacMibSetRequestConfirm( &mibReq );

                eLoraWanState = LORA_WAN_STATE_SEND;
#else

                JoinNetwork( );
#endif
                break;
            }
            case LORA_WAN_STATE_SEND:
            {
              /*
               *
               * for send the data
               */
              printf( "\r\n## STATE_SEND ##\r\n" );
                if( NextTx == true )
                {
                    PrepareTxFrame( AppPort );

                    NextTx = SendFrame( );
                }
                eLoraWanState = LORA_WAN_STATE_CYCLE;
                break;
            }
            case LORA_WAN_STATE_CYCLE:
            {
              printf( "\r\n## STATE_CYCLE ##\r\n" );
                eLoraWanState = LORA_WAN_STATE_SLEEP;
                if( ComplianceTest.Running == true )
                {
                    // Schedule next packet transmission
                    TxDutyCycleTime = 5000; // 5000 ms
                }
                else
                {
                    // Schedule next packet transmission
                    //TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
		    TxDutyCycleTime = APP_TX_DUTYCYCLE;
                }

                // Schedule next packet transmission
                TimerSetValue( &TxNextPacketTimer, TxDutyCycleTime );
                TimerStart( &TxNextPacketTimer );
                break;
            }
            case LORA_WAN_STATE_SLEEP:
            {
//              printf( "\r\n## STATE_SLEEP ##\r\n" );
                if( 1 == 0 )
                {
                    printf( "\r\n###### ===== CTXS STORED ==== ######\r\n" );
                }

                CRITICAL_SECTION_BEGIN( );
                if( IsMacProcessPending == 1 )
                {
                    // Clear flag and prevent MCU to go into low power modes.
                    IsMacProcessPending = 0;
                }
                else
                {
                    // The MCU wakes up through events
                    BoardLowPowerHandler( );
                }
                CRITICAL_SECTION_END( );
                break;
            }
            default:
            {
                eLoraWanState = LORA_WAN_STATE_START;
                break;
            }
        }
    }
}


