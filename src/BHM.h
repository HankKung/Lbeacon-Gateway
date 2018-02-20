#include <stdio.h>
#include <stdbool.h>
#include "NSI Module.h"

#define PERIOD_TO_MONITOR 100000

/*
 *EXTERNAL CONSTANTS
 */
extern bool BHM_initialization_complete;

/*
* VARIABLES
*/

bool health_report[MAX_NUMBER_NODES];

/*
* EXTERNAL VARIABLES
*/

extern int beacon_count;

/*
*FUNCTIONS
*/

/*
*  BHM_routine:
*
*  This function initializes the table to record beacon health. After then,
*  keep maintaining the table which stores the beacons health state currently.
*
*  Parameters:
*
*  Node
*
*  Return value:
*
*  None
*/
int BHM_routine(){
    or (int i = 0; i<MAX_NUMBER_NODES; i++) {
        /* Default value is true; If beacon is failed, then set to false */
        health_report[i] = true;
    }
    // when initialization completes,
    BHM_initialization_complete = true;
     while (system_is_shutting_down == false) {
    //    do a chunk of work and/or sleep for a short time
         RFHR();
         sleep(PERIOD_TO_MONITOR);
    }
    BHM_cleanup_exit();
    return 0;
}

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
void RFHR(){
    int scan_number = 0;
    /*check beacons one by one. Default value is true; If beacon is failed,
     then set to false */
    for(int i = 0; i< MAX_NUMBER_NODES ; i++){
        scan_number++;
        //startthread(send_command_to_beacon)
        //Send signal to the beacon according beaconID
        //Wait till the beacon sends back the health report
        
        if(scan_number == beacon_count)break;
    }
}

/*
*  BHM_cleanup_exit:
*
*  Set raady_to_work to false and free the allocated memory.
*
*  Parameters:
*
*  Node
*
*  Return value:
*
*  None
*/
void BHM_cleanup_exit();