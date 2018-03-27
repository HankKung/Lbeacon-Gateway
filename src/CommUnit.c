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
*     CommUnit.c
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

#include "CommUnit.h"
#include "Gateway.h"

void *CommUnit_routine(){

    init_zigbee_buffer(zigbee_front,zigbee_rear);
    init_udp_buffer(udp_front,udp_rear);
    bool zigbee_queue_is_locked = false;
    bool udp_queue_is_locked = false;
    bool zigbee_queue_isempty = true;
    bool udp_queue_is_empty = true;
    //when initialization completes,
    CommUnit_initialization_complete = true;
    while (system_is_shutting_down == false) {
        //   do a chunk of work and/or sleep for a short time

        /* If both Zigbee queue and UDP queue are empty then sleep 
        a short time*/
        if(!zigbee_queue_is_empty || !udp_queue_is_empty){
            if (startThead (zigbee_dequeue()) != WORK_SCUCESSFULLY)
                printf("Zigbee dequeue failure");
            if (startThead (udp_dequeue()) != WORK_SCUCESSFULLY)
                printf("UDP dequeue failure");
        }
        else sleep(A_SHORT_TIME);
        

        }
    CommUnit_cleanup_exit();
    return;
 }

void init_zigbee_buffer(Zigbeebuffer *front, Zigbeebuffer *rear){
    front = rear = (Zigbeebuffer*) malloc(sizeof(Zigbeebuffer));
    front->next = rear->next = NULL;
}

void init_udp_buffer(UDPbuffer *front, UDPbuffer *rear){
    front = rear = (UDPbuffer*) malloc(sizeof(UDPbuffer));
    front->next = rear->next = NULL;
}

void RFHR(){

}

void zigbee_dequeue(Zigbeebuffer *front, Zigbeebuffer *rear){
    if(front==rear){
        //printf("Zigbee queue is empty currently, can not dequeue anymore");
        zigbee_queue_is_empty = true;
        return;
    }   
    /* Execute function according the command name */

}

void zigbee_enqueue(Zigbeebuffer *front, Zigbeebuffer *rear, Zigbeebuffer *item){
    //if()

    /* Set flag true anyway */

}

void udp_dequeue(UDPbuffer *front, UDPbuffer *rear){
    if(front==rear){
        //printf("UDP queue is empty currently, can not dequeue anymore");
        udp_queue_is_empty = true;
        return;
    }

    /* Execute function according the command name */

}

void udp_enqueue(UDPbuffer *front, UDPbuffer *rear, UDPbuffer *item){


    /* Set flag true anyway */
}