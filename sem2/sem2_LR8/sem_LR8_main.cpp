#include "sem2_LR8_func.hpp"
int main(){
    int size1,size2;
    while(true){
        cout<<"input size of vector M1\n";
        cin>>size1;
        if(size1==0){
            break;
        }
        if(size1<0){
            cout<<"try again (-_-)\n";
            continue;
        }
        
        double * p1=new double[size1]; 
        cout<<"input vector M1\n";
        for(int i=0;i<size1;i++){
            cin>>p1[i];
        }
        TVector<double> V(size1,p1);
        for(auto it=V.begin();it!=V.end();++it){
            cout<<*it<<" ";
        }
        for(auto it: V){
            cout<<it<<" ";
        }
        cout<<endl;
        delete[] p1;
    }
    cout<<"the end";

    return 0;
}