#ifndef IOG_LIST_DUMPER_H
#define IOG_LIST_DUMPER_H

#include "iog_list.h"

const size_t MAX_DATETIME_STR_LEN = 100;
const size_t MAX_FILENAME_LEN     = 200;
const size_t MAX_CMD_STR_LEN      = 300;

const char LIST_DUMPS_FOLDER[MAX_FILENAME_LEN]      = "dumps";
const char LIST_GV_DUMPS_FOLDER[MAX_FILENAME_LEN]   = "gv_dumps";
const char LIST_PNG_DUMPS_FOLDER[MAX_FILENAME_LEN]  = "png_dumps";

int iog_ListGraphDump (const IogList_t *list, const IogDebugInfo_t debug, size_t *dumps_count);

#endif // IOG_LIST_DUMPER_H
