#include "sem2_LR4_func.hpp"
int main(){
    int size1,size2,n;
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
        cout<<"M1"<<endl;
        M1.print();
        cout<<"M1 mod "<<M1()<<endl;
        
        cout<<"input size of vector M2\n";
        cin>>size2;
        double * p2=new double[size2];
        cout<<"input vector M2\n";
        for(int i=0;i<size2;i++){
            cin>>p2[i];
        }
        CVector M2(size2,p2);
        cout<<"M2"<<endl;
        M2.print();
        cout<<"M2 mod "<<M2()<<endl;

        cout<<"scale_prod M1,M2 "<<M1*M2<<endl;

        CVector Msum=M1+M2,Mdet=M1-M2;
        cout<<"Msum"<<endl;
        Msum.print();
        cout<<"Mdet"<<endl;
        Mdet.print();

        cout<<"M1+M2"<<endl;
        M1+=M2;
        M1.print();

        cout<<"M1+M2-M2"<<endl;
        M1-=M2;
        M1.print();

        cout<<"check r-value M1[0] "<<M1[0]<<endl;
        M1[0]=999;
        cout<<"check l-value M1 ";
        M1.print();
        delete[] p1;
        delete[] p2;
    }
    cout<<"the end";

    return 0;
}