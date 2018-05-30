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
*      Hank Kung, hank910140@gmail.com
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

/* coordinator initializes the zigbee network:
- if (PAN ID == 0) scan nearby network and chooses a PAN ID;
- channel scan to find a good operating channel;
- ready to access join requests from Lbeacons;
/* Set up Zigbee connection by calling Zigbee_routine in LBeacon_Zigbee.h */
void *NSI_routine(){

    int beacon_count = 0;

    /* UDP Socket Set Up */
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
    
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        error("Wrong Socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // Make sure WiFi has been correctly configured ....
    int ping_ret, status;
    status = system("ping google.com");
    if (-1 != status)
        ping_ret = WEXITSTATUS(status);

     /* initialize beacon_address []
     - enter a 16-bit network address in each address_map struct in the array
     .....
     // start a thread to maintain beacon_address map. The thread
     // should also check system_is_shutting_down flag periodically
     // and returns when it finds the flag is true.*/

     if (startThead (Zigbee_routine()) != WORK_SCUCESSFULLY) {
        printf("Zigbee network failed\n")
        initialization_failed = true;
        NSIcleanupExit( );
    }
     if (startThead (address_map_manager()) != WORK_SCUCESSFULLY) {
         printf("addrss_map_manager initialization failed\n")
         initialization_failed = true;
         NSIcleanupExit( );
     }
     // finish phase 2 initialization (in ways TBD)
     NSI_initialization_complete = true;
    
     // wait for other components to complete initialization
     while ( (system_is_shutting_down == false) &&
     (ready_to_work == false))
     {
         sleep(A_SHORT_TIME);
     }
    
    // ready to work, check for system shutdown flag periodically

     while (system_is_shutting_down == false) {
        //do a chunk of work and/or sleep for a short time
     
        sleep(A_SHORT_TIME);
     }

    close(s);
    /* Upon fatal failure, set ready_to_work = false and
    then call NSIcleanupExit( )*/
    ready_to_work = false;
    //NSIcleanupExit();
    // wait for all threads to have exited then returns
    return;
}

void *address_map_manager(){

    beacon_count = 1;
    //gateway info
    unsigned zigbee_macaddr;
    coordinates gateway_coordinates;
    char * gateway_loc_description;
    double gateway_barcode;
    
    //Fill the gateway information into the address table
    //Gateway's index is always 0
    beacon_join_request(0, zigbee_macaddr, gateway_coordinates,
                        gateway_loc_description, gateway_barcode);
    while(system_is_shutting_down == false){
        
        //if a new join request && (beacon_count>=32)
        //startthread(beacon_join_request());
    }
    return;
}


void *beacon_join_request(int index, unsigned ID,Coordinates Beacon_Coordinates,
                         char *Loc_Description[MAX_LENGTH_LOC_DESCRIPTION]
                         ,double Barcode){

    beacon_address[index].network_address = index;//tempt
    beacon_address[index].beacon_uuid = ID;
    beacon_address[index].beacon_coordinates = Coordinates;
    beacon_address[index].loc_description = Loc_Description;
    beacon_address[index].beacon_barcode = Barcode;
}

void *BHM_routine(){

    for (int i = 0; i<beacon_count; i++) {
        /* Default value is true; If beacon is failed, then set to false */
        health_report[i] = true;
    }
    // when initialization completes,
    BHM_initialization_complete = true;
     while (system_is_shutting_down == false) {
    //    do a chunk of work and/or sleep for a short time
         //RFHR(); //might return a boolean array
         //broadcast
         sleep(PERIOD_TO_MONITOR);
    }
    ready_to_work = false;
    BHM_cleanup_exit();
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

void error(char * msg){

    perror(msg);
    exit(0);
    
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

    Address_map beacon_address [MAX_NUMBER_NODES];

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

    while(1){
        sleep(A_LONG_TIME);
    }

    cleanup_exit();

}

