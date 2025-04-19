#include <iostream>
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
    };