/* $Id: //dvt/mti/rel/10.4a/src/vsim/svdpi_src.h#1 $ */


/* macros for declaring variables to represent the SystemVerilog */
/* packed arrays of type bit or logic */
/* WIDTH= number of bits,NAME = name of a declared field/variable */

#define SV_BIT_PACKED_ARRAY(WIDTH,NAME) \
    svBitVec32 NAME [SV_CANONICAL_SIZE(WIDTH)]

#define SV_LOGIC_PACKED_ARRAY(WIDTH,NAME) \
    svLogicVec32 NAME [SV_CANONICAL_SIZE(WIDTH)]

