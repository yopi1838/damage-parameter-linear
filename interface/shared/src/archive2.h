#pragma once
#if defined __LINUX || __WINCMAKE 
#include "stdafx.h"
#endif
#include "exportshared.h"


namespace itasca {
    class Archive2;
    class IThing;
    class Mat;

    namespace archive_utility {
        constexpr uint64 encode_char(uint64 c) {
            return (c>=65)&&(c<=90) ? c-64 : // Capital map to 1-26
                   ((c>=97)&&(c<=122) ? c-96 :  // Lower case map to 1-26
                   ((c>=48)&&(c<=52) ? c-21 :  // Digits 0-4 map to 27-31
                    0)); // All other characters map to 0
        }
        constexpr uint64 encode_helper(const char *arr,unsigned N,unsigned i) {
            return i<N-1 ? encode_char(arr[i]) + (encode_helper(arr,N,i+1) << 5) : 0;
        }
    }
    template <unsigned N> constexpr uint64 enc(const char (&arr)[N]) {
        return archive_utility::encode_helper(arr,N,0);
    }
    SHARED_EXPORT uint64 runtime_encode(const QString &str);
    SHARED_EXPORT QString runtime_decode(uint64 encode);
    struct Label {
        Label(uint64 e,const char *s) : encode_(e), string_(s) { }
        Label(void *v) : encode_(0), string_(nullptr) { v;  assert(v==nullptr); }
        template <unsigned N> constexpr Label(const char (&arr)[N]) : encode_(enc(arr)), string_(arr) { }

        uint64     encode_;
        const char *string_;
    };

    namespace archive_utility {
        // These crazy class are to fix a problem with deeply nested data structures.   
        // Since the Archive2 system saves/restores/remaps this stuff with recursive function calls, you can blow out the stack.
        // This is part of a system that detects for that, and continues the save/restore/remap in a separate thread with
        //    a new stack - for as many times as is necessary.
        template <typename T> class StackThreadSave : public QThread {
        public:
            StackThreadSave(Archive2 &a,const Label &label,T t,const std::type_info &ti) :
                a_(a), label_(label), t_(t), ti_(ti) { }
            void rethrow() { if (exception_.length()) throw Exception(exception_); }
        protected:
            virtual void run();
        private:
            Archive2 &            a_;
            Label                 label_;
            T                     t_;
            const std::type_info &ti_;
            QString               exception_;
        };

        template <typename T> class StackThreadRestore : public QThread {
        public:
            StackThreadRestore(Archive2 &a,T t) : a_(a), t_(t) { }
            bool ret() const { return ret_; }
            void rethrow() { if (exception_.length()) throw Exception(exception_); }
        protected:
            virtual void run();
        private:
            Archive2 &a_;
            T         t_;
            bool      ret_ = false;
            QString   exception_;
        };

        template <typename T> class StackThreadRemap : public QThread {
        public:
            StackThreadRemap(Archive2 &a,T &t) : a_(a), t_(t) { }
            void rethrow() { if (exception_.length()) throw Exception(exception_); }
        protected:
            virtual void run();
        private:
            Archive2 &a_;
            T &       t_;
            QString   exception_;
        };
    }

#ifdef __INTEL_COMPILER
#  pragma warning(disable:3503)
#endif
    class SHARED_EXPORT Archive2 {
    public:
        friend class Private;

        enum class State { No=0, Save, Restore, Remap };
        enum class Mode { Binary = 0, Text = 1 };
        enum class WhichOS { Windows = 0, Linux = 1 };
        enum class Target { Save=0, Results=1, Buffer=2, Project=3 }; // No remap allowed on buffer or project targets.
        enum class Type { None=0     , Bool=1           , QInt8=2     , QInt16=3       , QInt32=4   , 
                          QInt64=5   , QUInt8=6         , QUInt16=7   , QUInt32=9      , QUInt64=10 ,
                          Float=11   , QString=12       , Object=13   , Double=14      , DVect2=15  ,
                          DVect3=16  , IVect2=17        , IVect3=18   , UVect2=19      , UVect3=20  ,
                          Array=21   , OptionalObject=22, SymTensor=23, Mat=24         , String=25  ,
                          Variant=26 , Quat2=27         , Quat3=28    , QVariant=29    , DExtent2=30,
                          DExtent3=31, CAxes2D=32       , CAxes3D=33  , Orientation2=34, Orientation3=35,
                          Axes2D=36  , Axes3D=37        , DAVect2=38 };
        enum class ArrayFormat { Inline, Newline, Auto };
        enum class FileFormat { Original=0, BytePack=1 };
        static const uint64 start_ = enc("ArchStart");
        static const uint64 finish_ = enc("ArchFinish");
        static QString limitDoubleMax_;
        static QString limitDoubleMin_;

        Archive2(QIODevice *dev,State state,Mode mode,Target target);
        ~Archive2();

        // Configuration control
        void       setState(State state);
        State      state() const;
        bool       isSaveState() const { return state() == State::Save; }
        bool       isRestore() const { return state() == State::Restore; }
        bool       isRemap() const { return state() == State::Remap; }
        bool       isSaveTarget() const { return target_==Target::Save; }
        bool       isResults() const { return target_==Target::Results; }
        bool       isBuffer() const { return target_==Target::Buffer; }
        bool       isProject() const { return target_==Target::Project; }
        bool       allowRemap() const { return isSaveTarget() || isResults(); }
        void       setMode(Mode mode);
        Mode       mode() const;
        void       setRestoreFromOS(WhichOS v);
        WhichOS    restoreFromOS() const;
        bool       isText() const { return mode()==Mode::Text; }
        void       setDevice(QIODevice *dev);
        QIODevice *getDevice();
        void       setSixtyFour(bool b);
        bool       sixtyFour() const;
        FileFormat getRestoreFormat(); 
        void       setRestoreFormat(FileFormat format);
        FileFormat getSaveFormat(); 
        void       setSaveFormat(FileFormat format);
        void       setReportSkippedLabels(bool b) { reportSkippedLabels_ = b; }

        // High Level Control
        bool        init();
        bool        skipValue();
        void        cleanup();
        void        warning(const QString &s,const QVariant &v1=QVariant(),const QVariant &v2=QVariant(),
                            const QVariant &v3=QVariant(),const QVariant &v4=QVariant());  ///< \copydoc getDataStream
        QStringList warnings();

        bool    saveLabel(const Label &label);
        uint64 restoreLabel();

        void    startObjectSave(const std::type_info &ti);
        void    startObjectRestore();
        void    stopObjectSave();
        void    stopObjectRestore();

        void    startArraySave(uint64 count,Type type,ArrayFormat format=ArrayFormat::Auto);
        void    startArrayRestore();
        bool    checkArray();
        void    stopArray();

        uint64  nextIndex() { uint64 ret = nextIndex_;  ++nextIndex_;  return ret; }

        template <typename T> void save(const Label &label,const T &t);
        template <typename T> void saveValue(const T &t);
        template <typename T> void saveObjFunc(const Label &label,T t,const std::type_info &ti);
        template <typename T> void saveObj(const Label &label,const T &t) { saveObjFunc(label,[&](Archive2 &a){t.save(a);},typeid(t)); }
        template <typename T> void saveOptionalObj(const Label &label,const T *t);
        template <typename T> void saveObjFuncStack(const Label &label,T t,const std::type_info &ti);

        template <typename T> T    restore() { T t;  restore(t);  return t; }
        template <typename T> bool restore(T &t) { return restoreValue(t); }
        template <typename T> bool restoreValue(T& t);
        template <typename T> bool restoreObjFunc(T t);
        template <typename T> bool restoreObjFuncStack(T t);
        template <typename T> bool restoreObj(T &t) { restoreObjFunc([&](Archive2 &a,const uint64 label)->bool{return t.restore(a,label);});  return true; }
        template <typename T> bool restoreOptionalObj(T *&t); 

        template <typename T> void remap(T& t); 
        template <typename T> void remapObj(T &t);
        template <typename T> void remapObjStack(T &t);
        template <typename T> void remapOptionalObj(T *t) { assert(allowRemap()); if (t) t->remap(*this); }
        template <typename T> T &  getContextInfo(const QString &name) { return reinterpret_cast<T &>(*getContextInfoVoid(name)); }
        template <typename T> Type getType(const T &t);

        class Private;
        Private *priv() { return p_; }

        uint32 instance() { return instance_; }

        static bool isRemapArchiveActive() { return remapArchiveActive_; }

        // Generic header class
        struct SHARED_EXPORT GenericSaveFileHeader {
        public:
            GenericSaveFileHeader() { }
            GenericSaveFileHeader(const QString &type,const QString &name,double version,uint32 update,bool binary);

            virtual void save(Archive2 &a) const;
            virtual bool restore(Archive2 &a,uint64 label);
            virtual QStringList describe() const;
            void output(QIODevice *out) const;

            static void saveHeader(Archive2 &a,const GenericSaveFileHeader &headIn);
            static void restoreHeader(Archive2 &a,const QString &typeToCompare,GenericSaveFileHeader *headOut);

            QString              type_;
            QString              name_;
            float                version_ = 0.0;
            uint32               update_ = 0;
            QDateTime            dateTime_;
            bool                 binary_ = true;
            Archive2::FileFormat format_ = Archive2::FileFormat::Original; // Set during *save* as a.getSaveFormat(), in case of changes.
        };

        // Occasionally it is useful to add context information to the archive class (where to get info from, etc).
        // Since adding parameter to restore() is problematic (without a LOT of template futzing) we can add
        //    named type data here.  It is assumed data is on the stack, and it's lifetime is at least until
        //    AddContextInfo is destroyed.  This is dangerous, but efficient.
        template <typename T> 
        class AddContextInfo {
        public:
            AddContextInfo(Archive2 &a,const QString &name,T &data) : a_(a), name_(name) { a.addContextInfoVoid(name,&data); }
            ~AddContextInfo() { a_.removeContextInfoVoid(name_); }
        private:
            Archive2 &a_;
            QString name_;
        };

    private:
        // Increment the nest count and decrement again when done
        struct IncNestDepth {
            IncNestDepth(Archive2 &a) : a_(a) {  
                ++a.nestDepth_;
#ifdef _DEBUG
                ++a.totalNestDepth_;
                a.maxNestDepth_ = std::max(a.maxNestDepth_,a.totalNestDepth_);
#endif
            }
            ~IncNestDepth() {
                --a_.nestDepth_;
#ifdef _DEBUG
                --a_.totalNestDepth_;
#endif
            }
            Archive2 &a_;
        };
        // Reset the nest count for a new thread stack, and put back when done.
        struct ResetNestDepth {
            ResetNestDepth(Archive2 &a) : a_(a), savedDepth_(a.nestDepth_) { a.nestDepth_ = 0; }
            ~ResetNestDepth() { a_.nestDepth_ = savedDepth_; }
            Archive2 &a_;
            uint32 savedDepth_;
        };
        
        void  addDivider();
        void  addCount();
        void  startOptionalObjectSaveBin(bool b);
        void  stopOptionalObjectSaveBin();
        bool  startOptionalObjectRestoreBin();
        void  stopOptionalObjectRestoreBin();
        char *getContextInfoVoid(const QString &name);
        void  addContextInfoVoid(const QString &name,void *v);
        void  removeContextInfoVoid(const QString &name);

        Private *p_;
        uint64  nextIndex_;
        Target   target_;
        bool     reportSkippedLabels_;
        uint32  instance_;
        uint32  nestDepth_=0; // Count of the depth of saveObj/restoreObj/remapObj call in this stack frame
#ifdef _DEBUG
        uint32  totalNestDepth_=0; // Count of the depth in all stack frames
        uint32  maxNestDepth_=0; // Maximum depth reached
#endif

        static uint32 nextInstance_;
        static const uint32 nestDepthLimit_ = 512; // Maximum recursion into save/restore/remap allowed before you need a new stack.
        static bool remapArchiveActive_;
    };

    template <typename T> void archive_utility::StackThreadSave<T>::run() {
        try {
            a_.saveObjFunc(label_, t_, ti_);
        }
        catch (std::exception& e) {
            exception_ = e.what();
        }
    }

    template <typename T> void archive_utility::StackThreadRestore<T>::run() {
        try {
            ret_ = a_.restoreObjFunc(t_);
        }
        catch (std::exception& e) {
            exception_ = e.what();
        }
    }

    template <typename T> void archive_utility::StackThreadRemap<T>::run() {    
        try {
            a_.remapObj(t_);
        }
        catch (std::exception& e) {
            exception_ = e.what();
        }
    }

#ifdef __INTEL_COMPILER
#    pragma warning(enable:3503)
#endif

    template <typename T> void Archive2::save(const Label &label,const T &value) {
        if (label.string_) saveLabel(label);
        saveValue(value);
    }

    template <typename T> void Archive2::saveObjFunc(const Label &label,T t,const std::type_info &ti) {
        IncNestDepth inc(*this);
        if (nestDepth_>nestDepthLimit_) {
            saveObjFuncStack(label,t,ti);
            return;
        }
        if (label.string_) saveLabel(label); // null label means skip label, value only
        startObjectSave(ti);
        t(*this);
        stopObjectSave();
    }

    template <typename T> void Archive2::saveObjFuncStack(const Label &label,T t,const std::type_info &ti) {
        ResetNestDepth res(*this);

        archive_utility::StackThreadSave<T> sthread(*this,label,t,ti);
        sthread.start();
        sthread.wait();
        sthread.rethrow();
    }

    template <typename T> bool Archive2::restoreObjFunc(T t) {
        IncNestDepth inc(*this);
        if (nestDepth_>nestDepthLimit_)
            return restoreObjFuncStack(t);

        startObjectRestore();
        t(*this,start_);
        for (;;) {
            auto label =  restoreLabel();
            bool ret = t(*this,label);
            if (label==finish_) break;
            if (!ret) {
                if (reportSkippedLabels_) {
                    warning("Skipping label %1.",runtime_decode(label));
                    qDebug() << "Skipping label " << runtime_decode(label);
                }
                skipValue();
            }
        }
        stopObjectRestore();
        return true;
    }

    template <typename T> bool Archive2::restoreObjFuncStack(T t) {
        ResetNestDepth res(*this);

        archive_utility::StackThreadRestore<T> sthread(*this,t);
        sthread.start();
        sthread.wait();
        sthread.rethrow();
        return sthread.ret();
    }

    template <typename T> void Archive2::remapObj(T &t) {
        assert(allowRemap());
        IncNestDepth inc(*this);
        if (nestDepth_>nestDepthLimit_) {
            remapObjStack(t);
            return;
        }

        t.remap(*this); 
    }

    template <typename T> void Archive2::remapObjStack(T &t) {
        assert(allowRemap());
        ResetNestDepth res(*this);

        archive_utility::StackThreadRemap<T> sthread(*this,t);
        sthread.start();
        sthread.wait();
        sthread.rethrow();
    }

    template <typename T> void Archive2::saveOptionalObj(const Label &label,const T *t) {
        if (label.string_) saveLabel(label);
        startArraySave(t ? 1 : 0, Archive2::Type::Object,ArrayFormat::Inline);
            if (t)
                saveObj(nullptr,*t);
        stopArray();
    }

    template <typename T> bool Archive2::restoreOptionalObj(T *&t) {
        startArrayRestore();
        if (checkArray()) {
            if (!t) t = NEWC(T());
            restoreObj(*t);
            if (checkArray()) throw Exception("Optional object format error.");
        } else if (t) {
            delete t;
            t = nullptr;
        }
        return true;
    }

    template <typename T> void saveToBuffer(QBuffer &buf,const T &source) {
        buf.open(QBuffer::WriteOnly);
        {   Archive2 a(&buf,Archive2::State::Save,Archive2::Mode::Text,Archive2::Target::Buffer);
            a.init();
            a.saveObj("Object",source);
        }
        buf.close();
    }

    // Note:  Does not call remap, assumes anything that can be atomically archived doesn't need it.
    template <typename T> void restoreFromBuffer(QBuffer &buf,T &dest,bool reportSkippedLabels=true) {
        buf.open(QIODevice::ReadOnly);
        Archive2 a(&buf,Archive2::State::Restore,Archive2::Mode::Text,Archive2::Target::Buffer);
        a.setReportSkippedLabels(reportSkippedLabels);
        a.init();
        a.restoreLabel();
        a.restoreObj(dest);
    }

    // Note:  Does not call remap, assumes anything that can be atomically archived doesn't need it.
    template <typename T> void copyObject(T &dest,const T &source,bool reportSkippedLabels=true) {
        QBuffer buf;
        saveToBuffer(buf,source);
        restoreFromBuffer(buf,dest,reportSkippedLabels);
    }

} // namespace itasca

namespace archive {
    // Overloads for base type save/restore/getType support  (remap never necessary)
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const bool &v);
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a, const char &v);
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const int8 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const uint8 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const int16 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const uint16 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const int32 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const uint32 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const int64 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const uint64 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const float &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const double &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const String &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const QString &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const DVect2 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const DVect3 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const IVect2 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const IVect3 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const UVect2 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const UVect3 &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const QVariant &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Variant &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Orientation2 &o);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Orientation3 &o);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Axes2D &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Axes3D &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const SymTensor &s);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Quat2 &o);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Quat3 &o);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const itasca::Mat &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const Variant &v);  
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const DExtent2 &v);
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const DExtent3 &v);
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const CAxes2D &v);
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const CAxes3D &v);
    SHARED_EXPORT void saveBaseType(itasca::Archive2 &a,const DAVect2 &v);

    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,bool &v);
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,char &v);
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,int8 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,uint8 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,int16 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,uint16 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,int32 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,uint32 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,int64 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,uint64 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,float &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,double &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,String &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,QString &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,DVect2 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,DVect3 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,IVect2 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,IVect3 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,UVect2 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,UVect3 &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,QVariant &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Variant &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Orientation2 &o);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Orientation3 &o);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Axes2D &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Axes3D &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,SymTensor &s);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Quat2 &o);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Quat3 &o);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,itasca::Mat &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,Variant &v);  
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,DExtent2 &v);
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,DExtent3 &v);
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,CAxes2D &v);
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,CAxes3D &v);
    SHARED_EXPORT bool restoreBaseType(itasca::Archive2 &a,DAVect2 &v);

    template <typename T> itasca::Archive2::Type getBaseType() { static_assert(sizeof(T{})==0, "Base Base Type");  return itasca::Archive2::Type::Object;  }
    template<> inline itasca::Archive2::Type getBaseType<bool       >() { return itasca::Archive2::Type::Bool;      }
    template<> inline itasca::Archive2::Type getBaseType<char       >() { return itasca::Archive2::Type::QInt8;     }
    template<> inline itasca::Archive2::Type getBaseType<int8       >() { return itasca::Archive2::Type::QInt8;     }
    template<> inline itasca::Archive2::Type getBaseType<int16      >() { return itasca::Archive2::Type::QInt16;    }
    template<> inline itasca::Archive2::Type getBaseType<int32      >() { return itasca::Archive2::Type::QInt32;    }
    template<> inline itasca::Archive2::Type getBaseType<int64      >() { return itasca::Archive2::Type::QInt64;    }
    template<> inline itasca::Archive2::Type getBaseType<uint8      >() { return itasca::Archive2::Type::QUInt8;    }
    template<> inline itasca::Archive2::Type getBaseType<uint16     >() { return itasca::Archive2::Type::QUInt16;   }
    template<> inline itasca::Archive2::Type getBaseType<uint32     >() { return itasca::Archive2::Type::QUInt32;   }
    template<> inline itasca::Archive2::Type getBaseType<uint64     >() { return itasca::Archive2::Type::QUInt64;   }
    template<> inline itasca::Archive2::Type getBaseType<float       >() { return itasca::Archive2::Type::Float;     }
    template<> inline itasca::Archive2::Type getBaseType<double      >() { return itasca::Archive2::Type::Double;    }
    template<> inline itasca::Archive2::Type getBaseType<String      >() { return itasca::Archive2::Type::String;    }
    template<> inline itasca::Archive2::Type getBaseType<QString     >() { return itasca::Archive2::Type::QString;   }
    template<> inline itasca::Archive2::Type getBaseType<DVect2      >() { return itasca::Archive2::Type::DVect2;    }
    template<> inline itasca::Archive2::Type getBaseType<DVect3      >() { return itasca::Archive2::Type::DVect3;    }
    template<> inline itasca::Archive2::Type getBaseType<IVect2      >() { return itasca::Archive2::Type::IVect2;    }
    template<> inline itasca::Archive2::Type getBaseType<IVect3      >() { return itasca::Archive2::Type::IVect3;    }
    template<> inline itasca::Archive2::Type getBaseType<UVect2      >() { return itasca::Archive2::Type::UVect2;    }
    template<> inline itasca::Archive2::Type getBaseType<UVect3      >() { return itasca::Archive2::Type::UVect3;    }
    template<> inline itasca::Archive2::Type getBaseType<SymTensor   >() { return itasca::Archive2::Type::SymTensor; }
    template<> inline itasca::Archive2::Type getBaseType<itasca::Mat >() { return itasca::Archive2::Type::Mat;       }
    template<> inline itasca::Archive2::Type getBaseType<Variant     >() { return itasca::Archive2::Type::Variant;   }
    template<> inline itasca::Archive2::Type getBaseType<Quat2       >() { return itasca::Archive2::Type::Quat2;     }
    template<> inline itasca::Archive2::Type getBaseType<Quat3       >() { return itasca::Archive2::Type::Quat3;     }
    template<> inline itasca::Archive2::Type getBaseType<QVariant    >() { return itasca::Archive2::Type::QVariant;  }
    template<> inline itasca::Archive2::Type getBaseType<DExtent2    >() { return itasca::Archive2::Type::DExtent2;  }
    template<> inline itasca::Archive2::Type getBaseType<DExtent3    >() { return itasca::Archive2::Type::DExtent3;  }
    template<> inline itasca::Archive2::Type getBaseType<CAxes2D     >() { return itasca::Archive2::Type::CAxes2D;   }
    template<> inline itasca::Archive2::Type getBaseType<CAxes3D     >() { return itasca::Archive2::Type::CAxes3D;   }
    template<> inline itasca::Archive2::Type getBaseType<Orientation2>() { return itasca::Archive2::Type::Orientation2; }  
    template<> inline itasca::Archive2::Type getBaseType<Orientation3>() { return itasca::Archive2::Type::Orientation3; }  
    template<> inline itasca::Archive2::Type getBaseType<Axes2D      >() { return itasca::Archive2::Type::Axes2D;   }
    template<> inline itasca::Archive2::Type getBaseType<Axes3D      >() { return itasca::Archive2::Type::Axes3D;   }
    template<> inline itasca::Archive2::Type getBaseType<DAVect2     >() { return itasca::Archive2::Type::DAVect2;   }
    template<> inline itasca::Archive2::Type getBaseType<DAVect3     >() { return itasca::Archive2::Type::DVect3;    }

    // Traits class - so we can provide template specialization for custom types
    //                that let the Archive2 system know how to save/restore them
    template <typename T> 
    class Traits {
    public:
        static void save(itasca::Archive2 &, const T &) { static_assert(sizeof(T{})==0, "Bad archive::Traits"); }
        static bool restore(itasca::Archive2 &, T &) { static_assert(sizeof(T{})==0, "Bad archive::Traits"); return false;  }
        static void remap(itasca::Archive2 &, T &) { static_assert(sizeof(T{})==0, "Bad archive::Traits"); }
        static itasca::Archive2::Type type() { static_assert(sizeof(T{})==0, "Bad archive::Traits"); return itasca::Archive2::Type::Object; }
    };

    // Common template for base traits we provide overloaded support for
    template <typename T> 
    class TraitsBaseType {
    public:
        static void save(itasca::Archive2 &a, const T &t) { saveBaseType(a,t); }
        static bool restore(itasca::Archive2 &a, T &t) { return restoreBaseType(a, t);  }
        static void remap(itasca::Archive2 &, T &) { }
        static itasca::Archive2::Type type() { return getBaseType<T>(); }
    };

    // Common template for types that have object save support (save/restore remap methods)
    template <typename T>
    class TraitsObject {
    public:
        static void save(itasca::Archive2 &a, const T &t) { a.saveObj(nullptr,t); }
        static bool restore(itasca::Archive2 &a, T &t) { return a.restoreObj(t);  }
        static void remap(itasca::Archive2 &a, T &t) { a.remapObj(t);  }
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Object; }
    };

    // Traits specializaitions for all the base types.
    template <> class Traits<bool       > : public TraitsBaseType<bool       > {};
    template <> class Traits<char       > : public TraitsBaseType<char       > {};
    template <> class Traits<int8       > : public TraitsBaseType<int8       > {};
    template <> class Traits<int16      > : public TraitsBaseType<int16      > {};
    template <> class Traits<int32      > : public TraitsBaseType<int32      > {};
    template <> class Traits<int64      > : public TraitsBaseType<int64      > {};
    template <> class Traits<uint8      > : public TraitsBaseType<uint8      > {};
    template <> class Traits<uint16     > : public TraitsBaseType<uint16     > {};
    template <> class Traits<uint32     > : public TraitsBaseType<uint32     > {};
    template <> class Traits<uint64     > : public TraitsBaseType<uint64     > {};
    template <> class Traits<float       > : public TraitsBaseType<float       > {};
    template <> class Traits<double      > : public TraitsBaseType<double      > {};
    template <> class Traits<String      > : public TraitsBaseType<String      > {};
    template <> class Traits<QString     > : public TraitsBaseType<QString     > {};
    template <> class Traits<DVect2      > : public TraitsBaseType<DVect2      > {};
    template <> class Traits<DVect3      > : public TraitsBaseType<DVect3      > {};
    template <> class Traits<IVect2      > : public TraitsBaseType<IVect2      > {};
    template <> class Traits<IVect3      > : public TraitsBaseType<IVect3      > {};
    template <> class Traits<UVect2      > : public TraitsBaseType<UVect2      > {};
    template <> class Traits<UVect3      > : public TraitsBaseType<UVect3      > {};
    template <> class Traits<SymTensor   > : public TraitsBaseType<SymTensor   > {};
    template <> class Traits<itasca::Mat > : public TraitsBaseType<itasca::Mat > {};
    template <> class Traits<Variant     > : public TraitsBaseType<Variant     > {};
    template <> class Traits<Quat2       > : public TraitsBaseType<Quat2       > {};
    template <> class Traits<Quat3       > : public TraitsBaseType<Quat3       > {};
    template <> class Traits<QVariant    > : public TraitsBaseType<QVariant    > {};
    template <> class Traits<DExtent2    > : public TraitsBaseType<DExtent2    > {};
    template <> class Traits<DExtent3    > : public TraitsBaseType<DExtent3    > {};
    template <> class Traits<CAxes2D     > : public TraitsBaseType<CAxes2D     > {};
    template <> class Traits<CAxes3D     > : public TraitsBaseType<CAxes3D     > {};
    template <> class Traits<Orientation2> : public TraitsBaseType<Orientation2> {};
    template <> class Traits<Orientation3> : public TraitsBaseType<Orientation3> {};
    template <> class Traits<Axes2D      > : public TraitsBaseType<Axes2D      > {};
    template <> class Traits<Axes3D      > : public TraitsBaseType<Axes3D      > {};
    template <> class Traits<DAVect2     > : public TraitsBaseType<DAVect2     > {};
    template <> class Traits<DAVect3     > : public TraitsBaseType<DAVect3     > {};

} // namespace archive

namespace itasca {
    template <typename T> 
    bool Archive2::restoreValue(T& t) { 
        return archive::Traits<T>::restore(*this, t); 
    }

    template <typename T> 
    void Archive2::remap(T& t) { 
        assert(allowRemap()); 
        archive::Traits<T>::remap(*this, t); 
    }

    template <typename T> 
    void Archive2::saveValue(const T& t) {
        addDivider();
        archive::Traits<T>::save(*this, t);
        addCount();
    }

    template <typename T>
    Archive2::Type Archive2::getType(const T &) {
        return archive::Traits<T>::type();
    }
}
// EoF
