#include "worst_categorizer.h"
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<algorithm>
#include<iostream>

void fillRandomData(int *arr, int n, const int range) {
	for (int i = 0; i < n; i++) {
		arr[i] = rand() % range;
	}
}

void fillSeqData(int *arr, int n, const int range) {
	int k = 0;
	for (int i = 0; i < n; i++, k++) {
		if (k == range) {
			k = 0;
		}
		arr[i] = k;
	}
}

void printData(int *arr, const int n) {
	printf("[");
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("]\n");
}

void sortData(int *arr, int n) {
	int tn = 4;

	#pragma omp parallel num_threads(tn)
	{
		int id = omp_get_thread_num();
		int start = id * n / tn;
		int end = (id + 1) * n / tn;
		std::sort(arr + start, arr + end);
	}
	
	for (int i = 1; i < tn; i++) {
		int s = 0;
		int m = i * n / tn;
		int e = (i + 1) * n / tn;
		int l = e - s;
		int *temp = (int *)(malloc(sizeof(int) * l));
		
		int k1 = s;
		int k2 = m;
		int k = 0;
		while (k1 < m || k2 < e) {
			if (k1 == m) {
				temp[k] = arr[k2];
				k2++;
			}
			else if (k2 == e) {
				temp[k] = arr[k1];
				k1++;
			}
			else if (arr[k1] < arr[k2]) {
				temp[k] = arr[k1];
				k1++;
			}
			else {
				temp[k] = arr[k2];
				k2++;
			}
			k++;
			//printf("%d, %d, %d\n", s, m, e);
			//printf("%d, %d, %d\n", k1, k2, k);
		}

		for (int k = 0; k < l; k++) {
			arr[s + k] = temp[k];
		}

		//printData(arr, n);
	}
}

void Section1() {
	int n = 7*1000000;
	int range = 4000;

	int *arr = (int *)(malloc(sizeof(int) * n));
	fillRandomData(arr, n, range);

	//printData(arr, n);
	
	double start = omp_get_wtime();

	int *categories = (int *)(malloc(sizeof(int) * n));
	for (int i = 0; i < n; i++) {
		categories[i] = categorize(arr[i]);
	}

	double end = omp_get_wtime();

	//printData(categories, n);

	printf("Section 1 Elapse Time: %lf\n", end - start);
}

void Section2() {
	int n = 7 * 1000000;
	int range = 4000;

	int *arr = (int *)(malloc(sizeof(int) * n));
	fillSeqData(arr, n, range);

	//printData(arr, n);

	double start = omp_get_wtime();

	int *categories = (int *)(malloc(sizeof(int) * n));
	for (int i = 0; i < n; i++) {
		categories[i] = categorize(arr[i]);
	}

	double end = omp_get_wtime();

	//printData(categories, n);

	printf("Section 2 Elapse Time: %lf\n", end - start);
}

void Section3() {
	int n = 7 * 10000000;
	int range = 4000;

	int *arr = (int *)(malloc(sizeof(int) * n));
	fillRandomData(arr, n, range);

	//printData(arr, n);

	double start = omp_get_wtime();

	int *categories = (int *)(malloc(sizeof(int) * n));
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		categories[i] = categorize(arr[i]);
	}

	double end = omp_get_wtime();

	//printData(categories, n);

	printf("Section 3 Elapse Time: %lf\n", end - start);
}

void Section4() {
	int n = 7 * 10000000;
	int range = 4000;

	int *arr = (int *)(malloc(sizeof(int) * n));
	fillSeqData(arr, n, range);

	//printData(arr, n);

	double start = omp_get_wtime();

	int *categories = (int *)(malloc(sizeof(int) * n));
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		categories[i] = categorize(arr[i]);
	}

	double end = omp_get_wtime();

	//printData(categories, n);

	printf("Section 4 Elapse Time: %lf\n", end - start);
}

void Section5() {
	int n = 7 * 10000000;
	int range = 4000;

	int *arr = (int *)(malloc(sizeof(int) * n));
	fillRandomData(arr, n, range);
	sortData(arr, n);

	//printData(arr, n);

	double start = omp_get_wtime();

	int *categories = (int *)(malloc(sizeof(int) * n));

	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		categories[i] = categorize(arr[i]);
	}

	double end = omp_get_wtime();

	//printData(categories, n);

	printf("Section 5 Elapse Time: %lf\n", end - start);
}

int main()
{
	/*
	Section1();
	Section2();
	Section3();
	Section4();
	
	*/
	Section1();

	system("pause");
}
