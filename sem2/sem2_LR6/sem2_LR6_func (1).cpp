#include "sem2_LR6_func.hpp"
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
    cout<<"copy constructor\n";
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

void CVector :: vprint(){
    print();
}

void CVector :: print(){ 
    cout<<"Num: "<<size<<" "<<"Data: ";
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

//LR4 operators
CVector& CVector:: operator=(const CVector &A){
    cout<<"= copy\n";
    if(this!=&A){
       copy(A);
    }
    return *this;
}

double& CVector:: operator[](int index){
    return buffer[index];
}

const double CVector:: operator[](int index) const{
    return buffer[index];
}

double operator*(const CVector& A, const CVector& B){
    return scale_prod(A,B);
}

CVector operator+(const CVector& A, const CVector& B){
    return sum(A,B);
}

CVector operator-(const CVector& A, const CVector& B){
    return det(A,B);
}

CVector& CVector:: operator+=(const CVector& B){
    this->add(B);
    return *this;
}

CVector& CVector:: operator-=(const CVector& B){
    this->minus(B);
    return *this;
}

double CVector:: operator()(){
    return moduleV();
}

//CVector2
void CVector2 :: print(){ 
    cout<<"Num: "<<size<<"\n";
    for(int i=0;i<size;i++){
        cout<<i+1<<": "<<buffer[i]<<"\n";
    }
    cout<<endl;
    return;
}

CVector2 :: ~CVector2(){}

//CVector3
void CVector3 :: vprint(){ 
    cout<<"Num: "<<size<<"\n";
    for(int i=0;i<size;i++){
        cout<<i+1<<": "<<buffer[i]<<"\n";
    }
    cout<<endl;
    return;
}

CVector3 :: ~CVector3(){}
//lab 6
CVector& CVector:: operator=(CVector&& v){
    cout<<"= move\n";
    if(this != &v){
        clear();
        size=v.size;
        buffer=v.buffer;
        v.init();
    }
    return *this;
}
