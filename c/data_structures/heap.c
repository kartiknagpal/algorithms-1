/*   heap.c
 * 
 *   Implementation of a heap using an array
 */
#include <stdio.h>
#include <stdlib.h>

struct heap_s {
	int size;
	int length;
	int data[0];
};

//http://forge.voodooprojects.org/p/chameleon/source/tree/HEAD/branches/meklort/i386/modules/klibc/memswap.c
void memswap(void *m1, void *m2, int size)
{
	char *p = m1;
	char *q = m2;
	char temp;
	
	while (size--) {
		temp = *p;
		*p = *q;
		*q = temp;
		p++; q++;
	}
}
static struct heap_s* create_heap(int N)
{
	struct heap_s *heap;
	heap = (struct heap_s*)malloc(sizeof(struct heap_s) + (sizeof(int)*N));
	heap->size = N;
	heap->length = 0;	
	return heap;
}

static void max_heapify_down(struct heap_s *heap, int idx)
{
	int *data = heap->data;
	int largest = idx;
	int left, right;
	
	left = (idx << 1) + 1;
	right = (idx << 1) + 2;

	if (left+1 > heap->length && right+1 > heap->length)
		return;
	
	if (data[idx] < data[left])
		largest = left;
	if (right <= heap->length && data[idx] < data[right])
		largest = right;

	if (largest != idx) {
		memswap(data+idx, data+largest, sizeof(int));	
		max_heapify_down(heap, largest);
	}
}

static void max_heapify_up(struct heap_s *heap, int idx)
{
	int *data = heap->data;
	int largest = 0;

	if (idx <= 0)
		return;
	
	if (data[idx] > data[(idx-1)>>1])
		largest = idx;
	if (largest != 0 || largest == idx) {
		memswap(data+((idx-1)>>1), data+idx, sizeof(int));	
		max_heapify_up(heap, (idx-1)>>1);
	}
}
int insert(struct heap_s* heap, int data)
{
	if (heap->size <= heap->length) 
		return -1;
	if (heap->length == 0) {
		heap->data[0] = data;
		heap->length++;
		return 0;
	}
	heap->data[heap->length] = data;
	max_heapify_up(heap, heap->length);
	heap->length++;
}

int delete(struct heap_s *heap)
{
	int *data = heap->data;
	int root_value = data[0];

	data[0] = data[heap->length-1];
	heap->length--;

	// doesn't need to heapify
	if (heap->length <= 1)
		return root_value;

	max_heapify_down(heap, 0);
	return root_value;
}

int main(int argc, char **argv)
{
	int i;
	struct heap_s *heap;
	heap = create_heap(6);
	
	int input[6] = {4, 9, 3, 121, 15, 32};

	for (i = 0; i < 6; i++) {
		insert(heap, input[i]);
	}

	for (i = 0; i < 6; i++) {
		printf("heap data : %d\n", heap->data[i]);
	}

	for (i =0 ; i < 6; i++) {
		printf("delete %d\n", delete(heap));
	}
	return 0;
}
