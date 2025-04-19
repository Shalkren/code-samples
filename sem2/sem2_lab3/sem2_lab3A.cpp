#include <iostream>
#include <cmath>
#include <vector>
using namespace std;
vector<int> tribo={0,0,1,1,2,4,7,13}; 
void trib(int n){
    if(n<tribo.size()){
        return;
    }else{
        for(int i=tribo.size();i<=n;i++){
            tribo.push_back(tribo[i-1]+tribo[i-2]+tribo[i-3]);
        }
    }
}

int main() {
	int n;
	cin>>n;
	trib(n+3);
    cout<<tribo[n+3];
    return 0;
}