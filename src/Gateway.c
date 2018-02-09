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

