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

    init_buffer(FILE *sendToBeaconBufferFront, FILE *sendToBeaconBufferRear,);
    

    bool send_to_beacon_buffer_is_locked = false;
    bool recieve_from_beacon_buffer_is_locked = false;
    bool send_tos_server_buffer_is_locked = false;
    bool revieve_from_server_buffer_is_locked = false;

    bool send_to_beacon_buffer_is_empty = true;
    bool recieve_from_beacon_buffer_is_empty = true;
    bool send_tos_server_buffer_is_empty = true;
    bool revieve_from_server_buffer_is_empty = true;

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

            if (startThead (zigbee_enqueue()) != WORK_SCUCESSFULLY)
                printf("Zigbee enqueue failure");
            if (startThead (udp_enqueue()) != WORK_SCUCESSFULLY)
                printf("UDP enqueue failure");
        }
        else sleep(A_SHORT_TIME);
        

        }
    CommUnit_cleanup_exit();
    free(Zbuffer);
    free(Ubuffer);
    return;
 }

void init_buffer(void *front, void *rear, void *buffer){
    front = rear = buffer;
}

void RFHR(){

}

void *buffer_dequeue(void *front, void *rear, void *buffer){

    /* Wait for the turn to use the queue */
    while(zigbee_queue_is_locked){
        sleep(A_SHORT_TIME);
    }
    zigbee_queue_is_locked = true;
    if(*front == *rear){
        printf("Zigbee queue is empty currently, can not dequeue anymore");
        zigbee_queue_is_empty = true;
        zigbee_queue_is_locked = false;
        return;
    }   
    /* Execute function according the command name */

    /* Free the control*/
    zigbee_queue_is_locked = false;
}

void buffer_enqueue(void *front, void *rear, void *buffer){
    /* Wait for the turn to use the queue */
    while(zigbee_queue_is_locked){
        sllep(A_SHORT_TIME);
    }
    zigbee_queue_is_locked = true;
    /* If buffer is full currently then just skip Enqueue till there's
    room for it.P.S. Overflow problem will got solved later */
    if( (*front == *rear + 1) || (*front == 0 && *rear == BUFFER_SIZE-1))
    return;
    /* *front is -1 when the buffer is empty */
    if(*front == -1) *front = 0;
    *rear = (*rear + 1) % BUFFER_SIZE;
    Zbuffer[rear] = *item;
    /* Set flag true anyway */
    zigbee_queue_is_locked = false;
}

void *udp_dequeue(int *front, int *rear){
    /* Wait for the turn */
    while(udp_queue_is_locked){
        sleep(A_SHORT_TIME);
    }
    udp_queue_is_locked = true;
    if(*front == *rear){
        //printf("UDP queue is empty currently, can not dequeue anymore");
        udp_queue_is_empty = true;
        udp_queue_is_locked = false;
        return;
    }
    /* Execute function according the command name */
    //

    /* Free the control*/
    udp_queue_is_locked = false;
}

void udp_enqueue(int *front, int *rear, UDPbuffer *item){
    /* Wait for the turn to use the queue */
    while(udp_queue_is_locked){
        sllep(A_SHORT_TIME);
    }
    udp_queue_is_locked = true;
    /* If buffer is full currently then just skip Enqueue till there's
    room for it.P.S. Overflow problem will got solved later */
    if( (*front == *rear + 1) || (*front == 0 && *rear == BUFFER_SIZE-1))
    return;
    /* *front is -1 when the buffer is empty */
    if(*front == -1) *front = 0;
    *rear = (*rear + 1) % BUFFER_SIZE;
    Ubuffer[rear] = *item;
    /* Set flag true anyway */
    udp_queue_is_locked = false;
}