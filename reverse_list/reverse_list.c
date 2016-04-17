#include <stdio.h>
#include <stdlib.h>

/**
 * 简悦云风 微博http://weibo.com/2388714105/DjvJZC1Qc?from=page_1005052388714105_profile&wvr=6&mod=weibotime&type=comment
 * 最近为招应届生出了道笔试编程题,翻转一个单向链表.结果 7,8 个人里没有一个人可以正确的写出 O(1) 空间复杂度 O(n) 时间复杂度的程序. 搞得我都怀疑人生了. 刚才自己试了一下, 不就是十几行大白话的 C 程序吗? 真的有那么难?
 * */

typedef struct list_tag {
	struct list_tag *next;
	int value;
} List;

List * reverse_list(List *list){
	List *first = list;
	if (first->next == NULL){
		return first;
	}
	List *second = first->next;
	if (first->next->next == NULL){
		second->next = NULL;
		first->next = NULL;
		return second;
	}
	first->next = NULL;
	while (second->next != NULL){
		List *prev = first;
		first = second;
		second = second->next;
		first->next = prev;
	}
	second->next = first;
	return second;
}

List *init_list(int value){
	List *p = (List *)malloc(sizeof(List));
	p->next = NULL;
	p->value = value;
	return p;
}

void dump_list(List *p){
	while (p != NULL){
		printf("%d ", p->value);
		p = p->next;
	}
	printf("\n");
}

void free_list(List *p){
	while (p != NULL){
		List *prev = p;
		p = p->next;
		free(prev);
	}
}

int main(){
	List *first = init_list(1);
	List *second = init_list(2);
	List *third = init_list(3);
	List *fourth = init_list(4);
	List *fifth = init_list(5);
	first->next = second;
	second->next = third;
	third->next = fourth;
	fourth->next = fifth;
	dump_list(first);
	List *result = reverse_list(first);
	dump_list(result);
	free_list(result);
	return 0;
}
