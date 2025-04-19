#include <iostream>
#include <cmath>


using namespace std;

template<typename T> T* createMas(T* buffer,const int n){
    buffer=new T[n];
    for(int i=0;i<n;i++){
            cin>>buffer[i];
    }
    return buffer;
};

template<typename T> double scaleProd(const T*pM1, const T*pM2,const int n){
    double sum=0;
    for(int i=0;i<n;i++){
            sum+=pM1[i]*pM2[i];
    }
    return sum;
};

template<typename T> double modMas(const T*pM, const int n){
    double res=0;
    res=sqrt(scaleProd(pM,pM,n));
    return res;    
};

template<typename T> double eagle(T*pM1,T*pM2,const int n){
    double eagle=0;
    double scale,m1,m2;
    scale=scaleProd(pM1,pM2,n);
    m1=modMas(pM1,n);
    m2=modMas(pM2,n);
    eagle=(scale)/m1/m2;
    return acos(eagle)*180/acos(-1);
}



















/*double * createMas(const int n){
    double * pM = new double[n];
    if(pM){
        return pM;
    }
    delete[] pM;
    cout<<"Error of pointer createMas";
    return NULL;
}

double * createMasFill(const int n){
    double * pM = new double[n];
    cout<<"Введите массив\n";
    if(pM){
        for(int i=0;i<n;i++){
            cin>>pM[i];
        }
        return pM;
    }
    delete[] pM;
    cout<<"Error of pointer createMasFill";
    return NULL;
}

void printMas(const double *pM, int n){
    if(pM){
        for(int i=0;i<n;i++){
            cout<<pM[i]<<" ";
        }
        cout<<endl;
    }else{
        cout<<"error of pointer printMas"<<endl;
    }
}

double* sumMas(const double *pM1, const double *pM2, int n){
    double* res= createMas(n);
    if(res && pM1 && pM2){
        for(int i=0;i<n;i++){
            res[i]=pM1[i]+pM2[i];
        }
        return res;
    }
    delete[] res;
    cout<<"error of pointer sumMas"<<endl;
    return NULL;
}


double* subMas(const double *pM1, const double *pM2, int n){
    double* res= createMas(n);
    if(res && pM1 && pM2){
        for(int i=0;i<n;i++){
            res[i]=pM1[i]-pM2[i];
        }
        return res;
    }
    delete[] res;
    cout<<"error of pointer subMas"<<endl;
    return NULL;
}

double scaleProd(const double *pM1, const double *pM2, int n){
    double sum=0;
    if(pM1 && pM2){
        for(int i=0;i<n;i++){
            sum+=pM1[i]*pM2[i];
        }
        return sum;
    }
    cout<<"error of pointer scaleProd"<<endl;
    return 0;
}

double modMas(const double *pM, int n){
    double res=0;
    if(pM){
        res=sqrt(scaleProd(pM,pM,n));
        return res;
    }
    cout<<"error of pointer modMas"<<endl;
    return 0;
}

double eagle(const double *pM1, const double *pM2, int n){
    double eagle=0;
    double scale,m1,m2;
    if(pM1 && pM2){
        scale=scaleProd(pM1,pM2,n);
        m1=modMas(pM1,n);
        m2=modMas(pM2,n);
        eagle=(scale)/m1/m2;
        return acos(eagle)*180/M_PI; 
    }
    cout<<"error of pointer eagle"<<endl;       
    return 0;
}*/
