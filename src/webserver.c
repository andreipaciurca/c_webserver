#include "../header/common.h"

int main(int argc, char const *argv[])
{
    /* Defining useful variables for sockets, adress, buffer, etc */
    // for SSL socket connection
    int      socket;
    SSL_CTX  *ctx;
    //for client messages
    int      client, n;
    uint8_t  buff[MAXLINE+1];
    uint8_t  recvline[MAXLINE+1];

    /* init TLS connection */
    init_openssl();
    ctx = create_context();
    configure_context(ctx);

    /* init socket */
    socket = create_socket(SERVER_PORT);
    
    //Server infinite loop / handle connection
    for(;;){
        struct sockaddr_in addr;
        socklen_t addr_len;
        SSL *ssl;
        char client_address[MAXLINE+1];

        // accept blocks until an incoming connection arrives
        // it return a 'file descriptor' to the connection
        fprintf(stdout, "Waiting for a connection on port %d\n", SERVER_PORT);
        flush;

        client = accept(socket, (SA *) &addr, &addr_len);
        if (client < 0){
            err_n_die("unable to accept.");
        }

        inet_ntop(AF_INET, &addr, client_address, MAXLINE);
        fprintf(stdout,"Client conection: %s\n",client_address);

        /*
        //zero out the receive buffer to make sure it ends up null terminated
        memset(recvline, 0, MAXLINE);

        //Read the client's message
        while((n = read(client, recvline, MAXLINE-1)) > 0){
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

            //hacky way to detect the end of the message
            if(recvline[n-1] == 0x00)
                break;

            memset(recvline, 0, MAXLINE);
        }

        if(n < 0)
            err_n_die("read error.");
        */

        //send harcoded response
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");

        //Normally you may want to check the results from write and close
        //in case errors oucr. For now, I'm ignoring them :)
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0)
            ERR_print_errors_fp(stderr);
        else
            SSL_write(ssl, (char*)buff, strlen((char *)buff));

        /* close part */
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }
    close(socket);
    SSL_CTX_free(ctx);
    cleanup_openssl();
    return 0;
}
