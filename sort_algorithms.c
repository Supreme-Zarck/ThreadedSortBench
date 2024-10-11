#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Main function
 * This program compares different sorting algorithms (QuickSort, MergeSort, HeapSort, InsertionSort)
 * by measuring their execution time using threads and plotting the results with GNUplot.
*/


// Function declarations

int* generateRandomArray(int size);
void swap(int* a, int* b);
int partition(int arr[], int low, int high);

void quickSort(int arr[], int low, int high);

void merge(int arr[], int left, int mid, int right);
void mergeSort(int arr[], int left, int right);

void insertionSort(int arr[], int n);
void BubbleSort(int arr[], int n);
void selectionSort(int arr[], int n);


void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);

void* mergeSortThread(void* args);
void* heapSortThread(void* args);
void* quickSortThread(void* args);
void* insertionSortThread(void* args);
void* BubblesortThread(void* args);
void* selectionSortThread(void* args);


// Struct to hold arguments for threading.
struct SortArgs {
    int* arr;
    int size;
};

clock_t start, end, total_start, total_end;
double cpu_time_used;

int main() {
    
    total_start = clock();

    FILE *file = fopen("execution_times.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    for (int n = 10; n <= 100000; n += n / 1.5) {
        int* arr = generateRandomArray(n);

        
        int* arrQuick = (int*)malloc(n * sizeof(int));
        int* arrMerge = (int*)malloc(n * sizeof(int));
        int* arrHeap = (int*)malloc(n * sizeof(int));
        int* arrInsert = (int*)malloc(n * sizeof(int));
        int* arrBubble = (int*)malloc(n * sizeof(int));
        int* arrSelection = (int*)malloc(n * sizeof(int));
        
        if (arrQuick == NULL || arrMerge == NULL || arrHeap == NULL || arrInsert == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour les copies du tableau.\n");
            free(arr);
            return 1;
        }

        memcpy(arrQuick, arr, n * sizeof(int));
        memcpy(arrMerge, arr, n * sizeof(int));
        memcpy(arrHeap, arr, n * sizeof(int));
        memcpy(arrInsert, arr, n * sizeof(int));
        memcpy(arrBubble, arr, n * sizeof(int));
        memcpy(arrSelection, arr, n * sizeof(int));


        struct SortArgs argsQuick = {arrQuick, n};
        struct SortArgs argsMerge = {arrMerge, n};
        struct SortArgs argsHeap = {arrHeap, n};
        struct SortArgs argsInsert = {arrInsert, n};
        struct SortArgs argsBubble = {arrBubble, n};
        struct SortArgs argsSelection = {arrSelection, n};


        pthread_t thread1, thread2, thread3, thread4,thread5, thread6;

        
        start = clock();
        pthread_create(&thread1, NULL, quickSortThread, &argsQuick);
        pthread_join(thread1, NULL);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(file, "%d %f ", n, cpu_time_used);

        
        start = clock();
        pthread_create(&thread2, NULL, mergeSortThread, &argsMerge);
        pthread_join(thread2, NULL);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(file, "%f ", cpu_time_used);

        
        start = clock();
        pthread_create(&thread3, NULL, heapSortThread, &argsHeap);
        pthread_join(thread3, NULL);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(file, "%f ", cpu_time_used);

        
        start = clock();
        pthread_create(&thread4, NULL, insertionSortThread, &argsInsert);
        pthread_join(thread4, NULL);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(file, "%f ", cpu_time_used);

        start = clock();
        pthread_create(&thread5, NULL, BubblesortThread, &argsBubble);
        pthread_join(thread5, NULL);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(file, "%f ", cpu_time_used);

        start = clock();
        pthread_create(&thread6, NULL, selectionSortThread, &argsInsert);
        pthread_join(thread6, NULL);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(file, "%f\n", cpu_time_used);


        
        free(arr);
        free(arrQuick);
        free(arrMerge);
        free(arrHeap);
        free(arrInsert);
        free(arrBubble);
        free(arrSelection);
    }

    fclose(file);

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture de GNUplot.\n");
        return 1;
    }

    fprintf(gnuplotPipe, "set title 'Temps d execution des algorithmes de tri'\n");
    fprintf(gnuplotPipe, "set xlabel 'Taille du tableau'\n");
    fprintf(gnuplotPipe, "set ylabel 'Temps (secondes)'\n");
    fprintf(gnuplotPipe, "set logscale xy\n");
    fprintf(gnuplotPipe, "plot 'execution_times.txt' using 1:2 with lines title 'QuickSort', \\\n");
    fprintf(gnuplotPipe, "     '' using 1:3 with lines title 'MergeSort', \\\n");
    fprintf(gnuplotPipe, "     '' using 1:4 with lines title 'HeapSort', \\\n");
    fprintf(gnuplotPipe, "     '' using 1:5 with lines title 'InsertionSort', \\\n");
    fprintf(gnuplotPipe, "     '' using 1:6 with lines title 'BubbleSort', \\\n");
    fprintf(gnuplotPipe, "     '' using 1:7 with lines title 'SelectionSort'\n");


    
    pclose(gnuplotPipe);

    total_end = clock();
    double total_time_used = ((double)(total_end - total_start)) / CLOCKS_PER_SEC;
    printf("Temps total d'exécution : %f secondes\n", total_time_used);

    return 0;
}




int* generateRandomArray(int size) {
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le tableau.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000; 
    }
    return arr;
}


void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


int partition(int arr[], int low, int high) {
    int pivot = arr[high]; 
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    if (L == NULL || R == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les tableaux temporaires.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}


void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void BubbleSort(int arr[],int size){
    for(int i=size-1;i>0;i--){
        for (int j=0;j<i;j++){
            if (arr[j]>arr[j+1]) swap(&arr[j],&arr[j+1]);
        }
    }
}

void selectionSort(int arr[],int n){
    for(int i=0 ; i<n-1 ; i++){
        int min = i;
        for (int j=i ; j<n ; j++){
            if (arr[j+1] < arr[j]) min = j;
        }
        swap(&arr[i], &arr[min]);
    }
}

void* quickSortThread(void* arg) {
    struct SortArgs* args = (struct SortArgs*)arg;
    quickSort(args->arr, 0, args->size - 1);
    pthread_exit(NULL);
}

void* mergeSortThread(void* arg) {
    struct SortArgs* args = (struct SortArgs*)arg;
    mergeSort(args->arr, 0, args->size - 1);
    pthread_exit(NULL);
}

void* heapSortThread(void* arg) {
    struct SortArgs* args = (struct SortArgs*)arg;
    heapSort(args->arr, args->size);
    pthread_exit(NULL);
}

void* insertionSortThread(void* arg) {
    struct SortArgs* args = (struct SortArgs*)arg;
    insertionSort(args->arr, args->size);
    pthread_exit(NULL);
}

void* BubblesortThread(void* arg) {
    struct SortArgs* args = (struct SortArgs*)arg;
    BubbleSort(args->arr, args->size);
    pthread_exit(NULL);
}

void* selectionSortThread(void* arg) {
    struct SortArgs* args = (struct SortArgs*)arg;
    selectionSort(args->arr, args->size);
    pthread_exit(NULL);
}


void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

