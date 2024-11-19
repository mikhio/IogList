#ifndef IOG_LIST_H
#define IOG_LIST_H

#include "iog_return_codes.h"

/* @file iog_list.h */

/// Macros for dump with debuging info
#define IOG_LIST_DUMP(list) {                                                     \
  iog_ListDump(list, #list, __FILE__, __LINE__, __PRETTY_FUNCTION__); \


typedef double  IogListData_t;  ///< Definition of list data type
typedef int     IogListId_t;    ///< Definition of list id type
                                  
const size_t INIT_LIST_CAPACITY = 4;   ///< Constant with initial data, next, prev capacities

/** @struct IogList_t
 * Defines list structure
 */
struct IogList_t {
  IogListData_t *data;        ///< Pointer to array with data
  IogListId_t   *next;        ///< Pointer to array with next indices
  IogListId_t   *prev;        ///< Pointer to array with previous indices
                                  
  size_t size;                ///< Amount of valuable elements in list arrays
  size_t capacity;            ///< Size of allocated memory list arrays
};

//--------------------- PUBLIC FUNCTIONS --------------------------------------------

int iog_ListInit    (IogList_t *list);
int iog_ListDestroy (IogList_t *list);

//int iog_ListInsertEnd (IogList_t *list);

int iog_ListDump   (const IogList_t *list,
    const char *name, const char *filename, int line_num, const char *func_name);

int iog_ListVerify (const IogList_t *list);


//--------------------- PRIVATE FUNCTIONS --------------------------------------------

/// Reallocates data of list
static int allocate_list (IogList_t *list, size_t new_capacity);

static int allocate_more (IogList_t *list); ///< Allocates more memory for data
static int free_rest     (IogList_t *list); ///< Free all memory after list size.

#endif // IOG_LIST_H
