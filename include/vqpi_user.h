/*
 * vqpi_user.h
 *
 * Interface to an alternate VPI implementation
 *
 * The VQPI API interface routines are identical to their VPI counterparts,
 * and are as defined in the IEEE 1800 VPI standard, except
 * they are named with the vqpi_ prefix instead of vpi_
 * 
 * Please note that VPI handles may not be passed directly between the alternate
 * vpi and vqpi implementations as they are not mutually compatible.
 *
 * Functionally there can be minor differences, intentional or otherwise.
 * For example, the order in which objects are returned from an iteration
 * may differ.
 * The vqpi_ implementation is incomplete and may require tool initialization.
 * The vqpi_ does not substitute VPI routines that have no interaction with 
 * simulation objects (such as the mcd and printf routines). If a routine is not
 * listed here, the vpi routine may be safely used.
 */
#ifndef VQPIUSER_H
#define VQPIUSER_H

#ifdef  __cplusplus
extern "C" {
#endif

extern vpiHandle vqpi_handle_by_name(PLI_BYTE8 *name, vpiHandle scope);
extern PLI_INT32 vqpi_compare_objects(vpiHandle obj1, vpiHandle obj2);
extern PLI_INT32 vqpi_control(PLI_INT32 operation, ...);
extern PLI_INT32 vqpi_free_object(vpiHandle object);
extern PLI_INT32 vqpi_release_handle(vpiHandle object);
extern void * vqpi_get_userdata(vpiHandle obj);
extern PLI_INT32 vqpi_put_userdata(vpiHandle obj, void *userdata);
extern PLI_BYTE8 * vqpi_get_str(PLI_INT32 property, vpiHandle object);
extern PLI_INT32 vqpi_get(PLI_INT32 property, vpiHandle object);
extern PLI_INT64 vqpi_get64(PLI_INT32 property, vpiHandle object);
extern vpiHandle vqpi_handle(PLI_INT32 type, vpiHandle referenceHandle);
extern vpiHandle vqpi_iterate(PLI_INT32 type, vpiHandle referenceHandle);
extern vpiHandle vqpi_scan(vpiHandle iterator);
extern void vqpi_get_value(vpiHandle object, p_vpi_value value_p);
extern vpiHandle vqpi_put_value(vpiHandle   object,
                                p_vpi_value value_p,
                                p_vpi_time  time_p,
                                PLI_INT32   flags);
extern void vqpi_get_delays(vpiHandle object, p_vpi_delay delay_p);
extern void vqpi_put_delays(vpiHandle object, p_vpi_delay delay_p);
extern vpiHandle vqpi_handle_by_index(vpiHandle object, PLI_INT32 indx);
extern vpiHandle vqpi_handle_by_multi_index(vpiHandle   object,
                                            PLI_INT32   num_index,
                                            PLI_INT32 * index_array);
extern vpiHandle vqpi_handle_multi(PLI_INT32 type,
                                   vpiHandle ref1,
                                   vpiHandle ref2,
                                   ...);
extern void vqpi_get_time(vpiHandle object, p_vpi_time time_p);
extern vpiHandle vqpi_register_cb(p_cb_data cb_data_p);
extern PLI_INT32 vqpi_remove_cb(vpiHandle cb_obj);
extern void vqpi_get_cb_info(vpiHandle object, p_cb_data cb_data_p);
#ifdef  __cplusplus
}
#endif

#endif
