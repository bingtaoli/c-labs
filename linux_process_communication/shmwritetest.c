/**
 * 共享内存
 */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	char name[4];
	int age;
} people;

int main(){
	int shm_id, i;
	char temp;
	people *p_map;
	//IPC_PRIVATE 0 会建立新共享内存对象
	shm_id = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0600);
	if (shm_id == -1){
		perror("shmget error");
		return 1;
	}
	void *address = shmat(shm_id, NULL ,0);
	if (*((int *)address) == -1){
		perror("attach failed\n");
		return 1;
	}
	p_map = (people *)address;
	temp = 'a';
	for (i = 0; i < 10; i++){
		temp += 1;
		memcpy((*(p_map+i)).name, &temp, 1);
		(*(p_map+i)).age = 20 + i;
	}
	for (i = 0; i < 10; i++){
		printf("people %d, name: %s\n", i, (*(p_map+i)).name);
	}
	if (shmdt(p_map) == -1){
		perror("detach error\n");
	}
	return 0;
}
