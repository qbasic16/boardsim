#ifndef QFD_H
#define QFD_H

#include "vpi_user.h"

/* 
 * The QFD API (Questa Fast Logging) is meant to be used by Questa
 * waveform-logging clients. The API's assume that there is only one
 * client of the API in a given simulation. In case of multiple clients,
 * i.e. two clients try to set a fast callback on same object, or the same
 * client tries to set more than one fast callback on the same object,
 * the second callback request will fail.
 *
 * The QFD API provides the following functionality:
 * 1. Help filter the signals/variables that should be logged.
 * 2. Setup fast value change callbacks.
 * 3. Provide reconstruction expressions for signals/variables
 *    that are not logged. These object values should be computed at
 *    visualization time based on the map expression provided by QFD.
 *
 * The QFD API does not provide any design iteration capability.
 * It is meant to be used in conjunction with standard VPI design iteration.
 * Design iteration returns vpiHandle's to signals and variables to be logged.
 * Use the QFD API to set up fast callbacks and perform value reconstruction
 * on objects that have been optimized out of the simulator's memory image.
 *
 * qfd_getValType() is the filtering routine that should be called on all 
 * signal/variable objects discovered by VPI iteration. The routine helps
 * the waveform logger determine if the objects need to be logged.
 *
 * qfd_setCbAndGetValPtr() should be called on all objects that need to be 
 * logged (QFD_VAL_PRIMARY). The function may return NULL, which means fast 
 * logging is not possible for the object. The client should log such objects 
 * using standard VPI callbacks instead. Some of the reasons that fast callbacks
 * can fail are:
 * 1. Unsupported object types like unpacked arrays, struct and class objects, etc
 * 2. The object already has a fast callback.
 *
 * qfd_getMapExpr() should be called on all objects that need to be 
 * reconstructed (QFD_VAL_SECONDARY). The map expression tree for reconstruction
 * is returned in terms of the TQfdSecMapExpr structures defined in this API.
 *
 * A client can ask for QFD_VAL_SECONDARY objects to be logged using 
 * qfd_setCbAndGetValPtrForSecondary(). These objects will then be reconstructed
 * by the Questa simulator and the callback function will be called as if the
 * objects were primaries. This will adversely affect simulation performance
 * and memory compared to performing visualization-side reconstruction.
 */

/* 
 *******************************************************************************
 * Filter objects for logging
 *******************************************************************************
 */

/* 
 * Enum TQfdValType
 *
 * Value types returned by qfd_getValType()
 *
 * QFD_VAL_CANCELLED objects can not be logged or reconstructed.
 * The waveform for these objects will not be available.
 *
 * QFD_VAL_PRIMARY objects are loggable. 
 *
 * QFD_VAL_SECONDARY objects can be reconstructed.
 *
 * QFD_VAL_LITERAL objects do not change value during simulation,
 * as determined by post optimization analysis.
 */
typedef enum {
    QFD_VAL_NONE,        
    QFD_VAL_CANCELLED,   /* Object not available               */
    QFD_VAL_PRIMARY,     /* Loggable                           */
    QFD_VAL_SECONDARY,   /* Value can be reconstructed         */
    QFD_VAL_LITERAL      /* Constant value                     */
} TQfdValType;

/*
 * qfd_getValType()
 *
 * Get value type for an object handle.
 */
TQfdValType 
qfd_getValType(
    vpiHandle varHandle);


/* 
 *******************************************************************************
 * Fast Value Change Callback
 *******************************************************************************
 */

typedef void (*qfd_func_p)(void* userData, uint32_t highTime, uint32_t lowTime);

/*
 * qfd_setCbAndGetValPtr()
 *
 * Should be called for the following value types.
 *   QFD_VAL_PRIMARY
 *   QFD_VAL_LITERAL
 *
 * For QFD_VAL_PRIMARY:
 *   - Sets a callback on an object handle.
 *   - A change in value of the object will result in 'qfd_func_p' getting called
 *     with the corresponding userData.
 *   - Sets userData for this var which can be retrieved using qfd_getUserData();
 *   - The return value is the value location of this object in aval/bval format.
 *       - A null value means fast logging callbacks could not be set up.
 *         Use standard VPI callbacks for such objects.
 *
 * For QFD_VAL_LITERAL:
 *   - userData is ignored.
 *   - The return value will hold the actual value of this constant in aval/bval format.
 *   - No callback happens for these since these objects do not change value.
 */
void* 
qfd_setCbAndGetValPtr(
    vpiHandle  varHandle,
    qfd_func_p funcp,
    void*      userData);


/* 
 *******************************************************************************
 * Value Reconstruction Expressions and Secondary Objects
 *******************************************************************************
 */


/*
 * Expression types supported for reconstruction
 *
 * More types may be added in future Questa releases. The client code should be written
 * in a way that if client code sees a type not defined here, it should ask Questa to 
 * log the secondary (see qfd_setCbAndGetValPtrForSecondary)
 *
 * Example user code to print map expr
 * void print_map_expr(TQfdSecMapBaseExpr* mapExpr) {
 *     if (mapExpr == NULL) {
 *         printf("NULL");
 *         return;
 *     }
 *     switch(mapExpr->opType) {
 *         case QFD_SECMAP_VAR: 
 *         {
 *             TQfdSecMapVarExpr* expr = (TQfdSecMapVarExpr*)mapExpr);
 *             printf("%s", vpi_get_str(vpifullName, expr->handle));
 *             break;
 *         }
 *         case QFD_SECMAP_BITSEL:
 *         {
 *             TQfdSecMapBitSelectExpr* expr = (TQfdSecMapBitSelectExpr*)mapExpr;
 *             printf("%s[%d]", vpi_get_str(vpifullName, expr->handle), expr->idx).
 *             break;
 *         }
 *         case QFD_SECMAP_PARTSEL:
 *         {
 *             TQfdSecMapPartSelectExpr* expr = (TQfdSecMapPartSelectExpr*)mapExpr;
 *             printf("%s[%d:%d]", vpi_get_str(vpifullName, expr->handle), expr->lsb,
 *                 expr->msb);
 *             break;
 *         }
 *         case QFD_SECMAP_BITAND:
 *         case QFD_SECMAP_BITOR:
 *         case QFD_SECMAP_BITXOR:
 *         {
 *             TQfdSecMapInfixExpr* expr = (TQfdSecMapInfixExpr*)expr;
 *             print_map_expr(expr->lhs);
 *             print_operator(expr->opType);
 *             print_map_expr(expr->rhs);
 *             break;
 *         }
 *         .
 *         .
 *         .
 *         default:
 *             printf("Unexpected map expression\n");
 *     }
 * }
 *
 * void print_secondary_map_expr(vpiHandle handle) {
 *     print_map_expr(qfd_getMapExpr(handle));
 * }
 */

typedef enum {
    QFD_SECMAP_VAR,              /* TQfdSecMapVarExpr        */
    QFD_SECMAP_BITSEL,           /* TQfdSecMapBitSelectExpr  */
    QFD_SECMAP_PARTSEL,          /* TQfdSecMapPartSelectExpr */
    QFD_SECMAP_CONCAT,           /* TQfdSecMapExpr           */
    QFD_SECMAP_BITAND,           /* TQfdSecMapInfixExpr      */
    QFD_SECMAP_BITOR,            /* TQfdSecMapInfixExpr      */
    QFD_SECMAP_BITXOR,           /* TQfdSecMapInfixExpr      */
    QFD_SECMAP_BITNEG,           /* TQfdSecMapPrefixExpr     */
    QFD_SECMAP_BITBUF,           /* TQfdSecMapPrefixExpr     */
    QFD_SECMAP_TERNARY,          /* TQfdSecMapExpr           */
    QFD_SECMAP_LITERAL           /*                          */
} TQfdSecMapExprOpType;

#define QFD_MAPEXPR_BASE \
    TQfdSecMapExprOpType opType

/*
 * Base class for recon expression tree
 */
typedef struct {
    QFD_MAPEXPR_BASE;
} TQfdSecMapBaseExpr;

/*
 * Var alias expr
 * QFD_SECMAP_VAR
 *
 * The 'vpiHandle handle' value type (as return by qfd_getValType) can be 
 * 1. Primary. The client should add a fast callback on the handle.
 * 2. Secondary. This is a case where multiple secondaries have same map expr.
 *    In such case the map expr for one secondary is returned, and other
 *    secondaries are considered aliases of the first map expr.
 *    Clients can take advantage of this property by mapping
 *    multiple secondaries to the same map expr.
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    vpiHandle handle;
} TQfdSecMapVarExpr;

/* 
 * Bit select expr
 * QFD_SECMAP_BITSEL
 *
 * The bit select idx is the bit offset index and not necessarily the user code index.
 * e.g. for a signal declared as 'wire [7:4] w', w[6] will have bit offset index 3.
 *
 * The array handle is always the vpiHandle of an object.
 * The array can not be another expression.
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    int       idx;
    vpiHandle handle;
} TQfdSecMapBitSelectExpr;

/*
 * Part Select expr
 * QFD_SECMAP_PARTSEL
 *
 * The lsb and msb of part select expr are the part select offset range as commented 
 * for bit select. The msb is always greater than the lsb regardless of the direction
 * of the selected array.
 *
 * The array handle is always the vpiHandle of an object.
 * The array cannot be another expression.
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    int       lsb;
    int       msb;
    vpiHandle handle;
} TQfdSecMapPartSelectExpr;

/*
 * Prefix expr
 * QFD_SECMAP_BITNEG    bitwise negation
 * QFD_SECMAP_BITBUF    equivalent to verilog buf primitive
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    TQfdSecMapBaseExpr* operand;
} TQfdSecMapPrefixExpr;

/*
 * Infix expr
 * QFD_SECMAP_BITAND    bitwise AND
 * QFD_SECMAP_BITOR     bitwise OR
 * QFD_SECMAP_BITXOR    bitwise XOR
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    TQfdSecMapBaseExpr* lhs;
    TQfdSecMapBaseExpr* rhs;
} TQfdSecMapInfixExpr;

/*
 * Expressions that do not belong to above types
 * QFD_SECMAP_CONCAT numOperands can be any number. {operand[0], operand[1]....}
 * QFD_SECMAP_TERNARY numOperands is 3. operand[0] ? operand[1] : operand[2];
 */
typedef struct {
    QFD_MAPEXPR_BASE;
    int                  numOperands;
    TQfdSecMapBaseExpr** operands;
} TQfdSecMapExpr;

/*
 * qfd_getMapExpr()
 *
 * Get a map expression for a QFD_VAL_SECONDARY variable in terms of vpiHandles
 * of the primaries.
 *
 * A terminal vpiHandle can be a handle to a secondary in case varHandle is an alias 
 * of another secondary object. If multiple secondaries have the same map expr (e.g.
 * multiple port formals map to same apex actual expression), then qfd_getMapExpr() 
 * will return the map expr for only one of the secondaries and mark the other
 * secondaries as aliases of the first one.
 */
TQfdSecMapBaseExpr*
qfd_getMapExpr(
    vpiHandle varHandle,
	int*      isAliasOfSecondaryP);


/*
 * qfd_setCbAndGetValPtrForSecondary()
 *
 * Set callback on secondary object.
 *  - To be used if client does not intend to use the map expression returned
 *    by qfd_GetMapExpr() to evaluate the value of a secondary.
 *   - A change in value of the variable will result in 'funcp' getting called with
 *     the corresponding userData.
 *   - Sets userData for this var which can be retrieved using qfd_getUserData();
 *   - Return value is the value location of this variable in aval/bval format.
 *       - A null value means fast logging callbacks could not be set up.
 *         Use standard VPI callbacks for such objects.
 *
 * Note: Setting a callback on a secondary will cause the simulator to reconstruct
 * the secondary.
 */
void*
qfd_setCbAndGetValPtrForSecondary(
    vpiHandle  varHandle,
    qfd_func_p funcp,
    void*      userData);


/*
 * qfd_setUserData()
 *
 * Return value :
 *     0 - User data could not be set for this var.
 *     1 - Setting of user_data was successful.
 */
int
qfd_setUserData(
    vpiHandle varHandle,
	void*     userData);


/*
 * qfd_getUserData()
 *
 * Returns the userData associated with an object which was used
 * when setting QFD callback on this object.
 */
void*
qfd_getUserData(
    vpiHandle varHandle);


/*
 * qfd_removeCb()
 *
 * Remove QFD callback for this object.
 * Return value :
 *     0 - Callback could not be removed or does not exist.
 *     1 - Removal was successful.
 */
int
qfd_removeCb(
    vpiHandle varHandle);


/* 
 *******************************************************************************
 * Miscellaneus Functions
 *******************************************************************************
 */

/*
 * qfd_init()
 *
 * Call this once before accessing any other QFD APIs.
 * Return value :
 *     0 - QFD is not available.
 *     1 - QFD is available.
 */
int
qfd_init();


/*
 * qfd_cleanup()
 *
 * Call at the end of design iteration. The function frees QFD memory.
 * Any subsequent QFD call will result in undefined behavior.
 */
void
qfd_cleanup();


#endif
