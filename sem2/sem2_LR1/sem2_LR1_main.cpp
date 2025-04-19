#include "sem2_LR1.hpp"
int main(){
    int n;
    while(true){
        cin>>n;
        if(n==0){
            break;
        }
        if(n<0){
            continue;
        }
        int *M1,*M2;
        M1=createMas(M1,n);
        M2=createMas(M2,n);
        cout<<"длина1\n";
        cout<<modMas(M1,n)<<endl;
        cout<<"длина2\n";
        cout<<modMas(M2,n)<<endl;
        cout<<"угол\n";
        cout<<eagle(M1,M2,n)<<endl;
        delete[] M1;
        delete[] M2; 
    }
    cout<<"the end";

    return 0;
}
