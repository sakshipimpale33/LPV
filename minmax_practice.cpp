#include<iostream>
#include<omp.h>
#include<cstdlib>
#include<ctime>
using namespace std;



int Seq_min(int arr[],int n){
    int minval=arr[0];
    for(int i=1;i<n;i++){
        if(arr[i]<minval){
            minval=arr[i];
        }
    }
    return minval;
}


int Seq_max(int arr[],int n){
    int val=arr[0];
    for(int i=1;i<n;i++){
        if(arr[i]<val){
            val=arr[i];
        }
    }
    return val;
}

int Seq_sum(int arr[],int n){
    int sum=0;
    for(int i=1;i<n;i++){
        sum+=arr[i];
    }
    return sum;
}

double Seq_average(int arr[],int n){
    return (double)Seq_sum(arr,n)/n;
}


int parMin(int arr[],int n){
    int minval=arr[0];
    #pragma omp parallel for reduction(min:minval)
    for(int i=1;i<n;i++){
        if(arr[i]<minval){
            minval=arr[i];
        }
    }
    return minval;
}
int parMax(int arr[],int n){
    int maxval=arr[0];
    #pragma omp parallel for reduction(max:maxval)
    for(int i=1;i<n;i++){
        if(arr[i]>maxval){
            maxval=arr[i];
        }
    }
    return maxval;
}

long long parSum(int arr[],int n){
    long long sum=0;
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<n;i++){
        sum+=arr[i];
    }
    return sum;
}

double parAverage(int arr[],int n){
    return (double)parSum(arr,n)/n;
}

int main(){
    int n=1000000;
    int *arr=new int[n];
    srand(time(0));

    for(int i=0;i<n;i++){
        arr[i]=rand()%10000;
    }

    cout<< "Sequential Operations"<<endl;
    double start,end;
    start=omp_get_wtime();
    int minval=Seq_min(arr,n);
    int maxval=Seq_max(arr,n);
    int sum=Seq_sum(arr,n);
    double average=Seq_average(arr,n);
    end=omp_get_wtime();

    cout<<"time taken: "<<end-start<<" seconds"<<endl;


    cout<<"Parallel Operations"<<endl;
    start=omp_get_wtime();
    int par_min=parMin(arr,n);
    int par_max=parMax(arr,n);
    long long par_sum=parSum(arr,n);
    double par_average=parAverage(arr,n);   
    end=omp_get_wtime();
    cout<<"Parallel Operations"<<endl;
    cout<<"time taken: "<<end-start<<" seconds"<<endl;
}


