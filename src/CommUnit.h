#include <stdio.h>
#include <stdbool.h>
#include "Gateway.h"
#include "LinkedList.h"

/*
* CONSTANTS
*/
#define COMMAND_BUFFER 1024

/*
* EXTERNAL VARIABLES
*/
extern bool CommUnit_initialization_complete;

// A global flag which is initially false and is set by main thread to
// true to tell other threads to shutdown, i.e., clean up and return.
extern bool system_is_shutting_down;

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

/*
* TYPEDEF STRUCTS
*/

/* Command format in the queue */
typedef struct Command{
    /* Command kind */
    enum command_request command_kind;
    /* If the command is from server, set 0; Otherwise, BeaconID(Table in NSI Module) */
    int sender_ID;
    /* Command Priority */
    int priority;
    /* Point to the next command in the queue */
    struct Command *next;
}Command;


Command *front, *rear;

/*
* FUNCTIONS
*/

/*
*  CommUnit_routine:
*/
int CommUnit_routine () {
    
    inti_Command_Queue();
    //when initialization completes,
    CommUnit_initialization_complete = true;
    while (system_is_shutting_down == false) {
     //   do a chunk of work and/or sleep for a short time
            }
    CommUnit_cleanup_exit();
    return 0;
}
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
void inti_Command_Queue(){
    ront = rear = (Command*) malloc(sizeof(Command));
    front->next = rear->next = NULL;
}

/*
*  CommUnit_cleanup_exit:
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
void CommUnit_cleanup_exit(){
    //free the allocated memory
}

/*
 *  CommUnit_cleanup_exit:
 *
 *  
 *
 *  Parameters:
 *
 *
 *
 *  Return value:
 *
 *
 */
int receive_request(char *, int);