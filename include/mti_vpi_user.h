/*****************************************************************************
 * Questa VPI extensions
 * Note that VPI is an interface whose definition is standardized in the 
 * IEEE 1800 SystemVerilog standard.
 * Extensions defined here may change to align with this standard as it is
 * updated, in which case any dependent applications dependent would
 * need to be recompiled.
 *
 *****************************************************************************/

#ifndef MTI_VPI_USER_H
#define MTI_VPI_USER_H

#ifndef VPI_USER_H
#include "vpi_user.h"
#endif

#ifndef ACC_USER_H
#include "acc_user.h"
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

#define vpiWreal		2001  /* Net subtype for wired real */
#define vpiInterconnect	2002  /* Net subtype for interconnect */

/*****************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

handle vpi_to_mti(vpiHandle vpi_object);
vpiHandle mti_to_vpi(handle internal_object);


#ifdef  __cplusplus
}
#endif

#endif /* MTI_VPI_USER_H */
