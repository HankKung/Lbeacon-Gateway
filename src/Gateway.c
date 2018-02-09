
#include "Gateway.h"



int lengthOfU(unsigned char * str)
{
    int i = 0;

    while(*(str++)){
        i++;
        if(i == INT_MAX)
            return -1;
    }

    return i;
}


int main(int argc, char **argv)
{
    int i=0,carry_count=0,carry_map,carry_num;
    char *hostname,*port;
    char A_IP[]="00000000";
    pthread_t Device_cleaner_id,ZigBee_id,TCP_id;
    configstruct = get_config(CONFIG_FILENAME);
    
    //*****Node Dectet
    ZigBee_Node_dectet();
    for(i=0;i<ZigBee_addr_Scan_count;i++)
    {
    IpTable[i].live = 1;
    carry_map=10000000;
    carry_num = i;
    for(carry_count = 0;carry_count < 8;carry_count++)
        {
            A_IP[carry_count] = 48+(carry_num/carry_map);
            carry_num = carry_num-(carry_num/carry_map)*carry_map;
            carry_map/=10;
        }
    Assign_IP(IpTable[i].address,A_IP);//naosu
    }
    //*****TCP init
    hostname = malloc(configstruct.hostname_len);
    memcpy(hostname,configstruct.hostname,configstruct.port_len);
    port = malloc(configstruct.port_len);
    memcpy(port,configstruct.port,configstruct.port_len);
    //TCPinit(port,hostname);
    //*****Device Cleaner
    pthread_create(&Device_cleaner_id,NULL,(void *) DeviceCleaner,NULL);
    //*****ZigBee Receiver
    pthread_create(&ZigBee_id,NULL,(void *) ZigBee_Receiver,NULL);
    //*****TCP Receiver
    pthread_create(&TCP_id,NULL,(void *) T_TCP_Receiver,NULL);
    usleep(20000);
    for(i=0;i<MAX_OF_DEVICE;i++)
    UsedDeviceQueue.DeviceUsed[i]= 0;
    while (1) {
        scanner_start();
    }

    return 0;
}

