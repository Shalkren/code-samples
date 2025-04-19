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
        ~CVector();
        void clear();
        int copy(int s, double* p);
        int copy(const CVector &A){
            return copy(A.size,A.buffer);
        }
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
};