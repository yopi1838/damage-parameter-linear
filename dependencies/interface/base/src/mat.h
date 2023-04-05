#pragma once
#include "baseexception.h"
#include "basestring.h"
#include "symtensor.h"
#include "matrix.h"
#include "basememory.h" 
// mat.h

// Mat.H - Header file for the matrix & vector classes.
//
// History:  03-Jul-96  Dave Potyondy
// last mod. 11-Jul-96  DOP
// last mod. 22-Jul-96  DOP  added scalar multiply
// last mod. 24-Jul-96  DOP  inlined oper. () and arr_idx functions
// last mod. 09-Aug-96  DOP  Added read/write member functions
// last mod. 03-May-11  JTD  Adapted for use as FISH matrix primitive.
//
// =======================================================================

namespace itasca {
    /* Exception handler for Matrix Class
    */
    struct NegMatExcept : public iexception {
        NegMatExcept();
    };

    /* General matrix class for (m x n) matrices */
    class BASE_EXPORT Mat {
    public:
        typedef uint32 size_type;

        Mat();               //default constructor
        Mat(size_type m, size_type n);   //constructor of (m x n) matrix

        Mat(const Mat& mtx); //copy constructor
        Mat(Mat &&     mtx); // rvalue-reference copy constructor

        Mat(const SymTensor & t);
        Mat(const DVect2 & v);
        Mat(const DVect3 & v);
        template <unsigned SX,unsigned SY> 
        Mat(const Matrix<double,SX,SY> &v);
        Mat(const DMatrix<2,2> & v);
        Mat(const DMatrix<3,3> & v);

        ~Mat();      //destructor

        double& operator ()(size_type i, size_type j) { //full access arr[i,j]
            return( arr[arr_idx(i,j)] ) ; }
        const double& operator ()(size_type i, size_type j) const { //read access arr[i,j]
            return( arr[arr_idx(i,j)] ) ; }

        Mat& operator =(const Mat& mtx);       //assignment
        Mat& operator= (Mat&& mtx);

        Mat operator +(const Mat& mtx) const;  //matrix addition
        Mat operator -(const Mat& mtx) const;  //matrix subtraction
        Mat operator *(const Mat& mtx) const;  //matrix multiply
        Mat operator *(const double& scal) const;  //scalar multiply
        Mat operator* (const DVect2 & v) const; // 2d vector multiply
        Mat operator* (const DVect3 & v) const; // 3d vector multiply
        void operator*=(const double &s);
        void operator/=(const double &s);
        void operator+=(const Mat &mtx);
        void operator-=(const Mat &mtx);

        void fill(const double& val) {  //fill matrix with given value
            for( size_type i=0; i<len; ++i )  arr[i] = val; }
        void zero() { fill( 0.0 ) ; }
        void identity();                                           // set matrix to identity matrix
        Mat transpose() const;                                     // return matrix transpose
        void scalMult(const double& scal) {                        // multiply by given scalar
            for (size_type i=0; i<len; i++)  arr[i] = scal*arr[i]; }

        bool equals( const Mat& mtx ) const;                       // are the matrices equal?
        bool operator== (const Mat& mtx) const { return equals(mtx); }

        virtual bool symmetric() const;                            // is the matrix symmetric?
        virtual double maxNorm() const;                            // return infinity-norm: max abs(elem)
        UVect2 size() const { UVect2 v(msize,nsize);  return v; }

        UVect2 blockSize() const { UVect2 v(blk_m,blk_n);  return v; }
        void   setBlockSize( size_type blk_msize, size_type blk_nsize ) { blk_m = blk_msize; blk_n = blk_nsize; }
        void   addBlock( const Mat& src,           //source matrix
                         size_type src_bi, size_type src_bj,   //block index of source
                         size_type dst_bi, size_type dst_bj ); //block index of dest.
        virtual void addGenBlock( const Mat& src,        //source matrix
                                  size_type src_i, size_type src_j,   //u.l.-corner of source
                                  size_type dst_i, size_type dst_j ); //u.l.-corner of dest.

        SymTensor toTensor() const;
        DVect2    toVect2() const;
        DVect3    toVect3() const;
        template <unsigned SX,unsigned SY>
        Matrix<double,SX,SY> toMatrix() const;
        double *  data() const { return arr; }
    protected:
        double *arr = nullptr;  //store in row-major order as C-array -- (1,2,3,4) - (1,2)
        size_type msize, nsize;   //matrix is (msize x nsize)              (3,4)
        size_type len;            //number of stored entries
        size_type blk_m=1, blk_n=1;   //block size (m x n)

        bool same( const double* v1, const double* v2 ) const;
        inline virtual size_type arr_idx( size_type i, size_type j ) const;
    };

    template <unsigned SX,unsigned SY> 
    Mat::Mat(const Matrix<double,SX,SY> &m) : arr(nullptr) {
        msize = SX;
        nsize = SY;
        len = SX*SY;
        arr = NEWC(double[SX*SY]);
        for (uint32 i=0;i<SX;++i)
            for (uint32 j=0;j<SY;++j)
                operator()(i,j) = m(i,j);
    }

    template <unsigned SX,unsigned SY>
    Matrix<double,SX,SY> Mat::toMatrix() const {
        Matrix<double,SX,SY> ret;
        if (msize!=SX || nsize!=SY) {
            if (msize==1 && SY==1 && nsize==SX) { // Allow generalized vectors to switch
                for (uint32 i=0;i<SX;++i)
                    ret(i,0) = operator()(0,i);
                return ret;
            }
            if (nsize==1 && SX==1 && msize==SY) { // Allow 
                for (uint32 i=0;i<SY;++i)
                    ret(0,i) = operator()(i,0);
                return ret;
            }
            throw iexception("Matrix size does not match {},{}, is {},{} instead.",SX,SY,msize,nsize);
        }
        for (uint32 i=0;i<SX;++i)
            for (uint32 j=0;j<SY;++j)
                ret(i,j) = operator()(i,j);
        return ret;
    }

    // =========================================
    // Returns index into the array.
    // Assumes indexing starts at (0,0).
    Mat::size_type Mat::arr_idx( size_type i, size_type j ) const {
#ifdef _DEBUG
        if( (i >= msize) || (j >= nsize) )
            throw iexception("Matrix ({} x {}) : index ({},{}) is out of bounds.", msize, nsize, i, j);
#endif
        return nsize*(i) + j;
    }

}
// EOF Mat.H