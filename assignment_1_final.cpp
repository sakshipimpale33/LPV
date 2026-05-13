#include<iostream>
#include<vector>
#include<queue>
#include<omp.h>

using namespace std;


class Graph{

    int V;
     vector<vector<int>>adj;

     public:
     Graph(int Vertices){
        this->V=Vertices;
        adj.resize(V);
    }

    void addEdge(int u, int v){
        adj[u].push_back(v);
        adj[v].push_back(u); 
    }


    void sequentialBFS(int start){
        vector<bool>visited(V,false);
        queue<int>q;
        visited[start]=true;
        q.push(start);
        while(!q.empty()){
            int node=q.front();
            q.pop();
            cout<<node<<" ";
            for(int neighbor:adj[node]){
                if(!visited[neighbor]){
                    visited[neighbor]=true;
                    q.push(neighbor);
                }
            }
        }

    }

    void ParallelBFS(int start){
        vector<bool>visited(V,false);
        queue<int>q;
        visited[start]=true;
        q.push(start);
        while(!q.empty())
        {
            int node;
            #pragma omp critical
            {
                node=q.front();
                q.pop();
                cout<<node<<" ";
            }
            #pragma omp parallel for
            for(int i=0;i<adj[node].size();i++){
                int neighbor=adj[node][i];

                #pragma omp critical
                {
                if(!visited[neighbor]){
                    visited[neighbor]=true;
                    q.push(neighbor);
                }
            }
        }
        }
    }

    void SDFSutil(int start,vector<int>&visited){
        visited[start]=true;
        cout<<start<<" ";

        for(int neighbor:adj[start]){
            if(!visited[neighbor]){
                SDFSutil(neighbor,visited);
            }
        }

    }

    void SDFS(int start){
        vector<int>visited(V,false);
        cout<<"Sequential DFS"<<endl;
        SDFSutil(start,visited);
        cout<<endl;
    }


    void PDFSutil(int start, vector<int>& visited) {
    bool alreadyVisited;

        #pragma omp critical
        {
            alreadyVisited = visited[start];

            if (!visited[start]) {
                visited[start] = true;
                cout << start << " ";
            }
        }

        if (alreadyVisited)
            return;

        #pragma omp parallel for
        for (int i = 0; i < adj[start].size(); i++) {

            int neighbor = adj[start][i];

            if (!visited[neighbor]) {

                #pragma omp task
                PDFSutil(neighbor, visited);
            }
        }
}

    void PDFS(int start){
        vector<int>visited(V,false);
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

    cout<<"Enter Starting node"<<endl;
    int startNode;
    cin>>startNode;


    cout<<"Sequential BFS: "<<endl;
    auto start = omp_get_wtime();
    g.sequentialBFS(startNode);
    auto end = omp_get_wtime();
    cout << "Execution Time: " << end - start << " seconds" << endl;


    cout<<"Parallel BFS: "<<endl;
    start = omp_get_wtime();
    g.ParallelBFS(startNode);
    end = omp_get_wtime();
    cout << "Execution Time: " << end - start << " seconds" << endl;


    cout<<"Sequential DFS: "<<endl;
    start = omp_get_wtime();
    g.SDFS(startNode);
    end = omp_get_wtime();
    cout << "Execution Time: " << end - start << " seconds" << endl;


    cout<<"Parallel DFS: "<<endl;
    start = omp_get_wtime();
    g.PDFS(startNode);
    end = omp_get_wtime();
    cout << "Execution Time: " << end - start << " seconds" << endl;


    return 0;


}
