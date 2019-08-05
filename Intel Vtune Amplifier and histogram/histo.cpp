#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void fillData(int *arr, const int n , const int range) {
	for (int i = 0; i < n; i++) {
		arr[i] = rand() % range;
	}
}

void printData(int *arr, const int n) {
	printf("[");
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("]\n");
}

void histogram(int *arr, const int n, int *h, const int range) {
	int i;

	#pragma omp parallel num_threads(4) private(i)
	{
		int *hist = (int *)malloc(sizeof(int) * range);
		for (i = 0; i < range; i++) {
			hist[i] = 0;
		}

		int id = omp_get_thread_num();
		int start = id * n / omp_get_num_threads();
		int end = (id + 1) * n / omp_get_num_threads();
		for (i = start; i < end; i++) {
			hist[arr[i]]++;
			//printf("thread id %d, hist val:%d\n", omp_get_thread_num(), hist[arr[i]]);
		}

		#pragma omp critical
		for (i = 0; i < range; i++) {
			h[i] += hist[i];
		}
	}
}

int main()
{
	int n = 10000000, range = 5000;
	int *arr = (int *)malloc(sizeof(int) * n);
	int *h = (int *)malloc(sizeof(int) * range);
	
	for (int i = 0; i < range; i++) {
		h[i] = 0 ;
	}
	fillData(arr, n, range);
	
	//printData(arr, n);

	double start = omp_get_wtime();
	histogram(arr, n, h, range);
	double end = omp_get_wtime();

	printf("Elpase Time: %lf \n", end - start);
	//printData(h, range);

	system("pause");
}