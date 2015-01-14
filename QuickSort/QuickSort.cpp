#include<stdio.h>

void print(int a[], int n)//打印数组
{
	for (int i = 0; i<n; ++i)
		printf("%d ", a[i]);
	printf("\n");
}

void quickSort(int a[], int begin, int end)//快速排序.begin为数组下标开始号，end为数组某一个元素的标号
{
	if (begin<end)
	{
		int i = begin, j = end, base = a[begin];
		while (i<j)
		{
			while (i<j && a[j] >= base)
				--j;
			if (i<j)
				a[i++] = a[j];
			while (i<j && a[i] <= base)
				++i;
			if (i<j)
				a[j--] = a[i];
		}
		a[i] = base;
		quickSort(a, begin, i);
		quickSort(a, i + 1, end);
	}
}

int main()
{
	int n;
	int a[] = { 49, 38, 65, 97, 76, 13, 27, 49 };
	n = sizeof(a) / sizeof(a[0]);

	quickSort(a, 0, n - 1);
	print(a, n);

	return 0;
}