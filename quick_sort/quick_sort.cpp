//https://www.jianshu.com/p/e3909369295e
//快速排序讲解挺好的
#include <iostream>
using namespace std;

//把arr[l,r]进行分区，以第一个元素为关键点。
template<typename T>
int _partition(T arr[], int l, int r)
{
    T pivot = arr[l];
    //从l的右边开始遍历并整理数组，使得前面一部分小于v，后面一部分大于v
    //并用j记录下大于v和小于v的分界点的索引，i记录当前位置的索引。
    int j = l;  
    // arr[l+1...j] < v ; arr[j+1...i+1] > v
    for (int i = l+1; i <= r; i++)
    {
        if (arr[i] < pivot)
        {
            j++; //小于pivot的数据+1，所以j作为索引也+1
            //不断把比v小的数字调到前面j++的位置
            swap(arr[i], arr[j]);
        }
    }
    //此时j的位置也是小于v的数，再和v交换下位置
    swap(arr[l], arr[j]);
    return j;
}

// 对arr[l...r]部分进行快速排序
template<typename T>
void _quick_sort(T arr[], int l , int r)
{
    if (l >= r)
    {
        //只有一个元素就停止
        return;
    }
    int j = _partition(arr, l, r);
    _quick_sort(arr, l, j-1);
    _quick_sort(arr, j+1, r);
}

template<typename T>
void quick_sort(T arr[], int n)
{
    _quick_sort(arr, 0, n-1);
}

int main(int argc, char const *argv[])
{
    int a[] = {49,38,65,97,76,13,27};
    int n = sizeof(a)/sizeof(int);
    quick_sort(a, n);
    for(int i=0; i<=n-1; i++)
        printf("%4d", a[i]);
    printf("\n");
    return 0;
}
