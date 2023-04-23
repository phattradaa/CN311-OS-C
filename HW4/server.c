/* fpont 1/00 */
/* pont.net    */
/* tcpServer.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR 1
#define END_LINE 0x0
#define SERVER_PORT_1 12099
#define SERVER_PORT_2 22099
#define MAX_MSG 100

int main(int argc, char *argv[])
{

    int server_fd, sd1, sd2, n3;
    int n1, n2,n4;
    int opt = 1;
    struct sockaddr_in servAddr1, servAddr2;
    struct sockaddr_in cliAddr;
    int newSd1, newSd2, cliLen;
    char buffer[MAX_MSG];

    /* create socket 1 */
    sd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sd1 < 0)
    {
        perror("cannot open socket ");
        return ERROR;
    }

    
    /* bind server port 1 */
    servAddr1.sin_family = AF_INET;
    servAddr1.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr1.sin_port = htons(SERVER_PORT_1);

    if (bind(sd1, (struct sockaddr *)&servAddr1, sizeof(servAddr1)) < 0)
    {
        perror("cannot bind port ");
        return ERROR;
    }

    /* create socket 2 */
    sd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sd2 < 0)
    {
        perror("cannot open socket ");
        return ERROR;
    }


    /* bind server port 2 */
    servAddr2.sin_family = AF_INET;
    servAddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr2.sin_port = htons(SERVER_PORT_2);

    if (bind(sd2, (struct sockaddr *)&servAddr2, sizeof(servAddr2)) < 0)
    {
        perror("cannot bind port ");
        return ERROR;
    }

    /*Server 1: Listening for coming connection from client*/
    listen(sd1, 5);
    /*Server 2: Listening for coming connection from client*/
    listen(sd2, 5);

    while (1)
    {
        /*Accept 12099*/
        printf("waiting for connecting\n");
        cliLen = sizeof(cliAddr);
        newSd1 = accept(sd1, (struct sockaddr *)&cliAddr, &cliLen);
        if (newSd1 < 0)
        {
            perror("cannot accept connection socket 1");
            return ERROR;
        }
        printf("12099 Connection accept\n");

        /*Accept 22099*/
        newSd2 = accept(sd2, (struct sockaddr *)&cliAddr, &cliLen);
        if (newSd2 < 0)
        {
            perror("cannot accept connection socket 2");
            return ERROR;
        }
        printf("22099 Connection accept\n");

        /*Read */
        memset(buffer, 0, MAX_MSG);
        n1 = recv(newSd1, buffer, MAX_MSG, 0);
        printf("Client 12099 : %s\n", buffer);
        memset(buffer, 0, MAX_MSG);
        n2 = recv(newSd1, buffer, MAX_MSG, 0);
        printf("Client 12099 : %s\n", buffer);

        char message[] = "Server recieved";
        
        if (send(newSd1, message, strlen(message), 0) < 0)
        {
            perror("12099 Send Error");
            close(newSd1);
            close(newSd2);
            continue;
        }
        printf("./server: data sent (%s)\n", message);

        /*Receive Birthday*/
        memset(buffer, 0, MAX_MSG);
        int birthday_int;
        char birthday_str[8];
        n3 = recv(newSd1, buffer, MAX_MSG, 0);
        if (n3 < 0)
        {
            perror("cannot receive data(birthday)");
            return ERROR;
        }
        int i;
        for (i = 0; i < strlen(buffer);i++)
        {
            birthday_str[i] = buffer[i];
        }
        //printf("%s\n", birthday_str);
        birthday_int = atoi(birthday_str);
        //printf("%d\n", birthday_int);
        birthday_int -= 543;
        sprintf(buffer, "%d", birthday_int);
        //printf("%s\n", buffer);

        if (send(newSd2, buffer, MAX_MSG, 0) < 0)
        {
            perror("22099 Send Error");
            close(newSd1);
            close(newSd2);
            continue;
        }

        memset(buffer, 0, MAX_MSG);
        n4 = recv(newSd2, buffer, MAX_MSG, 0);
        if (n4 < 0)
        {
            perror("Error from 22099");
            close(newSd1);
            close(newSd2);
            continue;
        }

        printf("Client 22099 : %s\n", buffer);
        close(sd1);
        close(sd2);
        return 0;
    }
}
