#include <iostream>
#include <omp.h>
void bubble(int *, int);
void swap(int &, int &);
void bubble(int *a, int n)
{
    bool swapped;
    for (int i = 0; i < n - 1; i++)
    {
        swapped = false;
#pragma omp parallel for shared(a, n, swapped)
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
#pragma omp critical
                {
                    swap(a[j], a[j + 1]);
                    swapped = true;
                }
            }
        }
        if (!swapped)
            break; // Array is already sorted
    }
}
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
int main()
{
    int *a, n;
    std::cout << "Enter total number of elements: ";
    std::cin >> n;
    a = new int[n];
    std::cout << "Enter elements: ";
    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
    }
    bubble(a, n);
    std::cout << "Sorted array: ";
    for (int i = 0; i < n; i++)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    delete[] a; // Deallocate memory
    return 0;
}
