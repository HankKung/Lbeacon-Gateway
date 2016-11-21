#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <signal.h>
#include <obexftp/client.h> /*!!!*/
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>
#include <xbee.h>
#include <limits.h>  
#include <ctype.h>
#define MAX_OF_DEVICE 30 //max number of device to storage
#define NTHREADS 30    //max number of threads used in multithreading
#define PUSHDONGLES 2
#define NUMBER_OF_DEVICE_IN_EACH_PUSHDONGLE 9
#define BLOCKOFDONGLE 5
#define IDLE         -1
#define GATEWAY       1
#define BEACON        0
//***************Parser define********************
#define S_IPTABLE 0x30
#define MAXBUF 64
#define DELIM "="
#define CONFIG_FILENAME "config.conf"
const long long Timeout = 20000; // TimeOut in 20s
const unsigned char broadcase_zigbee_address[]="000000FF";
char addr[30][18] = { 0 };
char addrbufferlist[PUSHDONGLES * NUMBER_OF_DEVICE_IN_EACH_PUSHDONGLE][18] = { 0 };
char *filepath="/home/pi/a.txt";
char *filepath1="/home/pi/a.txt";
char *filepath2="/home/pi/b.txt";
char *filepath3="/home/pi/106right.png";
char *filepath4="/home/pi/b.jpg";
unsigned char my_zigbee_address[]="00000001";
char port[]="3333";
char hostname[]="140.109.17.72";
int IdleHandler[PUSHDONGLES * NUMBER_OF_DEVICE_IN_EACH_PUSHDONGLE] = {0};
int Beacon_Type = -1;
pthread_t thread_id[NTHREADS] = {0};
void *send_file(void *address); //prototype for the file sending function used in the new thread
void *DeviceCleaner(void); //prototype for clean the device by time
//searching for devices code is taken from some bluez examples
//struct xbee_pkt **Global_pkt;
int xbee_pkt_count = 0;
sem_t ndComplete;
//TCP/IP parameters
int sockfd, portno;
/* IP table: |Location|GatewayIpv4|Beacon address|Tx Power| */
int RSSI_RANGE = -60;
struct xbee *xbee;
void *d;
struct xbee_con *con;
struct xbee_conAddress address;
struct xbee_conSettings settings;
struct config configstruct;
/*-----------------------Package-----------------------*/
// |1byte|8bytes|2bytes|2byte|32bytes|
struct config
{
   unsigned char my_zigbee_address[MAXBUF];
   char port[MAXBUF];
   char hostname[MAXBUF];
   char filepath_head[MAXBUF];
   char filename[MAXBUF];
   char coordinate_X[MAXBUF];
   char coordinate_Y[MAXBUF];
   char level[MAXBUF];
   int my_zigbee_address_len;
   int port_len;
   int hostname_len;
   int filepath_head_len;
   int filename_len;
   int coordinate_X_len;
   int coordinate_Y_len;
   int level_len;
};
/*********************************************************************
 * TYPEDEFS
 */
/*-----------------------IP table-----------------------*/
typedef struct 
{
    float coordinate[3];
    /* SH = 0xXX XX XX XX SL = 0xXX XX XX XX*/
    struct xbee_conAddress address;
    unsigned char assign_IP[8];
    int live;
}Beacon_IP_table;
Beacon_IP_table IpTable[255];//storage is max to 255's Beacons address

typedef struct {
    char addr[18];
    int threadId;
    pthread_t t;


} Threadaddr;

typedef struct {
    long long DeviceAppearTime[MAX_OF_DEVICE];
    char DeviceAppearAddr[MAX_OF_DEVICE][18];
    char DeviceUsed[MAX_OF_DEVICE];

} DeviceQueue;
typedef struct 
{
    unsigned char COMM;
    unsigned char data[64];
    int Packagelen;
    int Datalen;
    int count;
    /* data */
} DataPackage;
DeviceQueue UsedDeviceQueue;

int ZigBee_addr_Scan_count=0;
struct xbee_conAddress Gatewayaddr;
void Parse_package(unsigned char *packet);
void error(char *msg);
void TCPinit(char port[],char hostname[]);
void myCB(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);
void *T_TCP_Receiver(void);
void nodeCB(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data);
int ZigBee_Node_dectet();
long long getSystemTime();
int UsedAddrCheck(char addr[]);
int compare_strings(char a[], char b[]);
void ifTequal(pthread_t tid);
static void print_result(bdaddr_t *bdaddr, char has_rssi, int rssi);
static void sendToPushDongle(bdaddr_t *bdaddr, char has_rssi, int rssi);
static void scanner_start();
void *send_file(void *ptr);
void *DeviceCleaner(void);
int lengthOfU(unsigned char * str);
