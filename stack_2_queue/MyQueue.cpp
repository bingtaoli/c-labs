#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

class MyQueue
{
public:
	/** Push element x to the back of queue. */
    void push(int x) 
    {
    	std::lock_guard<std::mutex> lk(m_mutex);
    	s1.push_back(x);
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() 
    {
    	std::lock_guard<std::mutex> lk(m_mutex);
    	if (s2.size() > 0)
    	{
    		int v = s2.back();
    		s2.pop_back();
    		return v;
    	}
    	if (s1.size())
    	{
    		//从s1搬到s2中
    		for (int i = s1.size()-1; i >= 0; i--)
    		{
    			s2.push_back(s1[i]);
    		}
    		s1.clear();
    		int v = s2.back();
    		s2.pop_back();
    		return v;
    	}
    	return -1;
    }
    
    /** Get the front element. */
    int peek() 
    {
    	std::lock_guard<std::mutex> lk(m_mutex);
    	if (s2.size() > 0)
    	{
    		int v = s2.back();
    		return v;
    	}
    	if (s1.size())
    	{
    		//从s1搬到s2中
    		for (int i = s1.size()-1; i >= 0; i--)
    		{
    			cout << "i:" << i << endl;
    			cout << "move" << " " << s1[i] << endl;
    			s2.push_back(s1[i]);
    		}
    		s1.clear();
    		int v = s2.back();
    		return v;
    	}
    	return -1;
    }
    
    /** Returns whether the queue is empty. */
    bool empty() 
    {
    	std::lock_guard<std::mutex> lk(m_mutex);
    	return (s1.size() + s2.size()) > 0;
    }

private:
	std::vector<int> s1;
	std::vector<int> s2;
	std::mutex m_mutex;
};

int main(int argc, char const *argv[])
{
	MyQueue queue;

	queue.push(1);
	queue.push(2);  
	cout << queue.peek() << endl;  // 返回 1
	cout << queue.pop() << endl;   // 返回 1
	queue.empty(); // 返回 false

	return 0;
}