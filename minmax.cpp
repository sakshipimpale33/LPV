#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// ---------------- SEQUENTIAL FUNCTIONS ----------------

int seqMin(int arr[], int n) {
    int minval = arr[0];

    for(int i = 1; i < n; i++) {
        if(arr[i] < minval)
            minval = arr[i];
    }

    return minval;
}

int seqMax(int arr[], int n) {
    int maxval = arr[0];

    for(int i = 1; i < n; i++) {
        if(arr[i] > maxval)
            maxval = arr[i];
    }

    return maxval;
}

long long seqSum(int arr[], int n) {
    long long sum = 0;

    for(int i = 0; i < n; i++) {
        sum += arr[i];
    }

    return sum;
}

double seqAverage(int arr[], int n) {
    return (double)seqSum(arr, n) / n;
}

// ---------------- PARALLEL FUNCTIONS ----------------

int parMin(int arr[], int n) {
    int minval = arr[0];

    #pragma omp parallel for reduction(min:minval)
    for(int i = 0; i < n; i++) {
        if(arr[i] < minval)
            minval = arr[i];
    }

    return minval;
}

int parMax(int arr[], int n) {
    int maxval = arr[0];

    // FIXED: reduction should be max
    #pragma omp parallel for reduction(max:maxval)
    for(int i = 0; i < n; i++) {
        if(arr[i] > maxval)
            maxval = arr[i];
    }

    return maxval;
}

long long parSum(int arr[], int n) {
    long long sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++) {
        sum += arr[i];
    }

    return sum;
}

double parAverage(int arr[], int n) {
    return (double)parSum(arr, n) / n;
}

// ---------------- MAIN ----------------

int main() {

    omp_set_num_threads(4);

    const int n = 1000000;

    int *arr = new int[n];

    srand(time(0));

    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    double start, end;

    // ---------------- SEQUENTIAL ----------------

    start = omp_get_wtime();

    int smin = seqMin(arr, n);
    int smax = seqMax(arr, n);
    long long ssum = seqSum(arr, n);
    double savg = seqAverage(arr, n);

    end = omp_get_wtime();

    double seqTime = end - start;

    // ---------------- PARALLEL ----------------

    start = omp_get_wtime();

    int pmin = parMin(arr, n);
    int pmax = parMax(arr, n);
    long long psum = parSum(arr, n);    
    double pavg = parAverage(arr, n);

    end = omp_get_wtime();

    double parTime = end - start;

    // ---------------- OUTPUT ----------------

    cout << "===== Sequential Results =====" << endl;
    cout << "Minimum Value : " << smin << endl;
    cout << "Maximum Value : " << smax << endl;
    cout << "Sum           : " << ssum << endl;
    cout << "Average       : " << savg << endl;
    cout << "Execution Time: " << seqTime << " sec\n" << endl;

    cout << "===== Parallel Results =====" << endl;
    cout << "Minimum Value : " << pmin << endl;
    cout << "Maximum Value : " << pmax << endl;
    cout << "Sum           : " << psum << endl;
    cout << "Average       : " << pavg << endl;
    cout << "Execution Time: " << parTime << " sec\n" << endl;

    delete[] arr;

    return 0;
}