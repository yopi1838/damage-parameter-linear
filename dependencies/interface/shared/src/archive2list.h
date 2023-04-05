#pragma once

#include "archive2.h"
#include "base/src/farray.h"
#include "base/src/flatarray.h"
#include "shared/src/blockmemarray.h"
#include <array>
#include <set>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// std::list<T>
// std::vector<T,A>
// std::vector<bool,A>
// SinglyLinkedList of T (with next(), next(ptr) methods), uses object save/restore/remap
// std::map<K,T,A,B>
// std::set<K>
// std::multimap<K,T,A,B>
// std::multimap<K,T *,A,B>
// std::unordered_map<K,T,A,B>
// std::unordered_map<K,T *,A,B>
// std::unordered_set<K>
// std::pair<T,V>
// FArray<T>
// std::array<A,N>

#include <array>

namespace archive {
    // Common template for array types
    // T must be STL conforming - specifically value_type, size(), push_back(), begin(), end()
    template <typename T>
    class TraitsArray {
    public:
        static void save(itasca::Archive2 &a, const T &t);
        static bool restore(itasca::Archive2 &a, T &t);
        static void remap(itasca::Archive2 &a, T &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array; }
    };

    // Common template for POINTERS to array types
    // T must be STL conforming - specifically value_type, size(), push_back(), begin(), end()
    template <typename T>
    class TraitsArrayPointer {
    public:
        static void save(itasca::Archive2 &a, const T &t);
        static bool restore(itasca::Archive2 &a, T &t);
        static void remap(itasca::Archive2 &a, T &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array; }
    };

    template <typename T>  // std::list<T>
    class Traits<std::list<T>> : public TraitsArray<std::list<T>> {};

    template <typename T,typename A>  // std::vector<T>
    class Traits<std::vector<T,A>> : public TraitsArray<std::vector<T,A>> {};

    template <typename A> // std::vector<bool>, goddamn it people
    class Traits<std::vector<bool,A>> {
    public:
        static void save(itasca::Archive2 &a, const std::vector<bool,A> &t);
        static bool restore(itasca::Archive2 &a, std::vector<bool,A> &t);
        static void remap(itasca::Archive2 &, std::vector<bool, A> &) {}
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    // Support for singly linked lists - using a head pointer and next(), next(n) methods
    template <typename T>
    class TraitsSinglyLinkedList {
    public: // Interface is different!
        static void save(itasca::Archive2 &a, const itasca::Label &label, const T *head);
        static bool restore(itasca::Archive2 &a, T *&head);
        static void remap(itasca::Archive2 &a, T *head);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    // Generic template for STL map types - iterators are key/value pairs
    // Types need for (auto &a : v) support, insert({k,v}) method, size()
    template <typename T>
    class TraitsMap {
    public:
        static void save(itasca::Archive2 &a, const T &t);
        static bool restore(itasca::Archive2 &a, T &t);
        static void remap(itasca::Archive2 &a, T &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    template <typename K,typename T,typename C,typename A> // std::map<K,T,C,A>
    class Traits<std::map<K, T, C, A>> : public TraitsMap<std::map<K, T, C, A>> {};

    // Generic template for STL set types - iterators are values
    // Types need for (auto &a : v) support, insert(v) method, size()
    template <typename T>
    class TraitsSet {
    public:
        static void save(itasca::Archive2 &a, const T &t);
        static bool restore(itasca::Archive2 &a, T &t);
        static void remap(itasca::Archive2 &a, T &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    template <typename T,typename K> // std::set<T,K>
    class Traits<std::set<T, K>> : public TraitsSet<std::set<T, K>> {};

    // Generic template for STL map types - iterators are key/value pairs
    // Types need for (auto &a : v) support, insert({k,v}) method, size()
    template <typename T>
    class TraitsMapPointer {
    public:
        static void save(itasca::Archive2 &a, const T &t);
        static bool restore(itasca::Archive2 &a, T &t);
        static void remap(itasca::Archive2 &a, T &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    // Need a separate template for Multimaps - they don't return a pair on insert! :(
    // I copy the base templates because other STL map types might do the same (like unordered_multimap)
    template <typename T>
    class TraitsMultiMap {
    public:
        static void save(itasca::Archive2 &a, const T &t);
        static bool restore(itasca::Archive2 &a, T &t);
        static void remap(itasca::Archive2 &a, T &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    template <typename T>
    class TraitsMultiMapPointer {
    public:
        static void save(itasca::Archive2 &a, const T &t);
        static bool restore(itasca::Archive2 &a, T &t);
        static void remap(itasca::Archive2 &a, T &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array;  }
    };

    template <typename K,typename T,typename C,typename A> // std::multimap<K,T,C,A>
    class Traits<std::multimap<K, T, C, A>> : public TraitsMultiMap<std::multimap<K, T, C, A>> {};

    template <typename K,typename T,typename C,typename A> // std::multimap<K,T*,C,A>
    class Traits<std::multimap<K, T *, C, A>> : public TraitsMultiMapPointer<std::multimap<K, T *, C, A>> {};

    template <typename K,typename T,typename C,typename A> // std::unordered_map<K,T,C,A>
    class Traits<std::unordered_map<K, T, C, A>> : public TraitsMap<std::unordered_map<K, T, C, A>> {};

    template <typename K,typename T,typename C,typename A> // std::unordered_map<K, T*, C, A>
    class Traits<std::unordered_map<K, T *, C, A>> : public TraitsMapPointer<std::unordered_map<K, T *, C, A>> { };

    template <typename K,typename H,typename E,typename A> // std::unordered_set<K,T,E,A>
    class Traits<std::unordered_set<K, H, E, A>> : public TraitsSet<std::unordered_set<K, H, E, A>> {};

    template <typename T,typename V> // std::pair
    class Traits<std::pair<T,V>> {
    public:
        static void save(itasca::Archive2 &a, const std::pair<T,V> &t);
        static bool restore(itasca::Archive2 &a, std::pair<T,V> &t);
        static void remap(itasca::Archive2 &a, std::pair<T,V> &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Object;  }
    };

    template <typename T,uint64 S> // FArray<T,S>
    class Traits<FArray<T, S>> : public TraitsArray<FArray<T, S>> {};

    template <typename T> // FlatArray<T>
    class Traits<FlatArray<T>> : public TraitsArray<FlatArray<T>> {};

    template <typename T> // FlatArrayVec<T>
    class Traits<FlatArrayVec<T>> : public TraitsArray<FlatArrayVec<T>> {};

    template <typename T,size_t S> // std::array<T,S>
    class Traits<std::array<T, S>> {
    public:
        static void save(itasca::Archive2 &a, const std::array<T, S> &t);
        static bool restore(itasca::Archive2 &a, std::array<T, S> &t);
        static void remap(itasca::Archive2 &a, std::array<T, S> &t);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array; }
    };

    // Support for C-Style arrays anymore - they really should be refactored away anyway
    template <typename T>
    class TraitsCArray {
    public:
        static void save(itasca::Archive2 &a, const itasca::Label &label, const T *v, uint64 len,
            itasca::Archive2::Type type = Traits<T>::type());
        static std::tuple<bool, uint64> restore(itasca::Archive2 &a, T *v, uint64 len);
        static void remap(itasca::Archive2 &a, T *v, uint64 len);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array; }
    };



    //
    // --------------------- IMPLEMENTATIONS ----------------------
    //

    template <typename T> 
    void TraitsArray<T>::save(itasca::Archive2 &a, const T &t) {
        a.startArraySave(t.size(),Traits<typename T::value_type>::type());
        for (auto &i : t)
            a.saveValue(i);
        a.stopArray();
    }

    template <typename T> 
    bool TraitsArray<T>::restore(itasca::Archive2 &a, T &t) {
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            typename T::value_type i;
            Traits<typename T::value_type>::restore(a,i);
            t.push_back(i);
        }
        return true;
    }

    template <typename T> 
    void TraitsArray<T>::remap(itasca::Archive2 &a, T &t) {
        for (auto &i : t)
            archive::Traits<typename T::value_type>::remap(a,i);
    }

    template <typename T> 
    void TraitsArrayPointer<T>::save(itasca::Archive2 &a, const T &t) {
        using value_type = typename std::remove_pointer<typename T::value_type>::type;
        a.startArraySave(t.size(),Traits<value_type>::type());
        for (auto &i : t)
            a.saveValue(*i);
        a.stopArray();
    }

    template <typename T> 
    bool TraitsArrayPointer<T>::restore(itasca::Archive2 &a, T &t) {
        using value_type = typename std::remove_pointer<typename T::value_type>::type;
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            value_type *i = new value_type{};
            Traits<value_type>::restore(a,*i);
            t.push_back(i);
        }
        return true;
    }

    template <typename T> 
    void TraitsArrayPointer<T>::remap(itasca::Archive2 &a, T &t) {
        using value_type = typename std::remove_pointer<typename T::value_type>::type;
        for (auto &i : t)
            archive::Traits<value_type>::remap(a,*i);
    }

    template <typename A>
    void Traits<std::vector<bool,A>>::save(itasca::Archive2 &a, const std::vector<bool,A> &t) {
        a.startArraySave(t.size(),Traits<bool>::type());
        for (auto i : t)
            a.saveValue(i);
        a.stopArray();
    }

    template <typename A>
    bool Traits<std::vector<bool,A>>::restore(itasca::Archive2 &a, std::vector<bool,A> &t) {
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            bool i;
            Traits<bool>::restore(a,i);
            t.push_back(i);
        }
        return true;
    }

    template <typename T> 
    void TraitsSinglyLinkedList<T>::save(itasca::Archive2 &a,const itasca::Label &label,const T *head) {
        if (label.string_) a.saveLabel(label);
        quint64 count=0;
        for (const T *t=head;t;t=t->next()) 
            ++count;
        a.startArraySave(count,Traits<T>::type());
        for (const T *t=head;t;t=t->next())
            a.saveValue(*t);
        a.stopArray();
    }

    template <typename T> 
    bool TraitsSinglyLinkedList<T>::restore(itasca::Archive2 &a,T *&head) {
        while (head) {
            auto next = head->next();
            delete head;
            head = next;
        }
        a.startArrayRestore();
        T *last = nullptr;
        while (a.checkArray()) {
            T *t = NEWC(T());
            t->next(nullptr);
            if (last) last->next(t);
            else      head = t;
            last = t;
            Traits<T>::restore(a,*t);
        }
        return true;
    }

    template <typename T> 
    void TraitsSinglyLinkedList<T>::remap(itasca::Archive2 &a,T *head) {
        for (auto t=head;t;t=t->next())
            Traits<T>::remap(a,*t);
    }

    template <typename T> 
    void TraitsMap<T>::save(itasca::Archive2 &a,const T &t) {
        a.startArraySave(t.size(),itasca::Archive2::Type::Object);
        for (auto [key,value] : t) {
            a.startObjectSave(typeid(t));
            a.save("Key",key);
            a.save("Value",value);
            a.stopObjectSave();
        }
        a.stopArray();
    }

    template <typename T> 
    bool TraitsMap<T>::restore(itasca::Archive2 &a,T &t) {
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            a.startObjectRestore();
            typename T::key_type k;
            typename T::mapped_type v;
            for (auto lab=a.restoreLabel();lab!=itasca::Archive2::finish_;lab=a.restoreLabel()) {
                switch (lab) {
                case itasca::enc("Key"):  a.restore(k);  break;
                case itasca::enc("Value"):  a.restore(v);  break;
                default:  a.skipValue();  break;
                }
            }
            t.insert({k,v});
            a.stopObjectRestore();
        }
        return true;
    }

    template <typename T> 
    void TraitsMap<T>::remap(itasca::Archive2 &a,T &v) {
        T copy;
        for (auto [key,value] : v) {
            typename T::key_type k{key};
            Traits<typename T::key_type>::remap(a,k);
            auto j = copy.insert({k,value}).first;
            Traits<typename T::mapped_type>::remap(a,j->second);
        }
        v = copy;
    }

    template <typename T> 
    void TraitsSet<T>::save(itasca::Archive2 &a,const T &t) {
        a.startArraySave(t.size(),Traits<typename T::value_type>::type());
        for (auto &v : t)
            a.saveValue(v);
        a.stopArray();
    }

    template <typename T> 
    bool TraitsSet<T>::restore(itasca::Archive2 &a,T &t) {
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            typename T::value_type v;
            Traits<typename T::value_type>::restore(a, v);
            t.insert(v);
        }
        return true;
    }

    template <typename T> 
    void TraitsSet<T>::remap(itasca::Archive2 &a,T &t) {
        T copy;
        for (auto &v : t) {
            typename T::key_type k = v;
            Traits<typename T::key_type>::remap(a,k);
            copy.insert(k);
        }
        t = copy;
    }

    template <typename T> 
    void TraitsMapPointer<T>::save(itasca::Archive2 &a,const T &t) {
        a.startArraySave(t.size(),itasca::Archive2::Type::Object);
        for (auto [key,value] : t) {
            a.startObjectSave(typeid(t));
            a.save("Key",key);
            assert(value);
            a.save("Value",*value);
            a.stopObjectSave();
        }
        a.stopArray();
    }

    template <typename T> 
    bool TraitsMapPointer<T>::restore(itasca::Archive2 &a,T &t) {
        using value_type = typename std::remove_pointer<typename T::mapped_type>::type;
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            a.startObjectRestore();
            typename T::key_type k;
            typename T::mapped_type v = NEWC(value_type{});
            for (auto lab=a.restoreLabel();lab!=itasca::Archive2::finish_;lab=a.restoreLabel()) {
                switch (lab) {
                case itasca::enc("Key"):  a.restore(k);  break;
                case itasca::enc("Value"):  a.restore(*v);  break;
                default:  a.skipValue();  break;
                }
            }
            t.insert({k,v});
            a.stopObjectRestore();
        }
        return true;
    }

    template <typename T> 
    void TraitsMapPointer<T>::remap(itasca::Archive2 &a,T &v) {
        using value_type = typename std::remove_pointer<typename T::mapped_type>::type;
        T copy;
        for (auto [key,value] : v) {
            typename T::key_type k{key};
            Traits<typename T::key_type>::remap(a,k);
            auto j = copy.insert({k,value}).first;
            Traits<value_type>::remap(a,*value);
        }
        v = copy;
    }

    template <typename T,typename V> 
    void Traits<std::pair<T,V>>::save(itasca::Archive2 &a,const std::pair<T,V> &v) {
        a.startObjectSave(typeid(v));
        a.save("First",v.first);
        a.save("Second",v.second);
        a.stopObjectSave();
    }

    template <typename T,typename V> 
    bool Traits<std::pair<T,V>>::restore(itasca::Archive2 &a,std::pair<T,V> &v) {
        a.startObjectRestore();
        for (auto lab=a.restoreLabel();lab!=itasca::Archive2::finish_;lab=a.restoreLabel()) {
            switch (lab) {
            case itasca::enc("First"):  a.restore(v.first);  break;
            case itasca::enc("Second"):  a.restore(v.second);  break;
            default:  a.skipValue();  break;
            }
        }
        a.stopObjectRestore();
        return true;
    }

    template <typename T,typename V> 
    void Traits<std::pair<T,V>>::remap(itasca::Archive2 &a,std::pair<T,V> &v) {
        Traits<T>::remap(a,v->first);
        Traits<V>::remap(a,v->second);
    }

    template <typename T,size_t N> 
    void Traits<std::array<T,N>>::save(itasca::Archive2 &a,const std::array<T,N> &v) {
        a.startArraySave(N,Traits<T>::type());
        for (auto &i : v) 
            a.saveValue(i);
        a.stopArray();
    }

    template <typename T,size_t N> 
    bool Traits<std::array<T,N>>::restore(itasca::Archive2 &a,std::array<T,N> &v) {
        a.startArrayRestore();
        uint64 s(0);
        while (a.checkArray()) {
            assert(s<N);
            v[s] = a.restore<T>();
            ++s;
        }
        return true;
    }

    template <typename T,size_t N> 
    void Traits<std::array<T,N>>::remap(itasca::Archive2 &a,std::array<T,N> &v) {
        for (auto &i : v)
            Traits<T>::remap(a,i);
    }

    template <typename T> 
    void TraitsMultiMap<T>::save(itasca::Archive2 &a,const T &t) {
        a.startArraySave(t.size(),itasca::Archive2::Type::Object);
        for (auto [key,value] : t) {
            a.startObjectSave(typeid(t));
            a.save("Key",key);
            a.save("Value",value);
            a.stopObjectSave();
        }
        a.stopArray();
    }

    template <typename T> 
    bool TraitsMultiMap<T>::restore(itasca::Archive2 &a,T &t) {
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            a.startObjectRestore();
            typename T::key_type k;
            typename T::mapped_type v;
            for (auto lab=a.restoreLabel();lab!=itasca::Archive2::finish_;lab=a.restoreLabel()) {
                switch (lab) {
                case itasca::enc("Key"):  a.restore(k);  break;
                case itasca::enc("Value"):  a.restore(v);  break;
                default:  a.skipValue();  break;
                }
            }
            t.insert({k,v});
            a.stopObjectRestore();
        }
        return true;
    }

    template <typename T> 
    void TraitsMultiMap<T>::remap(itasca::Archive2 &a,T &v) {
        T copy;
        for (auto [key,value] : v) {
            typename T::key_type k{key};
            Traits<typename T::key_type>::remap(a,k);
            auto j = copy.insert({k,value});
            Traits<typename T::mapped_type>::remap(a,j->second);
        }
        v = copy;
    }

    template <typename T> 
    void TraitsMultiMapPointer<T>::save(itasca::Archive2 &a,const T &t) {
        a.startArraySave(t.size(),itasca::Archive2::Type::Object);
        for (auto [key,value] : t) {
            a.startObjectSave(typeid(t));
            a.save("Key",key);
            assert(value);
            a.save("Value",*value);
            a.stopObjectSave();
        }
        a.stopArray();
    }

    template <typename T> 
    bool TraitsMultiMapPointer<T>::restore(itasca::Archive2 &a,T &t) {
        using value_type = typename std::remove_pointer<typename T::mapped_type>::type;
        t.clear();
        a.startArrayRestore();
        while (a.checkArray()) {
            a.startObjectRestore();
            typename T::key_type k;
            typename T::mapped_type v = NEWC(value_type{});
            for (auto lab=a.restoreLabel();lab!=itasca::Archive2::finish_;lab=a.restoreLabel()) {
                switch (lab) {
                case itasca::enc("Key"):  a.restore(k);  break;
                case itasca::enc("Value"):  a.restore(*v);  break;
                default:  a.skipValue();  break;
                }
            }
            t.insert({k,v});
            a.stopObjectRestore();
        }
        return true;
    }

    template <typename T> 
    void TraitsMultiMapPointer<T>::remap(itasca::Archive2 &a,T &v) {
        using value_type = typename std::remove_pointer<typename T::mapped_type>::type;
        T copy;
        for (auto [key,value] : v) {
            typename T::key_type k{key};
            Traits<typename T::key_type>::remap(a,k);
            copy.insert({k,value});
            Traits<value_type>::remap(a,*value);
        }
        v = copy;
    }

    template <typename T>
    void TraitsCArray<T>::save(itasca::Archive2 &a, const itasca::Label &label, const T *v, uint64 len, 
                                itasca::Archive2::Type type) {
        if (label.string_) a.saveLabel(label);
        a.startArraySave(len,type);
        auto end = v + len;
        for (auto p=v;p<end;++p)
            a.saveValue(*p);
        a.stopArray();
    }

    template <typename T>
    std::tuple<bool,uint64> TraitsCArray<T>::restore(itasca::Archive2 &a, T *v, uint64 len) {
        a.startArrayRestore();
        uint64 index(0);
        while (a.checkArray()) {
            if (index>=len) {
                a.warning("Too many objects for array.");
                a.skipValue();
            } else
                Traits<T>::restore(a,v[index]);
            ++index;
        }
        return {true,index};
    }

    template <typename T>
    void TraitsCArray<T>::remap(itasca::Archive2 &a, T *v, uint64 len) {
        auto end = v + len;
        for (auto p=v;p<end;++p)
            Traits<T>::remap(a,*p);
    }

    template <typename T,itasca::BlockMemory &B>
    class Traits<itasca::BlockMemArray<T,B>> {
    public:
        static void save(itasca::Archive2 &a, const itasca::BlockMemArray<T,B> &v);
        static bool restore(itasca::Archive2 &a, itasca::BlockMemArray<T, B> &v);
        static void remap(itasca::Archive2 &a, itasca::BlockMemArray<T, B> &v);
        static itasca::Archive2::Type type() { return itasca::Archive2::Type::Array; }

    };

    template <typename T,itasca::BlockMemory &B> 
    void Traits<itasca::BlockMemArray<T,B>>::save(itasca::Archive2 &a,const itasca::BlockMemArray<T,B> &v) {
        a.startArraySave(v.size(),Traits<T>::type());
        for (auto &i : v)
            a.saveValue(i);
        a.stopArray();
    }
    
    template <typename T,itasca::BlockMemory &B> 
    bool Traits<itasca::BlockMemArray<T,B>>::restore(itasca::Archive2 &a,itasca::BlockMemArray<T,B> &v) {
        v.clear();
        FArray<T,128> f;
        a.startArrayRestore();
        while (a.checkArray()) {
            T t;
            Traits<T>::restore(a,t);
            f.push_back(t);
        }
        v.resize(to<int>(f.size()));
        for (decltype(f.size()) i=0;i<f.size();++i)
            v[to<int>(i)] = f[i];
        return true;
    }
    
    template <typename T,itasca::BlockMemory &B> 
    void Traits<itasca::BlockMemArray<T,B>>::remap(itasca::Archive2 &a,itasca::BlockMemArray<T,B> &v) {
        for (auto &i : v)
            Traits<T>::remap(a,i);
    }
} // namespace archive
// EoF
