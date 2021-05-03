#include "../header/common.h"

int main(int argc, char const *argv[])
{
    /* Defining useful variables for sockets, adress, buffer, etc */
    int                 listenfd, connfd, n;
    struct sockaddr_in  servaddr;
    uint8_t             buff[MAXLINE+1];
    uint8_t             recvline[MAXLINE+1];

    /* Creating new socket / Allocating new resources 
     * AF_INET = ADRESS FAMILY _ INTERNET = an internet socket
     * SOCK_STREAM for TCP Stream, it's like a file
     * 0 it's default value for TCP
    */

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("socket error.");

    /* Address */

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET; // an internet address
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //InternetAddress_Any = I'm responding to anythinh
    servaddr.sin_port        = htons(SERVER_PORT); //server port, in this case hardcoded on 443

    /* Bind & Listen */

    //This basically says to the OS: this socket is going to listen to this address
    if((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
        err_n_die("bind error.");

    if((listen(listenfd, 10)) < 0)
        err_n_die("listen error.");

    //Server infinite loop
    for(;;){
        struct sockaddr_in addr;
        socklen_t addr_len;

        // accept blocks until an incoming connection arrives
        // it return a 'file descriptor' to the connection
        fprintf(stdout, "Waiting for a connection on port %d\n", SERVER_PORT);
        flush;

        connfd = accept(listenfd, (SA *) NULL, NULL);

        //zero out the receive buffer to make sure it ends up null terminated
        memset(recvline, 0, MAXLINE);

        //Read the client's message
        while((n = read(connfd, recvline, MAXLINE-1)) > 0){
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

            //hacky way to detect the end of the message
            if(recvline[n-1] == '\n')
                break;

            memset(recvline, 0, MAXLINE);
        }

        if(n < 0)
            err_n_die("read error.");
        
        //send harcoded response
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");

        //Normally you may want to check the results from write and close
        //in case errors oucr. For now, I'm ignoring them :)
        write(connfd, (char*)buff, strlen((char *)buff));
        close(connfd);
    }
    return 0;
}
