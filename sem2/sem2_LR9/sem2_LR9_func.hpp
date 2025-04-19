#include <iostream>
#include <cmath>
#include <cstdio>
#include <initializer_list>
using namespace std;

template <typename T>
class TVector{
    private:
        void init();
    protected:
        int size;
        T* buffer;
    public:
        TVector(int s=0, T*p=nullptr){
            init();
            copy(s,p);
        };
        TVector(const TVector &A);
        virtual ~TVector();
        void clear();
        int copy(int s, T* p);
        int copy(const TVector &A){
            return copy(A.size,A.buffer);
        }
        virtual void vprint();
        void print();
        template<typename U>
        friend TVector<U> sum(const TVector<U> &A, const TVector<U> &B);
        
        template<typename U>
        friend TVector<U> det(const TVector<U> &A, const TVector<U> &B);
        void add(const TVector<T> &B);
        void minus(const TVector<T> &B);
        template<typename U>
        friend double scale_prod(const TVector<U> &A, const TVector<U> &B);
        double moduleV();
        //operators
        TVector<T>& operator=(const TVector<T> &A);
        T& operator[](int index);
        T operator[](int index) const;
        template<typename U>
        friend U operator*(const TVector<U>& A, const TVector<U>& B);
        template<typename U>
        friend TVector<U> operator+(const TVector<U>& A, const TVector<U>& B);
        template<typename U>
        friend TVector<U> operator-(const TVector<U>& A, const TVector<U>& B);
        TVector<T>& operator+=(const TVector<T>& B);
        TVector<T>& operator-=(const TVector<T>& B);
        T operator()();
        //lab 6
        TVector(TVector&& v): size(v.size), buffer(v.buffer){
            cout<<"move constructor\n";
            v.init();
        }
        TVector<T>& operator=(TVector<T>&& v);
        /*class iterator{
        protected:
            T* data;
        public:
            iterator(T* dat): data(dat){}
            const T& operator*(){return *data;}
            iterator operator++(){
                data++;
                return *this;
            }
            iterator operator--(){
                data--;
                return *this;
            }
            bool operator==(const iterator& x)const {return data==x.data;}
            bool operator!=(const iterator& x)const {return data!=x.data;}
        };*/
        using iterator=T*;
        iterator begin(){ return iterator(buffer);}
        iterator end(){ return iterator(buffer+size);}
        //lab9
        TVector(initializer_list<T> init_list);
};


template <typename T>
void TVector<T> :: init(){
    size=0;
    buffer=nullptr;
}

template <typename T>
void TVector<T> :: clear(){
    if(buffer){
        delete[] buffer;
        init();
    }
}


template <typename T>
TVector<T> :: TVector(const TVector<T> &A){
    cout<<"copy constructor\n";
    init();
    copy(A);
}

template <typename T>
TVector<T> :: ~TVector(){
    clear();
}

template <typename T>
int TVector<T> :: copy(int s, T* p){
    if(buffer!=p){
        clear();
        buffer= new T[s];
        if(buffer){
            size=s;
            for(int i=0;i<size;i++){
                buffer[i]=p[i];
            }
        }
    }
    return s;
}

template <typename T>
void TVector<T> :: vprint(){
    print();
}

template <typename T>
void TVector<T> :: print(){ 
    cout<<"Num: "<<size<<" "<<"Data: ";
    for(int i=0;i<size;i++){
        cout<<buffer[i]<<" ";
    }
        cout<<endl;
        return;
}
//LR3
template <typename T>
TVector<T> sum(const TVector<T> &A, const TVector<T> &B){
    int newsize=max(A.size,B.size);
    int smallsize=min(A.size,B.size);
    T*P=new T[newsize];
    for(int i=0; i<smallsize;i++){
        P[i]=A.buffer[i]+B.buffer[i];
    }
    for(int i=smallsize; i<B.size;i++){
        P[i]=B.buffer[i];
    }
    for(int i=smallsize; i<A.size;i++){
        P[i]=A.buffer[i];
    }
    TVector<T> result;
    result.size=newsize;
    result.buffer=P;
    return result;
}

template <typename T>
TVector<T> det(const TVector<T> &A, const TVector<T> &B){
    int newsize=max(A.size,B.size);
    int smallsize=min(A.size,B.size);
    T*P=new T[newsize];
    for(int i=0; i<smallsize;i++){
        P[i]=A.buffer[i]-B.buffer[i];
    }
    for(int i=smallsize; i<B.size;i++){
        P[i]=-B.buffer[i];
    }
    for(int i=smallsize; i<A.size;i++){
        P[i]=A.buffer[i];
    }
    TVector<T> result;
    result.size=newsize;
    result.buffer=P;
    return result;
}

template <typename T>
void TVector<T> :: add(const TVector<T> &B){
    copy(sum(*this,B));
}

template <typename T>
void TVector<T> :: minus(const TVector<T> &B){
    copy(det(*this,B));
}

template <typename T>
double scale_prod(const TVector<T> &A, const TVector<T> &B){
    int result=0;
    int count=min(A.size,B.size);
    for(int i=0;i<count;i++){
        result+=A.buffer[i]*B.buffer[i];
    }    
    return result;
}

template <typename T>
double TVector<T>:: moduleV(){
    T result =sqrt(scale_prod(*this,*this));
    return result;
}

//LR4 operators
template <typename T>
TVector<T>& TVector<T>:: operator=(const TVector<T> &A){
    cout<<"= copy\n";
    if(this!=&A){
       copy(A);
    }
    return *this;
}

template <typename T>
T& TVector<T>:: operator[](int index){
    return buffer[index];
}

template <typename T>
T TVector<T>:: operator[](int index) const{
    return buffer[index];
}

template <typename T>
T operator*(const TVector<T>& A, const TVector<T>& B){
    return scale_prod(A,B);
}

template <typename T>
TVector<T> operator+(const TVector<T>& A, const TVector<T>& B){
    return sum(A,B);
}

template <typename T>
TVector<T> operator-(const TVector<T>& A, const TVector<T>& B){
    return det(A,B);
}

template <typename T>
TVector<T>& TVector<T>:: operator+=(const TVector<T>& B){
    this->add(B);
    return *this;
}

template <typename T>
TVector<T>& TVector<T>:: operator-=(const TVector<T>& B){
    this->minus(B);
    return *this;
}

template <typename T>
T TVector<T>:: operator()(){
    return moduleV();
}

template <typename T>
TVector<T>& TVector<T>:: operator=(TVector<T>&& v){
    cout<<"= move\n";
    if(this != &v){
        clear();
        size=v.size;
        buffer=v.buffer;
        v.init();
    }
    return *this;
}

template <typename T>
TVector<T>::TVector(initializer_list<T> init_list){
    init();
    int i=0;
    size=init_list.size();
    buffer = new T[size];
    for(auto x: init_list){
        buffer[i++]=x;
    }
}