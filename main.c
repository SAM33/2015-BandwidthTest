/*  LOCAL NETWORK BANDWIDTH TEST PROGRAM  
 *
 *  Author : Sam33
 *  Data   : 20150514
 *
 *  Useage : 1.Prepare two computer in the same local network. (example : 192.168.1.101, 192.168.1.102)
 *           2.Chose one to be server , another one to be client (example : 192.168.1.101 --> Server)
 *           3.Edit line 35 to be server address "127.0.0.1" --> "192.168.1.101"
 *           4.run server program on 192.168.1.101
 *           5.run client program on 192.168.1.102
 *           6.server program will tell you how much bandwidth you have.
 *
 *  NOTICE : When this program is running , your local network will be very slow.
 *
 */

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>

#ifdef CLIENT
int main(int argc,char *argv[])
{
    int socketfd = socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd<0)
        return -1;
    char data[4096];
    long long total=0;
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0x0,sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //Edit your server address here
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( 9999 );
    while(1)
    {
        int s = sendto(socketfd, data, sizeof(data), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    }
}
#endif

#ifdef SERVER
int main(int argc,char *argv[])
{
    int socketfd = socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd<0)
        return -1;
    struct sockaddr_in myaddr;
    memset(&myaddr,0x0,sizeof(myaddr));
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons( 9999 );
    int b = bind(socketfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
    if(b<0)
        return -1;
    char data[4096];
    long long total=0;
    struct sockaddr_in clientaddr;
    socklen_t l = sizeof(clientaddr);
    time_t t1,t2;
    bool isstart=false;
    while(1)
    {
        int r = recvfrom(socketfd, data, sizeof(data), 0, (struct sockaddr *)&clientaddr, &l);
        if(r>0)
        {
            if(isstart)
            {
                total+=r;
                t2=time(NULL);
                //show bandwidth result per second
                if(t2-t1>=1)
                {
                    double MByteSpeed = (double)((double)total/(double)(1024*1024));
		    double kbitSpeed = (double)((double)(total*8)/(double)(1024));
                    printf("%.2lf MBytes/sec (%.0lf kbps)\n",MByteSpeed,kbitSpeed);
                    total=0;
                    t1=time(NULL);
                }
            }
            else
            {
                //wait client to connect and start the timer
                printf("Start test!\n");
                t1=time(NULL);
                isstart=true;
            }
        }
    }
}


#endif
