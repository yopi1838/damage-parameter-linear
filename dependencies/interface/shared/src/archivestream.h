#pragma once
// archivestream.h

#include "exportshared.h"
#include <iostream>
#include <streambuf>
#include <vector>
using namespace std;

class QDataStream;

class SHARED_EXPORT ArchiveBufferResources {
public:
    ArchiveBufferResources(size_t s);
    virtual ~ArchiveBufferResources();
    virtual void reset(size_t s);
protected:
    char * reallocbuf(size_t s);
protected:
    char *bfptr_;
    size_t size_;
    size_t inisize_;
};

class ArchiveBuffer : private ArchiveBufferResources, public streambuf {
public:
    SHARED_EXPORT ArchiveBuffer(size_t s);
    SHARED_EXPORT size_t inisize() {return inisize_;}
    SHARED_EXPORT size_t size() {return size_;}
    SHARED_EXPORT size_t shrinkToFit();
    SHARED_EXPORT void reset(size_t s);
    SHARED_EXPORT streamsize writeToStream(QDataStream &ds,streamsize n);
    SHARED_EXPORT streamsize readFromStream(QDataStream &ds,streamsize n);
protected:
    virtual ArchiveBuffer *setbuf(char *s,streamsize n);
    char * reallocbuf();
    void setpp(char_type *first,char_type *next,char_type *last);
private:
    int_type underflow();
    int_type overflow(int_type ch);
};

class ArchiveStream : public ArchiveBuffer, public iostream {
public:
    enum ArchiveState {Save=0,Restore};
    SHARED_EXPORT ArchiveStream(size_t s,ArchiveState state=ArchiveStream::Save);
    SHARED_EXPORT void reset(size_t s,ArchiveState state);
    SHARED_EXPORT size_t bufferSize() {return shrinkToFit();}
    SHARED_EXPORT ArchiveState getArchiveState() const {return state_;}
    SHARED_EXPORT void setRestoreVersion(UInt i) {restoreVersion_ = i;}
    SHARED_EXPORT UInt getRestoreVersion() const {return restoreVersion_;}
    SHARED_EXPORT void setCount(UInt i) {count_ = i;}
    SHARED_EXPORT UInt getCount() const {return count_;}

private:
    ArchiveState state_;
    UInt restoreVersion_;
    UInt count_;
};

template <class T> inline ArchiveStream &operator&(ArchiveStream &a,T &t) { 
    if (a.getArchiveState()==ArchiveStream::Save) {
        a.write(reinterpret_cast<const char*>(&t),sizeof(t)); 
    } else {
        a.read(reinterpret_cast<char*>(&t),sizeof(t));
        if (a.fail())
            throw std::runtime_error("Memory overflow in ArchiveStream.");
    }
    a.setCount(a.getCount()+1);
    return a;
}

inline ArchiveStream &operator&(ArchiveStream &a,String &s) {
    s;
    throw Exception("Archive error saving/restoring string type.");
    // MOO NOTE:  Talk to me - using << here is structured text IO and probably isn't doing what you think!
    //if (a.getArchiveState()==ArchiveStream::Save)
    //    a << s;
    //else
    //    a >> s;
    //a.setCount(a.getCount()+1);
    return a; 
}

template <class T> inline ArchiveStream &operator&(ArchiveStream &a,std::vector<T> &t) { 
    if (a.getArchiveState()==ArchiveStream::Save) {
        int size = t.size();
        a & size;
        for (int i=0;i<size;i++) {
            T v = t[i]; 
            a & v;
        }
    } else {
        int size(0);
        a & size;
        t.resize(size);
        for (int i=0;i<size;i++) {
            T v;
            a & v;
            t[i]=v;
        }
    }
    a.setCount(a.getCount()+1);
    return a;
}

// EoF
