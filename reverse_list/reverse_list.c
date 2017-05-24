#include <stdio.h>
#include <stdlib.h>

/**
 * 简悦云风 微博http://weibo.com/2388714105/DjvJZC1Qc?from=page_1005052388714105_profile&wvr=6&mod=weibotime&type=comment
 * 最近为招应届生出了道笔试编程题,翻转一个单向链表.结果 7,8 个人里没有一个人可以正确的写出 O(1) 空间复杂度 O(n) 时间复杂度的程序. 搞得我都怀疑人生了.
 * 刚才自己试了一下, 不就是十几行大白话的 C 程序吗? 真的有那么难?
 * */

typedef struct node_t {
    int value;
    struct node_t *next;
} Node;

Node * reverse_list(Node *head){
    Node *slow = NULL;
    Node *fast = head;
    Node *faster = NULL;
    while (fast){
        faster = fast->next;
        fast->next = slow;
        slow = fast;
        fast = faster;
    }
    return slow;
}

void dump_list(Node *p){
    while (p != NULL){
        printf("%d ", p->value);
        p = p->next;
    }
    printf("\n");
}

int main(){
    Node first = {1, NULL};
    Node second = {2, NULL};
    Node third = {3, NULL};
    Node fourth = {4, NULL};
    Node fifth = {5, NULL};
    first.next = &second;
    second.next = &third;
    third.next = &fourth;
    fourth.next = &fifth;
    dump_list(&first);
    Node *result = reverse_list(&first);
    dump_list(result);
    return 0;
}
