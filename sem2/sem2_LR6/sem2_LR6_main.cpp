#include "sem2_LR6_func.hpp"
int main(){
    int size1;
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

        CVector M1(size1,p1);
        cout<<"M1 ";
        M1.print();
        delete[] p1;

        cout<<"vector M2: M2=M1(copy)\n";
        CVector M2;
        M2=M1;
        M2.print();

        cout<<"vector M3: M3=M1+M2(move)\n";
        CVector M3;
        M3=M1+M2;
        M3.print();


        cout<<"vector M4 (constructor)\n";
        CVector M4(move(M3));
        M4.print();
        cout<<"M3 check\n";
        M3.print();
    }
    cout<<"the end";

    return 0;
}
