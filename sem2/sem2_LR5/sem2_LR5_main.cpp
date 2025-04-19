#include "sem2_LR5_func.hpp"
int main(){
    int size1;
    while(true){
        cout<<"input size of vector M\n";
        cin>>size1;
        if(size1==0){
            break;
        }
        if(size1<0){
            cout<<"try again (-_-)\n";
            continue;
        }
        
        double * p1=new double[size1]; 
        cout<<"input vector M\n";
        for(int i=0;i<size1;i++){
            cin>>p1[i];
        }

        CVector2 M2(size1,p1);
        CVector&rvM2=M2;
        CVector*pvM2=&M2;
        M2.print();
        rvM2.print();
        pvM2->print();

        CVector3 M3(size1,p1);
        CVector&rvM3=M3;
        CVector*pvM3=&M3;
        M3.vprint();
        rvM3.vprint();
        pvM3->vprint();
        
       
        delete[] p1;
    }
    cout<<"the end";

    return 0;
}