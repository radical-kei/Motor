/**
[sample_motor_server]

Copyright (c) [2021] [radical-kei]

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <pigpio.h>

#define IN1_GPIO_NO 18
#define IN2_GPIO_NO 19

#define PMW_FREQ    200000
#define MAX_DUTY    1000000

#define QUEUELIMIT  5
#define SERVER_PORT 50001
#define BUF_SIZE    8

struct clientdata {
  int sock;
  struct sockaddr_in saddr;
};

int set_motor_rev(int t_duty)
{
    int ret = 0;
    unsigned int in1_duty = MAX_DUTY;
    unsigned int in2_duty = MAX_DUTY;

    if(t_duty < 0)
    {
        in1_duty = MAX_DUTY - abs(t_duty);
    }
    else
    {
        in2_duty = MAX_DUTY - t_duty;
    }

    printf("in1:%d in2:%d\n", in1_duty, in2_duty);
    gpioHardwarePWM(IN1_GPIO_NO, PMW_FREQ, in1_duty);
    if(ret != 0)
    {
        return ret;
    }

    gpioHardwarePWM(IN2_GPIO_NO, PMW_FREQ, in2_duty);
    if(ret != 0)
    {
        return ret;
    }

    return ret;
}

void* motor_control_thread(void* pArg)
{
    int recvMsgSize, sendMsgSize;
    struct clientdata *cdata = pArg;
    char recv_buf[BUF_SIZE];
    int req_duty;

    while(1)
    {
        memset(recv_buf, 0, BUF_SIZE);
        if ((recvMsgSize = recv(cdata->sock, &recv_buf, BUF_SIZE, 0)) < 0)
        {
            fprintf(stderr, "Failed recv(). errno:%d\n", errno);
            break;
        }
        else if(recvMsgSize == 0)
        {
            fprintf(stderr, "connection closed by foreign host.\n");
            break;
        }

        req_duty = atoi(recv_buf);
        if(set_motor_rev(req_duty) != 0)
        {
            fprintf(stderr, "Failed set_motor_param().\n");
            break;
        }
    }

    close(cdata->sock);
    free(cdata);
    return NULL;
}

int motor_control_server(void){
    int servSock;
    struct clientdata* clit;
    struct sockaddr_in servSockAddr; //server internet socket address
    struct sockaddr_in clitSockAddr; //client internet socket address
    unsigned int clitLen; // client internet socket address length

    pthread_t th;

    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
        fprintf(stderr, "Failed socket().\n");
        return -1;
    }

    memset(&servSockAddr, 0, sizeof(servSockAddr));
    servSockAddr.sin_family      = AF_INET;
    servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servSockAddr.sin_port        = htons(SERVER_PORT);

    if (bind(servSock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr) ) < 0 ) {
        fprintf(stderr, "Failed bind().\n");
        close(servSock);
        return -1;
    }

    if (listen(servSock, QUEUELIMIT) < 0) {
        fprintf(stderr, "Failed listen().\n");
        close(servSock);
        return -1;
    }

    while(1) {
        clit = malloc(sizeof(struct clientdata));
        clitLen = sizeof(clit->saddr);
        if ((clit->sock = accept(servSock, (struct sockaddr *) &clit->saddr, &clitLen)) < 0) {
            fprintf(stderr, "Failed accept().\n");
            close(servSock);
            break;
        }
        printf("Connected from %s.\n", inet_ntoa(clit->saddr.sin_addr));

        if (pthread_create(&th, NULL, motor_control_thread, clit) != 0) {
            fprintf(stderr, "Failed pthread_create().\n");
            break;
        }

        if (pthread_detach(th) != 0) {
            fprintf(stderr, "Failed pthread_detach().\n");
            break;
        }
    }
    close(servSock);
    return 0;
}

int main(void)
{
    int ret = gpioInitialise();
    if (ret < 0)
    {
        return ret;
    }
    else
    {
        ret = motor_control_server();
        gpioTerminate();
    }

    return ret;
}
