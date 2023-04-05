#pragma once

#include "exportfish.h"
#include "shareddata.h"
#include "base/src/mat.h"

namespace itasca {
    class Archive2;
}

namespace fish {
    using itasca::Archive2;
    class Array;
    class List;

    class MatrixBase : public SharedData<MatrixBase>, 
                       public itasca::Mat {
    public:
        MatrixBase() { }
        MatrixBase(const MatrixBase &b) : Mat(b) { }
        //MatrixBase(MatrixBase &&b) : Mat(std::move(b)) { }
        MatrixBase(const itasca::Mat &m) : itasca::Mat(m) { }
        MatrixBase(itasca::Mat &&m) : itasca::Mat(std::move(m)) { }
        ~MatrixBase() { }

        virtual MatrixBase *getT() { return this;  }
        virtual const MatrixBase *getT() const { return this;  }
        FISH_EXPORT virtual void save(Archive2 &a) const;
        FISH_EXPORT virtual void restore(Archive2 &a);
        FISH_EXPORT virtual void remap(Archive2 &a);
    };

    // Uses copy-on-write
    class Matrix {
    public:
        Matrix() { d_ = NEWC(MatrixBase()); }
        Matrix(const Matrix &m) : d_(m.d_) { }
        Matrix(const itasca::Mat &m) { d_ = NEWC(MatrixBase(m)); }
        Matrix(Matrix &&m) noexcept : d_(std::move(m.d_)) { }
        Matrix(itasca::Mat &&m) noexcept { d_ = NEWC(MatrixBase(std::move(m)));  }
        ~Matrix() { }

        const Matrix &operator=(const Matrix &m) { d_ = m.d_; return *this; }
        const Matrix &operator=(Matrix &&m) noexcept { d_ = std::move(m.d_);  return *this;  }
        const Matrix &operator=(const itasca::Mat &m) { d_ = NEWC(MatrixBase(m)); return *this; }
        const Matrix &operator=(itasca::Mat &&m) noexcept { d_ = NEWC(MatrixBase(std::move(m)));  return *this; }
        bool operator==(const Matrix &m) const;
        bool operator<(const Matrix &m) const;

        QString toString() const;

        itasca::Mat &getForSet() { return *d_; }
        const itasca::Mat &get() const { return *d_; }

        void save(Archive2 &a) const;
        bool restore(Archive2 &a,quint64 label);
        void remap(Archive2 &a);
    private:
        SharedDataPointer<MatrixBase> d_;
    };

    Matrix matrixFromArray(const Array &arr);
    Matrix matrixFromList(const List &arr);
} // namespace fish
// EoF

