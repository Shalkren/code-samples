
#include "sem2_LR2.hpp"
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

/*
class CVector{
    protected:
        int size;
        T* buffer;
    public:
        CVector(){
            size=0;
            buffer=nullptr;
        }
        CVector(int size){
            this->size=size;
            buffer= new T[size];
            cout<<"vector\n";
            for(int i=0;i<size;i++){
                cin>>buffer[i];
            }
        }
        CVector(const CVector &A){
            size=A.size;
            buffer= new T[size];
            for(int i=0;i<size;i++){
                buffer[i]=A.buffer[i];
            }
        }
        void print(){
            cout<<"size \n"<<size<<endl<<"vector ";
            for(int i=0;i<size;i++){
                cout<<buffer[i]<<" ";
            }
            cout<<endl;
            return;
        }
        copy(const CVector &A){
            if(buffer!=A.buffer){
            size=A.size;
            delete[] buffer;
            buffer= new T[size];
            for(int i=0;i<size;i++){
                buffer[i]=A.buffer[i];
            }
        }

}
        ~CVector(){
            delete[] buffer;
        }
};
*/




/*CVector(const CVector &A){
        this->size=A.size;
        this->buffer= new T[size];
        for(int i=0;i<size;i++){
            buffer[i]=A.buffer[i];
        }
}*/