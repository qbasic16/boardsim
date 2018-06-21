#ifndef MTI_TRAVERSE_H
#define MTI_TRAVERSE_H

#ifndef PROTO
#if defined(_WIN32) || defined(__STDC__) || defined(__cplusplus)
#define PROTO(arg) arg
#else
#define PROTO(arg) ()
#endif
#endif /* PROTO */

/*********************************************************************
    The mtiLangT enum type will tell us what the HDL type is for the 
    specific handle.
*********************************************************************/
typedef enum {
    MTI_LANG_NONE = 0,
    MTI_LANG_VLOG,     /** Refer to vpi field of union uHandle. Use VPI API to handle it. */
    MTI_LANG_VHDL,     /** Refer to mti field of union uHandle. Use MTI API to handle it. */
    MTI_LANG_SYSC,     /** Refer to mti field of union uHandle. Use MTI API to handle it. */

    MTI_LANG_LAST
} mtiLangT;

/*********************************************************************
    Unify(union) Handle Type
    Including vpiHandle/mtiRegionIdT types.
    Refer to mtiLangT enum type, then we know which field should be used.
*********************************************************************/
typedef union {
    vpiHandle    vpi;
    mtiRegionIdT mti;
} mtiUHandleT;

/*********************************************************************
    The mtiLangT enum type will tell us what the HDL type is for the 
    specific handle.
*********************************************************************/
typedef enum {
    MTI_API_NONE = 0,
    MTI_API_PLI,     /** A PLI handle  */
    MTI_API_VPI,     /** A VPI handle  */
    MTI_API_FLI,     /** An FLI handle */
    MTI_API_LAST
} mtiApiT;

/*********************************************************************
    MTI API for traversing all the scopes.(Verilog/VHDL/SystemC)
*********************************************************************/
typedef int  (*mti_TraverseRegionCBFuncChkType)   PROTO((mtiLangT langType, 
                                                         mtiUHandleT *uHandle,
                                                         void *user_data));
typedef void (*mti_TraverseRegionCBFuncBeginType) PROTO((mtiLangT langType, 
                                                         mtiUHandleT *uHandle,
                                                         void *user_data));
typedef void (*mti_TraverseRegionCBFuncEndType)   PROTO((mtiLangT langType, 
                                                         mtiUHandleT *uHandle,
                                                         void *user_data));
extern  void mti_TraverseRegion PROTO((mtiLangT langType,
                                       mtiUHandleT *uHandle,
                                       int nDepth,
                                       mti_TraverseRegionCBFuncChkType   fp_chk,
                                       mti_TraverseRegionCBFuncBeginType fp_begin,
                                       mti_TraverseRegionCBFuncEndType   fp_end,
                                       void *user_data));

#endif // MTI_TRAVERSE_H
