#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int radix_sort(int *arr, int len,    // length of data
                int maxdigit)   // ex) if max Num is 20321,  maxdigit is 5
{
	int i, n;
	int pval, idx;
	int *count, *temp;

 
	count = (int*)malloc(10 * sizeof(int));
	temp = (int*)malloc(len * sizeof(int));
 
	for (n = 0; n < maxdigit; n++) {
		memset(count, 0, sizeof(int) * 10);
 
		pval = (int)pow((double)10, n);
		for (i = 0; i < len; i++) {
			idx = (arr[i]/pval) % 10;
			count[idx]++;
		}

		for (i = 1; i < 10; i++)
			count[i] += count[i-1];

 
		for (i = len - 1; i >= 0; --i) {
			idx = (int)(arr[i]/pval) % 10;
			memcpy(temp+(count[idx]-1), arr+i, sizeof(int));
			count[idx]--;
		}
 
		memcpy(arr, temp, sizeof(int) * len);
	}
	free(temp);
	free(count);
	return 0;
}

int main(int argc, char**argv)
{
	int i;
	int arr[] = {4, 5, 2, 6, 2, 3, 1, 1};
	int arr1[] = {23, 51, 232, 61324, 1161, 22};
	radix_sort(arr, 8, 1); 

	for (i = 0; i < 8; ++i) {
		printf("%d ", arr[i]);
	}

	printf("\n");

	radix_sort(arr1, 6, 5);
	for (i = 0; i < 6; ++i) {
		printf("%d ", arr1[i]);
	}

	printf("\n");

	return 0;
}
