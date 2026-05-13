#include<iostream>
#include<omp.h>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<algorithm>
using namespace std;
#define SIZE 10000



void bubbleSortSeq(vector<int>&arr){

    for(int i=0;i<arr.size()-1;i++){
        for(int j=0;j<arr.size()-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }

}

void ParalleBubble(vector<int>&arr){
    int n=arr.size();
    for(int i=0;i<n;i++){

        #pragma omp paralle for
        for(int j=0;j<n-1;j+=2){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }

        #pragma omp paralle for
        for(int j=1;j<n-1;j+=2){
            if(arr[j]>arr[j+1]){
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


void merge(vector<int>&arr,int l,int r,int m ){

    vector<int>temp;
    int i=l;
    int j=m+1;
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
void mergeSortSeq(vector<int>&arr,int l,int r){


    if(l<r){
        int m=(l+r)/2;
        mergeSortSeq(arr,l,m);
        mergeSortSeq(arr,m+1,r);
        merge(arr,l,r,m);
    }

}

void parallelMerge(vector<int>& arr, int l, int r, int depth) {

    if (l < r) {

        int m = (l + r) / 2;

        if (depth <= 0) {

            mergeSortSeq(arr, l, m);
            mergeSortSeq(arr, m + 1, r);

        } else {

            #pragma omp task shared(arr)
            parallelMerge(arr, l, m, depth - 1);

            #pragma omp task shared(arr)
            parallelMerge(arr, m + 1, r, depth - 1);

            #pragma omp taskwait
        }

        merge(arr, l, r, m);
    }
}
void generateRandom(vector<int>&arr){
    for(int &x:arr){
        x=rand()%100000;
    }
}
int main(){
    vector<int>arr(SIZE),temp;
    srand(time(0));
      generateRandom(arr);

      double start,end;


        temp=arr;
      start = omp_get_wtime();
      bubbleSortSeq(temp);
      end = omp_get_wtime();
    cout<<"Sequential bubble sort"<<endl;
    cout<<"Time: "<<(end-start)<<" sec"<<endl;
      printArray(temp);
      cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end())<<endl;




      temp=arr;
      start = omp_get_wtime();
      ParalleBubble(temp);
      end = omp_get_wtime();
      cout<<"p b sort"<<endl;
      cout<<"Time: "<<(end-start)<<" sec"<<endl;
      printArray(temp);
      cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end())<<endl;


    
      temp=arr;
      start = omp_get_wtime();
      mergeSortSeq(temp,0,SIZE-1);
      end = omp_get_wtime();
        cout<<"Sequential merge sort"<<endl;
    cout<<"Time: "<<(end-start)<<" sec"<<endl;
      printArray(temp);
      cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end())<<endl;


      temp=arr;
      start = omp_get_wtime();

    cout<<"p m sort"<<endl;
      #pragma omp parallel 
      {

        #pragma omp single 
        parallelMerge(temp,0,SIZE-1,4);
      
      }

      end = omp_get_wtime();

      printArray(temp);
      cout<<"Time: "<<(end-start)<<" sec"<<endl;
      cout<<"Sorted?"<<is_sorted(temp.begin(),temp.end())<<endl;



}