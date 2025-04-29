#include<iostream>
#include<stdlib.h>
#include<omp.h>

using namespace std;

void merge(int a[], int i1, int j1, int i2, int j2)
{
    int i = i1;
    int j = i2;
    int* temp = new int[j2 - i1 + 1];
    int k = 0;
    
    while (i <= j1 && j <= j2)
    {
        if (a[i] < a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }
    
    while (i <= j1)
        temp[k++] = a[i++];
    
    while (j <= j2)
        temp[k++] = a[j++];
    
    for (i = i1, k = 0; i <= j2; i++, k++)
        a[i] = temp[k];
    
    delete[] temp;
}

void mergeSort_seq(int a[], int i, int j)
{
    if (i < j)
    {
        int mid = (i + j) / 2;
        mergeSort_seq(a, i, mid);
        mergeSort_seq(a, mid + 1, j);
        merge(a, i, mid, mid + 1, j);
    }
}

void mergeSort(int a[], int i, int j)
{
    if (i < j)
    {
        if ((j - i) < 1000) // cutoff for small arrays
        {
            mergeSort_seq(a, i, j);
            return;
        }

        int mid = (i + j) / 2;
        
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(a, i, mid);
            
            #pragma omp section
            mergeSort(a, mid + 1, j);
        }
        
        merge(a, i, mid, mid + 1, j);
    }
}

int main()
{
    int *a, *b, n = 100000;
    
    a = new int[n];
    b = new int[n];
    
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 100000;
        b[i] = a[i];
    }
    
    double start1 = omp_get_wtime();
    mergeSort_seq(b, 0, n - 1);
    double end1 = omp_get_wtime();
    
    double start = omp_get_wtime();
    mergeSort(a, 0, n - 1);
    double end = omp_get_wtime();
    
    cout << "Sorted array (Parallel Merge Sort):" << endl;
    for (int i = 0; i < 100; i++)
    {
        cout << a[i] << "\t";
        if ((i + 1) % 10 == 0)
            cout << endl;
    }

    cout << endl;
    
    cout << "Time for Sequential Merge Sort: " << (end1 - start1) << " seconds" << endl;
    cout << "Time for Parallel Merge Sort: " << (end - start) << " seconds" << endl;
    
    delete[] a;
    delete[] b;

    return 0;
}
