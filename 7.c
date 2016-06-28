#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define MSGKEY 75

struct msgform{
	long mtype;
	char mtext[1030];
}msg;

int msgqid, i;

void CLIENT(){
	int i;
	msgqid = msgget(MSGKEY, 0777);
	for(i = 10; i >= 1; i--){
		msg.mtype = i;
		if(msgsnd(msgqid, &msg, 1024, 0) != -1){
			printf("(client) sent \n");
		}
		else printf("sent error!\n");
		sleep(1);
	}
	exit(0);
}

void SERVER(){
	msgqid = msgget(MSGKEY, 0777|IPC_CREAT);
	do{
		msgrcv(msgqid, &msg, 1030, 0, 0);
		printf("(Server) recieved\n");
		sleep(1);
	}while(msg.mtype != 1);
	msgctl(msgqid, IPC_RMID, 0);
	exit(0);
}


void main(){
	while((i=fork()) == -1);
	if(!i) SERVER();
	while((i=fork()) == -1);
	if(!i) CLIENT();
	wait(0);
	wait(0);
}
