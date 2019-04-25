#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<pthread.h>
#include<time.h>
#include<errno.h>


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 1024

static int  server_address_size, client_address_size;
static struct sockaddr_in client, server;

/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd2, const void*data, size_t size) {
    ssize_t ret = 0;

    while (size > 0) {
        ssize_t r;

        if ((r = write(fd2, data, size)) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }

    return ret;
}

int fd,fd1;
void* receive()
{

FILE *file_out ;    
	file_out=fopen("output2.txt","w+"); 
char *name="paserver";
    pa_simple *s = NULL;
int file = fileno(file_out);
         if (dup2(fd, STDIN_FILENO) < 0) {
            fprintf(stderr, __FILE__": dup2() failed: %s\n", strerror(errno));
            goto finish;
        } 
    /* The Sample format to use */
static const   pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };     
    int ret = 1;
    int error;
   /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }  

     uint8_t buf[BUFSIZE];
     uint8_t recvbuf[BUFSIZE];
int len;
ssize_t r;
while(1)
{
   len = recvfrom(fd, recvbuf, sizeof(recvbuf),0,(struct sockaddr *)&client, &client_address_size) ;
   if( len <=0)
   {
       perror("receive");
      exit(4);
   }
  
        /* And write it to STDOUT */
       if (loop_write(file, recvbuf, sizeof(recvbuf)) != sizeof(recvbuf)) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
            }

          
              /* Read some data ... */
        if ((r = read(STDIN_FILENO, recvbuf, sizeof(recvbuf))) <= 0) {
            if (r == 0) /* EOF */
                break;

            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            goto finish;
        }
           
 if (pa_simple_write(s, recvbuf, sizeof(recvbuf) , &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
      }

 /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
        } 
 
  }
   ret = 0;

finish:

    if (s)
        pa_simple_free(s);

}
void* send1()
{
/* The sample type to use */
    static const pa_sample_spec sss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;        
      char *name=" paclient ";
   /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, name , PA_STREAM_RECORD, NULL, "record", &sss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }


uint8_t sendbuf[BUFSIZE];
while(1)
{
      
        


   /* Record some data ... */
        if (pa_simple_read(s, sendbuf, sizeof(sendbuf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

    
if(sendto(fd,sendbuf,sizeof(sendbuf),0,(struct sockaddr *)&client,sizeof(client))<0)
{
	perror("send");
	exit(5);
} 
}

   ret = 0;

finish:

    if (s)
        pa_simple_free(s);
}
int main( int argc, char*argv[])

{
     
  int buf[1024];
 static  int len=0;

   if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       exit(1);
   }

   /*
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network
    * interface.)
    */
   server.sin_family      = AF_INET;  /* Server is in Internet Domain */
   server.sin_port        = 0;         /* Use any available port      */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */
   
   if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
      
       exit(2);
   }
   /* Find out what port was really assigned and print it */
   server_address_size = sizeof(server);
   if (getsockname(fd, (struct sockaddr *) &server, &server_address_size) < 0)
   {  
       exit(3);
   }
   printf("Port assigned is %d\n", ntohs(server.sin_port));

len=recvfrom(fd, buf, sizeof(buf),0,(struct sockaddr *)&client, &client_address_size) ;   
pthread_t p1,p2,p3;
pthread_create(&p1,NULL,send1,NULL);
pthread_create(&p2,NULL,receive,NULL);
pthread_join(p1,NULL);
pthread_join(p2,NULL);
return 0;

}

