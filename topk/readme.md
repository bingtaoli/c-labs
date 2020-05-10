
Top K是很常见的一种问题，是指在N个数的无序序列中找出最大的K个数，而其中的N往往都特别大，对于这种问题，最容易想到的办法当然就是先对其进行排序，然后直接取出最大的K的元素就行了，但是这种方法往往是不可靠的，不仅时间效率低而且空间开销大，排序是对所有数都要进行排序，而实际上，这类问题只关心最大的K个数，并不关心序列是否有序，因此，排序实际上是浪费了的很多资源都是没必要的。
————————————————
版权声明：本文为CSDN博主「HerofH_」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_28114615/java/article/details/86231822

### 堆排序法

堆排序是通过维护大顶堆或者小顶堆来实现的。堆排序法来解决N个数中的TopK的思路是：先随机取出N个数中的K个数，将这N个数构造为小顶堆，那么堆顶的数肯定就是这K个数中最小的数了，然后再将剩下的N-K个数与堆顶进行比较，如果大于堆顶，那么说明该数有机会成为TopK，就更新堆顶为该数，此时由于小顶堆的性质可能被破坏，就还需要调整堆；否则说明这个数最多只能成为Top K+1 th，因此就不用管它。然后就将下一个数与当前堆顶的数作比较，根据大小关系如上面所述方法进行操作，直到N-K个数都遍历完，此时还在堆中的K个数就是TopK了。

```cpp
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
```