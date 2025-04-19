#include <iostream>
#include <cmath>
using namespace std;
class CVector{
    private:
        void init();
    protected:
        int size;
        double* buffer;
    public:
        CVector(int s=0, double*p=nullptr);
        CVector(const CVector &A);
        virtual ~CVector();
        void clear();
        int copy(int s, double* p);
        int copy(const CVector &A){
            return copy(A.size,A.buffer);
        }
        virtual void vprint();
        void print();

        friend CVector sum(const CVector &A, const CVector &B);
        friend CVector det(const CVector &A, const CVector &B);
        void add(const CVector &B);
        void minus(const CVector &B);
        friend double scale_prod(const CVector &A, const CVector &B);
        double moduleV();
        //operators
        CVector& operator=(const CVector &A);
        double& operator[](int index);
        const double CVector:: operator[](int index) const;
        friend double operator*(const CVector& A, const CVector& B);
        friend CVector operator+(const CVector& A, const CVector& B);
        friend CVector operator-(const CVector& A, const CVector& B);
        CVector& operator+=(const CVector& B);
        CVector& operator-=(const CVector& B);
        double operator()();
        //lab 6
        CVector(CVector&& v): size(v.size), buffer(v.buffer){
            cout<<"move constructor\n";
            v.init();
        }
        CVector& operator=(CVector&& v);
};


class CVector2 : public CVector{
    public:
        CVector2(int s=0, double*p=nullptr): CVector(s, p){}
        CVector2(const CVector2& CV2): CVector(CV2){};
        ~CVector2();
        void print();
};

class CVector3 : public CVector{
    public:
        CVector3(int s=0, double*p=nullptr): CVector(s, p){}
        CVector3(const CVector3& CV3): CVector(CV3){};
        ~CVector3();
        virtual void vprint();
};