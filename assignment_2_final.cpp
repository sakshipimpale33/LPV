#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <algorithm>

using namespace std;

#define SIZE 10000


void printArray(const vector<int>& arr) {
    int n = arr.size();

    cout << "First 10: ";
    for (int i = 0; i < 10; i++)
        cout << arr[i] << " ";

    cout << "\nLast 10: ";
    for (int i = n - 10; i < n; i++)
        cout << arr[i] << " ";

    cout << "\n";
}

void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}


void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}


// void merge(vector<int>& arr, int l, int m, int r) {
//     vector<int> left(arr.begin() + l, arr.begin() + m + 1);
//     vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

//     int i = 0, j = 0, k = l;

//     while (i < left.size() && j < right.size()) {
//         if (left[i] <= right[j])
//             arr[k++] = left[i++];
//         else
//             arr[k++] = right[j++];
//     }

//     while (i < left.size()) arr[k++] = left[i++];
//     while (j < right.size()) arr[k++] = right[j++];
// }



void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }

    while (i <= m) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);

    // Copy back to original array
    for (int k = 0; k < temp.size(); k++) {
        arr[l + k] = temp[k];
    }
}


void mergeSortSeq(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSortSeq(arr, l, m);
        mergeSortSeq(arr, m + 1, r);
        merge(arr, l, m, r);
    }   
}

void mergeSortParallel(vector<int>& arr, int l, int r, int depth) {
    if (l < r) {
        int m = (l + r) / 2;

        if (depth <= 0) {
            mergeSortSeq(arr, l, r);
        } else {
            #pragma omp task shared(arr)
            mergeSortParallel(arr, l, m, depth - 1);

            #pragma omp task shared(arr)
            mergeSortParallel(arr, m + 1, r, depth - 1);

            #pragma omp taskwait
        }

        merge(arr, l, m, r);
    }
}

void printArray (vector<int>&arr){
    int n=arr.size();
    cout<<"First 10"<<endl;
    for(int i=0;i<10;i++){
        cout<<arr[i]<< " ";
    }
    cout<<"Last 10"<<endl;

    for(int i=n-10;i<n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;

}
void generateRandom(vector<int>& arr) {
    for (int& x : arr)
        x = rand() % 100000;
}

int main() {
    vector<int> arr(SIZE), temp;

    srand(time(0));
    generateRandom(arr);

    double start, end;

    // -------- Sequential Bubble --------
    temp = arr;
    start = omp_get_wtime();
    bubbleSortSeq(temp);
    end = omp_get_wtime();

    cout << "\nSequential Bubble Sort Time: " << (end - start) << " sec\n";
    printArray(temp);
    cout << "Sorted? " << is_sorted(temp.begin(), temp.end()) << "\n";

    // -------- Parallel Bubble --------
    temp = arr;
    start = omp_get_wtime();
    bubbleSortParallel(temp);
    end = omp_get_wtime();

    cout << "\nParallel Bubble Sort Time: " << (end - start) << " sec\n";
    printArray(temp);
    cout << "Sorted? " << is_sorted(temp.begin(), temp.end()) << "\n";

    // -------- Sequential Merge --------
    temp = arr;
    start = omp_get_wtime();
    mergeSortSeq(temp, 0, SIZE - 1);
    end = omp_get_wtime();

    cout << "\nSequential Merge Sort Time: " << (end - start) << " sec\n";
    printArray(temp);
    cout << "Sorted? " << is_sorted(temp.begin(), temp.end()) << "\n";

    // -------- Parallel Merge --------
    temp = arr;
    start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        mergeSortParallel(temp, 0, SIZE - 1, 4);
    }

    end = omp_get_wtime();

    cout << "\nParallel Merge Sort Time: " << (end - start) << " sec\n";
    printArray(temp);
    cout << "Sorted? " << is_sorted(temp.begin(), temp.end()) << "\n";

    return 0;
}

// openmp command= -fopenmp
//g++ your_file.cpp -fopenmp -o output
//./output

// Sakshi Pimpale@DESKTOP-T4TQLDO UCRT64 ~
// $ cd /c/Users/HP/Desktop/LPV

// Sakshi Pimpale@DESKTOP-T4TQLDO UCRT64 /c/Users/HP/Desktop/LPV
// $ g++ assignment_2_final.cpp -fopenmp -o output.exe

// Sakshi Pimpale@DESKTOP-T4TQLDO UCRT64 /c/Users/HP/Desktop/LPV
// $ ./output.exe~
// -bash: ./output.exe~: No such file or directory

// Sakshi Pimpale@DESKTOP-T4TQLDO UCRT64 /c/Users/HP/Desktop/LPV
// $ ./output.exe
// Sequential Bubble Sort Time: 0.622 sec
// Parallel Bubble Sort Time: 1.478 sec
// Sequential Merge Sort Time: 0.00699997 sec
// Parallel Merge Sort Time: 0.00500011 sec

// Sakshi Pimpale@DESKTOP-T4TQLDO UCRT64 /c/Users/HP/Desktop/LPV


//pacman -S mingw-w64-ucrt-x86_64-gcc

//g++ assignment_2_practice.cpp -fopenmp -o output.exe

///g++ assignment_2_practice.cpp -fopenmp -o output.exe