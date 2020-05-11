/*
归并排序 使用aux变量作为辅助空间
*/
#include <iostream>
using namespace std;
 
// 将arr[l...mid]和arr[mid+1...r]两部分进行归并，这两部分已经是排好序的
// 其中aux为完成merge过程所需要的辅助空间
template<typename T>
void merge(T arr[], T aux[], int l, int mid, int r)
{
	int pa = l; 
	int pb = mid+1;
	int i = l;
	while (i <= r)
	{
		if (pa == mid+1)
		{
			aux[i] = arr[pb];
			pb++;
		}
		else if (pb == r+1)
		{
			aux[i] = arr[pa];
			pa++;
		}
		else if (arr[pa] < arr[pb])
		{
			aux[i] = arr[pa];
			pa++;
		}
		else 
		{
			aux[i] = arr[pb];
			pb++;
		}
		i++;
	}
	for (i = l; i <= r; i++)
	{
		arr[i] = aux[i];
	}
}

//使用优化的归并排序算法, 对arr[l...r]的范围进行排序
//其中aux为完成merge过程所需要的辅助空间
template<typename T>
void _mergeSort(T arr[], T aux[], int l, int r)
{
	if (l >= r)
	{
		//split到只有一个元素
		return ;
	}
	int mid = (l+r)/2;
	_mergeSort(arr, aux, l, mid);
	_mergeSort(arr, aux, mid+1, r);
	merge(arr, aux, l, mid, r);
}

template<typename T>
void mergeSort(T arr[], int n)
{
	T *aux = new T[n];
	_mergeSort(arr, aux, 0, n-1);
	delete[] aux;
 }

int main(int argc, char const *argv[])
{
	int nums[20];
	for (int i = 0; i < 20; i++)
	{
		int n = rand()%100;
		nums[i] = n;
	}
	mergeSort(nums, 20);
	for (unsigned int i = 0; i < sizeof(nums)/sizeof(int); i++)
	{
		cout << nums[i] << " ";
	}
	cout << endl;
	return 0;
}