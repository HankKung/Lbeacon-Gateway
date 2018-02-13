
/*
* Copyright (c) 2016 Academia Sinica, Institute of Information Science
*
* License:
*
*      GPL 3.0 : The content of this file is subject to the terms and
*      conditions defined in file 'COPYING.txt', which is part of this source
*      code package.
*
* Project Name:
*
*      BeDIPS
*
* File Description:
*
*      This is the header file containing the function declarations and
*      variables used in the Gateway.c file.
*
* File Name:
*
*      Gateway.h
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

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <obexftp/client.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>


/* 
* CONSTANTS 
*/

/* Maximum number of nodes (LBeacons) per star network */
#define MAX_NUMBER_NODES 32

/*Length of the beacon's UUID*/
#define UUID_LENGTH 32

/*Length of the address of the network */
#define NETWORK_ADD_LENGTH 16

/* Maximum number of characters in location description*/
#define MAX_LENGTH_LOC_DESCRIPTION  64

#define COORDINATE_LENGTH 64

/*
* TYPEDEF STRUCTS
*/

typedef struct coordinates {

  char X_coordinates[COORDINATE_LENGTH];
  char Y_coordinates[COORDINATE_LENGTH];
  char Z_coordinates[COORDINATE_LENGTH];


}Coordinates;

typedef struct address_map {

  char network_address[NETWORK_ADD_LENGTH];
  char beacon_uuid[UUID_LENGTH];
  Coordinates beacon_coordinates;
  char loc_description[MAX_LENGTH_LOC_DESCRIPTION];


}Address_map;


/*
* GLOBAL VARIABLES
*/

/* A global flag which is initially false and is set by main thread to true 
* to tell other threads to shutdown, i.e. clean up and return */
bool system_is_shutting_down;

/*A global flag that is initially false and is set nby main thread to ture 
* when initialization completes Afterward, the flag is used by other threads 
* to inform the main thread the need to shutdown. */
bool ready_to_work;

/* A global flag set to be true by a thread when its inintialiazation failed. */
bool initialization_failed;

/* Initialization of gateway components invole network activates that may take 
* time. The main thread should wait until their initialization is sufficiently 
* compete. These flags enable the modules to inform the main thread when it 
* happens. */
bool NSI_initialization_complete;
bool BHM_initialization_complete;
bool CommUnit_initialization_complete;

/* An array of address maps */
Address_map beacon_address[MAX_NUMBER_NODES];






/*
*  get_system_time:
*
*  This helper function fetches the current time according to the system
*  clock in terms of the number of milliseconds since January 1, 1970.
*
*  Parameters:
*
*  None
*
*  Return value:
*
*  system_time - system time in milliseconds
*/
long long get_system_time();

/*
*  startThread:
*
*  This function initializes the threads.
*
*  Parameters:
*
*  threads - name of the thread
*  thfunct - the function for thread to do
*  arg - the argument for thread's function
*
*  Return value:
*
*  Error_code: The error code for the corresponding error
*/
Error_code startThread(pthread_t threads, void * (*thfunct)(void*), void *arg);

/*
*  cleanup_exit:
*
*  This function releases all the resources and set the flag.
*
*  Parameters:
*
*  None
*
*  Return value:
*
*  None
*/
void cleanup_exit();