#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define SHMKEY 75
int shmid;
int *loc;

void CLIENT(){
	int i;
	shmid = shmget(SHMKEY, 1024, 0777);
	loc = (int *) shmat(shmid, 0, 0);
	for(i = 9; i >= 0; i--){
		while(*loc != -1);
		*loc = i;
		printf("%d", *loc);
		printf("(client) sent\n");
	}
	exit(0);
}

void SERVER(){
	int i;
	shmid = shmget(SHMKEY, 1024, 0777|IPC_CREAT);
	loc = (int *)shmat(shmid, 0, 0);
	do{
		*loc = -1;
		while(*loc == -1);
		printf("%d", *loc+10);
		printf("(server) received\n");
	}while(*loc);
	shmctl(shmid, IPC_RMID, 0);
	exit(0);
}

int main(){
	int f1, f2;
	while((f1=fork()) == -1);
	if(f1 == 0) SERVER();
	while((f2=fork()) == -1);
	if(f2 == 0) CLIENT();
	wait(0);
	wait(0);
	return 0;
}
