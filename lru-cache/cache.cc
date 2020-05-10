#include <iostream>
#include <map>
#include <unordered_map>
using namespace std;

class LRUCache {
public:
    /*
    使用双向链表，因为可以在常数时间内删除节点
    这里使用环形双向链表，即首尾相连。第一个节点的prev即可以找到队列的尾部，常数时间。
    
    实现lrucache使用两个队列，free_queue和cache_queue，一开始cache_queue没有数据，put时候从free_queue摘下一个node来放到到cache_queue的头部去。
    等free_queue空了之后，从cache_queue尾部摘下node存储新数据再放到cache_queue的头部。

    get: 从key2node可以迅速找到是否有node存储了这个key，找到后从cache_queue中把这个node移动到头部去。

    */
    struct DoubleLinkedNode
    {
        int val;
        DoubleLinkedNode *prev;
        DoubleLinkedNode *next;
        DoubleLinkedNode():val(0),prev(NULL),next(NULL){}
    };

    LRUCache(int num)
    {
        //init free cache
        free_queue = init_queue(num);
        cache_queue = init_queue(0);
    }

    ~LRUCache()
    {
        cout << "destruct queue" << endl;
        delete[] free_queue;
        delete[] cache_queue;
    }

    DoubleLinkedNode* init_queue(int num)
    {
        //初始化存储队列，双向链表可以使用prev和来
        DoubleLinkedNode *head = new DoubleLinkedNode[num+1];
        if (0 == num)
        {
            head[0].prev = head;
            head[0].next = head;
        }
        else
        {
            DoubleLinkedNode *prev = &head[0];
            for (int i = 1; i < num+1; i++)
            {
                DoubleLinkedNode& cur = head[i];
                cur.prev = prev;
                prev->next = &cur;
                prev = &cur;
            }
            head[num].next = &head[0];
            head[0].prev = &head[num];
        }
        return head;
    }

    void queue_remove(DoubleLinkedNode *node)
    {
        //从队列中移除node，涉及到四个箭头
        DoubleLinkedNode *next = node->next;
        DoubleLinkedNode *prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
        next->prev = prev;
        prev->next = next;
    }

    void queue_insert_head(DoubleLinkedNode *queue, DoubleLinkedNode *node)
    {
        //把node放在队列头部，涉及到四个箭头
        node->next = queue[0].next;
        node->prev = &queue[0];
        node->next->prev = node;
        queue[0].next = node;
    }

    bool queue_is_empty(DoubleLinkedNode *queue)
    {
        return queue == queue[0].prev;
    }

    DoubleLinkedNode* queue_last(DoubleLinkedNode *queue)
    {
        return queue[0].prev;
    }

    DoubleLinkedNode* queue_head(DoubleLinkedNode *queue)
    {
        //返回第一个节点
        return queue[0].next;
    }

    void put(int key, int value)
    {
        DoubleLinkedNode *node;
        if (key2node.find(key) != key2node.end())
        {
            node = key2node[key];
        }
        else
        {
            if (queue_is_empty(free_queue))
            {
                //take the least used node
                node = queue_last(cache_queue);
                //erase map
                int old_key = node2key[node];
                key2node.erase(old_key);
                cout << "free queue full, get node from cache remove key:" << old_key << endl;
            }
            else
            {
                cout << "take a node from free_queue" << endl;
                //take a free node
                node = queue_head(free_queue);
            }
        }
        //remove from list
        queue_remove(node);
        node->val = value;
        queue_insert_head(cache_queue, node);
        key2node[key] = node;
        node2key[node] = key;
    }

    int get(int key)
    {
        if ( key2node.find(key) != key2node.end() )
        {
            //把node放在最前面
            DoubleLinkedNode *node = key2node[key];
            queue_remove(node);
            queue_insert_head(cache_queue, node);
            return node->val;
        }
        return -1;
    }

private:
    int size;
    int capacity;
    DoubleLinkedNode *free_queue;
    DoubleLinkedNode *cache_queue;
    unordered_map<int, DoubleLinkedNode*> key2node;
    unordered_map<DoubleLinkedNode*, int> node2key;
};

int main()
{
    LRUCache cache(2);
    cache.put(2, 1);
    cache.put(1, 2);
    cache.put(2, 3); 
    cache.put(4, 1);
    cout << cache.get(1) << endl;
    cout << cache.get(2) << endl;
    return 0;
}