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
*      This file contains the program to transmit the data or information from
*      LBeacons through Zigbee or UDP. Main tasks includes network setup and 
*      initialization, Beacon health monitor and comminication unit. Gateway 
*      takes the role as coordinator.
*
* File Name:
*
*      Gateway.c
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
*      Han Wang, hollywang@iis.sinica.edu.tw
*      Jake Lee, jakelee@iis.sinica.edu.tw
*      Johnson Su, johnsonsu@iis.sinica.edu.tw
*      
*/



#include "Gateway.h"



long long get_system_time() {
    /* A struct that stores the time */
    struct timeb t;

    /* Return value as a long long type */
    long long system_time;

    /* Convert time from Epoch to time in milliseconds of a long long type */
    ftime(&t);
    system_time = 1000 * t.time + t.millitm;

    return system_time;
}


void *NSI_routine() {


    return;
}

void *BHM_routine() {

    return;
}

void *CommUnit_routine()
 {
   
    return;
 }


Error_code startThread(pthread_t threads ,void * (*thfunct)(void*), void *arg){

    pthread_attr_t attr;

    if ( pthread_attr_init(&attr) != 0
      || pthread_create(&threads, &attr, thfunct, arg) != 0
      || pthread_attr_destroy(&attr) != 0
      || pthread_detach(threads) != 0) {

    return E_START_THREAD;
  }

  return WORK_SCUCESSFULLY;

}


void cleanup_exit(){

    ready_to_work = false;
    send_message_cancelled = true;
    free_list(scanned_list);
    free_list(waiting_list);
    free_list(tracked_object_list);
    free(g_idle_handler);
    free(g_push_file_path);
    return;

}



int main(int argc, char **argv)
{
    
    /* Define and initialize all importent golbal variables including */
    system_is_shutting_down = false;
    ready_to_work = false;
    initialization_failed = false;
    NSI_initialization_complete = false;
    BHM_initilaization_complete = false;
    CommUnit_initialization_complete = false;

    int return_value;

    pthread_t NSI_routine_thread;

    return_value = startThread(NSI_routine_thread, NSI_routine, NULL);

    if(return_value != WORK_SCUCESSFULLY){

        perror(errordesc[E_START_THREAD].message);
        cleanup_exit();
    }

    pthread_t BHM_routine_thread;

    return_value = startThread(BHM_routine_thread, BHM_routine, NULL);

    if(return_value != WORK_SCUCESSFULLY){

        perror(errordesc[E_START_THREAD].message);
        cleanup_exit();
    }

    pthread_t CommUnit_routine_thread;

    return_value = startThread(CommUnit_thread, CommUnit_routine, NULL);

    if(return_value != WORK_SCUCESSFULLY){

        perror(errordesc[E_START_THREAD].message);
        cleanup_exit();
    }




}

