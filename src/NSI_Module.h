#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "Gateway.h"
#include "LBeacon_Zigbee_Gateway.h"


/*
 * CONSTANTS
 */
// Maximum number of nodes (Lbeacons) per star network
#define MAX_NUMBER_NODES 32
// Maximum number of characters in location description
#define MAX_LENGTH_LOC_DESCRIPTION 64
// Length and struct of coordinates
#define COORDINATE_LENGTH 64
#define A_SHORT_TIME 1000
#define A_LONG_TIME 5000

/*
* VARIABLES
*/
//current number of beacons
int beacon_count;
// NSI is the only writer of beacon_address; it has many readers.
bool Beacon_address_lock;

/*
* EXTERNAL GLOBAL VARIABLES
*/
extern bool ready_to_work;
extern bool system_is_shutting_down;
extern bool initialization_failed;
extern bool NSI_initialization_complete;

/*
* TYPEDEF STRUCTS
*/

typedef struct coordinates {
    char X_coordinates[COORDINATE_LENGTH];
    char Y_coordinates[COORDINATE_LENGTH];
    char Z_coordinates[COORDINATE_LENGTH];
}coordinates;

/* A struct linking network address assigned to a Lbeacon to its
 UID, coordinates, and location description. Each beacon also has
 a 2D barcode on its enclosure. The barcode was assigned by BDE
 tool and is used during installation and maintenance. */
typedef struct address_map {
    unsigned int network_address; // 16 bit network address
    unsigned int beacon_id; // internal UID of the Lbeacon
    coordinates beacon_coordinates;
    char *loc_description [MAX_LENGTH_LOC_DESCRIPTION];
    double beacon_barcode;
}address_map;

// an array of address maps
extern struct address_map beacon_address [MAX_NUMBER_NODES];


/*
* FUNCTIONS
*/

int NSI_routine(){
    /* coordinator initializes the zigbee network:
     - if (PAN ID == 0) scan nearby network and chooses a PAN ID;
     - channel scan to find a good operating channel;
     - ready to access join requests from Lbeacons;
     */
    /* Set up Zigbee connection by calling Zigbee_routine in LBeacon_Zigbee.h */
    if (startThead (Zigbee_routine()) != WORK_SCUCESSFULLY) {
        initialization_failed = true;
        NSIcleanupExit( );
    }
    
    /* Socket Set Up */
    int sock, length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char * buffer[BUFFER_SIZE];
    
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        error("Wrong Socket");
    }
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if(hp == 0)
    {
        error("Unknown Host");
    }
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length = sizeof(struct sockaddr_in);
    bzero(buffer, BUFFER_SIZE);

    // make sure WiFi has been correctly configured ....
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
     int beacon_count = 1;
     if (startThead (address_map_manager()) != WORK_SCUCESSFULLY) {
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
         
     /* Upon fatal failure, set ready_to_work = true and
     then call NSIcleanupExit( )*/

    
    //NSIcleanupExit();
    // wait for all threads to have exited then returns
    return 0;
     }
}

/*
*
*/
int Zigbee_routine(){

}

/*
*  addrss_map_manager:
*
*  This function initializes and mainains the table which stores the information
*  of each Lbeacon. Keep monitoring if there's a new beacon send the command to
*  join the gateway. And if there is, call beacon_join_request(). At the
*  meanwhile, it also counts the current number of beacons in this gateway
*  coverage.
*
*  Parameters:
*
*  None
*
*  Return value:
*
*  O
*/
int address_map_manager(){
    //gateway info
    unsigned zigbee_macaddr;
    coordinates gateway_coordinates;
    char * gateway_loc_description;
    double gateway_barcode;
    
    //initialize address table
    struct address_map beacon_address [MAX_NUMBER_NODES];
    beacon_join_request(zigbee_macaddr, gateway_coordinates,
                        gateway_loc_description, gateway_barcode);
    while(system_is_shutting_down == false){
        
        //if a new join request||(beacon_count>=32)
        //then beacon_count++;
        //startthread(beacon_join_request());
        
    }
    return 0;
}

/*
*  beacon_join_request:
*  This function is executed when a beacon sends command to join the gateway
*  and fills the table with the inputs. And set the network_address according
*  the current number of beacons.
*
*  Parameters:
*
*  ID -
*  Coordinates -
*  Loc_Description[MAX_LENGTH_LOC_DESCRIPTION -
*  Barcode -
*
*  Return value:
*
*  None
*/
void beacon_join_request(unsigned ID,coordinates Coordinates,
                         char *Loc_Description[MAX_LENGTH_LOC_DESCRIPTION]
                         ,double Barcode){
    beacon_address[beacon_count-1].network_address = beacon_count-1;//tempt
    beacon_address[beacon_count-1].beacon_id = ID;
    beacon_address[beacon_count-1].beacon_coordinates = Coordinates;
    beacon_address[beacon_count-1].loc_description = Loc_Description;
    beacon_address[beacon_count-1].beacon_barcode = Barcode;
}
/*
*  NSI_cleanup_exit:
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
void NSI_cleanup_exit(){
    ready_to_work = false;
    //free allocated memory
}

/*
*
*/
void error(char * msg){
    perror(msg);
    exit(0);
}
