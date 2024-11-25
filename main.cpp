#include <stdio.h>

#include "iog_list.h"
#include "iog_return_codes.h"

#include "cli_colors.h"
#include "iog_assert.h"

int main(const int argc, const char *argv[]) {
  IogList_t list = {};

  iog_ListInit(&list);

  iog_ListInsertEnd(&list, 10.1);
  iog_ListInsertEnd(&list, 11.1);
  iog_ListInsertEnd(&list, 12.1);
  iog_ListInsertEnd(&list, 13.1);
  iog_ListInsertEnd(&list, 14.1);

  IogListData_t value = 0;
  iog_ListGetLast(&list, &value);
  fprintf(stderr, "value: %lg\n", value);

  iog_ListDeleteFirst(&list);
  iog_ListDeleteFirst(&list);
  iog_ListDeleteLast(&list);
  //iog_ListDeleteById(&list, 2);

  
  IOG_LIST_DUMP(&list);

  iog_ListDestroy(&list);

  return 0;
}
