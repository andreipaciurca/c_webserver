#include "../header/common.h"

void err_n_die(const char *fmt, ...){
    int errno_save;
    va_list ap;

    //all system calls can set errno, so we need to save it now
    errno_save = errno;

    //print out the fmt+args to standard output
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    endl; /* fprintf(stdout,"\n"); */
    flush; /* fflush(stdout); */

    //print out error messages if errno was set
    if(errno_save != 0){
        fprintf(stdout, "(errno = %d) : %s\n",errno_save, strerror(errno_save));
        endl;
        flush;
    }

    va_end(ap);

    //this is the ...and_die part. Terminate with an error.
    exit(1); /* EXIT_FAILURE */
}

char *bin2hex(const unsigned char *input, size_t len){
    char *result;
    char *hextits = "0123456789ABCDEF";

    if(input == NULL || len <= 0)
        return NULL;
    
    // (2 hexits+scpace)/chr + NULL
    int resultlength = (len*3)+1;

    result = malloc(resultlength);
    bzero(result,resultlength);

    for(int i = 0; i < len; i++){
        result[i*3] = hextits[input[i]>>4];
        result[(i*3)+1] = hextits[input[i] & 0x0F];
        result[(i*3)+2] = ' '; // for readability
    }

    return result;
}

void init_openssl(void){
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl(void){
    EVP_cleanup();
}

int create_socket(int port){
    /* Defining useful variables for sockets and address, buffer */
    int                 listenfd;
    struct sockaddr_in  servaddr;

    /* Address */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET; // an internet address
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //InternetAddress_Any = I'm responding to anything
    servaddr.sin_port        = htons(port); //server port, in this case hardcoded on 443


    /* Creating new socket / Allocating new resources 
     * AF_INET = ADRESS FAMILY _ INTERNET = an internet socket
     * SOCK_STREAM for TCP Stream, it's like a file
     * 0 it's default value for TCP
    */

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("socket error.");


    /* Bind & Listen */

    //This basically says to the OS: this socket is going to listen to this address
    if((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
        err_n_die("bind error.");

    if((listen(listenfd, 10)) < 0)
        err_n_die("listen error.");

    return listenfd;

}

SSL_CTX *create_context(void){
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();
    
    ctx = SSL_CTX_new(method);
    if (!ctx) {
	perror("Unable to create SSL context");
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_chain_file(ctx, SERVER_CERT_FILE) != 1 ||
        SSL_CTX_use_PrivateKey_file(ctx, SERVER_PRIVATE_KEY_FILE, SSL_FILETYPE_PEM) != 1) {
        SSL_CTX_free(ctx);
        ctx = NULL;
	    err_n_die("unable to locate or use certs.");
    }

}