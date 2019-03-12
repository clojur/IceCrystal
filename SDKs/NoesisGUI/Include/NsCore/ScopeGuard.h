////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_SCOPEGUARD_H__
#define __CORE_SCOPEGUARD_H__


namespace Noesis
{

// ScopeGuard implementation adapted from 
// http://www.ddj.com/cpp/184403758
// http://www.zete.org/people/jlehrer/scopeguard.html


/*
  Scopeguard, by Andrei Alexandrescu and Petru Marginean, December 2000.
  Modified by Joshua Lehrer, FactSet Research Systems, November 2005.
*/

template <class T>
class RefHolder
{
    T& ref_;
public:
    RefHolder(T& ref) : ref_(ref) {}
    operator T& () const 
    {
        return ref_;
    }
private:
    // Disable assignment - not implemented
    RefHolder& operator=(const RefHolder&);
};

template <class T>
inline RefHolder<T> ByRef(T& t)
{
    return RefHolder<T>(t);
}

class ScopeGuardImplBase
{
    ScopeGuardImplBase& operator=(const ScopeGuardImplBase&);
protected:
    ~ScopeGuardImplBase()
    {
    }
    ScopeGuardImplBase(const ScopeGuardImplBase& other) throw() 
        : dismissed_(other.dismissed_)
    {
        other.Dismiss();
    }
    template <typename J>
    static void SafeExecute(J& j) throw() 
    {
        if (!j.dismissed_)
        {
            j.Execute();
        }
    }

    mutable bool dismissed_;
public:
    ScopeGuardImplBase() throw() : dismissed_(false) 
    {
    }
    void Dismiss() const throw() 
    {
        dismissed_ = true;
    }
};

typedef const ScopeGuardImplBase& ScopeGuard;

template <typename F>
class ScopeGuardImpl0 : public ScopeGuardImplBase
{
public:
    static ScopeGuardImpl0<F> MakeGuard(F fun)
    {
        return ScopeGuardImpl0<F>(fun);
    }
    ~ScopeGuardImpl0() throw() 
    {
        SafeExecute(*this);
    }
    void Execute() 
    {
        fun_();
    }
protected:
    ScopeGuardImpl0(F fun) : fun_(fun) 
    {
    }
    F fun_;
};

template <typename F> 
inline ScopeGuardImpl0<F> MakeGuard(F fun)
{
    return ScopeGuardImpl0<F>::MakeGuard(fun);
}

template <typename F, typename P1>
class ScopeGuardImpl1 : public ScopeGuardImplBase
{
public:
    static ScopeGuardImpl1<F, P1> MakeGuard(F fun, P1 p1)
    {
        return ScopeGuardImpl1<F, P1>(fun, p1);
    }
    ~ScopeGuardImpl1() throw() 
    {
        SafeExecute(*this);
    }
    void Execute()
    {
        fun_(p1_);
    }
protected:
    ScopeGuardImpl1(F fun, P1 p1) : fun_(fun), p1_(p1) 
    {
    }
    F fun_;
    const P1 p1_;
};

template <typename F, typename P1> 
inline ScopeGuardImpl1<F, P1> MakeGuard(F fun, P1 p1)
{
    return ScopeGuardImpl1<F, P1>::MakeGuard(fun, p1);
}

template <typename F, typename P1, typename P2>
class ScopeGuardImpl2: public ScopeGuardImplBase
{
public:
    static ScopeGuardImpl2<F, P1, P2> MakeGuard(F fun, P1 p1, P2 p2)
    {
        return ScopeGuardImpl2<F, P1, P2>(fun, p1, p2);
    }
    ~ScopeGuardImpl2() throw() 
    {
        SafeExecute(*this);
    }
    void Execute()
    {
        fun_(p1_, p2_);
    }
protected:
    ScopeGuardImpl2(F fun, P1 p1, P2 p2) : fun_(fun), p1_(p1), p2_(p2) 
    {
    }
    F fun_;
    const P1 p1_;
    const P2 p2_;
};

template <typename F, typename P1, typename P2>
inline ScopeGuardImpl2<F, P1, P2> MakeGuard(F fun, P1 p1, P2 p2)
{
    return ScopeGuardImpl2<F, P1, P2>::MakeGuard(fun, p1, p2);
}

template <typename F, typename P1, typename P2, typename P3>
class ScopeGuardImpl3 : public ScopeGuardImplBase
{
public:
    static ScopeGuardImpl3<F, P1, P2, P3> MakeGuard(F fun, P1 p1, P2 p2, P3 p3)
    {
        return ScopeGuardImpl3<F, P1, P2, P3>(fun, p1, p2, p3);
    }
    ~ScopeGuardImpl3() throw() 
    {
        SafeExecute(*this);
    }
    void Execute()
    {
        fun_(p1_, p2_, p3_);
    }
protected:
    ScopeGuardImpl3(F fun, P1 p1, P2 p2, P3 p3) : fun_(fun), p1_(p1), p2_(p2), p3_(p3) 
    {
    }
    F fun_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
};

template <typename F, typename P1, typename P2, typename P3>
inline ScopeGuardImpl3<F, P1, P2, P3> MakeGuard(F fun, P1 p1, P2 p2, P3 p3)
{
    return ScopeGuardImpl3<F, P1, P2, P3>::MakeGuard(fun, p1, p2, p3);
}

//************************************************************

template <class Obj, typename MemFun>
class ObjScopeGuardImpl0 : public ScopeGuardImplBase
{
public:
    static ObjScopeGuardImpl0<Obj, MemFun> MakeObjGuard(Obj& obj, MemFun memFun)
    {
        return ObjScopeGuardImpl0<Obj, MemFun>(obj, memFun);
    }
    ~ObjScopeGuardImpl0() throw() 
    {
        SafeExecute(*this);
    }
    void Execute() 
    {
        (obj_.*memFun_)();
    }
protected:
    ObjScopeGuardImpl0(Obj& obj, MemFun memFun) 
        : obj_(obj), memFun_(memFun) {}
    Obj& obj_;
    MemFun memFun_;
};

template <class Obj, typename Ret>
inline ScopeGuardImpl0<Ret(*)()> MakeGuard(Obj*, Ret(*memFun)())
{
    return ScopeGuardImpl0<Ret(*)()>::MakeGuard(memFun);
}

template <typename Ret, class Obj1, class Obj2>
inline ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)()> MakeGuard(Obj1 *obj, Ret(Obj2::*memFun)())
{
  return ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)()>::MakeObjGuard(*obj,memFun);
}

template <typename Ret, class Obj1, class Obj2>
inline ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)() const> MakeGuard(Obj1 *obj,
    Ret(Obj2::*memFun)() const)
{
  return ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)() const>::MakeObjGuard(*obj,memFun);
}

template <class Obj, typename MemFun, typename P1>
class ObjScopeGuardImpl1 : public ScopeGuardImplBase
{
public:
    static ObjScopeGuardImpl1<Obj, MemFun, P1> MakeObjGuard(Obj& obj, MemFun memFun, P1 p1)
    {
        return ObjScopeGuardImpl1<Obj, MemFun, P1>(obj, memFun, p1);
    }
    ~ObjScopeGuardImpl1() throw() 
    {
        SafeExecute(*this);
    }
    void Execute() 
    {
        (obj_.*memFun_)(p1_);
    }
protected:
    ObjScopeGuardImpl1(Obj& obj, MemFun memFun, P1 p1) 
        : obj_(obj), memFun_(memFun), p1_(p1) {}
    Obj& obj_;
    MemFun memFun_;
    const P1 p1_;
};

template <class Obj, typename Ret, typename P1a, typename P1b>
inline ScopeGuardImpl1<Ret(*)(P1a), P1b> MakeGuard(Obj*, Ret(*memFun)(P1a), P1b p1)
{
    return ScopeGuardImpl1<Ret(*)(P1a), P1b>::MakeGuard(memFun, p1);
}

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b>
inline ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b> MakeGuard(Obj1 *obj, Ret(Obj2::*memFun)(P1a),
    P1b p1) 
{
    return ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b>::MakeObjGuard(*obj,memFun,p1);
}

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b>
inline ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a) const,P1b> MakeGuard(Obj1 *obj,
    Ret(Obj2::*memFun)(P1a) const, P1b p1) 
{
    return ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a) const ,P1b>::MakeObjGuard(*obj,memFun,p1);
}

template <class Obj, typename MemFun, typename P1, typename P2>
class ObjScopeGuardImpl2 : public ScopeGuardImplBase
{
public:
    static ObjScopeGuardImpl2<Obj, MemFun, P1, P2> MakeObjGuard(Obj& obj, MemFun memFun, P1 p1, 
        P2 p2)
    {
        return ObjScopeGuardImpl2<Obj, MemFun, P1, P2>(obj, memFun, p1, p2);
    }
    ~ObjScopeGuardImpl2() throw()
    {
        SafeExecute(*this);
    }
    void Execute() 
    {
        (obj_.*memFun_)(p1_, p2_);
    }
protected:
    ObjScopeGuardImpl2(Obj& obj, MemFun memFun, P1 p1, P2 p2) 
        : obj_(obj), memFun_(memFun), p1_(p1), p2_(p2) {}
    Obj& obj_;
    MemFun memFun_;
    const P1 p1_;
    const P2 p2_;
};

template <class Obj, typename Ret, typename P1a, typename P1b, typename P2a, typename P2b>
inline ScopeGuardImpl2<Ret(*)(P1a, P2a), P1b, P2b> MakeGuard(Obj*, Ret(*memFun)(P1a, P2a),
    P1b p1, P2b p2)
{
    return ScopeGuardImpl2<Ret(*)(P1a, P2a), P1b, P2b>::MakeGuard(memFun, p1, p2);
}

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b, typename P2a, 
    typename P2b>
inline ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b> MakeGuard(Obj1 *obj,
    Ret(Obj2::*memFun)(P1a,P2a), P1b p1, P2b p2) 
{
    return ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b>::MakeObjGuard(*obj,memFun,p1,p2);
}

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b, typename P2a, 
    typename P2b>
inline ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a) const,P1b,P2b> MakeGuard(Obj1 *obj,
    Ret(Obj2::*memFun)(P1a,P2a) const, P1b p1, P2b p2) 
{
    return ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a) const,P1b,P2b>::MakeObjGuard(*obj,
        memFun, p1, p2);
}

}

// This is the original code from the author that gives a warning on Visual Studio 2005 due to
// “initialized but not referenced?warning. I prefer to use a variadic macro than globally
// disabling that warning.
//
//#define CONCATENATE_DIRECT(s1, s2) s1##s2
//#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)
//#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
//
//#define ON_BLOCK_EXIT ScopeGuard ANONYMOUS_VARIABLE(scopeGuard) = MakeGuard

#define __SCOPEGUARD_CONCAT__(a, b) __SCOPEGUARD_CONCAT0__(a,b)
#define __SCOPEGUARD_CONCAT0__(a, b) a##b

#define NS_ON_SCOPE_EXIT(...) \
    Noesis::ScopeGuard __SCOPEGUARD_CONCAT__(scope, __LINE__) = Noesis::MakeGuard( \
        __VA_ARGS__); \
    NS_UNUSED(__SCOPEGUARD_CONCAT__(scope, __LINE__));

#endif
