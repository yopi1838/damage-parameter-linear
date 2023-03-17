#pragma once
/** \file basebool.h
  * \brief Base C++ types, kept out of basedef.h in case one needs to include it in C files
  * \addtogroup Base Base interface specification
  * @{
  */
#include "basedef.h"
#include <string>
#include <cctype>
#define FMT_API BASE_EXPORT
#define FMT_EXTERN_TEMPLATE_API BASE_EXPORT
#ifdef _WIN32
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#endif
#ifdef INTELCOMP
#  pragma warning(disable:177)
#  pragma warning(disable:280) // selector expression is constant
#endif
#include "base/fmt/format.h"
#pragma warning(pop)
using wstring = std::wstring; // Old string class to avoid interface compatibility breaks
using string = std::string; // Bring into global namespace like the other base types (uint32, etc)
using String = std::wstring;  // DEPRECATED - should not be used in new code.

BASE_EXPORT bool isInt(const string &in,int32 *i=nullptr);
BASE_EXPORT bool isUInt(const string &in,uint32 *u=nullptr); ///< \overload
BASE_EXPORT bool isLong(const string &in,int64 *l=nullptr); ///< \overload
BASE_EXPORT bool isULong(const string &in,uint64 *ul=nullptr); ///< \overload
BASE_EXPORT bool isDouble(const string &in,double *d=nullptr); ///< \overload
BASE_EXPORT bool isBool(const string &in,bool *b=nullptr,const string &out="on,off,true,false,yes,no");

inline int32  toInt(const string &in) { int32 ret(0);  isInt(in, &ret); return ret; }
inline uint32 toUInt(const string &in) { uint32 ret(0);  isUInt(in, &ret); return ret; } ///< \overload
inline int64  toLong(const string &in) { int64 ret(0);  isLong(in, &ret); return ret; }; ///< \overload
inline uint64 toULong(const string &in) { uint64 ret(0);  isULong(in, &ret); return ret; }; ///< \overload
inline double toDouble(const string &in) { double ret(0);  isDouble(in, &ret); return ret; }; ///< \overload
inline bool   toBool(const string &in, const string &out = "on,off,true,false,yes,no") { bool b{false};  isBool(in, &b, out); return b; }

class StringList : public std::vector<string> {
public:
    using std::vector<string>::vector;

    StringList() {}
    StringList(std::initializer_list<string> list) : std::vector<string>(list) {}
    StringList(const StringList &s) : std::vector<string>(s) {}
    StringList(const std::vector<string> &v) : std::vector<string>(v) {}
    StringList(StringList &&v) noexcept : std::vector<string>(std::move(v)) {}
    StringList(std::vector<string> &&v) noexcept : std::vector<string>(std::move(v)) {}

    const StringList &operator=(const StringList &in) { std::vector<string>::operator=(in);  return *this; }

    const StringList &operator+=(const string &s) { push_back(s);  return *this; }
    const StringList &operator+=(const std::vector<string> &v) { for (auto &s : v) push_back(s); return *this; }
    const StringList &operator<<(const string &s) { return operator+=(s);  }
    StringList operator+(const StringList &s) const { StringList ret(*this);  ret += s; return ret;  }
};

//BASE_EXPORT string      tostd::string(const std::u16string &s);
BASE_EXPORT string              tostring(const std::wstring &s);
BASE_EXPORT std::wstring        towstring(const string &s);
BASE_EXPORT int32               caseInsensitiveCompare(const string &s1, const string &s2);
BASE_EXPORT string              toUpper(const string &s);
BASE_EXPORT string              toLower(const string &s);
BASE_EXPORT string              join(const std::vector<string> &s,const string &sep);
BASE_EXPORT std::vector<string> split(const string &s, const string &sep,bool keepEmptyParts=false);
BASE_EXPORT string              trimmed(const string &s); // Remove whitespace at beginning and end

// < and == functors for strings - to use in std containers when CI comparisons are wanted!
struct StringCILess {
public:
    bool operator()(const string &left, const string &right) const { 
        return caseInsensitiveCompare(left,right) < 0;
    }
};
struct StringCIEqual {
public:
    bool operator()(const string &left, const string &right) const { 
        return caseInsensitiveCompare(left,right) == 0;
    }
};
struct StringCIHash {
public:
    uint32 operator()(const string &in) const {
        uint32 h = 0;
        auto len = in.size();
        auto *p = in.data();
        for (size_t i = 0; i<len; ++i)
            h = 31 * h + std::toupper(p[i]);
        return h;
    }
};

// Might want to move this to a dedicated header so not everybody has to include these containers

#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

template <typename T> using StringMap = std::map<string, T, StringCILess>;

template <typename T> using StringHashMap = std::unordered_map<string, T, StringCIHash, StringCIEqual>;

template <typename T> using StringSet = std::set<string, T, StringCILess>;

template <typename T> using StringHashSet = std::unordered_set<string, T, StringCIHash, StringCIEqual>;

/// @}

// EoF
