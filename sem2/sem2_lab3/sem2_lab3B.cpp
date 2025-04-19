#include <iostream>
#include <vector>
using namespace std;
int min(int a, int b, int c){
    int res=min(a,b);
    res=min(res,c);
    return res;
}
int main(){
    int n;
    cin>>n;
    vector<vector<int>> info(n, vector<int>(3,0));
    vector<int> mincost(n,0);
    for(int i=0;i<n;i++){
        cin>>info[i][0]>>info[i][1]>>info[i][2];
    }
    if(n==1){
        mincost[0]=info[0][0];
        cout<<mincost[0];
        return 0;
    }
    if(n==2){
        mincost[0]=info[0][0];
        mincost[1]=min(mincost[0]+info[1][0],info[0][1]);
        cout<<mincost[1];
        return 0;
    }
    if(n>=3){
        mincost[0]=info[0][0];
        mincost[1]=min(mincost[0]+info[1][0],info[0][1]);
        mincost[2]=min(mincost[1]+info[2][0],mincost[0]+info[1][1],info[0][2]);
    }
    for(int i=3;i<n;i++){
        mincost[i]=min(mincost[i-1]+info[i][0],mincost[i-2]+info[i-1][1],mincost[i-3]+info[i-2][2]);
    }
    cout<<mincost[n-1];
    return 0;
}