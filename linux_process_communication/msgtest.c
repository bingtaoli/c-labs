/**
 *  消息队列
 **/
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>

void msg_stat(int, struct msqid_ds);

int main(){
	int gflags, sflags, rflags;
	key_t key;
	int msgid;
	int reval;
	struct msgsbuf{
		long int mtype;
		char mtext[1];
	} msg_sbuf;
	struct msgmbuf {
		int mtype;
		char mtext[10];
	} msg_rbuf;
	struct msqid_ds msg_ginfo, msg_sinfo;
	char *msgpath = "/usr/local/msgtest";
	key = ftok(msgpath, 0X111);
	/**
	 * NOTE 如果队列不存在或者被删除了，则必须CREAT
	 * 存在则不能CREAT
	 */
	gflags = IPC_CREAT | IPC_EXCL;
	msgid = msgget(key,  gflags | 0666);
	if (msgid == -1){
		printf("msg create error\n");
		return 1;
	}
	/*创建一个消息队列后，输出消息队列的默认属性*/
	msg_stat(msgid, msg_ginfo);
	sflags = IPC_NOWAIT;
	msg_sbuf.mtype = 11;
	msg_sbuf.mtext[0] = 'a';
	reval = msgsnd(msgid, &msg_sbuf, sizeof(msg_sbuf.mtext), sflags);
	if (reval == -1){
		printf("send message error\n");
		return 1;
	}
	/*发送一个消息后，输出消息队列的属性*/
	msg_stat(msgid, msg_ginfo);
	rflags = IPC_NOWAIT | MSG_NOERROR;
	reval = msgrcv(msgid, &msg_rbuf, 4, 11, rflags);
	if (reval == -1){
		printf("message read error\n");
		return 1;
	} else {
		printf("read from msg queue %d bytes \n", reval);
	}
	msg_stat(msgid, msg_ginfo);
	msg_sinfo.msg_perm.uid = 8;
	msg_sinfo.msg_perm.gid = 8;
	msg_sinfo.msg_qbytes = 16388;
	/**
	 * 验证超级用户可以更改消息队列的默认msg_qbytes
	 * 普通权限执行程序会报错 
	 */
	reval = msgctl(msgid, IPC_SET, &msg_sinfo);
	if (reval == -1){
		printf("msg set info error \n");
		return 1;
	}
	msg_stat(msgid, msg_ginfo);
	reval = msgctl(msgid, IPC_RMID, NULL); /*删除*/
	if (reval == -1){
		printf("unlink msg queue error \n");
		return 1;
	}
	return 0;
}

void msg_stat(int msgid, struct msqid_ds msg_info){
	int reval;
	sleep(1);
	reval = msgctl(msgid, IPC_STAT, &msg_info);
	if (reval == -1){
		printf("get msg info error\n");
		return ;
	}
	printf("\n");
	printf("current number of bytes on queue is %lu\n", msg_info.msg_cbytes);
	printf("number of messages in queue is %lu\n", msg_info.msg_qnum);
	printf("max number of bytes on queue is %lu\n", msg_info.msg_qbytes);
	printf("\n");
}
