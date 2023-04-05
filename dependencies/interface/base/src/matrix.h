#pragma once
/** \file matrix.h
*  \brief A template-based matrix class, size fixed at compile time.
* \addtogroup Base Base interface specification
* @{
*/

#ifdef __LINUX 
#include <string.h>
#endif

#include "symtensor.h"
#include <cassert>

/** \brief A template-based matrix class, size fixed at compile time.  Defaults to symmetric sized matrix.
 
* This primitive template-based matrix class should be used where the matrix size is fixed
*   at compile time.
* May use m(x,y) to access members, or m[x][y] as well.
*/
template <class T,unsigned SX,unsigned SY=SX> class Matrix {
private:
    /// A matrix utility class for returning a single row of a given Matrix.
    class Column {
    public:
        /// Constructor - return row \a y of matrix \a m.
        Column(unsigned x,Matrix<T,SX,SY> &m) : x_(x), m_(m) { }
        /// Copy constructor
        Column(const Column &r) : x_(r.x_), m_(r.m_) { }
        /// Access column \a x of the row.
        const T &operator[](unsigned y) const { return m_.get(x_,y); }
        /// Access column \a x of the row.
        T &operator[](unsigned y) { return m_.get(x_,y); }
    private:
        void operator=(const Column &r); // DISALLOWED
        unsigned x_;
        Matrix<T,SX,SY> &m_;
    };
    // Using template-meta programming to unroll innermost loop of matrix multiply
    template <unsigned SZ,unsigned I,unsigned J,unsigned K>
    class innerLoop {
    public:
        static double execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,SZ> &m) {
            return l.t_[SX-I][SY-K]*m.t_[SY-K][SZ-J] + innerLoop<SZ,I,J,K-1>::execute(l,m);
        }
    };
    template <unsigned SZ,unsigned I,unsigned J> // Termination specialization
    class innerLoop<SZ,I,J,1> {
    public:
        static double execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,SZ> &m) {
            return l.t_[SX-I][SY-1]*m.t_[SY-1][SZ-J];
        }
    };
    template <unsigned I,unsigned K> // Specialization for column matrix (SZ=1)
    class innerLoop<1,I,1,K> {
    public:
        static double execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,1> &m) {
            return l.t_[SX-I][SY-K]*m.t_[SY-K] + innerLoop<1,I,1,K-1>::execute(l,m);
        }
    };
    template <unsigned I>  // termination specialization for column matrix specialization
    class innerLoop<1,I,1,1> {
    public:
        static double execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,1> &m) {
            return l.t_[SX-I][SY-1]*m.t_[SY-1];
        }
    };
    // Using template meta-programming to unroll second loop of matrix multiply.
    template <unsigned SZ,unsigned I,unsigned J>
    class loop2Multiply {
    public:
        static void execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,SZ> &m,Matrix<T,SX,SZ> &ret) {
            ret.t_[SX-I][SZ-J] = innerLoop<SZ,I,J,SY>::execute(l,m);
            loop2Multiply<SZ,I,J-1>::execute(l,m,ret);
        }
    };
    template <unsigned SZ,unsigned I>  // Termination specialization
    class loop2Multiply<SZ,I,1> {
    public:
        static void execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,SZ> &m,Matrix<T,SX,SZ> &ret) {
            ret.t_[SX-I][SZ-1] = innerLoop<SZ,I,1,SY>::execute(l,m);
        }
    };
    template <unsigned I>  // Column matrix specialization (SZ=1)
    class loop2Multiply<1,I,1> {
    public:
        static void execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,1> &m,Matrix<T,SX,1> &ret) {
            ret.t_[SX-I] = innerLoop<1,I,1,SY>::execute(l,m);
        }
    };
    // Using template meta-programming to unroll outermost loop of matrix multiply.
    template <unsigned SZ,unsigned I>
    class loopMultiply {
    public:
        static void execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,SZ> &m,Matrix<T,SX,SZ> &ret) {
            loop2Multiply<SZ,I,SZ>::execute(l,m,ret);
            loopMultiply<SZ,I-1>::execute(l,m,ret);
        }
    };
    template <unsigned SZ> // Termination specialization
    class loopMultiply<SZ,1> {
    public:
        static void execute(const Matrix<T,SX,SY> &l,const Matrix<T,SY,SZ> &m,Matrix<T,SX,SZ> &ret) {
            loop2Multiply<SZ,1,SZ>::execute(l,m,ret);
        }
    };

public:
    /// Default constructor, does nothing and no initialization.
#ifdef _DEBUG
    Matrix() { set(initVal<T>()); }
#else
#pragma warning(push)
#pragma warning(disable:26495) // Uninitialize warning
    Matrix() {} // Uninitialized on purpose
#pragma warning(pop)
#endif
    /// Explicit constructor, initializes all elements to value t.
    explicit Matrix(const T &t) { set(t); }
    /// Copy constructor
    Matrix(const Matrix<T,SX,SY> &m) { operator=(m); }
    /// Equality operator
    const Matrix<T,SX,SY> &operator=(const Matrix<T,SX,SY> &m) { memcpy(t_,m.t_,sizeof(T)*SX*SY);  return *this; }

    /// Retrieve value at row \a x column \a y.   Bounds checking is done in a debug compile.
    const T & get(unsigned x,unsigned y) const { assert(x<SX);  assert(y<SY); return t_[x][y]; }
    /// Retrieve value at row \a x column \a y.   Bounds checking is done in a debug compile.
    T &       get(unsigned x,unsigned y) { assert(x<SX);  assert(y<SY); return t_[x][y]; }
    /// Array access operator returns a Column helper class, which has it's own [] operator to access a column of the row.
    const Column operator[](unsigned y) const { return Column(y,*this); }
    /// Array access operator returns a Column helper class, which has it's own [] operator to access a column of the row.
    Column       operator[](unsigned y) { return Column(y,*this); }
    /// () operator access to get(x,y)
    const T & operator()(unsigned x,unsigned y) const { return get(x,y); }
    /// () operator access to get(x,y)
    T &       operator()(unsigned x,unsigned y) { return get(x,y); }

    /// In-place matrix addition
    const Matrix<T,SX,SY> &operator+=(const Matrix<T,SX,SY> &m) { for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) t_[i][j] += m.t_[i][j];  return *this; }
    /// In-place matrix subtraction
    const Matrix<T,SX,SY> &operator-=(const Matrix<T,SX,SY> &m) { for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) t_[i][j] -= m.t_[i][j];  return *this; }
    /// In-place multiplication by a scalar
    const Matrix<T,SX,SY> &operator*=(const T &t) { for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) t_[i][j] *= t;  return *this; }
    /// In-place division by a scalar
    const Matrix<T,SX,SY> &operator/=(const T &t) { for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) t_[i][j] /= t;  return *this; }

    /// Binary addition operator.
    Matrix<T,SX,SY> operator+(const Matrix<T,SX,SY> &m) const { Matrix<T,SX,SY> ret; for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) ret.t_[i][j] = t_[i][j] + m.t_[i][j]; return ret; }
    /// Binary subtraction operator.
    Matrix<T,SX,SY> operator-(const Matrix<T,SX,SY> &m) const { Matrix<T,SX,SY> ret; for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) ret.t_[i][j] = t_[i][j] - m.t_[i][j]; return ret; }
    /// Binary scalar multiplication operator.
    Matrix<T,SX,SY> operator*(const T &t) const { Matrix<T,SX,SY> ret; for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) ret.t_[i][j] = t_[i][j] * t;  return ret; }
    /// Binary scalar division operator.
    Matrix<T,SX,SY> operator/(const T &t) const { Matrix<T,SX,SY> ret; for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) ret.t_[i][j] = t_[i][j] / t;  return ret; }
    /// Binary matrix multiplication operator -- simplest naive approach.
    template <unsigned SZ>
    Matrix<T,SX,SZ> operator*(const Matrix<T,SY,SZ> &m) const {
        Matrix<T,SX,SZ> ret;
        // OK - we are using template expansion to do loop unrolling at compile time.   Fun!
        //for (unsigned i=0;i<SX;++i) { // ROWS OF FIRST
        //    for (unsigned j=0;j<SZ;++j) { // COLUMNS OF SECOND
        //        T &val = ret.get(i,j);
        //        val = 0;
        //        for (unsigned k=0;k<SY;++k)  // Add up
        //            val += get(i,k)*m.get(k,j);
        //    }
        //}
        loopMultiply<SZ,SX>::execute(*this,m,ret);
        return ret;
    }
    //// Matrix multiplication specialization for when the RHS has size 1 in Y.
    //template <class T>
    //Matrix<T,SX,1> operator*(const Matrix<T,SY,1> &m) const {
    //    Matrix<T,SX,1> ret;
    //    // OK - we are using template expansion to do loop unrolling at compile time.   Fun!
    //    //for (unsigned i=0;i<SX;++i) { // ROWS OF FIRST
    //    //    T &val = ret.get(i,0);
    //    //    val = 0.0;
    //    //    for (unsigned k=0;k<SY;++k)  // Add up
    //    //        val += get(i,k)*m.get(k,0);
    //    //}
    //    loopMultiply<1,SX>::execute(*this,m,ret);
    //    return ret;
    //}

    // Block operations
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from block indices (iSrc,jSrc) to block indices (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2,unsigned SY2>
    void addBlock(const Matrix<T,SX2,SY2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        addGenBlock<BX,BY,SX2,SY2>(m,iSrc*BX,jSrc*BY,iDst*BX,jDst*BY);
    }
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from normal *not block* indices (iSrc,jSrc) to (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2,unsigned SY2>
    void addGenBlock(const Matrix<T,SX2,SY2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        for (unsigned i=iSrc,id=iDst;i<(iSrc+BX);i++,id++)
            for (unsigned j=jSrc,jd=jDst;j<(jSrc+BY);j++,jd++)
                get(id,jd) += m(i,j);
    }

    /// Returns the infinity norm of the matrix, or the maximum absolute magnitude of any element.
    T maxNorm() const { T ret(0);  for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) ret = std::max(ret,std::abs(t_[i][j]));  return ret; }
    /// Return the transpose of the matrix
    Matrix<T,SY,SX> transpose() const { Matrix<T,SY,SX> ret;  for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) ret(j,i) = t_[i][j];  return ret; }
    /// Return the trace of the matrix or the sum of the diagonal components. Works also if the matrix is not square.
    T trace() const { T tt = t_[0][0]; unsigned len = std::min(SX,SY); for (unsigned i=1; i<len; ++i) tt += t_[i][i]; return tt; }
    /// Set all entries in the matrix to t.
    void set(const T &t=T()) { for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SY;++j) t_[i][j] = t; }

    /// Returns an identity matrix (or as close as you can get if not diagonal).
    static Matrix<T,SX,SY> identity() { Matrix<T,SX,SY> ret(0.0);  for (unsigned i=0;i<std::min(SX,SY);++i) ret(i,i) = 1.0;  return ret; }

    T t_[SX][SY];
};


/** \brief A specialization of the Matrix class for the case when SY=1 */
template <class T,unsigned SX> class Matrix<T,SX,1> {
public:
    /// Default constructor, does nothing and no initialization.
#ifdef _DEBUG
    Matrix() { set(initVal<T>()); }
#else
    Matrix()  { }
#endif
    /// Explicit constructor, initializes all elements to value t.
    explicit Matrix(const T &t) { set(t); }
    /// Copy constructor
    Matrix(const Matrix<T,SX,1> &m) { operator=(m); }
    /// Equality operator
    const Matrix<T,SX,1> &operator=(const Matrix<T,SX,1> &m) { memcpy(t_,m.t_,sizeof(T)*SX);  return *this; }

    /// Retrieve constant value at row \a x column \a y.   Bounds checking is done in a debug compile.
    const T & get(unsigned x,unsigned y) const { assert(x<SX);  assert(!y); y; return t_[x]; }
    /// Retrieve constant value at row \a x.   Bounds checking is done in a debug compile.
    const T & get(unsigned x) const { assert(x<SX);  return t_[x]; }
    /// Retrieve value at row \a x column \a y.   Bounds checking is done in a debug compile.
    T &       get(unsigned x,unsigned y) { assert(x<SX);  assert(!y); y; return t_[x]; }
    /// Retrieve value at row \a x.   Bounds checking is done in a debug compile.
    T &       get(unsigned x) { assert(x<SX); return t_[x]; }
    /// () operator access to const get(x,y)
    const T & operator()(unsigned x,unsigned y) const { y; return get(x,y); }
    /// () operator access to const get(x)
    const T & operator()(unsigned x) const { return get(x); }
    /// () operator access to get(x,y)
    T &       operator()(unsigned x,unsigned y) { y; return get(x,y); }
    /// () operator access to get(x)
    T &       operator()(unsigned x) { return get(x); }

    /// In-place matrix addition
    const Matrix<T,SX,1> &operator+=(const Matrix<T,SX,1> &m) { for (unsigned i=0;i<SX;++i) t_[i] += m.t_[i];  return *this; }
    /// In-place matrix subtraction
    const Matrix<T,SX,1> &operator-=(const Matrix<T,SX,1> &m) { for (unsigned i=0;i<SX;++i) t_[i] -= m.t_[i];  return *this; }
    /// In-place multiplication by a scalar
    const Matrix<T,SX,1> &operator*=(const T &t) { for (unsigned i=0;i<SX;++i) t_[i] *= t;  return *this; }
    /// In-place division by a scalar
    const Matrix<T,SX,1> &operator/=(const T &t) { for (unsigned i=0;i<SX;++i) t_[i] /= t;  return *this; }

    /// Binary addition operator.
    Matrix<T,SX,1> operator+(const Matrix<T,SX,1> &m) const { Matrix<T,SX,1> ret; for (unsigned i=0;i<SX;++i) ret.t_[i] = t_[i] + m.t_[i]; return ret; }
    /// Binary subtraction operator.
    Matrix<T,SX,1> operator-(const Matrix<T,SX,1> &m) const { Matrix<T,SX,1> ret; for (unsigned i=0;i<SX;++i) ret.t_[i] = t_[i] - m.t_[i]; return ret; }
    /// Binary scalar multiplication operator.
    Matrix<T,SX,1> operator*(const T &t) const { Matrix<T,SX,1> ret; for (unsigned i=0;i<SX;++i) ret.t_[i] = t_[i] * t;  return ret; }
    /// Binary scalar division operator.
    Matrix<T,SX,1> operator/(const T &t) const { Matrix<T,SX,1> ret; for (unsigned i=0;i<SX;++i) ret.t_[i] = t_[i] / t;  return ret; }
    /// Binary matrix multiplication operator -- simplest naive approach.
    template <unsigned SZ> Matrix<T,SX,SZ> operator*(const Matrix<T,1,SZ> &m) const {
        Matrix<T,SX,SZ> ret;
        for (unsigned i=0;i<SX;++i) // ROWS OF FIRST
            for (unsigned j=0;j<SZ;++j) // COLUMNS OF SECOND
                ret.get(i,j) = get(i,0)*m.get(0,j);
        return ret;
    }

    // Block operations
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from block indices (iSrc,jSrc) to block indices (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2,unsigned SY2> void addBlock(const Matrix<T,SX2,SY2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        addGenBlock<BX,BY,SX2,SY2>(m,iSrc*BX,jSrc*BY,iDst*BX,jDst*BY);
    }
    /// Adds a block to this matrix from matrix m using the block size BX from block indice iSrc to indice iDst
    template <unsigned BX,unsigned SX2> void addBlock(const Matrix<T,SX2,1> &m,unsigned iSrc,unsigned iDst) {
        addGenBlock<BX,SX2>(m,iSrc*BX,iDst*BX);
    }
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from normal *not block* indices (iSrc,jSrc) to (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2,unsigned SY2> void addGenBlock(const Matrix<T,SX2,SY2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        for (unsigned i=iSrc,id=iDst;i<(iSrc+BX);i++,id++)
            for (unsigned j=jSrc,jd=jDst;j<(jSrc+BY);j++,jd++)
                get(id,jd) += m(i,j);
    }
    /// Adds a block to this matrix from matrix m using the block size BX from normal non block indice iSrc to indice iDst
    template <unsigned BX,unsigned SX2> void addGenBlock(const Matrix<T,SX2,1> &m,unsigned iSrc,unsigned iDst) {
        for (unsigned i=iSrc,id=iDst;i<(iSrc+BX);i++,id++)
            get(id) += m(i);
    }

    /// Returns the infinity norm of the matrix, or the maximum absolute magnitude of any element.
    T maxNorm() const { T ret(0);  for (unsigned i=0;i<SX;++i) ret = std::max(ret,std::abs(t_[i]));  return ret; }
    /// returns the transposed matrix of this matrix
    Matrix<T,1,SX> transpose() const { Matrix<T,1,SX> ret;  for (unsigned i=0;i<SX;++i) ret(0,i) = t_[i];  return ret; }
    /// Sets all matrix elemnts to T
    void set(const T &t=T()) { for (unsigned i=0;i<SX;++i) t_[i] = t; }

    /// Returns an identity matrix (or as close as you can get if not diagonal).
    static Matrix<T,SX,1> identity() { Matrix<T,SX,1> ret(0.0);  ret.t_[0] = 1.0; return ret; }
    T t_[SX];
};

/** \brief A template-based symmetric matrix class, size fixed at compile time.
* This primitive template-based matrix class should be used where the matrix size is fixed
*   at compile time.
*/
template <class T,unsigned SX> class SymMatrix {
private:
    /// A matrix utility class for returning a single row of a given Matrix.
    class Column {
    public:
        /// Constructor - return row \a y of matrix \a m.
        Column(unsigned x,SymMatrix<T,SX> &m) : x_(x), m_(m) { }
        /// Copy constructor
        Column(const Column &r) : x_(r.x_), m_(r.m_) { }
        /// Access column \a x of the row.
        const T &operator[](unsigned y) const { return m_.get(x_,y); }
        /// Access column \a x of the row.
        T &operator[](unsigned y) { return m_.get(x_,y); }
    private:
        void operator=(const Column &r); // DISALLOWED
        unsigned x_;
        SymMatrix<T,SX> &m_;
    };
    // Using template-meta programming to unroll innermost loop of matrix multiply
    template <unsigned SZ,unsigned I,unsigned J,unsigned K>
    class innerLoop {
    public:
        static double execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,SZ> &m) {
            return l.t_[index(SX-I,SX-K)]*m.t_[SX-K][SZ-J] + innerLoop<SZ,I,J,K-1>::execute(l,m);
        }
    };
    template <unsigned SZ,unsigned I,unsigned J> // Termination specialization
    class innerLoop<SZ,I,J,1> {
    public:
        static double execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,SZ> &m) {
            return l.t_[index(SX-I,SX-1)]*m.t_[SX-1][SZ-J];
        }
    };
    template <unsigned I,unsigned K> // Specialization for column matrix (SZ=1)
    class innerLoop<1,I,1,K> {
    public:
        static double execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,1> &m) {
            return l.t_[index(SX-I,SX-K)]*m.t_[SX-K] + innerLoop<1,I,1,K-1>::execute(l,m);
        }
    };
    template <unsigned I>  // termination specialization for column matrix specialization
    class innerLoop<1,I,1,1> {
    public:
        static double execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,1> &m) {
            return l.t_[index(SX-I,SX-1)]*m.t_[SX-1];
        }
    };
    // Using template meta-programming to unroll second loop of matrix multiply.
    template <unsigned SZ,unsigned I,unsigned J>
    class loop2Multiply {
    public:
        static void execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,SZ> &m,Matrix<T,SX,SZ> &ret) {
            ret.t_[SX-I][SZ-J] = innerLoop<SZ,I,J,SX>::execute(l,m);
            loop2Multiply<SZ,I,J-1>::execute(l,m,ret);
        }
    };
    template <unsigned SZ,unsigned I>  // Termination specialization
    class loop2Multiply<SZ,I,1> {
    public:
        static void execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,SZ> &m,Matrix<T,SX,SZ> &ret) {
            ret.t_[SX-I][SZ-1] = innerLoop<SZ,I,1,SX>::execute(l,m);
        }
    };
    template <unsigned I>  // Column matrix specialization (SZ=1)
    class loop2Multiply<1,I,1> {
    public:
        static void execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,1> &m,Matrix<T,SX,1> &ret) {
            ret.t_[SX-I] = innerLoop<1,I,1,SX>::execute(l,m);
        }
    };
    // Using template meta-programming to unroll outermost loop of matrix multiply.
    template <unsigned SZ,unsigned I>
    class loopMultiply {
    public:
        static void execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,SZ> &m,Matrix<T,SX,SZ> &ret) {
            loop2Multiply<SZ,I,SZ>::execute(l,m,ret);
            loopMultiply<SZ,I-1>::execute(l,m,ret);
        }
    };
    template <unsigned SZ> // Termination specialization
    class loopMultiply<SZ,1> {
    public:
        static void execute(const SymMatrix<T,SX> &l,const Matrix<T,SX,SZ> &m,Matrix<T,SX,SZ> &ret) {
            loop2Multiply<SZ,1,SZ>::execute(l,m,ret);
        }
    };


    // Using template-meta programming to unroll innermost loop of matrix multiply
    template <unsigned I,unsigned J,unsigned K>
    class innerLoopS {
    public:
        static double execute(const SymMatrix<T,SX> &l,const SymMatrix<T,SX> &m) {
            return l.t_[index(SX-I,SX-K)]*m.t_[index(SX-K,SX-J)] + innerLoopS<I,J,K-1>::execute(l,m);
        }
    };

    template <unsigned I,unsigned J> // Termination specialization
    class innerLoopS<I,J,1> {
    public:
        static double execute(const SymMatrix<T,SX> &l,const SymMatrix<T,SX> &m) {
            return l.t_[index(SX-I,SX-1)]*m.t_[index(SX-1,SX-J)];
            // what is X here? seems undefined??
        }
    };

    // Using template meta-programming to unroll second loop of matrix multiply.
    template <unsigned I,unsigned J>
    class loop2MultiplyS {
    public:
        static void execute(const SymMatrix<T,SX> &l,const SymMatrix<T,SX> &m,Matrix<T,SX,SX> &ret) {
            ret.t_[SX-I][SX-J] = innerLoopS<I,J,SX>::execute(l,m);
            loop2MultiplyS<I,J-1>::execute(l,m,ret);
        }
    };
    template <unsigned I>  // Termination specialization
    class loop2MultiplyS<I,1> {
    public:
        static void execute(const SymMatrix<T,SX> &l,const SymMatrix<T,SX> &m,Matrix<T,SX,SX> &ret) {
            ret.t_[SX-I][SX-1] = innerLoopS<I,1,SX>::execute(l,m);
        }
    };
    // Using template meta-programming to unroll outermost loop of matrix multiply.
    template <unsigned I>
    class loopMultiplyS {
    public:
        static void execute(const SymMatrix<T,SX> &l,const SymMatrix<T,SX> &m,Matrix<T,SX,SX> &ret) {
            loop2MultiplyS<I,SX>::execute(l,m,ret);
            loopMultiplyS<I-1>::execute(l,m,ret);
        }
    };
#ifndef __LINUX
    template <> // Termination specialization
    class loopMultiplyS<1> {
    public:
        static void execute(const SymMatrix<T,SX> &l,const SymMatrix<T,SX> &m,Matrix<T,SX,SX> &ret) {
            loop2MultiplyS<1,SX>::execute(l,m,ret);
        }
    };
#endif

public:
    /// Default constructor, does nothing and no initialization.
#ifdef _DEBUG
    SymMatrix() { set(initVal<T>()); }
#else
    SymMatrix()  { }
#endif
    /// Explicit constructor, initializes all elements to value t.
    explicit SymMatrix(const T &t) { set(t); }
    /// Copy constructor
    SymMatrix(const SymMatrix<T,SX> &m) {  memcpy(t_,m.t_,sizeof(T)*len_); }
    /// Equality operator
    const SymMatrix<T,SX> &operator=(const SymMatrix<T,SX> &m) { memcpy(t_,m.t_,sizeof(T)*len_);  return *this; }

    /// Retrieve value at row \a x column \a y.   Bounds checking is done in a debug compile.
    const T & get(unsigned x,unsigned y) const { return t_[index(x,y)]; }
    /// Retrieve value at row \a x column \a y.   Bounds checking is done in a debug compile.
    T &       get(unsigned x,unsigned y) { return t_[index(x,y)]; }
    /// Array access operator returns a Column helper class, which has it's own [] operator to access a column of the row.
    const Column operator[](unsigned y) const { return Column(y,*this); }
    /// Array access operator returns a Column helper class, which has it's own [] operator to access a column of the row.
    Column       operator[](unsigned y) { return Column(y,*this); }
    /// () operator access to get(x,y)
    const T & operator()(unsigned x,unsigned y) const { return get(x,y); }
    /// () operator access to get(x,y)
    T &       operator()(unsigned x,unsigned y) { return get(x,y); }

    /// In-place matrix addition
    const SymMatrix<T,SX> &operator+=(const SymMatrix<T,SX> &m) { for (unsigned i=0;i<len_;++i) t_[i] += m.t_[i]; return *this; }
    /// In-place matrix subtraction
    const SymMatrix<T,SX> &operator-=(const SymMatrix<T,SX> &m) { for (unsigned i=0;i<len_;++i) t_[i] -= m.t_[i]; return *this; }
    /// In-place multiplication by a scalar
    const SymMatrix<T,SX> &operator*=(const T &t) { for (unsigned i=0;i<len_;++i) t_[i] *= t;  return *this; }
    /// In-place division by a scalar
    const SymMatrix<T,SX> &operator/=(const T &t) { for (unsigned i=0;i<len_;++i) t_[i] /= t;  return *this; }

    /// Binary addition operator for a symetric matris.
    SymMatrix<T,SX> operator+(const SymMatrix<T,SX> &m) const { SymMatrix<T,SX> ret; for (unsigned i=0;i<len_;++i) ret.t_[i] = t_[i] + m.t_[i]; return ret; }
    /// Binary addition operator.
    Matrix<T,SX,SX> operator+(const Matrix<T,SX,SX> &m) const { Matrix<T,SX,SX> ret; for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SX;++j) ret(i,j) = get(i,j) + m.get(i,j); return ret; }
    /// Binary subtraction operator for a symetric matrix.
    SymMatrix<T,SX> operator-(const SymMatrix<T,SX> &m) const { SymMatrix<T,SX> ret; for (unsigned i=0;i<len_;++i) ret.t_[i] = t_[i] - m.t_[i]; return ret; }
    /// Binary subtraction operator.
    Matrix<T, SX, SX> operator-(const Matrix<T, SX, SX>& m) const { Matrix<T, SX, SX> ret; for (unsigned i = 0; i < SX; ++i) for (unsigned j = 0; j < SX; ++j) ret(i, j) = get(i, j) - m.get(i, j); return ret; }

    /// Binary scalar multiplication operator for a symetric matrix.
    SymMatrix<T,SX> operator*(const T &t) const { SymMatrix<T,SX> ret; for (unsigned i=0;i<len_;++i) ret.t_[i] = t_[i] * t;  return ret; }
    /// Binary scalar division operator for a symetric matrix.
    SymMatrix<T,SX> operator/(const T &t) const { SymMatrix<T,SX> ret; for (unsigned i=0;i<len_;++i) ret.t_[i] = t_[i] / t;  return ret; }
    /// Binary matrix multiplication operator -- simplest naive approach.
    Matrix<T,SX,SX> operator*(const SymMatrix<T,SX> &m) const {
        Matrix<T,SX,SX> ret;
        loopMultiplyS<SX>::execute(*this,m,ret);
        //for (unsigned i=0;i<SX;++i) { // ROWS OF FIRST
        //    for (unsigned j=0;j<SX;++j) { // COLUMNS OF SECOND
        //        T &val = ret.get(i,j);
        //        val = 0.0;
        //        for (unsigned k=0;k<SX;++k)  // Add up
        //            val += get(i,k)*m.get(k,j);
        //    }
        //}
        return ret;
    }
    /// Binary matrix multiplication operator -- simplest naive approach.
    template <unsigned SZ> Matrix<T,SX,SZ> operator*(const Matrix<T,SX,SZ> &m) const {
        Matrix<T,SX,SZ> ret;
        loopMultiply<SZ,SX>::execute(*this,m,ret);
        //for (unsigned i=0;i<SX;++i) { // ROWS OF FIRST
        //    for (unsigned j=0;j<SY;++j) { // COLUMNS OF SECOND
        //        T &val = ret.get(i,j);
        //        val = 0.0;
        //        for (unsigned k=0;k<SX;++k)  // Add up
        //            val += get(i,k)*m.get(k,j);
        //    }
        //}
        return ret;
    }
    ///// Binary matrix multiplication operator -- specialization for matrix of size 1 in Y
    //template <> Matrix<T,SX,1> operator*(const Matrix<T,SX,1> &m) const {
    //    Matrix<T,SX,1> ret;
    //    for (unsigned i=0;i<SX;++i) { // ROWS OF FIRST
    //        T &val = ret.get(i,0);
    //        val = 0.0;
    //        for (unsigned k=0;k<SX;++k)  // Add up
    //            val += get(i,k)*m.get(k,0);
    //    }
    //    return ret;
    //}

    // Block operations
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from block indices (iSrc,jSrc) to block indices (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2,unsigned SY2> void addBlock(const Matrix<T,SX2,SY2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        addGenBlock<BX,BY,SX2,SY2>(m,iSrc*BX,jSrc*BY,iDst*BX,jDst*BY);
    }
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from block indices (iSrc,jSrc) to block indices (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2> void addBlock(const SymMatrix<T,SX2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        addGenBlock<BX,BY,SX2>(m,iSrc*BX,jSrc*BY,iDst*BX,jDst*BY);
    }
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from normal *not block* indices (iSrc,jSrc) to (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2,unsigned SY2> void addGenBlock(const Matrix<T,SX2,SY2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        for (unsigned i=iSrc,id=iDst;i<(iSrc+BX);i++,id++) {
            for (unsigned j=jSrc,jd=jDst;j<(jSrc+BY);j++,jd++) {
                if (id<=jd)
                    get(id,jd) += m(i,j);
            }
        }
    }
    /// Adds a block to this matrix from matrix m using the block size (BX,BY), from normal *not block* indices (iSrc,jSrc) to (iDst,jDst)
    template <unsigned BX,unsigned BY,unsigned SX2> void addGenBlock(const SymMatrix<T,SX2> &m,unsigned iSrc,unsigned jSrc,unsigned iDst,unsigned jDst) {
        for (unsigned i=iSrc,id=iDst;i<(iSrc+BX);i++,id++) {
            for (unsigned j=jSrc,jd=jDst;j<(jSrc+BY);j++,jd++) {
                if (id<=jd)
                    get(id,jd) += m(i,j);
            }
        }
    }

    /// Returns the infinity norm of the matrix, or the maximum absolute magnitude of any element.
    T maxNorm() const { T ret(0);  for (unsigned i=0;i<len_;++i) ret = std::max(std::abs(t_[i]),ret);  return ret; }
    /// Return the transpose of the matrix
    SymMatrix<T,SX> transpose() const { return *this; }
    /// Return the trace of the matrix or the sum of the diagonal components. Works also if the matrix is not square.
    T trace() const { T tt = t_[0]; for (unsigned i=1; i<SX; ++i) tt += get(i,i); return tt; }
    /// Sets all matrix elements to \b t
    void set(const T &t=T()) { for (unsigned i=0;i<len_;++i) t_[i] = t; }
    /// Returns its copy
    Matrix<T,SX,SX> toMatrix() const { Matrix<T,SX,SX> ret; for (unsigned i=0;i<SX;++i) for (unsigned j=0;j<SX;++j) ret(i,j)=get(i,j);  return ret; }

    /// Returns an identity matrix (or as close as you can get if not diagonal).
    static SymMatrix<T,SX> identity() { SymMatrix<T,SX> ret(0.0);  for (unsigned i=0;i<SX;++i) ret(i,i) = 1.0;  return ret; }
    /// Assign from full matrix.
    static SymMatrix<T,SX> fromMatrix(const Matrix<T,SX,SX> &m) {
        SymMatrix<T,SX> ret;
#ifdef _DEBUG
        double maxdif = 0.0;
#endif
        for (unsigned i=0;i<SX;++i) {
            ret(i,i) = m(i,i);
            for (unsigned j=i+1;j<SX;++j) {
                ret(i,j) = (m(i,j) + m(j,i)) * 0.5;
#ifdef _DEBUG
                maxdif = std::max(std::abs(ret(i,j) - m(i,j)),maxdif);
#endif
            }
        }
#ifdef _DEBUG
        assert(maxdif <= ret.maxNorm()*limits<float>::epsilon());
#endif
        return ret;
    }

    static constexpr unsigned len_ = (SX*SX + SX) / 2;
    static constexpr unsigned indexConst_ = 1 + 2*SX;
    static constexpr unsigned index(unsigned x,unsigned y) {
        unsigned x2 = x > y ? y : x;
        unsigned y2 = x > y ? x : y;
        unsigned i = (((indexConst_-x2)*x2)/2) + (y2-x2);
        return i;
    }

    T t_[len_];
};

// MOO NOTE!:: There are optimizations that can be made here!
/// Multiplies a SX-SY Matrix by a SY symmetric matrix , and returns the resulting SX-SY Matrix.
/// \relates Matrix
// Using template-meta programming to unroll innermost loop of matrix multiply
template <class T,unsigned SX,unsigned SY,unsigned I,unsigned J,unsigned K>
class innerLoopMS {
public:
    static double execute(const Matrix<T,SX,SY> &l,const SymMatrix<T,SY> &m) {
        return l.t_[SX-I][SY-K]*m.t_[m.index(SY-K,SY-J)] + innerLoopMS<T,SX,SY,I,J,K-1>::execute(l,m);
    }
};
template <class T,unsigned SX,unsigned SY,unsigned I,unsigned J> // Termination specialization
class innerLoopMS<T,SX,SY,I,J,1> {
public:
    static double execute(const Matrix<T,SX,SY> &l,const SymMatrix<T,SY> &m) {
        return l.t_[SX-I][SY-1]*m.t_[m.index(SY-1,SY-J)];
    }
};
// Using template meta-programming to unroll second loop of matrix multiply.
template <class T,unsigned SX,unsigned SY,unsigned I,unsigned J>
class loop2MultiplyMS {
public:
    static void execute(const Matrix<T,SX,SY> &l,const SymMatrix<T,SY> &m,Matrix<T,SX,SY> &ret) {
        ret.t_[SX-I][SY-J] = innerLoopMS<T,SX,SY,I,J,SY>::execute(l,m);
        loop2MultiplyMS<T,SX,SY,I,J-1>::execute(l,m,ret);
    }
};
template <class T,unsigned SX,unsigned SY,unsigned I>  // Termination specialization
class loop2MultiplyMS<T,SX,SY,I,1> {
public:
    static void execute(const Matrix<T,SX,SY> &l,const SymMatrix<T,SY> &m,Matrix<T,SX,SY> &ret) {
        ret.t_[SX-I][SY-1] = innerLoopMS<T,SX,SY,I,1,SY>::execute(l,m);
    }
};
// Using template meta-programming to unroll outermost loop of matrix multiply.
template <class T,unsigned SX,unsigned SY,unsigned I>
class loopMultiplyMS {
public:
    static void execute(const Matrix<T,SX,SY> &l,const SymMatrix<T,SY> &m,Matrix<T,SX,SY> &ret) {
        loop2MultiplyMS<T,SX,SY,I,SY>::execute(l,m,ret);
        loopMultiplyMS<T,SX,SY,I-1>::execute(l,m,ret);
    }
};
template <class T,unsigned SX,unsigned SY> // Termination specialization
class loopMultiplyMS<T,SX,SY,1> {
public:
    static void execute(const Matrix<T,SX,SY> &l,const SymMatrix<T,SY> &m,Matrix<T,SX,SY> &ret) {
        loop2MultiplyMS<T,SX,SY,1,SY>::execute(l,m,ret);
    }
};

template <class T,unsigned SX,unsigned SY>
Matrix<T,SX,SY> operator*(const Matrix<T,SX,SY> &m,const SymMatrix<T,SY> &s) {
    Matrix<T,SX,SY> ret;
    loopMultiplyMS<T,SX,SY,SX>::execute(m,s,ret);
    //for (unsigned i=0;i<SX;++i) { // ROWS OF FIRST
    //    for (unsigned j=0;j<SY;++j) { // COLUMNS OF SECOND
    //        T &val = ret.get(i,j);
    //        val = 0.0;
    //        for (unsigned k=0;k<SY;++k)  // Add up
    //            val += m.get(i,k)*s.get(k,j);
    //    }
    //}
    return ret;
}

/** \brief A 1-Dimensional version of Matrix, to represent a vector.
*
* The SY dimension is fixed at 1.
* May want to specialize the get() function for optimal efficiency later.
*/
template <class T,unsigned S> class VMatrix : public Matrix<T,S,1> {
public:
    /// Default constructor - no data initialization
    VMatrix() : Matrix<T,S,1>() { }
    /// Explicit constructor, all elements initialzed to \a t.
    explicit VMatrix(const T &t) : Matrix<T,S,1>(t) { }
    /// Copy constructor
    VMatrix(const VMatrix<T,S> &m) : Matrix<T,S,1>(m) { }
    /// Copy contructor, works on Matrix if SY is 1.
    VMatrix(const Matrix<T,S,1> &m) : Matrix<T,S,1>(m) { }
    /// Equality operator
    const VMatrix<T,S> &operator=(const VMatrix<T,S> &m) { Matrix<T,S,1>::operator=(m);  return *this; }
    /// Equality operator, works on Matrix is SY is 1.
    const VMatrix<T,S> &operator=(const Matrix<T,S,1> &m) { Matrix<T,S,1>::operator=(m);  return *this; }

    ///// 1D version of get operation, but we may need to add explicity add back get(x,y) because of overloading rules.
    //const T & get(unsigned x) const { return Matrix<T,S,1>::get(x,0); }
    ///// 1D version of get operation, but we may need to add explicity add back get(x,y) because of overloading rules.
    //T &       get(unsigned x) { return Matrix<T,S,1>::get(x,0); }
    /// 1D version of array operator, which currently unfortunately eliminates [x][0] syntax on VMatrix (may need to fix later).
    const T &operator[](unsigned x) const { return this->get(x); }
    /// 1D version of array operator, which currently unfortunately eliminates [x][0] syntax on VMatrix (may need to fix later).
    T &      operator[](unsigned x) { return this->get(x); }
    ///// 1D version of function operator, but we may need to add explicity add back (x,y) because of overloading rules.
    //const T & operator()(unsigned x) const { return get(x); }
    ///// 1D version of function operator, but we may need to add explicity add back (x,y) because of overloading rules.
    //T &       operator()(unsigned x) { return get(x); }
};

/** \brief DMatrix is a Matrix that defaults to type double...
*
*/
template <unsigned SX,unsigned SY=SX> class DMatrix : public Matrix<double,SX,SY> {
public:
    /// Default constructor, no data initialization
    DMatrix() : Matrix<double,SX,SY>() { }
    /// Explicit contructor, initializes all elements to \a t.
    explicit DMatrix(const double &t) : Matrix<double,SX,SY>(t) { }
    /// Copy constructor
    DMatrix(const DMatrix<SX,SY> &m) : Matrix<double,SX,SY>(m) { }
    /// Copy constructor, for Matrix.
    DMatrix(const Matrix<double,SX,SY> &m) : Matrix<double,SX,SY>(m) { }
    /// Equality operator
    const DMatrix<SX,SY> &operator=(const DMatrix<SX,SY> &m) { Matrix<double,SX,SY>::operator=(m);  return *this; }
    /// Equality operator, for Matrix.
    const DMatrix<SX,SY> &operator=(const Matrix<double,SX,SY> &m) { Matrix<double,SX,SY>::operator=(m);  return *this; }
};

/** \brief DSymMatrix is a SymMatrix that defaults to type double...
*
*/
template <unsigned SX> class DSymMatrix : public SymMatrix<double,SX> {
public:
    /// Default constructor, no data initialization
    DSymMatrix() : SymMatrix<double,SX>() { }
    /// Explicit contructor, initializes all elements to \a t.
    explicit DSymMatrix(const double &t) : SymMatrix<double,SX>(t) { }
    /// Copy constructor
    DSymMatrix(const DSymMatrix<SX> &m) : SymMatrix<double,SX>(m) { }
    /// Copy constructor, for SymMatrix.
    DSymMatrix(const SymMatrix<double,SX> &m) : SymMatrix<double,SX>(m) { }
    /// Equality operator
    const DSymMatrix<SX> &operator=(const DSymMatrix<SX> &m) { SymMatrix<double,SX>::operator=(m);  return *this; }
    /// Equality operator, for Matrix.
    const DSymMatrix<SX> &operator=(const SymMatrix<double,SX> &m) { SymMatrix<double,SX>::operator=(m);  return *this; }
};

/** \brief DVMatrix is a double version of VMatrix.
*/
template <unsigned S> class DVMatrix : public VMatrix<double,S> {
public:
    /// Default constructor, no data initialization
    DVMatrix() : VMatrix<double,S>() { }
    /// Explicit contructor, initializes all elements to \a t.
    explicit DVMatrix(const double &d) : VMatrix<double,S>(d) { }
    /// Copy constructor
    DVMatrix(const DVMatrix<S> &m) : VMatrix<double,S>(m) { }
    /// Copy constructor, for Matrix class
    DVMatrix(const Matrix<double,S,1> &m) : VMatrix<double,S>(m) { }
    /// Equality operator
    const DVMatrix<S> &operator=(const DVMatrix<S> &m) { VMatrix<double,S>::operator=(m);  return *this; }
    /// Equality operator, for matrix class
    const DVMatrix<S> &operator=(const Matrix<double,S,1> &m) { VMatrix<double,S>::operator=(m);  return *this; }
};

template <class T,unsigned S>
double innerProduct(const VMatrix<T,S> &v1,const VMatrix<T,S> &v2) {
    double d=0.0;
    for (UInt i=0;i<S;++i)
        d += v1(i)*v2(i);
    return d;
}

template <class T,unsigned S> Matrix<T,S,S> outerProduct(const VMatrix<T,S> &v1,const VMatrix<T,S> &v2) {
    Matrix<T,S,S> ret;
    for (UInt i=0;i<S;++i) {
        for (UInt j=0;j<S;++j)
            ret(i,j) = v1(i) * v2(j);
    }
    return ret;
}

/** \brief Creates a 3X3 Matrix from the outer product of two Vector3 types.
* \relates Matrix
*/
template <class T> Matrix<T,3,3> outerProduct(const Vector3<T> &v1,const Vector3<T> &v2) {
    Matrix<T,3,3> ret;
    ret.get(0,0) = v1.x() * v2.x();
    ret.get(0,1) = v1.x() * v2.y();
    ret.get(0,2) = v1.x() * v2.z();
    ret.get(1,0) = v1.y() * v2.x();
    ret.get(1,1) = v1.y() * v2.y();
    ret.get(1,2) = v1.y() * v2.z();
    ret.get(2,0) = v1.z() * v2.x();
    ret.get(2,1) = v1.z() * v2.y();
    ret.get(2,2) = v1.z() * v2.z();
    return ret;
}

/** \brief Creates a 2X2 Matrix from the outer product of two Vector2 types.
* \relates Matrix
*/
template <class T> Matrix<T,2,2> outerProduct(const Vector2<T> &v1,const Vector2<T> &v2) {
    Matrix<T,2,2> ret;
    ret.get(0,0) = v1.x() * v2.x();
    ret.get(0,1) = v1.x() * v2.y();
    ret.get(1,0) = v1.y() * v2.x();
    ret.get(1,1) = v1.y() * v2.y();
    return ret;
}

/** \brief Returns the determinant of a 3X3 Matrix.
* \relates Matrix
*/
/// MOO NOTE: Eventually want to add specialization for SymMatrix.
template <class T> T determinant(const Matrix<T,3,3> &mat) {
    T t = mat.get(0,0) * (mat.get(1,1) * mat.get(2,2) - mat.get(2,1) * mat.get(1,2));
    t -= mat.get(1,0) * (mat.get(0,1) * mat.get(2,2) - mat.get(2,1) * mat.get(0,2));
    t += mat.get(2,0) * (mat.get(0,1) * mat.get(1,2) - mat.get(1,1) * mat.get(0,2));
    return t;
}

/** \brief Returns the determinant of a 2X2 Matrix.
* \relates Matrix
*/
template <class T> T determinant(const Matrix<T,2,2> &mat) {
    return mat.get(0,0) * mat.get(1,1) - mat.get(0,1) * mat.get(1,0);
}

/** \brief Returns the inverse of a 3X3 Matrix.
* \relates Matrix
*/
/// MOO NOTE: Eventually want to add specialization for SymMatrix.
template <class T> Matrix<T,3,3> inverse(const Matrix<T,3,3> &mat) {
    // First calculate the adjoint
    T a11 = mat.get(0,0), a21 = mat.get(1,0), a31 = mat.get(2,0);
    T a12 = mat.get(0,1), a22 = mat.get(1,1), a32 = mat.get(2,1);
    T a13 = mat.get(0,2), a23 = mat.get(1,2), a33 = mat.get(2,2);

    Matrix<T,3,3> rm;
    rm.get(0,0) = a22*a33 - a32*a23;
    rm.get(1,0) = -a21*a33 + a31*a23;
    rm.get(2,0) = a21*a32 - a31*a22;
    rm.get(0,1) = -a12*a33 + a32*a13;
    rm.get(1,1) = a11*a33 - a31*a13;
    rm.get(2,1) = -a11*a32 + a31*a12;
    rm.get(0,2) = a12*a23 - a22*a13;
    rm.get(1,2) = -a11*a23 + a21*a13;
    rm.get(2,2) = a11*a22 - a21*a12;
    // Divide by the determinant
    return rm / determinant(mat);
}

/// Converts a Vector2 into a VMatrix
/// \relates VMatrix
template <class T> VMatrix<T,2> toMatrix(const Vector2<T> &v) {
    VMatrix<T,2> ret;
    ret.get(0) = v.x();
    ret.get(1) = v.y();
    return ret;
}

/// Converts a Vector3  into a VMatrix
/// \relates VMatrix
template <class T> VMatrix<T,3> toMatrix(const Vector3<T> &v) {
    VMatrix<T,3> ret;
    ret.get(0) = v.x();
    ret.get(1) = v.y();
    ret.get(2) = v.z();
    return ret;
}

/// Converts a Vector3 into a VMatrix of arbitrary size, at an arbitrary starting index
template <class T,unsigned SX> VMatrix<T,SX> toVMatrix(const Vector3<T> &v,unsigned start=0) {
    VMatrix<T,SX> ret(0.0);
    ret.get(start) = v.x();
    ret.get(start+1) = v.y();
    ret.get(start+2) = v.z();
    return ret;
}

/// Converts a DVect3 into a DVMatrix of arbitrary length, at an arbitrary starting index.
/// Specialization of function toVMatrix, for convenience.
template <unsigned SX> DVMatrix<SX> toDVMatrix(const DVect3 &v,unsigned start=0) { return toVMatrix<double,SX>(v,start); }

/// Multiplies a 2X2 Matrix by a Vector2, and returns the resulting Vector2.
/// \relates Matrix
template <class T> Vector2<T> operator*(const Matrix<T,2,2> &m,const Vector2<T> &v) {
    Vector2<T> ret(v.x()*m.get(0,0) + v.y()*m.get(0,1),
        v.x()*m.get(1,0) + v.y()*m.get(1,1));
    return ret;
}

/// Multiplies a 2X2 SymMatrix by a Vector2, and returns the resulting Vector2.
/// \relates SymMatrix
template <class T> Vector2<T> operator*(const SymMatrix<T,2> &m,const Vector2<T> &v) {
    Vector2<T> ret(v.x()*m.get(0,0) + v.y()*m.get(0,1),
                   v.x()*m.get(1,0) + v.y()*m.get(1,1));
    return ret;
}

/// Multiplies a 3X3 Matrix by a Vector3, and returns the resulting Vector3.
/// \relates Matrix
template <class T> Vector3<T> operator*(const Matrix<T,3,3> &m,const Vector3<T> &v) {
    Vector3<T> ret(v.x()*m.get(0,0) + v.y()*m.get(0,1) + v.z()*m.get(0,2),
                   v.x()*m.get(1,0) + v.y()*m.get(1,1) + v.z()*m.get(1,2),
                   v.x()*m.get(2,0) + v.y()*m.get(2,1) + v.z()*m.get(2,2));
    return ret;
}

/// Multiplies a 3X3 SymMatrix by a Vector3, and returns the resulting Vector3.
/// \relates Matrix
template <class T> Vector3<T> operator*(const SymMatrix<T,3> &m,const Vector3<T> &v) {
    Vector3<T> ret(v.x()*m.get(0,0) + v.y()*m.get(0,1) + v.z()*m.get(0,2),
                   v.x()*m.get(1,0) + v.y()*m.get(1,1) + v.z()*m.get(1,2),
                   v.x()*m.get(2,0) + v.y()*m.get(2,1) + v.z()*m.get(2,2));
    return ret;
}

/// Converts a SymTensor into a  Symmetrix 3x3 double matrix (DMatrix).
/// \relates DMatrix
BASE_EXPORT DMatrix<3,3> toMatrix(const SymTensor &s);

/// Converts a SymTensor into a Symmetrix 3x3 double matrix (DSymMatrix).
/// \relates DMatrix
BASE_EXPORT DSymMatrix<3> toSymMatrix(const SymTensor &s);

/// Converts a 3x3 DMatrix into a SymTensor.
/// The diagonal components of \a m are averaged to create a Symmetric result.
/// \relates DMatrix
inline SymTensor toSymTensor(const DMatrix<3,3> &m) {
    SymTensor ret(m.get(0,0),
                  m.get(1,1),
                  m.get(2,2),
                  (m.get(0,1)+m.get(1,0))*0.5,
                  (m.get(0,2)+m.get(2,0))*0.5,
                  (m.get(1,2)+m.get(2,1))*0.5);
    return ret;
}

inline SymTensor toSymTensor(const DMatrix<2,2> &m) {
    SymTensor ret(m.get(0,0),
                  m.get(1,1),
                  0.0,
                  (m.get(0,1)+m.get(1,0))*0.5,
                  0.0,
                  0.0);
    return ret;
}

/// Converts a 3x3 DSymMatrix into a SymTensor.
/// \relates DSymMatrix
inline SymTensor toSymTensor(const DSymMatrix<3> &m) {
    SymTensor ret(m.get(0,0),m.get(1,1),m.get(1,2),
                  m.get(0,1),m.get(0,2),m.get(1,2));
    return ret;
}

/// Converts a VMatrix to a Vector2, using two elements starting at index \a start.
template <class T,unsigned SX> Vector2<T> toVector2(const VMatrix<T,SX> &m,unsigned start=0) { Vector2<T> ret(m(start),m(start+1));  return ret; }

/// Converts a VMatrix to a Vector3, using three elements starting at index \a start.
template <class T,unsigned SX> Vector3<T> toVector3(const VMatrix<T,SX> &m,unsigned start=0) { Vector3<T> ret(m(start),m(start+1),m(start+2));  return ret; }

/// Converts a VMatrix to a Vector3, using three elements starting at index \a start.
template <class T> Vector2<T> toVector(const VMatrix<T,2> &m) { Vector2<T> ret(m(0),m(1));  return ret; }

/// Converts a VMatrix to a Vector3, using three elements starting at index \a start.
template <class T> Vector3<T> toVector(const VMatrix<T,3> &m) { Vector3<T> ret(m(0),m(1),m(2));  return ret; }

/// returns in a Vector2<t> the column \a col from matrix Matrix<T,SX,2>
template <class T,unsigned SY> Vector2<T> columnToVector(const Matrix<T,2,SY> &m,unsigned col) { Vector2<T> ret(m(0,col),m(1,col));  return ret; }
/// returns in a Vector3<t> the column \a col from a matrix Matrix<T,SX,3>
template <class T,unsigned SY> Vector3<T> columnToVector(const Matrix<T,3,SY> &m,unsigned col) { Vector3<T> ret(m(0,col),m(1,col),m(2,col));  return ret; }
/// returns in a Vector2<t> the row \a row from matrix Matrix<T,SX,2>
template <class T,unsigned SX> Vector2<T> rowToVector(const Matrix<T,SX,2> &m,unsigned row) { Vector2<T> ret(m(row,0),m(row,1));  return ret; }
/// returns in a Vector3<t> the row \a row from matrix Matrix<T,SX,3>
template <class T,unsigned SX> Vector3<T> rowToVector(const Matrix<T,SX,3> &m,unsigned row) { Vector3<T> ret(m(row,0),m(row,1),m(row,2));  return ret; }

template <class T,unsigned SY>
void vectorToColumn(Matrix<T,2,SY> &m,const DVect2 &v,unsigned col) { m(0,col) = v.x();  m(1,col) = v.y(); }

template <class T,unsigned SY>
void vectorToColumn(Matrix<T,3,SY> &m,const DVect3 &v,unsigned col) { m(0,col) = v.x();  m(1,col) = v.y();  m(2,col) = v.z(); }

template <class T,unsigned SX>
void vectorToRow(Matrix<T,SX,2> &m,const DVect2 &v,unsigned row) { m(row,0) = v.x();  m(row,1) = v.y(); }

template <class T,unsigned SX>
void vectorToRow(Matrix<T,SX,3> &m,const DVect3 &v,unsigned row) { m(row,0) = v.x();  m(row,1) = v.y();  m(row,2) = v.z(); }

template <class T,unsigned SX,unsigned SY>
VMatrix<T,SX> column(const Matrix<T,SX,SY> &m,unsigned c) {
    VMatrix<T,SX> ret;
    for (unsigned i=0;i<SX;++i)
        ret(i) = m(i,c);
    return ret;
}
/// @}
// EoF
