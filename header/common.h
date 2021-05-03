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

// useful constants
#define SERVER_PORT   18000
#define MAXLINE       4096
#define SA            struct sockaddr
#define endl          fprintf(stdout,"\n")
#define flush         fflush(stdout)

void err_n_die(const char *fmt, ...);
char *bin2hex(const unsigned char *input, size_t len);

#endif