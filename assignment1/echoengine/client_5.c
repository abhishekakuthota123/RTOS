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
	int msqid_1,msqid_2;
	key_t key_1,key_2;
        int len;

	if ((key_1 = ftok("/etc", 'C')) == -1)
	{
		perror("ftok");
		exit(1);
	}

if ((key_2 = ftok("/bin", 'B')) == -1)
	{
		perror("ftok");
		exit(1);
	}


	if ((msqid_1 = msgget(key_1, 0644)) == -1)
	{
		perror("msgget");
		exit(1);
	}
	
        if ((msqid_2 = msgget(key_2, 0644)) == -1)
	{
		perror("msgget");
		exit(1);
	}
	  
        
		       	
	buf.mtype = 5;
	 
	while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL)

{
len = strlen(buf.mtext);
/* ditch newline at end, if it exists */
if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
if (msgsnd(msqid_2, &buf, len+1, 0) == -1) /* +1 for '\0' */
perror("msgsnd");
	
			
		len = 1000;
		if (msgrcv(msqid_1, &buf, len+1,5, 0) == -1) /* +1 for '\0' */
			perror("msgrcv");
		len=strlen(buf.mtext);
		printf("\n%d",len);
		buf.mtext[len]='\n';
	       printf("\n%s",buf.mtext);
	

	}	       
	if (msgctl(msqid_1, IPC_RMID, NULL) == -1)
	{
		perror("msgctl");
		exit(1);
	}

	return 0;
}


