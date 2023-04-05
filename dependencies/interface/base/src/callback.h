#pragma once

// Note - VS2010 does not support variadic templates :(

template <class R> 
class Callback0 {
public:
    virtual R execute()=0;
};

template <class R,class Arg1>
class Callback1 {
public:
    virtual R execute(Arg1 a)=0;
};

template <class R,class Arg1,class Arg2>
class Callback2 {
public:
    virtual R execute(Arg1 a,Arg2 b)=0;
};

template <class R,class Arg1,class Arg2,class Arg3>
class Callback3 {
public:
    virtual R execute(Arg1 a,Arg2 b,Arg3 c)=0;
};

template <class R,class Arg1,class Arg2,class Arg3,class Arg4>
class Callback4 {
public:
    virtual R execute(Arg1 a,Arg2 b,Arg3 c,Arg4 d)=0;
};

template <class R>
class Callback0Static : public Callback0<R> {
public:
    typedef R (*type)();
    Callback0Static(type t) : t_(t) { }
    virtual R execute() { return t_(); }
private:
    type t_;
};

template <class R,class Arg1>
class Callback1Static : public Callback1<R,Arg1> {
public:
    typedef R (*type)(Arg1 a);
    Callback1Static(type t) : t_(t) { }
    virtual R execute(Arg1 a) { return t_(a); }
private:
    type t_;
};

template <class R,class Arg1,class Arg2>
class Callback2Static : public Callback2<R,Arg1,Arg2> {
public:
    typedef R (*type)(Arg1 a,Arg2 b);
    Callback2Static(type t) : t_(t) { }
    virtual R execute(Arg1 a,Arg2 b) { return t_(a,b); }
private:
    type t_;
};

template <class R,class Arg1,class Arg2,class Arg3>
class Callback3Static : public Callback3<R,Arg1,Arg2,Arg3> {
public:
    typedef R (*type)(Arg1 a,Arg2 b,Arg3 c);
    Callback3Static(type t) : t_(t) { }
    virtual R execute(Arg1 a,Arg2 b,Arg3 c) { return t_(a,b,c); }
private:
    type t_;
};

template <class R,class Arg1,class Arg2,class Arg3,class Arg4>
class Callback4Static : public Callback4<R,Arg1,Arg2,Arg3,Arg4> {
public:
    typedef R (*type)(Arg1 a,Arg2 b,Arg3 c,Arg4 d);
    Callback4Static(type t) : t_(t) { }
    virtual R execute(Arg1 a,Arg2 b,Arg3 c,Arg4 d) { return t_(a,b,c,d); }
private:
    type t_;
};

template <class R,class S,R (S::*MFP)()>
class Callback0Method : public Callback0<R> {
public:
    Callback0Method(S *s) : s_(s) { }
    virtual R execute() { return (s_->*MFP)(); }
private:
    S *s_;
};

template <class R,class Arg1,class S,R (S::*MFP)(Arg1)>
class Callback1Method : public Callback1<R,Arg1> {
public:
    Callback1Method(S *s) : s_(s) { }
    virtual R execute(Arg1 a) { return (s_->*MFP)(a); }
private:
    S *s_;
};

template <class R,class Arg1,class Arg2,class S,R (S::*MFP)(Arg1,Arg2)>
class Callback2Method : public Callback2<R,Arg1,Arg2> {
public:
    Callback2Method(S *s) : s_(s) { }
    virtual R execute(Arg1 a,Arg2 b) { return (s_->*MFP)(a,b); }
private:
    S *s_;
};

template <class R,class Arg1,class Arg2,class Arg3,class S,R (S::*MFP)(Arg1,Arg2,Arg3)>
class Callback3Method : public Callback3<R,Arg1,Arg2,Arg3> {
public:
    Callback3Method(S *s) : s_(s) { }
    virtual R execute(Arg1 a,Arg2 b,Arg3 c) { return (s_->*MFP)(a,b,c); }
private:
    S *s_;
};

template <class R,class Arg1,class Arg2,class Arg3,class Arg4,class S,R (S::*MFP)(Arg1,Arg2,Arg3,Arg4)>
class Callback4Method : public Callback4<R,Arg1,Arg2,Arg3,Arg4> {
public:
    Callback4Method(S *s) : s_(s) { }
    virtual R execute(Arg1 a,Arg2 b,Arg3 c,Arg4 d) { return (s_->*MFP)(a,b,c,d); }
private:
    S *s_;
};

// EoF
