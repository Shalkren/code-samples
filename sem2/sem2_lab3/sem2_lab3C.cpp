#include <iostream>
#include <vector>
#include <set>
using namespace std;
int main(){
    int n,size=0;
    bool last=false;
    cin>>n;
    vector<int> coordinates(n,0);
    vector<int> length(n);
    set<int> repeats;
    for(int i=0;i<n;i++){
        cin>>coordinates[i];
        if(repeats.count(coordinates[i])){
            continue;
        }
        size++;
        repeats.insert(coordinates[i]);
        for(int j=size-1;j>0;j--){
            if(coordinates[j]<coordinates[j-1]){
                swap(coordinates[j],coordinates[j-1]);
            }
        }
    }
    length[0]=0;
    length[1]=coordinates[1]-coordinates[0];
    if(size>=3){
        length[2]=coordinates[2]-coordinates[0];
    }
    for(int i=3;i<size;i++){
        length[i]=min(length[i-1],length[i-2])+coordinates[i]-coordinates[i-1];
    }
    cout<<length[size-1];
    return 0;
}