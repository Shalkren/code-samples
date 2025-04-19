#include "sem2_LR3_func.hpp"
using namespace std;
void CVector :: init(){
    size=0;
    buffer=nullptr;
}
void CVector :: clear(){
    if(buffer){
        delete[] buffer;
        init();
    }
}
CVector :: CVector(int s=0, double* p=nullptr){
    init();
    copy(s,p);
}
CVector :: CVector(const CVector &A){
    init();
    copy(A);
}
CVector :: ~CVector(){
    clear();
}
int CVector :: copy(int s, double* p){
    if(buffer!=p){
        clear();
        buffer= new double[s];
        if(buffer){
            size=s;
            for(int i=0;i<size;i++){
                buffer[i]=p[i];
            }
        }
    }
    return s;
}

void CVector :: print(){ 
    cout<<"Num "<<size<<" "<<"Data ";
    for(int i=0;i<size;i++){
        cout<<buffer[i]<<" ";
    }
        cout<<endl;
        return;
}
//LR3


CVector sum(const CVector &A, const CVector &B){
    int newsize=max(A.size,B.size);
    int smallsize=min(A.size,B.size);
    double*P=new double[newsize];
    for(int i=0; i<smallsize;i++){
        P[i]=A.buffer[i]+B.buffer[i];
    }
    for(int i=smallsize; i<B.size;i++){
        P[i]=B.buffer[i];
    }
    for(int i=smallsize; i<A.size;i++){
        P[i]=A.buffer[i];
    }
    CVector result;
    result.size=newsize;
    result.buffer=P;
    return result;
}

CVector det(const CVector &A, const CVector &B){
    int newsize=max(A.size,B.size);
    int smallsize=min(A.size,B.size);
    double*P=new double[newsize];
    for(int i=0; i<smallsize;i++){
        P[i]=A.buffer[i]-B.buffer[i];
    }
    for(int i=smallsize; i<B.size;i++){
        P[i]=-B.buffer[i];
    }
    for(int i=smallsize; i<A.size;i++){
        P[i]=A.buffer[i];
    }
    CVector result;
    result.size=newsize;
    result.buffer=P;
    return result;
}

void CVector :: add(const CVector &B){
    copy(sum(*this,B));
}
void CVector :: minus(const CVector &B){
    copy(det(*this,B));
}

double scale_prod(const CVector &A, const CVector &B){
    int result=0;
    int count=min(A.size,B.size);
    for(int i=0;i<count;i++){
        result+=A.buffer[i]*B.buffer[i];
    }    
    return result;
}

double CVector:: moduleV(){
    double result =sqrt(scale_prod(*this,*this));
    return result;
}




/*
CVector sum(const CVector &A, const CVector &B){
    int newsize,smallsize;
    double*P;
    if(A.size>B.size){
        newsize=A.size;
        smallsize=B.size;
        P= new double[newsize];
        for(int i=smallsize; i<newsize;i++){
            P[i]=A.buffer[i];
        }
        for(int i=0; i<smallsize;i++){
            P[i]=A.buffer[i]+B.buffer[i];
        }
    }else{
        newsize=B.size;
        smallsize=A.size;
        P= new double[newsize];
        for(int i=smallsize; i<newsize;i++){
            P[i]=B.buffer[i];
        }
        for(int i=0; i<smallsize;i++){
            P[i]=A.buffer[i]+B.buffer[i];
        }
    }
    CVector result(newsize,P);
    return result;
}

CVector det(const CVector &A, const CVector &B){
    int newsize,smallsize;
    double*P;
    if(A.size>B.size){
        newsize=A.size;
        smallsize=B.size;
        P= new double[newsize];
        for(int i=smallsize; i<newsize;i++){
            P[i]=A.buffer[i];
        }
        for(int i=0; i<smallsize;i++){
            P[i]=A.buffer[i]-B.buffer[i];
        }
    }else{
        newsize=B.size;
        smallsize=A.size;
        P= new double[newsize];
        for(int i=smallsize; i<newsize;i++){
            P[i]=-B.buffer[i];
        }
        for(int i=0; i<smallsize;i++){
            P[i]=A.buffer[i]-B.buffer[i];
        }
    }
    CVector result(newsize,P);
    delete[] P;
    return result;
}

void CVector :: minus(const CVector &B){
    int newsize=max(A.size,B.size);
    double*P=new double[newsize];
    for(int i=0; i<A.size;i++){
        P[i]+=A.buffer[i];
    }
    for(int i=0; i<B.size;i++){
        P[i]-=B.buffer[i];
    }
    CVector result(newsize,P);
    delete[] P;
    return result;
}

double scale_prod(const CVector &A, const CVector &B){
    int result=0;
    if(A.size>B.size){
        for(int i=0;i<B.size;i++){
            result+=A.buffer[i]*B.buffer[i];
        }    
    }
    if(A.size<B.size){
        for(int i=0;i<A.size;i++){
            result+=A.buffer[i]*B.buffer[i];
        }    
    }
    return result;
}
*/
