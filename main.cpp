#include <stdio.h>

#include "iog_list.h"
#include "iog_return_codes.h"

#include "cli_colors.h"
#include "iog_assert.h"

int main(const int argc, const char *argv[]) {
  IogList_t list = {};

  iog_ListInit(&list);

  IOG_LIST_DUMP(&list);

  //iog_ListInsertEnd(&list, 10.1);
  //iog_ListInsertEnd(&list, 12.1);
  //iog_ListInsertEnd(&list, 15.1);
  //iog_ListInsertEnd(&list, 19.1);

  //iog_ListPrint(&list);

  iog_ListDestroy(&list);

  return 0;
}
