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
