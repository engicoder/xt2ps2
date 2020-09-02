/* =======================================================================
 * ps2d_xcvr.h
 *
 * Purpose:
 *  Transmit and receive data using the PS/2 device bus.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * --------------------------------------------------------------------- */ 
#include <stdint.h>
#include <stdbool.h>
#include "ps2.h"
#include "atomic_hal.h"

#ifndef PS2D_XCVR_H_
#define PS2D_XCVR_H_

/* === Type Definitions ================================================ */
typedef enum Ps2dXcvrStatus
{
    PS2D_XCVR_STATUS_NONE           = 0,
    PS2D_XCVR_RECV_BUFFER_FULL      = (1 << 0),
    PS2D_XCVR_RECV_BUFFER_OVERFLOW  = (1 << 1),
    PS2D_XCVR_RECV_FRAME_ERROR      = (1 << 2),
    PS2D_XCVR_XMIT_INTERRUPTED      = (1 << 3),
	PS2D_XCVR_XMIT_BUFFER_FULL      = (1 << 4),
    PS2D_XCVR_XMIT_COMPLETE         = (1 << 5),
    /* Reserved                     = (1 << 6), */
    /* Reserved                     = (1 << 7), */
} Ps2dXcvrStatus;

#define PS2D_XCVR_OPT_DEFAULT PS2D_XCVR_OPT_NONE

/* === Macro Defininitions ============================================ */
/* Convert a time interval to clock counts */
#define PS2D_XCVR_INTERVAL_US_TO_CLK_COUNT(interval) ((uint16_t)(((uint32_t)interval * 4UL) / (uint32_t)PS2_CLOCK_PERIOD))
#define PS2D_XCVR_INTERVAL_MS_TO_CLK_COUNT(interval) ((uint16_t)(((uint32_t)interval * 1000UL * 4UL) / (uint32_t)PS2_CLOCK_PERIOD))

#define PS2D_XCVR_CLOCKS_PER_MS (4000UL / PS2_CLOCK_PERIOD)

/* === External Variables ============================================== */
extern volatile uint8_t _ps2dXcvrStatus;
extern volatile uint16_t _ps2dXcvrClockCount;
extern volatile uint16_t _ps2dXcvrIdleCount;

/* === Forward declarations ============================================ */

/* -----------------------------------------------------------------------
* Description:
*  Initializes the PS/2 Transceiver module be configuring the PS/2 bus
*  bus lines, waiting for a specified start up delay to expire and
*  setting up interrupt to listen for data from the host.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dXcvr_Init(void);

/* -----------------------------------------------------------------------
* Description:
*  Reads a byte of data that has been received from the PS/2 host.
*
* Parameters:
*  n/a
*
* Returns: uint8_t
*  The byte of data received.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
uint8_t Ps2dXcvr_ReadReceivedData(void);

/* -----------------------------------------------------------------------
* Description:
*  Enables the PS/2 transceiver.
*
* Parameters:
*  n/a
*
* Returns: 
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dXcvr_Enable(void);

/* -----------------------------------------------------------------------
* Description:
*  Disbles the PS/2 transceiver.
*
* Parameters:
*  n/a
*
* Returns: 
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dXcvr_Disable(void);

/* -----------------------------------------------------------------------
* Description:
*  Initiates retransmission of the last byte of data transmitted to the
*  PS/2 host using the PS/2 bus.
*
* Parameters:
*  n/a
*
* Returns: 
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dXcvr_RetransmitLastByte(void);

/* -----------------------------------------------------------------------
* Description:
*  Initiates transmission of the specified byte of data to the PS/2 host
*  using the PS/2 bus.
* Note:
*  A status check should be made prior to calling this function.
*  Calling this function while a previous transmission is in progress is 
*  not supported. Behavior in this situation is undefined.
*
* Parameters:
*  data - the byte of data to be transmitted.
*
* Returns:
*  true if data was buffered for transmission, false otherwise
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Ps2dXcvr_TransmitDataAsync(uint8_t data);

/* === Forward declarations ============================================ */

/* -----------------------------------------------------------------------
* Description:
*  Returns the status value of the PS/2 Device Transceiver.
*
* Parameters:
*  n/a
*
* Returns: Ps2dXcvrStatus
*  The status value of the PS/2 Device Transceiver.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline Ps2dXcvrStatus Ps2dXcvr_GetStatus(void)
{
    return (Ps2dXcvrStatus)_ps2dXcvrStatus;
}

/* -----------------------------------------------------------------------
* Description:
*  Determines if a specified status flag is currently set.
*
* Parameters:
*  status - one or more of the Ps2dXcvrStatus status flags.
*
* Returns: bool
*  true  - if the specified status flag(s) is/are set.
*  false - otherwise.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool Ps2dXcvr_StatusIsSet(Ps2dXcvrStatus status)
{
    return (bool)((uint8_t)_ps2dXcvrStatus & status);
}

/* -----------------------------------------------------------------------
 * Description:
 *  Determines if the PS/2 bus is idle.
 *
 * Parameters:
 *  n/a
 *
 * Returns: bool
 *  true  - if the PS/2 bus is idle.
 *  false - otherwise.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Ps2dXcvr_BusIdle(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Determines if the Transceiver has received data from the PS/2 host.
 *
 * Parameters:
 *  n/a
 *
 * Returns: bool
 *  true  - if the Transceiver has received data.
 *  false - otherwise.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool Ps2dXcvr_DataReceived(void)
{
    return Ps2dXcvr_StatusIsSet(PS2D_XCVR_RECV_BUFFER_FULL);
}


/* -----------------------------------------------------------------------
 * Description:
 *  Gets the number of clock counts. This value can be compared to the
 *  result of earlier calls to determine duration.
 *  Note: Clock counts will wrap after ~2.6 seconds.
 *
 * Parameters:
 *  n/a
 *
 * Returns: bool
 *  The current clock count
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline uint16_t Ps2dXcvr_GetClockCount(void)
{
    uint16_t clockCount = 0;
    ATOMIC()
    {
        clockCount = _ps2dXcvrClockCount;
    }
    return clockCount;    
}

/* -----------------------------------------------------------------------
 * Description:
 *  Gets the number of clock counts since the bus last went idle.
 *
 * Parameters:
 *  n/a
 *
 * Returns: bool
 *  The clock count since bus went idle.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline uint16_t Ps2dXcvr_GetIdleCount(void)
{
    uint16_t idleCount = 0;
    ATOMIC()
    {
        idleCount = _ps2dXcvrIdleCount;
    }
    return idleCount;    
}


#endif /* PS2D_XCVR_H_ */