#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
	long mtype;
	char mtext[200];
};

int main(void)
{
	struct my_msgbuf buf;
	int msqid;
	key_t key;
        char buffer[]="HI";
	if ((key = ftok("client.c", 'B')) == -1)
	{
	//	perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644)) == -1)
	{
		perror("msgget");
		exit(1);
	}
	int i=0;
        for(;i==0;)
	{	       	
//	snprintf(buf.mtext,strlen(buffer),"%s",buffer); 
	buf.mtype = 1; /* we don't really care in this case */
	int len=1550;
	
	
			
		/* ditch newline at end, if it exists */
		if (msgrcv(msqid, &buf, len+1,0, 0) == -1) /* +1 for '\0' */
			perror("msgrcv");
                printf("bsdbbds:%s",buff);
                char c;
                fptr = fopen("marks.txt", "r"); 
                if (fptr == NULL) 
                { 
                  printf("Cannot open file \n"); 
                  exit(0); 
                } 
                // Read contents from file 
                c = fgetc(fptr); 
                 i=0;
                while (c != EOF) 
                { 
                 printf ("%c", c); 
                 c = fgetc(fptr);
                 my_msgbuf.mtext[i]=c;
                 } 
  
                 fclose(fptr); 
                 printf("hiii:%s",);
                /*
		len=strlen(buf.mtext);
		printf("\n%d",len);
		buf.mtext[len]='\n';
	       printf("\n%s",buf.m);
               */
	

	}	       
	if (msgctl(msqid, IPC_RMID, NULL) == -1)
	{
		perror("msgctl");
		exit(1);
	}

	return 0;
}


