/*
双向队列（deque，全名double-ended queue）

设计实现双端队列。
你的实现需要支持以下操作：

MyCircularDeque(k)：构造函数,双端队列的大小为k。
insertFront()：将一个元素添加到双端队列头部。 如果操作成功返回 true。
insertLast()：将一个元素添加到双端队列尾部。如果操作成功返回 true。
deleteFront()：从双端队列头部删除一个元素。 如果操作成功返回 true。
deleteLast()：从双端队列尾部删除一个元素。如果操作成功返回 true。
getFront()：从双端队列头部获得一个元素。如果双端队列为空，返回 -1。
getRear()：获得双端队列的最后一个元素。 如果双端队列为空，返回 -1。
isEmpty()：检查双端队列是否为空。
isFull()：检查双端队列是否满了。

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/design-circular-deque
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/

//设计思路：使用环形双向链表，第一个Node不作存储，只做标识作用
#include <iostream>
#include <thread>
using namespace std;

class MyCircularDeque {
public:

	struct LinkedListNode
	{
		int value;
		LinkedListNode *next;
		LinkedListNode *prev;
		LinkedListNode(int val):value(val),next(NULL),prev(NULL){}
	};

	~MyCircularDeque()
	{

	}

    /** Initialize your data structure here. Set the size of the deque to be k. */
    MyCircularDeque(int k) 
    {
    	size = 0;
    	capcity = k;
    	queue = new LinkedListNode(0);
    	queue->prev = queue;
    	queue->next = queue;
    }
    
    /** Adds an item at the front of Deque. Return true if the operation is successful. */
    bool insertFront(int value) 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	if (size == capcity)
    	{
    		return false;
    	}
    	//insert node to head of double-list
    	LinkedListNode *node = new LinkedListNode(value);
    	node->next = queue[0].next;
    	node->next->prev = node;
    	node->prev = &queue[0];
    	queue[0].next = node;
    	size++;
    	return true;
    }
    
    /** Adds an item at the rear of Deque. Return true if the operation is successful. */
    bool insertLast(int value) 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	if (size == capcity)
    	{
    		return false;
    	}
    	LinkedListNode *node = new LinkedListNode(value);
    	LinkedListNode *curTail = queue[0].prev;
    	queue[0].prev = node;
    	node->next = &queue[0];
    	node->prev = curTail;
    	curTail->next = node;
    	size++;
    	return true;
    }
    
    /** Deletes an item from the front of Deque. Return true if the operation is successful. */
    bool deleteFront() 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	if (size == 0)
    	{
    		return false;
    	}
    	LinkedListNode *node = queue[0].next;
    	node->next->prev = &queue[0];
    	queue[0].next = node->next;
    	node->next = NULL;
    	node->prev = NULL;
    	delete node;
    	size--;
    	return true;
    }
    
    /** Deletes an item from the rear of Deque. Return true if the operation is successful. */
    bool deleteLast() 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	if (size == 0)
    	{
    		return false;
    	}
    	LinkedListNode *node = queue[0].prev;
    	node->prev->next = &queue[0];
    	queue[0].prev = node->prev;
    	delete node;
    	size--;
    	return true;
    }
    
    /** Get the front item from the deque. */
    int getFront() 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	if (size == 0)
    	{
    		return -1;
    	}
    	if (queue[0].next)
    	{
    		return queue[0].next->value;
    	}
    	return -1;
    }
    
    /** Get the last item from the deque. */
    int getRear() 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	if (size == 0)
    	{
    		return -1;
    	}
    	if (queue[0].prev)
    	{
    		return queue[0].prev->value;
    	}
    	return -1;
    }
    
    /** Checks whether the circular deque is empty or not. */
    bool isEmpty() 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	return size == 0;
    }
    
    /** Checks whether the circular deque is full or not. */
    bool isFull() 
    {
    	std::lock_guard<std::mutex> lock(m_mutex);
    	return size == capcity;
    }

private:
	LinkedListNode *queue;
	int size;
	int capcity;
	std::mutex m_mutex;
};

/**
 * Your MyCircularDeque object will be instantiated and called as such:
 * MyCircularDeque* obj = new MyCircularDeque(k);
 * bool param_1 = obj->insertFront(value);
 * bool param_2 = obj->insertLast(value);
 * bool param_3 = obj->deleteFront();
 * bool param_4 = obj->deleteLast();
 * int param_5 = obj->getFront();
 * int param_6 = obj->getRear();
 * bool param_7 = obj->isEmpty();
 * bool param_8 = obj->isFull();
 */

int main(int argc, char const *argv[])
{
	MyCircularDeque *queue = new MyCircularDeque(10);
	queue->insertFront(1);
	cout << queue->getFront() << endl;
	return 0;
}