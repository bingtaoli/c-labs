#include <iostream>
#include <vector>
#include<stdlib.h>
using namespace std;

void adjustMinHeap(vector<int>& nums,int root,int len) //小顶堆结点调整
{
    int lch=2*root+1;  //左子结点
    int rch=lch+1;   //右子结点
    int index=root;  //较大结点
 	
 	//lee: 为什么先右子节点比较呢？最小堆没有只规定父节点<子节点，对左右没有规定，所以左右都可以。
    if(rch<len&&nums[rch]<nums[index])index=rch; 
    if(lch<len&&nums[lch]<nums[index])index=lch;
 
 	//lee: 因为是小顶堆，比顶点大的数不断下沉到叶子节点。
    if(index!=root) //当前结点非最小结点
    {
        swap(nums[index],nums[root]);
        adjustMinHeap(nums,index,len);
    }
    return;
}
 
vector<int> TopKInHeap(vector<int>& nums,int k,int len)
{
    vector<int>res(nums.begin(),nums.begin()+k); //取出前k个数
 
    for(int i=k/2-1;i>=0;i--)  //根据前K个数建立一个小顶堆
    {
        adjustMinHeap(res,i,k);
    }
 
    //将剩下的数与堆顶做比较
    for(int i=k;i<len;i++)
    {
        if(nums[i]>res[0])  //当前数比堆顶数大
        {
            res[0]=nums[i]; //将堆顶更新为该数
            adjustMinHeap(res,0,k); //重新调整堆
        }
    }
 
    return res;
}

int main(int argc, char const *argv[])
{
	//初始化nums
	std::vector<int> nums;
	nums.reserve(10000);
	for (int i = 0; i < 10000; i++)
	{
		int n = rand();
		nums.push_back(n);
	}
	vector<int> res = TopKInHeap(nums, 10, nums.size());
	for (unsigned int i = 0; i < res.size(); i++)
	{
		cout << res[i] << endl;
	}
	return 0;
}