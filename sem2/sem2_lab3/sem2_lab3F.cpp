#include <vector>
#include <iostream>
#include <string>
#include <climits>
using namespace std;
void balanceU(int ind,vector<int>& a) {
    while (ind > 0) {
        int parent = (ind - 1) / 2;
        if (a[parent] >= a[ind])
            break;
        swap(a[parent], a[ind]);
        ind = parent;
    }
}
void balanceD(int ind, vector<int>& a, int &cInd) {
    int largest = ind;
    int left = 2*ind + 1;
    int right = 2*ind + 2;

    if (left <= cInd && a[left] > a[largest])
        largest = left;
    if (right <= cInd && a[right] > a[largest])
        largest = right;

    if (largest != ind) {
        swap(a[ind], a[largest]);
        balanceD(largest, a, cInd);
    }
}
void insert(int k, vector<int>& a,int & cInd){
    a[cInd+1]=k;
    cInd++;
    balanceU(cInd,a);
}
void extract(vector<int>& a,int & cInd){
    cout<<a[0]<<endl;
    swap(a[0],a[cInd]);
    cInd--;
    if (cInd >= 0) balanceD(0, a, cInd);
}
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,com, cInd=-1;
    cin>>n;
    vector<int> heap(n+1,INT_MIN);
    for(int i=0;i<n;i++){
        cin>>com;
        if(com==0){
            cin>>com;
            insert(com,heap,cInd);
            continue;
        }
        if(com==1){
            extract(heap,cInd);
            continue;
        }
    }
    return 0;
}