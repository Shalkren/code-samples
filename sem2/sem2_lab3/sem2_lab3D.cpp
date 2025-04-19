#include <iostream>
#include <vector>
#include <stack>
using namespace std;
int main(){
    int n, m;
    cin>>n>>m;
    vector<vector<int>> graph(n, vector<int>(m,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin>>graph[i][j];
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(i==0 && j==0) continue;
            if(i==0){
                graph[i][j]+=graph[i][j-1];
                continue;
            }
            if(j==0){
                graph[i][j]+=graph[i-1][j];
                continue;
            }
            graph[i][j]+=max(graph[i-1][j],graph[i][j-1]);
        }
    }
    cout<<graph[n-1][m-1]<<"\n";
    stack<char> s;
    while(m!=1 || n!=1){
        if(n==1){
            while(m>1){
                s.push('R');
                m--;
            }
            break;
        }
        if(m==1){
            while(n>1){
                s.push('D');
                n--;
            }
            break;
        }
        if(graph[n-1][m-2]>=graph[n-2][m-1]){ 
            s.push('R');
            m--;
        }
        else{
            s.push('D');
            n--;
        } 
    }
    while(!s.empty()){
        cout<<s.top()<<" ";
        s.pop();
    }
    return 0;
}