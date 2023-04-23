/* fpont 12/99 */
/* pont.net    */
/* tcpClient.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT_1 12099
#define SERVER_PORT_2 22099
#define MAX_MSG 100

int main(int argc, char *argv[])
{

    int sd1, sd2, rc1, rc2, i, n1, n2, n3;
    struct sockaddr_in localAddr1, localAddr2, servAddr1, servAddr2;
    struct hostent *h;
    char buffer[MAX_MSG];

    if (argc < 3)
    {
        printf("usage: %s <server> <data1> <data2> ... <dataN>\n", argv[0]);
        exit(1);
    }

    h = gethostbyname(argv[1]);
    if (h == NULL)
    {
        printf("%s: unknown host '%s'\n", argv[0], argv[1]);
        exit(1);
    }

    servAddr1.sin_family = h->h_addrtype;
    memcpy((char *)&servAddr1.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servAddr1.sin_port = htons(SERVER_PORT_1);

    /* create socket 1*/
    sd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sd1 < 0)
    {
        perror("cannot open socket ");
        exit(1);
    }

    /* bind any port number 1 */
    localAddr1.sin_family = AF_INET;
    localAddr1.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr1.sin_port = htons(0);

    servAddr2.sin_family = h->h_addrtype;
    memcpy((char *)&servAddr2.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servAddr2.sin_port = htons(SERVER_PORT_2);

    /* create socket 2 */
    sd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sd2 < 0)
    {
        perror("cannot open socket ");
        exit(1);
    }

    /* bind any port number 2*/
    localAddr2.sin_family = AF_INET;
    localAddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr2.sin_port = htons(0);

    rc1 = bind(sd1, (struct sockaddr *)&localAddr1, sizeof(localAddr1));
    rc2 = bind(sd2, (struct sockaddr *)&localAddr2, sizeof(localAddr2));
    if (rc1 < 0)
    {
        printf("%s: cannot bind port TCP %u\n", argv[0], SERVER_PORT_1);
        perror("error ");
        exit(1);
    }

    if (rc2 < 0)
    {
        printf("%s: cannot bind port TCP %u\n", argv[0], SERVER_PORT_2);
        perror("error ");
        exit(1);
    }

    /* connect to server */
    rc1 = connect(sd1, (struct sockaddr *)&servAddr1, sizeof(servAddr1));
    rc2 = connect(sd2, (struct sockaddr *)&servAddr2, sizeof(servAddr2));
    
    if (rc1 < 0)
    {
        perror("cannot connect socket1 ");
        exit(1);
    }
    if (rc2 < 0)
    {
        perror("cannot connect socket2 ");
        exit(1);
    }

    for (i = 2; i < argc-1; i++)
    {

        rc1 = send(sd1, argv[i], strlen(argv[i]), 0);

        if (rc1 < 0)
        {
            perror("cannot send data ");
            close(sd1);
            exit(1);
        }

        printf("%s: data%u sent (%s)\n", argv[0], i - 1, argv[i]);
    }

    /*Read from Server1 and print*/
    memset(buffer, 0, MAX_MSG);
    n1 = recv(sd1, buffer, MAX_MSG, 0);
    if (n1 < 0)
    {
        perror("Error from 12099");
        exit(1);
    }
    printf("Server 12099 : %s\n", buffer);

    /*Sent to Server 1(Birthday)*/
    rc2 = send(sd1, argv[argc - 1], strlen(argv[argc - 1]), 0);
    if (rc2 < 0)
    {
        perror("cannot send data ");
        close(sd2);
        exit(1);
    }

    printf("%s: data%u sent (%s)\n", argv[0], 3, argv[argc - 1]);

    /*Read from Server2 and print*/
    memset(buffer, 0, MAX_MSG);
    n2 = recv(sd2, buffer, MAX_MSG, 0);
    if (n2 < 0)
    {
        perror("Error from 22099");
        exit(1);
    }
    printf("Server 22099 : %s\n", buffer);

    /*Send "bye" to Server2*/
    char *messege = "Bye";
    n3 = send(sd2, messege, strlen(messege), 0);
    if (n3 < 0)
    {
        perror("Error From 22099");
        exit(1);
    }
    printf("%s: data %d sent (bye)\n", argv[0],4);
    close(sd1);
    close(sd2);
    return 0;
}
