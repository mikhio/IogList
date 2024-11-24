#ifndef IOG_LIST_H
#define IOG_LIST_H

#include <stdlib.h>

#include "iog_return_codes.h"

/* @file iog_list.h */

/// Macros for dump with debuging info
#define IOG_LIST_DUMP(list) {                                            \
 iog_ListDump(list, { #list, __FILE__,  __PRETTY_FUNCTION__, __LINE__ }); \
}

typedef double  IogListData_t;  ///< Definition of list data type
typedef int     IogListId_t;    ///< Definition of list id type
                                  
const size_t INIT_LIST_CAPACITY = 4;   ///< Constant with initial data, next, prev capacities
                                       //
struct IogDebugInfo_t {
  const char *name;
  const char *file_name;
  const char *func_name;
  int line_num;
};

/** @struct IogList_t
 * Defines list structure
 */
struct IogList_t {
  IogListData_t *data;        ///< Pointer to array with data
  IogListId_t   *next;        ///< Pointer to array with next indices
  IogListId_t   *prev;        ///< Pointer to array with previous indices

  IogListId_t first_elem;     ///< Index of first element
  IogListId_t last_elem;      ///< Index of last element
                                  
  size_t size;                ///< Amount of valuable elements in list arrays
  size_t capacity;            ///< Size of allocated memory list arrays
};

//--------------------- PUBLIC FUNCTIONS --------------------------------------------

int iog_ListInit    (IogList_t *list);
int iog_ListDestroy (IogList_t *list);

int iog_ListInsertAfter  (IogList_t *list, IogListId_t elem_id,  IogListData_t value);
int iog_ListInsertBefore (IogList_t *list, IogListId_t elem_id,  IogListData_t value);
int iog_ListInsertStart  (IogList_t *list, IogListData_t value);
int iog_ListInsertEnd    (IogList_t *list, IogListData_t value);

int iog_ListGetFirst     (IogList_t *list, IogListData_t *value);
int iog_ListGetLast      (IogList_t *list, IogListData_t *value);
int iog_ListGetById      (IogList_t *list, IogListId_t id, IogListData_t *value);

int iog_ListDeleteFirst  (IogList_t *list);
int iog_ListDeleteLast   (IogList_t *list);
int iog_ListDeleteById   (IogList_t *list, IogListId_t id);

int iog_ListDump   (const IogList_t *list, const IogDebugInfo_t debug);

int iog_ListVerify (const IogList_t *list);


//--------------------- PRIVATE FUNCTIONS --------------------------------------------

/// Reallocates data of list
static int allocate_list (IogList_t *list, size_t new_capacity);

static int allocate_more (IogList_t *list); ///< Allocates more memory for data
static int free_rest     (IogList_t *list); ///< Free all memory after list size.

#endif // IOG_LIST_H
