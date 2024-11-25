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
    return ERR_CANT_ALLOCATE;
  }

  list->size = 0;
  list->first_elem = 0;
  list->last_elem = 0;

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

  list->first_elem = 0;
  list->last_elem = 0;
  
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

int iog_ListInsertAfter (IogList_t *list, IogListId_t elem_id, IogListData_t value) {
  IOG_ASSERT(iog_ListVerify(list) == OK);
  IOG_ASSERT(elem_id >= 0);
  IOG_ASSERT(elem_id <= list->size);
  IOG_ASSERT((elem_id == list->size) <= (list->size == 0))


  if (list->size == list->capacity) {
    int err = allocate_more(list);

    if (err != OK)  {
      fprintf(stderr, RED("InsertError: size > capacity and can't allocate more memory\n"));
      return ERR_CANT_ALLOCATE;
    }
  }

  list->data[list->size] = value;
  list->next[list->size] = list->next[elem_id];
  list->prev[list->size] = elem_id;

  list->next[elem_id] = list->size;
  list->prev[list->next[list->size]] = list->size;

  if (elem_id == list->last_elem) {
    list->last_elem = list->size;
  }

  list->size++;

  fprintf(stderr, BLUE("[INSERT]") " size: %lu, cap: %lu\n", list->size, list->capacity);

  return OK;
}

int iog_ListInsertBefore (IogList_t *list, IogListId_t elem_id, IogListData_t value) {
  IOG_ASSERT(iog_ListVerify(list) == OK);
  IOG_ASSERT(elem_id >= 0);
  IOG_ASSERT(elem_id <= list->size);
  IOG_ASSERT((elem_id == list->size) <= (list->size == 0))

  if (list->size == list->capacity) {
    int err = allocate_more(list);

    if (err != OK)  {
      fprintf(stderr, RED("InsertError: size > capacity and can't allocate more memory\n"));
      return ERR_CANT_ALLOCATE;
    }
  }

  list->data[list->size] = value;
  list->next[list->size] = elem_id;
  list->prev[list->size] = list->prev[elem_id];

  list->prev[elem_id] = list->size;
  list->next[list->prev[list->size]] = list->size;

  if (elem_id == list->first_elem) {
    list->first_elem = list->size;
  }

  list->size++;

  fprintf(stderr, BLUE("[INSERT]") " size: %lu, cap: %lu\n", list->size, list->capacity);

  return OK;
}

int iog_ListInsertStart (IogList_t *list, IogListData_t value) {
  return iog_ListInsertBefore(list, list->first_elem, value);
}

int iog_ListInsertEnd (IogList_t *list, IogListData_t value) {
  return iog_ListInsertAfter(list, list->last_elem, value);
}

int iog_ListGetById (IogList_t *list, IogListId_t elem_id, IogListData_t *value) {
  IOG_ASSERT(list);
  IOG_ASSERT(iog_ListVerify(list) == OK);
  IOG_ASSERT(value);

  if (list->size == 0)
    return ERR_SIZE_IS_NULL;

  IOG_ASSERT(elem_id >= 0);
  IOG_ASSERT(elem_id < list->size);

  *value = list->data[elem_id];

  return OK;
}

int iog_ListGetFirst (IogList_t *list, IogListData_t *value) {
  return iog_ListGetById(list, list->first_elem, value);
}

int iog_ListGetLast (IogList_t *list, IogListData_t *value) {
  return iog_ListGetById(list, list->last_elem, value);
}

int iog_ListDeleteById (IogList_t *list, IogListId_t elem_id) {
  IOG_ASSERT(list);
  IOG_ASSERT(iog_ListVerify(list) == OK);

  if (list->size == 0)
    return ERR_SIZE_IS_NULL;

  IOG_ASSERT(elem_id >= 0);
  IOG_ASSERT(elem_id < list->size);

  list->prev[list->next[elem_id]] = list->prev[elem_id];
  list->next[list->prev[elem_id]] = list->next[elem_id];

  if (elem_id == list->first_elem)
    list->first_elem = list->next[elem_id];
  if (elem_id == list->last_elem)
    list->last_elem = list->prev[elem_id];

  
  /*
  list->data[elem_id] = list->data[list->size-1];
  list->next[elem_id] = list->next[list->size-1];
  list->prev[elem_id] = list->prev[list->size-1];

  if ((list->size-1) == list->last_elem)
    list->last_elem = elem_id;

  if ((list->size-1) == list->first_elem)
    list->first_elem = elem_id;


  list->data[list->size-1] = 0;
  list->next[list->size-1] = 0;
  list->prev[list->size-1] = 0;

  if (list->last_elem == list->size-1)
    list->last_elem = list->size-2;
  list->size--;
  */

  if (list->size <= list->capacity/4)
    free_rest(list);

  fprintf(stderr, RED("[DELETE]") " size: %lu, cap: %lu\n", list->size, list->capacity);

  return OK;
}

int iog_ListDeleteFirst (IogList_t *list) {
  return iog_ListDeleteById(list, list->first_elem);
}

int iog_ListDeleteLast (IogList_t *list) {
  return iog_ListDeleteById(list, list->last_elem);
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
    return ERR_CANT_ALLOCATE;

  if (tmp_next == NULL)
    return ERR_CANT_ALLOCATE;

  if (tmp_prev == NULL)
    return ERR_CANT_ALLOCATE;


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
    return ERR_CANT_ALLOCATE;

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
