/*
* Copyright (c) 2016 Academia Sinica, Institute of Information Science
*
* License:
*
*      GPL 3.0 : The content of this file is subject to the terms and
*      cnditions defined in file 'COPYING.txt', which is part of this source
*      code package.
*
* Project Name:
*
*      BeDIPS
*
* File Description:
*
*       Communication unit: In the alpha version,the sole function of this 
*       component is to support the communication of NSI and BHM with location 
*       beacons and the BeDIS server. (In a later version that contains iGaD,
*       this components also receives commands from iGaD in the gateway and 
*       the BeDIS server and broadcasts the commands tolocal LBeacons.) 
*       Messages exchange happens in CommUnit. This file contain the 
*       formats of every kind of message and the buffers which store 
*       messages.And provide with functions which are executed according 
*       the messages received.
*       
* File Name:
*
*     CommUnit.h
*
* Abstract:
*
*      BeDIPS uses LBeacons to deliver 3D coordinates and textual
*      descriptions of their locations to users' devices. Basically, a
*      LBeacon is an inexpensive, Bluetooth Smart Ready device. The 3D
*      coordinates and location description of every LBeacon are retrieved
*      from BeDIS (Building/environment Data and Information System) and
*      stored locally during deployment and maintenance times. Once
*      initialized, each LBeacon broadcasts its coordinates and location
*      description to Bluetooth enabled user devices within its coverage
*      area.
*
* Authors:
*
*      Hank Kung, hank910140@gmail.com
*      
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define A_SHORT_TIME 1000
#define A_LONG_TIME 5000
#define BUFFER_SIZE 1024
/*
* ENUM
*/

typedef enum commandrequest {
    /* Request for health report */
    RFHR = 0,
    /**/
    BEACON_JOIN_REQUEST = 1,
    /* Server web sends coordinates of the beacon */
    SET_BEACON_COORDINATES = 2,
    /**/
    REMOVE_BEACON = 3,
    /**/
    GET_BEACON_INFO = 4
}CommandRequest;

// /* Command format in the queue */
// typedef struct zigbeebuffer{
//     /* Command kind */
//     CommandRequest command_name;
//     /* If the command is from server, set 0; Otherwise, BeaconID(Table in NSI Module) */
//     int beaconID;
//     /* The targeted Beacon ID */
//     void *data;
//     /* Point to the next command in the queue */
//     void *next;
// }Zigbeebuffer;


/**/
// typedef struct udpbuffer{
//     /**/
//     CommandRequest command_name;
//     /**/
//     int beaconID;
//     /**/
//     void *data;
//     /**/
//     void *next;
// }UDPbuffer;

// //Zigbeebuffer *zigbee_front, *zigbee_rear;
// Zigbeebuffer *Zbuffer;
// //UDPbuffer *udp_front, *udp_rear;
// UDPbuffer *Ubuffer;

/*
*   Variables
*/
bool send_to_beacon_buffer_is_locked;
bool recieve_from_beacon_buffer_is_locked;
bool send_tos_server_buffer_is_locked;
bool revieve_from_server_buffer_is_locked;

bool send_to_beacon_buffer_is_empty;
bool recieve_from_beacon_buffer_is_empty;
bool send_tos_server_buffer_is_empty;
bool revieve_from_server_buffer_is_empty;

FILE *sendToBeaconBufferFront;
FILE *sendToBeaconBufferRear;

FILE *revieveFromBeaconBufferFront;
FILE *revieveFromBeaconBufferRear;

FILE *sendToServerBufferFront;
FILE *sendToServerBufferRear;

FILE *revieveFromServerBufferFront;
FILE *revieveFromServerBufferRear;

FILE sendToBeaconBuffer[BUFFER_SIZE];
FILE recieveFromBeaconBuffer[BUFFER_SIZE];
FILE sendToServerBuffer[BUFFER_SIZE];
FILE revieveFromServerBuffer[BUFFER_SIZE];


/*
*   External Variables
*/ 

extern bool CommUnit_initialization_complete;

extern bool system_is_shutting_down;


/*
*  CommUnit_routine:
*/
void *CommUnit_routine();

/*
*  init_buffer:
*
*
*
*  Parameters:
*
*  Node
*
*  Return value:
*
*  None
*/
void init_buffer(void *front, void *rear, void *buffer);

/*
*  RFHR
*
*  Request For Health Report. This function scans each beacon in the gateway by
*  sending Zigbee singal to detect. If times to scan beacon successfully equals
*  the total beacon number then break.
*
*  Parameters:
*
*  Node
*
*  Return value:
*
*  None
*/
void RFHR();

/*
*
*/
void *buffer_dequeue(void *front, void *rear, void *buffer);

/*
*
*/
void buffer_enqueue(void *front, void *rear, void *buffer);