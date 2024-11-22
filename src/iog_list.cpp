#include "iog_list.h"
#include "iog_list_dumper.h"

#include "iog_assert.h"
#include "cli_colors.h"
#include "iog_memlib.h"

#include <stdlib.h>

//--------------------- PUBLIC FUNCTIONS --------------------------------------------

/** Initialize list with INIT_LIST_CAPACITY
 * @param[out] list pointer to list 
 * @return Error code (if ok return IogReturnCode.OK)
 */
int iog_ListInit (IogList_t *list) {
  IOG_ASSERT(list);

  if (allocate_list(list, INIT_LIST_CAPACITY) != OK) {
    fprintf(stderr, RED("InitError: can't allocate list\n"));
    return ERR_CANT_ALLOCATE_DATA;
  }

  list->size = 0;

  IOG_ASSERT(iog_ListVerify(list) == OK);

  return OK;
}

/**
 * Frees data and reset list to zero
 * @param[out] list pointer to list 
 * @return Error code (if ok return IogReturnCode.OK)
 */
int iog_ListDestroy (IogList_t *list) {
  IOG_ASSERT(list);

  free(list->data);
  free(list->next);
  free(list->prev);
  
  list->data = NULL;
  list->next = NULL;
  list->prev = NULL;
  
  list->size = 0;
  list->capacity = 0;
  
  return OK;
}


/**
 * Checks nullptrs, overflowing.
 * @param[in] list pointer to list
 * @return Error code (if ok return IogReturnCode.OK)
 */
int iog_ListVerify (const IogList_t *list) {
  if (list == NULL)
    return ERR_NULLPTR;

  if (list->size > list->capacity)
    return ERR_OVERFLOW;

  if (list->capacity < INIT_LIST_CAPACITY)
    return ERR_CAPACITY_UNDERFLOW;

  if (list->data == NULL)
    return ERR_DATA_NULLPTR;

  return OK;
}

/**
 * Make list dump.
 * @param[in] list  pointer to list
 * @param[in] debug structure with debug info 
 * @return Error code (if ok return IogReturnCode.OK)
 */
int iog_ListDump (const IogList_t *list, const IogDebugInfo_t debug) {
  IOG_ASSERT(list);

  iog_ListGraphDump(list, debug);

  return OK;
}


//--------------------- PRIVATE FUNCTIONS --------------------------------------------

/**
 * Allocates max( new_capacity, INIT_LIST_CAPACITY ).
 * Can't free data, for that use destroy!
 * @param[in] list         pointer to list
 * @param[in] new_capacity new capacity of list data
 * @return Error code (if ok return IogReturnCode.OK)
 */
static int allocate_list (IogList_t *list, size_t new_capacity) {
  IOG_ASSERT(list);

  if (new_capacity < INIT_LIST_CAPACITY)
      new_capacity = INIT_LIST_CAPACITY;

  IogListData_t *tmp_data = (IogListData_t *) iog_recalloc (
      list->data,
      list->capacity,
      new_capacity,
      sizeof(IogListData_t) 
  );

  IogListId_t *tmp_next = (IogListId_t *) iog_recalloc (
      list->next,
      list->capacity,
      new_capacity,
      sizeof(IogListId_t) 
  );

  IogListId_t *tmp_prev = (IogListId_t *) iog_recalloc (
      list->prev,
      list->capacity,
      new_capacity,
      sizeof(IogListId_t) 
  );

  if (tmp_data == NULL)
    return ERR_CANT_ALLOCATE_DATA;

  if (tmp_next == NULL)
    return ERR_CANT_ALLOCATE_DATA;

  if (tmp_prev == NULL)
    return ERR_CANT_ALLOCATE_DATA;


  list->data = tmp_data;
  list->next = tmp_next;
  list->prev = tmp_prev;

  list->capacity = new_capacity;

  return OK;
}

/**
 * Double capacity by allocating more memory
 * @param[in] list pointer to list 
 * @return Error code (if ok return IogReturnCode.OK)
 */
static int allocate_more (IogList_t *list) {
  IOG_ASSERT(iog_ListVerify(list) == OK);

  if (allocate_list(list, list->capacity * 2) != OK)
    return ERR_CANT_ALLOCATE_DATA;

  IOG_ASSERT(iog_ListVerify(list) == OK);

  return OK;
}

/**
 * @param[in] list pointer to list
 * @return Error code (if ok return IogReturnCode.OK)
 */
static int free_rest (IogList_t *list) {
  IOG_ASSERT(iog_ListVerify(list) == OK);

  if (allocate_list(list, list->size) != OK)
    return ERR_CANT_FREE_DATA;

  IOG_ASSERT(iog_ListVerify(list) == OK);

  return OK;
}
