#include <stdio.h>
#include <omp.h>

void merge(int array[], int low, int mid, int high)
{
    int temp[30];
    int i = low, j = mid + 1, k = low;

    while (i <= mid && j <= high)
    {
        if (array[i] <= array[j])
        {
            temp[k++] = array[i++];
        }
        else
        {
            temp[k++] = array[j++];
        }
    }

    while (i <= mid)
    {
        temp[k++] = array[i++];
    }

    while (j <= high)
    {
        temp[k++] = array[j++];
    }

    for (i = low; i <= high; i++)
    {
        array[i] = temp[i];
    }
}

void mergesort(int array[], int low, int high)
{
    int mid;

    if (low < high)
    {
        mid = (low + high) / 2;
#pragma omp parallel sections num_threads(2)
        {
#pragma omp section
            {
                mergesort(array, low, mid);
            }
#pragma omp section
            {
                mergesort(array, mid + 1, high);
            }
        }
        merge(array, low, mid, high);
    }
}

int main()
{
    int array[50];
    int i, size;
    printf("Enter total no. of elements:\n");
    scanf("%d", &size);
    printf("Enter %d elements:\n", size);
    for (i = 0; i < size; i++)
    {
        scanf("%d", &array[i]);
    }
    mergesort(array, 0, size - 1);
    printf("Sorted Elements as follows:\n");
    for (i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}