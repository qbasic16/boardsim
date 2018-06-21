/*
 * DPI Backwards compatibility header as defined in H.13.2 (P1800-2012).
 *
 * The functionality described in this header file is defined in Accellerat SystemVerilog SV3.1a.
 * It is deprecated by the IEEE-P1800 standard. This header file provides backwards compatibility
 * with SV3.1a regarding the behavior of packed array arguments.
 *
 * The main difference between SV3.1a and IEEE 1800 semantics is that in SV3.1a, packed data
 * arguments are passed by opaque handle types (svLogicPackedArrRef and svBitPackedArrRef).
 * An SV3.1a implementation need not do any conversion or marshalling of data into the canonical format.
 * The C programmer is provided a set of utility functions that copy data between actual vendor
 * format and canonical format. Other utilities are provided that put and get bit-selects and
 * part-selects to and from actual vendor representation. 
 *
 * Disclaimer:
 *
 * Questa doesn't support Accellera SV3.1a "DPI". Only "DPI-C" is supported.
 * This header provides an easy path to reuse existing C code written for SV3.1a.
 */

#ifndef INCLUDED_SVDPI_31A_H_
#define INCLUDED_SVDPI_31A_H_

#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SV_CANONICAL_SIZE(WIDTH) (((WIDTH)+31)>>5) 

/* (a chunk of) packed bit array */ 
typedef uint32_t svBitVec32;

/* (a chunk of) packed logic array */ 
typedef struct { unsigned int c; unsigned int d;}  svLogicVec32;

/* reference to a standalone packed array */ 
typedef void* svBitPackedArrRef; 

typedef void* svLogicPackedArrRef; 

/* total size in bytes of the simulator's representation of a packed array */ 
/* width in bits */
int svSizeOfBitPackedArr(int width); 
int svSizeOfLogicPackedArr(int width); 

/* s=source, d=destination, w=width */ 
/* actual <-- canonical */ 
void svPutBitVec32 (svBitPackedArrRef d, const svBitVec32* s, int w); 
void svPutLogicVec32 (svLogicPackedArrRef d, const svLogicVec32* s, int w); 

/* canonical <-- actual */ 
void svGetBitVec32 (svBitVec32* d, const svBitPackedArrRef s, int w); 
void svGetLogicVec32 (svLogicVec32* d, const svLogicPackedArrRef s, int w); 

/* d=destination, i=bit-index, s=scalar */
void svPutSelectBit(svBitPackedArrRef d, int i, svBit s); 
void svPutSelectLogic(svLogicPackedArrRef d, int i, svLogic s);

svBit svGetSelectBit(const svBitPackedArrRef s, int i);
svLogic svGetSelectLogic(const svLogicPackedArrRef s, int i);

/* 
 * functions for part-select 
 * 
 * a narrow (<=32 bits) part-select is copied between 
 * the implementation representation and a single chunk of 
 * canonical representation 
 * Normalized ranges and indexing [n-1:0] are used for both arrays: 
 * the array in the implementation representation and the canonical array. 
 * 
 * s=source, d=destination, i=starting bit index, w=width 
 * like for variable part-selects; limitations: w <= 32 
 * 
 * In part-select operations, the data are copied to or from the 
 * canonical representation part ('chunk') designated by range [w-1:0] 
 * and the implementation representation part designated by range [w+i-1:i]. 
 */ 

/* canonical <-- actual */ 
void       svGetPartSelectBit(svBitVec32* d, const svBitPackedArrRef s, int i, int w); 
svBitVec32 svGetBits(const svBitPackedArrRef s, int i, int w); 
svBitVec32 svGet32Bits(const svBitPackedArrRef s, int i);
uint64_t   svGet64Bits(const svBitPackedArrRef s, int i);
void       svGetPartSelectLogic(svLogicVec32* d, const svLogicPackedArrRef s, int i, int w); 

/* actual <-- canonical */ 
void svPutPartSelectBit(svBitPackedArrRef d, const svBitVec32 s, int i, int w); 
void svPutPartSelectLogic(svLogicPackedArrRef d, const svLogicVec32 s, int i, int w); 

/*
 * Questa-specific adaptors for conversions between the IEEE-1800 compliant types 
 * (svBitVecVal/svLogicVecVal) and SV3.1a opaque handle types.
 */
svLogicPackedArrRef svMakeLogicPackedArrRef(svLogicVecVal* s);
svBitPackedArrRef   svMakeBitPackedArrRef(svBitVecVal*     s);

svLogicVecVal* svGetLogicVecValFromLogicPackedArrRef(svLogicPackedArrRef s);
svBitVecVal*   svGetBitVecValFromBitPackedArrRef(svBitPackedArrRef s);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
