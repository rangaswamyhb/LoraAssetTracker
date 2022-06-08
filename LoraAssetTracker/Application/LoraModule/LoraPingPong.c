/*
 * LoraPingPong.c
 *
 *  Created on: 08-Jun-2022
 *      Author: Ranga
 */

/*!
 * \file      main.c
 *
 * \brief     Ping-Pong implementation
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

#include "LoraPingPong.h"
#include <string.h>
#include "board.h"
#include "lr1110_gpio.h"
#include "delay.h"
#include "lr1110_timer.h"
#include "radio.h"
#include "MyApplication.h"


#if defined( REGION_IN865 )
#define RF_FREQUENCY                                865000000 // Hz
#endif

#define TX_OUTPUT_POWER                             22        // dBm

#if defined( USE_MODEM_LORA )

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       12         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#elif defined( USE_MODEM_FSK )

#define FSK_FDEV                                    25000     // Hz
#define FSK_DATARATE                                50000     // bps
#define FSK_BANDWIDTH                               50000     // Hz
#define FSK_AFC_BANDWIDTH                           83333     // Hz
#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   false

#else
    #error "Please define a modem in the compiler options."
#endif

#define PING_PONG_TX_PACKET_TIME    4000

#define RX_TIMEOUT_VALUE                            1000//3000//1100
#define BUFFER_SIZE                                 10 // Define the payload size here


typedef enum
{
    LORA_PP_STATE_LOWPOWER,
    LORA_PP_STATE_RX,
    LORA_PP_STATE_RX_TIMEOUT,
    LORA_PP_STATE_RX_ERROR,
    LORA_PP_STATE_TX,
    LORA_PP_STATE_TX_TMEOUT,
	LORA_PP_STATE_FORCE_TX,
	LORA_PP_STATE_MAX

}eLoraPingPongStates_t;


const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

eLoraPingPongStates_t eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t PingPongTxNextPacketTimer;


uint32_t u32RxWindowTimeout = 0;


/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;


/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

bool isMaster = true;


void LoraPingPong_vSetStateMachineStatus(eLoraPingPongStates_t eState)
{
	if((eState < LORA_PP_STATE_MAX) && (eLoraPingPongStateMachineStatus != eState))
	{
		eLoraPingPongStateMachineStatus = eState;
	}
}

eLoraPingPongStates_t LoraPingPong_eGetStateMachineStatus(void)
{
	return eLoraPingPongStateMachineStatus;
}


static void OnPingPongTxNextPacketTimerEvent( void* context )
{
	TimerStop( &PingPongTxNextPacketTimer );
	TimerSetValue( &PingPongTxNextPacketTimer, PING_PONG_TX_PACKET_TIME );

	TimerStart( &PingPongTxNextPacketTimer );

	if(eLoraPingPongStateMachineStatus == LORA_PP_STATE_LOWPOWER)
	{
		eLoraPingPongStateMachineStatus = LORA_PP_STATE_FORCE_TX;
	}
}

uint8_t LoraPingPong_u8GetRssiValue(void)
{
	return RssiValue;
}

uint8_t LoraPingPong_u8GetSnrValue(void)
{
	return SnrValue;
}

void LoraPingPong_vInit(void)
{
    // Target board initialization
    BoardInitMcu( );

//    RtcInit();
//    BoardInitPeriph( );

    printf("#=========== Enter to Lora Main ==========#\r\n");

    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;


    Radio.Init( &RadioEvents );
    printf("Radio.Init\r\n");

    Radio.SetChannel( RF_FREQUENCY );
    printf("Radio.SetChannel\r\n");

#if defined( USE_MODEM_LORA )

    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 1000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

    Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE );

#elif defined( USE_MODEM_FSK )

    Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                                  FSK_DATARATE, 0,
                                  FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, 0, 3000 );

    Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                  0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                  0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                  0, 0,false, true );

    Radio.SetMaxPayloadLength( MODEM_FSK, BUFFER_SIZE );

#else
    #error "Please define a frequency band in the compiler options."
#endif

    printf("Radio Config\r\n");

    if(App_u8GetDeviceIsMaster())
    {
    	printf("Radio Rx\r\n");
    	Radio.Rx( RX_TIMEOUT_VALUE + RX_TIMEOUT_VALUE );
    	u32RxWindowTimeout =  5 * RX_TIMEOUT_VALUE;
    	printf("\r\n\r\n########### Master Mode #############\r\n");
    }
    else
    {
    	eLoraPingPongStateMachineStatus = LORA_PP_STATE_FORCE_TX;
    	u32RxWindowTimeout = RX_TIMEOUT_VALUE + 500;

	    TimerInit( &PingPongTxNextPacketTimer, OnPingPongTxNextPacketTimerEvent );
		TimerSetValue( &PingPongTxNextPacketTimer, PING_PONG_TX_PACKET_TIME );

		TimerStart( &PingPongTxNextPacketTimer );

		printf("\r\n\r\n########### Slave Mode #############\r\n");
    }
    printf("### Lora Ping Pong Init Done ###\r\n");
}

void LoraPingPong_vRunStateMachine(void)
{
	uint32_t i = 0;
    switch( eLoraPingPongStateMachineStatus )
    {
    case LORA_PP_STATE_RX:
    {
    	printf("RX State:\r\n");
    	printf("Rx RSSI: %d, 0x%x,  SNR value: %d, 0x%x\r\n", RssiValue, RssiValue, SnrValue, SnrValue);

    	if(App_u8GetDeviceIsMaster())
    	{
    		if(BufferSize > 0)
    		{
				if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
				{
					// Indicates on a LED that the received frame is a PING
					printf("Ping string Match\r\n");

					// Send the reply to the PONG string
					Buffer[0] = 'P';
					Buffer[1] = 'O';
					Buffer[2] = 'N';
					Buffer[3] = 'G';
					// We fill the buffer with numbers for the payload
					for( i = 4; i < BufferSize; i++ )
					{
						Buffer[i] = i - 4;
					}
					DelayMs( 1 );
					Radio.Send( Buffer, BufferSize );
					printf("Sending Pong....\r\n");
				}
    		}
    	}
    	else
    	{
    		if(BufferSize > 0)
    		{
    			if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
				{
					printf("PONG string matches\r\n");
				}
    		}
    	}

        eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
        break;
    }

    case LORA_PP_STATE_TX:
    {
        // Indicates on a LED that we have sent a PING [Master]
        // Indicates on a LED that we have sent a PONG [Slave]
    	printf("TX State\r\n");
    	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
    	        	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
    	        	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
    	        	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
        Radio.Rx( u32RxWindowTimeout );
        eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
        break;
    }

    case LORA_PP_STATE_RX_TIMEOUT:
    case LORA_PP_STATE_RX_ERROR:
    {
    	printf("RX_TIMEOUT State\r\n");
    	if( App_u8GetDeviceIsMaster() )
    	{
    		Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
    		    	        	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
    		    	        	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
    		    	        	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    		Radio.Rx( u32RxWindowTimeout );
    	}

        eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
        break;
    }

    case LORA_PP_STATE_TX_TMEOUT:
    {
    	printf("TX_TIMEOUT\r\n");
    	if(App_u8GetDeviceIsMaster())
    	{
    		Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
    		    	        	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
    		    	        	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
    		    	        	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    		Radio.Rx( u32RxWindowTimeout );
    	}
    	else
    	{
    		Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
    		    	        	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
    		    	        	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
    		    	        	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    		 Radio.Rx( u32RxWindowTimeout );
    	}
        eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
        break;
    }

    case LORA_PP_STATE_FORCE_TX:
    {
    	printf("## FORCE_TX ##\r\n");
    	printf("Sending PING\r\n");
		// Send the next PING frame
		Buffer[0] = 'P';
		Buffer[1] = 'I';
		Buffer[2] = 'N';
		Buffer[3] = 'G';
		// We fill the buffer with numbers for the payload
		for( i = 4; i < BufferSize; i++ )
		{
			Buffer[i] = i - 4;
		}
		DelayMs( 1 );
		Radio.Send( Buffer, BufferSize );
		eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
    	break;
    }

    case LORA_PP_STATE_LOWPOWER:
    default:
        // Set low power
        break;
    }

//    Radio.Sleep();
//    BoardLowPowerHandler( );

    // Process Radio IRQ
    if( Radio.IrqProcess != NULL )
    {
        Radio.IrqProcess( );
    }
}

/**
 * Main application entry point.
 */
int lora_ping_pong( void )
{
    bool isMaster = true;
    uint8_t i;

    // Target board initialization
    BoardInitMcu( );

//    RtcInit();
//    BoardInitPeriph( );

    printf("#=========== Enter to Lora Main ==========#\r\n");

    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;


    Radio.Init( &RadioEvents );
    printf("Radio.Init\r\n");

    Radio.SetChannel( RF_FREQUENCY );
    printf("Radio.SetChannel\r\n");

#if defined( USE_MODEM_LORA )

    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

    Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE );

#elif defined( USE_MODEM_FSK )

    Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                                  FSK_DATARATE, 0,
                                  FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, 0, 3000 );

    Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                  0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                  0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                  0, 0,false, true );

    Radio.SetMaxPayloadLength( MODEM_FSK, BUFFER_SIZE );

#else
    #error "Please define a frequency band in the compiler options."
#endif

    printf("Radio Config\r\n");

    if(App_u8GetDeviceIsMaster())
    {
    	Radio.Rx( RX_TIMEOUT_VALUE + RX_TIMEOUT_VALUE );
    }
    else
    {
    	Radio.Rx( RX_TIMEOUT_VALUE );
    }


    printf("Radio Rx\r\n");

    while( 1 )
    {
        switch( eLoraPingPongStateMachineStatus )
        {
        case LORA_PP_STATE_RX:
        {
            if( isMaster == true )
            {
                if( BufferSize > 0 )
                {
                    if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
                    {
                        // Indicates on a LED that the received frame is a PONG
                    	printf("PONG string matches\r\n");
                        // Send the next PING frame
                        Buffer[0] = 'P';
                        Buffer[1] = 'I';
                        Buffer[2] = 'N';
                        Buffer[3] = 'G';
                        // We fill the buffer with numbers for the payload
                        for( i = 4; i < BufferSize; i++ )
                        {
                            Buffer[i] = i - 4;
                        }
                        DelayMs( 1 );
                        Radio.Send( Buffer, BufferSize );
                    }
                    else if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
                    { // A master already exists then become a slave
                        isMaster = false;
                        Radio.Rx( RX_TIMEOUT_VALUE );
                    }
                    else // valid reception but neither a PING or a PONG message
                    {    // Set device as master ans start again
                        isMaster = true;
//                        Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
//                                	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
//                                	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
//                                	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
                        Radio.Rx( RX_TIMEOUT_VALUE );
                    }
                }
            }
            else
            {
                if( BufferSize > 0 )
                {
                    if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
                    {
                        // Indicates on a LED that the received frame is a PING
                    	printf("Ping string Match\r\n");

                        // Send the reply to the PONG string
                        Buffer[0] = 'P';
                        Buffer[1] = 'O';
                        Buffer[2] = 'N';
                        Buffer[3] = 'G';
                        // We fill the buffer with numbers for the payload
                        for( i = 4; i < BufferSize; i++ )
                        {
                            Buffer[i] = i - 4;
                        }
                        DelayMs( 1 );
                        Radio.Send( Buffer, BufferSize );
                    }
                    else // valid reception but not a PING as expected
                    {    // Set device as master and start again
//                        isMaster = true;
//                        Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
//                                	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
//                                	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
//                                	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
                        Radio.Rx( RX_TIMEOUT_VALUE );
                    }
                }
            }
            eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
            break;
        }

        case LORA_PP_STATE_TX:
        {
            // Indicates on a LED that we have sent a PING [Master]
            // Indicates on a LED that we have sent a PONG [Slave]
        	printf("TX State\r\n");
//        	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
//        	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
//        	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
//        	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
            Radio.Rx( RX_TIMEOUT_VALUE );
            eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
            break;
        }

        case LORA_PP_STATE_RX_TIMEOUT:
        case LORA_PP_STATE_RX_ERROR:
        {
        	printf("RX_TIMEOUT State\r\n");
            if( isMaster == true )
            {
                // Send the next PING frame
                Buffer[0] = 'P';
                Buffer[1] = 'I';
                Buffer[2] = 'N';
                Buffer[3] = 'G';
                for( i = 4; i < BufferSize; i++ )
                {
                    Buffer[i] = i - 4;
                }
                DelayMs( 1 );
                Radio.Send( Buffer, BufferSize );
            }
            else
            {
//            	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
//            	        	                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
//            	        	                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
//            	        	                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
                Radio.Rx( RX_TIMEOUT_VALUE );
            }
            eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
            break;
        }

        case LORA_PP_STATE_TX_TMEOUT:
            Radio.Rx( RX_TIMEOUT_VALUE );
            printf("TX_TIMEOUT\r\n");
            eLoraPingPongStateMachineStatus = LORA_PP_STATE_LOWPOWER;
            break;
        case LORA_PP_STATE_LOWPOWER:
        default:
            // Set low power
            break;
        }

        BoardLowPowerHandler( );
        // Process Radio IRQ
        if( Radio.IrqProcess != NULL )
        {
            Radio.IrqProcess( );
        }
    }
}

void OnTxDone( void )
{
    Radio.Sleep( );
    eLoraPingPongStateMachineStatus = LORA_PP_STATE_TX;
    printf("OnTxDone\r\n");
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    App_vLoraRssiSet(rssi);
    App_vLoraSnrSet(snr);
    App_vLoraPingPongPacketCounterIncreament();
    SnrValue = snr;
    eLoraPingPongStateMachineStatus = LORA_PP_STATE_RX;

    App_ReqAdvUpdateData();

    printf("OnRxDone\r\n");
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    eLoraPingPongStateMachineStatus = LORA_PP_STATE_TX_TMEOUT;
    printf("OnTxTimeout\r\n");
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    eLoraPingPongStateMachineStatus = LORA_PP_STATE_RX_TIMEOUT;
    printf("OnRxTimeout\r\n");
}

void OnRxError( void )
{
    Radio.Sleep( );
    eLoraPingPongStateMachineStatus = LORA_PP_STATE_RX_ERROR;
    printf("OnRxError\r\n");
}



