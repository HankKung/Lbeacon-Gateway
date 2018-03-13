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
*      
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

/*
* ENUM
*/

enum command_request {
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
};

/* Command format in the queue */
typedef struct command{
    /* Command kind */
    enum command_request command_kind;
    /* If the command is from server, set 0; Otherwise, BeaconID(Table in NSI Module) */
    int sender_ID;
    /* Command Priority */
    int priority;
    /* Point to the next command in the queue */
    struct command *next;
}Command;


Command *front, *rear;

extern bool CommUnit_initialization_complete;

extern bool system_is_shutting_down;


/*
*  CommUnit_routine:
*/
void *CommUnit_routine();

/*
*  inti_Command_Queu:
*
*  This function initializes the queue which contains the commands sent from
*  server and beacons.
*
*  Parameters:
*
*  Node
*
*  Return value:
*
*  None
*/
void inti_Command_Queue();

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