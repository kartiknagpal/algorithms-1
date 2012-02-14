#include <stdio.h>
#include <stdlib.h>
void counting_sort(int *arr, int len){
	int i,j, max = 0;
	int idx_arr;
	char *count_bucket;

	for (i = 0; i < len; i++) {
		if (max < arr[i])	
			max = arr[i];
	}
	count_bucket = malloc((max+1)*sizeof(char));
	max = max + 1;

	for (i = 0; i < max; i++) {
		count_bucket[i] = 0;
	}

	for (i = 0; i < len; i++) {
		count_bucket[arr[i]]++;
	}

	idx_arr = 0;
	for (i = 0; i < max; i++) {
		if (count_bucket[i] == 0)
			continue;
		else {
			// for the duplicated value
			for (j = 0; j < count_bucket[i]; j++) {
				*(arr+idx_arr) = i;
				idx_arr++;
			}
		}
	}
	free(count_bucket);
}

int main(int argc, char **argv)
{
	int arr[] = {4, 5, 20, 6, 2, 3, 12, 1};
	counting_sort(arr, sizeof(arr)/sizeof(int));

	int i;
	for (i = 0; i < sizeof(arr)/sizeof(int); i++) {
		printf("%d  ", arr[i]);
	}
	printf("\n");
	return 0;
}
