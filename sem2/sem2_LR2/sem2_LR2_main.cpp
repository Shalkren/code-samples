#include "sem2_LR2.hpp"
int main(){
    int n;
    while(true){
        cout<<"input size of vector\n";
        cin>>n;
        if(n==0){
            break;
        }
        if(n<0){
            cout<<"try again (-_-)\n";
            continue;
        }
        double * p=new double[n];
        CVector M;
        cout<<"M"<<endl;
        M.print();
        cout<<"input vector\n";
        for(int i=0;i<n;i++){
            cin>>p[i];
        }
        CVector M1(n,p);
        cout<<"M1"<<endl;
        M1.print();
        cout<<"M - copy of M1"<<endl;
        cout<<M.copy(M1)<<endl;
        M.print();
        delete[] p;
    }
    cout<<"the end";

    return 0;
}
