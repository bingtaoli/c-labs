# include <stdio.h>

void quick_sort(int list[], int left, int right){
    int i, j, x;
    if (left < right){
        i = left;
        j = right;
        x = list[i];
        while (i < j){
            while (i < j && list[j] > x){
                j--;
            }
            if (i < j){
                list[i] = list[j];
                i++;
            }
            while (i < j && list[i] < x){
                i++;
            }
            if (i < j){
                list[j] = list[i];
                j--;
            }
        }
        list[i] = x;
        quick_sort(list, left, i - 1);
        quick_sort(list, i + 1, right);
    }
}

int main(int argc, char const *argv[])
{
    int a[] = {49,38,65,97,76,13,27};
    int l = 0;  
    int r = 6;  
    quick_sort(a,l,r); 
    for(int i=0; i<=r; i++)
        printf("%4d", a[i]); 
    printf("\n");
    return 0;
}