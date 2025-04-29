#include<iostream>
#include<stdlib.h>
#include<omp.h>
#include<ctime>
using namespace std;

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void bubble(int *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        int first = i % 2;
        #pragma omp parallel for
        for (int j = first; j < n - 1; j += 2)
        {
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

int main()
{
    int *a, *b, n = 50000;
    
    a = new int[n];
    b = new int[n];

    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 10000;
        b[i] = a[i];
    }

    double start = omp_get_wtime();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (b[j] > b[j + 1])
            {
                swap(b[j], b[j + 1]);
            }
        }
    }
    double end = omp_get_wtime();

    double start1 = omp_get_wtime();
    bubble(a, n);
    double end1 = omp_get_wtime();

 
    cout << "\nSorted Array (First 100 elements):\n";
    for (int i = 0; i < 100; i++)
    {
        cout << a[i] << "\t";
        if ((i + 1) % 10 == 0)
            cout << endl;
    }

    cout << "\nSequential time: " << (end - start) << " seconds" << endl;
    cout << "Parallel time: " << (end1 - start1) << " seconds" << endl;

    delete[] a;
    delete[] b;

    return 0;
}

