#include<iostream>
#include<omp.h>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

class Graph{
        int V;
        vector<vector<int>>adj;
public:
        Graph(int vertices){
            this->V=vertices;
            adj.resize(V);
        }
        void addEdge(int u, int v){
    adj[u].push_back(v);
    adj[v].push_back(u); // remove this line for directed graph
}
        void SequentialBFS(int start){
            vector<bool>visited (V,false);
            queue<int>q;

            visited[start]=true;
            q.push(start);

            cout<<"Sequential BFS"<<endl;
            while(!q.empty()){
                int node=q.front();
                q.pop();
                cout<<node<< " ";

                for(int neighbor:adj[node]){
                    if(!visited[neighbor]){
                        visited[neighbor]=true;
                        q.push(neighbor);
                    }
                }
            }
            cout<<endl;
        }

        void ParallelBFS(int start){

            vector<bool>visited (V,false);
            queue<int>q;

            visited[start]=true;
            q.push(start);

            cout<<"Sequential BFS"<<endl;
            while(!q.empty()){

                int node;
                #pragma omp critical
                {
                node=q.front();
                q.pop();
                cout<<node<< " ";
                }
                #pragma omp parallel for
                for(int i=0;i<adj[node].size();i++){

                    int ng=adj[node][i];
                    #pragma omp critical
                    {
                        if(!visited[ng]){
                        visited[ng]=true;
                        q.push(ng);
                        }

                    }
                    
                }

            }

            cout<<endl;

        }
        void SDFSutil(int node,vector<bool>&visited){
            visited[node]=true;
            cout<<node<< " ";

            for(int ng:adj[node]){
                if(!visited[ng]){
                    SDFSutil(ng,visited);
                }
            }
        }
        void SequentialDFS(int start){
            vector<bool>visited(V,false);
            cout<<"Sequential DFS"<<endl;

            SDFSutil(start,visited);
            cout<<endl;
        }


        void PDFSutil(int node,vector<bool>&visited){
            bool av=false;
            #pragma omp critical
            {
                if(visited[node])
                   av = true;
                else{
                    visited[node]=true;
                cout<<node<< " ";
                }
            }

            if(av) {
                return;
            }
            
            for(int ng:adj[node]){
                #pragma omp task 
                {
                    if(!visited[ng]){
                    SDFSutil(ng,visited);
                    }
                }
            }
        }
        void ParallelDFS(int start){
            vector<bool>visited(V,false);
            cout<<"Parallel DFS"<<endl;
            #pragma omp parallel
            {
                #pragma omp single
                {
                    PDFSutil(start,visited);
                }
            }
            
            cout<<endl;
        }

};

int main(){


    int V,E;
    cout<<"Enter vertice"<<endl;
    cin>>V;

    Graph g(V+1);

    cout<<"enter edges"<<endl;
    cin>>E;

    cout<<"enter edges(u,v)"<<endl;
    for(int i=0;i<E;i++){
        int u,v;
        cin>>u>>v;
        g.addEdge(u,v);
    }

    cout<<endl;

    int start;
    cout<<"Enter Start"<<endl;
    cin>>start;


    int start_time,end_time ;

    start_time=omp_get_wtime();
    g.SequentialBFS(start);
    end_time=omp_get_wtime();


    cout << "Sequential BFS Execution Time: "
         << end_time - start_time << " seconds\n";

    start_time=omp_get_wtime();
    g.ParallelBFS(start);
    end_time=omp_get_wtime();


    cout << "Parallel BFS Execution Time: "
         << end_time - start_time << " seconds\n";


        start_time = omp_get_wtime();

    g.SequentialDFS(start);

    end_time = omp_get_wtime();

    cout << "Sequential DFS Execution Time: "
         << end_time - start_time << " seconds\n";


    start_time = omp_get_wtime();

    g.ParallelDFS(start);

    end_time = omp_get_wtime();

    cout << "Parallel DFS Execution Time: "
         << end_time - start_time << " seconds\n";
    
    return 0;
}

