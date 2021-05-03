#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/socket.h> /* basic socket definitions */
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> /* for inet_pton */
#include <stdarg.h> /* for variable argument functions, like err_n_die */
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>

// useful constants
#define SERVER_PORT   18000
#define MAXLINE       4096
#define SA            struct sockaddr
#define endl          fprintf(stdout,"\n")
#define flush         fflush(stdout)
#define SERVER_CERT_FILE "cert.pem"
#define SERVER_PRIVATE_KEY_FILE "key.pem"

void err_n_die(const char *fmt, ...);
char *bin2hex(const unsigned char *input, size_t len);
void init_openssl(void);
void cleanup_openssl(void);
int create_socket(int port);
SSL_CTX *create_context(void);
void configure_context(SSL_CTX *ctx);

#endif