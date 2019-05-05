#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>         
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <arpa/inet.h> 
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 1024

int main(int argc, char*argv[]){
	int client_fd, len_c, n, check;
	struct sockaddr_in server;
    uint8_t buf[BUFSIZE];
    int con_check;
    unsigned char *message;

    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 41000,
        .channels = 2
    };   // specifications of the recorded audio like sample rate
    pa_simple *s = NULL; // creating a simple connection object
    int ret = 1;
    int error;

    client_fd = socket(AF_INET,SOCK_STREAM,0);

    server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr(argv[1]);
	server.sin_port=htons(4000);
	memset(server.sin_zero, '\0', sizeof server.sin_zero);
        // pa_simple_new will create a connection to server and  by taking input arguments as record
	if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }

	con_check = connect(client_fd,(struct sockaddr *)&server, sizeof(server));

    while(1){
    	
		if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) { // will simply read the  audio stream from the server i.e microphone
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }
        /*
        message = (unsigned char*)buf;
		write(client_fd,message,BUFSIZE);
		*/
		write(client_fd,buf,sizeof(buf)); // will write the recorded audio stream to a buffer

	}
	ret = 0;

finish:

    if (s)
        pa_simple_free(s); //Close and free the connection to the server.

    return ret;
}
