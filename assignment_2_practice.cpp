#include<iostream>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<algorithm>
#include<omp.h>

#define SIZE 10000
using namespace std;


void seqBubble(vector<int>&arr){
    int  n=arr.size();
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j] > arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void parallelBubble(vector<int>&arr){
    int n=arr.size();
    for(int i=0;i<n;i++){
        #pragma omp parallel for 
        for(int j=0;j<n-1;j+=2){
            if(arr[j] > arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }

        #pragma omp parallel for 
        for(int j=1;j<n-1;j+=2){
            if(arr[j] > arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
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
void merge(vector<int>&arr,int l,int r,int m){
    vector<int>temp;
    int i=l,j=m+1;

    while(i<=m && j<=r){
        if(arr[i]<=arr[j]){
            temp.push_back(arr[i++]);
        }
        else {
            temp.push_back(arr[j++]);
        }
    }

    while(i<=m) temp.push_back(arr[i++]);
    while(j<=r) temp.push_back(arr[j++]);

    for(int k=0;k<temp.size();k++){
        arr[l+k]=temp[k];
    }
}

void seqMerge(vector<int>&arr,int l,int r){
    if(l<r){
        int m=(l+r)/2;
        seqMerge(arr,l,m);
        seqMerge(arr,m+1,r);
        merge(arr,l,r,m);
    }

}

void parallelMerge (vector<int>&arr,int l,int r,int depth){

    if(l<r){
        int m=(l+r)/2;
        if(depth<=0){
            seqMerge(arr,l,r);
        }else{
            #pragma omp task shared(arr)
            parallelMerge(arr,l,m,depth-1);
            #pragma omp task shared(arr)
            parallelMerge(arr,m+1,r,depth-1);

            #pragma omp taskwait
        }

        merge(arr,l,r,m);
    }

}
void generateRandom(vector<int>&arr){
    for(int &x:arr){
        x=rand()%100000;
    }
}

int main(){

    vector<int>arr(SIZE), temp;
    srand(time(0));
    generateRandom(arr);

    int start,end;


    temp=arr;
    start=omp_get_wtime();
    seqBubble(temp);
    end=omp_get_wtime();


    cout << "\nSequential Bubble Sort Time: " << (end - start) << " sec\n";
    printArray(temp); 
    cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end());


    temp=arr;
    start=omp_get_wtime();
    parallelBubble(temp);
    end=omp_get_wtime();

    cout << "\nParallel Bubble Sort Time: " << (end - start) << " sec\n";
    printArray(temp); 
    cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end());

    temp=arr;
    start=omp_get_wtime();
    seqMerge(temp,0,SIZE-1);
    end=omp_get_wtime();

    cout << "\nSequential Merge Sort Time: " << (end - start) << " sec\n";
    printArray(temp); 
    cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end());


    temp=arr;
    start=omp_get_wtime();


    #pragma omp parallel
    {
        #pragma omp  single 
        {
            parallelMerge(temp,0,SIZE-1,4);
        }
    }
    
    end=omp_get_wtime();

    cout << "\nParallel Merge Sort Time: " << (end - start) << " sec\n";
    printArray(temp); 
    cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end());



    
}