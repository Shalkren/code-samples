#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <queue>
using namespace std;
int main(){
    int n;
    string str1,str2,head;
    cin>>n;
    map<string,int> resdict;
    map<string, int> indexdict;
    map<int, string> reindexdict;
    map<string, string> data;
    for(int i=0; i<n-1; i++){
        cin>>str1>>str2;
        indexdict[str1]=0;
        indexdict[str2]=0;
        resdict[str1]=0;
        data[str1]=str2;
    }

    vector<vector<int>> graph(indexdict.size(), vector<int>(indexdict.size(),0));
    n=indexdict.size();

    int ind=0;
    for(auto it=indexdict.begin(); it!=indexdict.end(); it++){
        if(!data.count(it->first)){
            head=it->first;
        }
        it->second=ind;
        reindexdict[ind]=it->first;
        ind++;
    }

    for(auto it=data.begin(); it!=data.end(); it++){
        graph[indexdict[it->second]][indexdict[it->first]]=1;
    }

    queue<int> q;
    q.push(indexdict[head]);
    resdict[head]=0;
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        for (int i=0;i<n;i++) {
            if(graph[node][i]!=0){
                resdict[reindexdict[i]]=resdict[reindexdict[node]]+1;
                q.push(i);
            }
        }
    }
    
    for(auto& x: resdict){
        cout<<x.first<<" "<<x.second<<endl;
    }
    return 0;
}