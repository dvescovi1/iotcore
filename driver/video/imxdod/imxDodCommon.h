// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
//
// Module Name:
//
//  imxDodCommon.h
//
// Abstract:
//
//    This is IMXDOD shared utilities include file
//
// Environment:
//
//    Kernel mode only.
//

//
// Macros to be used for proper PAGED/NON-PAGED code placement
//

#ifndef _IMXDODCOMMON_HPP_
#define _IMXDODCOMMON_HPP_ 1

#define IMXDOD_NONPAGED_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    //__pragma(code_seg(.text))

#define IMXDOD_NONPAGED_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXDOD_PAGED_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    __pragma(code_seg("PAGE"))

#define IMXDOD_PAGED_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXDOD_INIT_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    __pragma(code_seg("INIT"))

#define IMXDOD_INIT_SEGMENT_END \
    __pragma(code_seg(pop))

//
// We have some non-paged functions that supposed to be called on low IRQL.
// The following macro defines unified assert to be put at the beginning of
// such functions.
//
// NOTE: We can't use standard PAGED_CODE macro as it requires function to be
// placed in paged segment during compilation.
//
#define IMXDOD_ASSERT_MAX_IRQL(Irql) NT_ASSERT(KeGetCurrentIrql() <= (Irql))
#define IMXDOD_ASSERT_LOW_IRQL() IMXDOD_ASSERT_MAX_IRQL(APC_LEVEL)

//
// Pool allocation tags for use by IMXDOD
//
enum IMXDOD_ALLOC_TAG : ULONG {
    TEMP                     = '0DXM', // will be freed in the same routine
    GLOBAL                   = '1DXM',
    DEVICE                   = '2DXM',
}; // enum IMXDOD_ALLOC_TAG

//
// Placement new and delete operators
//

_Notnull_
void* __cdecl operator new ( size_t, _In_ void* Ptr ) throw ();

void __cdecl operator delete ( void*, void* ) throw ();

_Notnull_
void* __cdecl operator new[] ( size_t, _In_ void* Ptr ) throw ();

void __cdecl operator delete[] ( void*, void* ) throw ();

//
// class IMXDOD_FINALLY
//
class IMXDOD_FINALLY {
private:

    IMXDOD_FINALLY () = delete;

    template < typename T_OP > class _FINALIZER {
        friend class IMXDOD_FINALLY;
        T_OP op;
        __forceinline _FINALIZER ( T_OP Op ) throw () : op(Op) {}
    public:
        __forceinline ~_FINALIZER () { (void)this->op(); }
    }; // class _FINALIZER

    template < typename T_OP > class _FINALIZER_EX {
        friend class IMXDOD_FINALLY;
        bool doNot;
        T_OP op;
        __forceinline _FINALIZER_EX ( T_OP Op, bool DoNot ) throw () : doNot(DoNot), op(Op) {}
    public:
        __forceinline ~_FINALIZER_EX () { if (!this->doNot) (void)this->op(); }
        __forceinline void DoNot (bool DoNot = true) throw () { this->doNot = DoNot; }
        __forceinline void DoNow () throw () { this->DoNot(); (void)this->op(); }
    }; // class _FINALIZER_EX

public:

    template < typename T_OP > __forceinline static _FINALIZER<T_OP> Do ( T_OP Op ) throw ()
    {
        return _FINALIZER<T_OP>(Op);
    }; // Do<...> (...)

    template < typename T_OP > __forceinline static _FINALIZER_EX<T_OP> DoUnless (
        T_OP Op,
        bool DoNot = false
        ) throw ()
    {
        return _FINALIZER_EX<T_OP>(Op, DoNot);
    }; // DoUnless<...> (...)
}; // class IMXDOD_FINALLY

#endif // _IMXDODCOMMON_HPP_
