#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MSGKEY 75

int shmid, f1, f2, *loc;

void CLIENT(){
	int i;
	shmid = shmget(MSGKEY, 1024, 0777);
	loc = shmat(shmid, NULL, 0);
	for(i = 9; i >= 0; i--){
		while(*loc != -1);
		*loc = i;
		printf("(client) sent %d\n", *loc);
		sleep(1);
	}
	exit(0);
}

void SERVER(){
	shmid = shmget(MSGKEY, 1024, 0777|IPC_CREAT);
	loc = shmat(shmid, NULL, 0);
	do{
		*loc = -1;
		while(*loc == -1);
		printf("(server) received %d\n", *loc);
		sleep(1);
	}while(*loc);
	shmctl(shmid, IPC_RMID, 0);
	exit(0);
}

int main(){
	while((f1 = fork()) == -1);
	if(f1 == 0) SERVER();
	while((f2 = fork()) == -1);
	if(f2 == 0) CLIENT();
	wait(0);
	wait(0);
	return 0;
}
