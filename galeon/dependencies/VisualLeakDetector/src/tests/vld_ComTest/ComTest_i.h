

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Jan 04 19:57:47 2016
 */
/* Compiler settings for ComTest.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ComTest_i_h__
#define __ComTest_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMyMath_FWD_DEFINED__
#define __IMyMath_FWD_DEFINED__
typedef interface IMyMath IMyMath;
#endif 	/* __IMyMath_FWD_DEFINED__ */


#ifndef __MyMath_FWD_DEFINED__
#define __MyMath_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyMath MyMath;
#else
typedef struct MyMath MyMath;
#endif /* __cplusplus */

#endif 	/* __MyMath_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMyMath_INTERFACE_DEFINED__
#define __IMyMath_INTERFACE_DEFINED__

/* interface IMyMath */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMyMath;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19823626-F6EA-472F-9AA3-1B2CA0693E53")
    IMyMath : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Test( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMyMathVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMyMath * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMyMath * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMyMath * This);
        
        HRESULT ( STDMETHODCALLTYPE *Test )( 
            IMyMath * This);
        
        END_INTERFACE
    } IMyMathVtbl;

    interface IMyMath
    {
        CONST_VTBL struct IMyMathVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyMath_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMyMath_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMyMath_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMyMath_Test(This)	\
    ( (This)->lpVtbl -> Test(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMyMath_INTERFACE_DEFINED__ */



#ifndef __ComTestLib_LIBRARY_DEFINED__
#define __ComTestLib_LIBRARY_DEFINED__

/* library ComTestLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ComTestLib;

EXTERN_C const CLSID CLSID_MyMath;

#ifdef __cplusplus

class DECLSPEC_UUID("B379BB26-80E6-4DCB-AD82-8F46BA81BBCF")
MyMath;
#endif
#endif /* __ComTestLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


