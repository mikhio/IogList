#include "iog_list.h"
#include "iog_list_dumper.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>
#include <time.h>
#include <string.h>


int iog_ListGraphDump (const IogList_t *list, const IogDebugInfo_t debug, size_t *dumps_count) {
  IOG_ASSERT(list);

  char gv_filename[MAX_FILENAME_LEN] = "";
  char png_filename[MAX_FILENAME_LEN] = "";

  char datetime_str[MAX_DATETIME_STR_LEN] = "";

  time_t now = time(NULL);
  tm *t = localtime(&now);

  strftime(datetime_str, sizeof(datetime_str)-1, "%Y-%m-%d_%H.%M", t);

  snprintf(gv_filename, MAX_FILENAME_LEN, "./%s/%s/graph_dump_%s-L%d-N%lu.gv",
      LIST_DUMPS_FOLDER,
      LIST_GV_DUMPS_FOLDER,
      datetime_str,
      debug.line_num,
      *dumps_count
  );

  snprintf(png_filename, MAX_FILENAME_LEN, "./%s/%s/graph_dump_%s-L%d-N%lu.png",
      LIST_DUMPS_FOLDER,
      LIST_PNG_DUMPS_FOLDER,
      datetime_str,
      debug.line_num,
      *dumps_count
  );

  FILE *dump_file = fopen(gv_filename, "wb");

  if (dump_file == NULL) {
    fprintf(stderr, RED("DumpError: can't open dump file\n"));
    return ERR_CANT_OPEN_FILE;
  }
  
  fprintf(dump_file, "digraph G {\n");
  fprintf(dump_file,
      "debug_info [\n"
      "shape=record,\n"
      "label=\"{debug info | { {list name | file name | function name | line number | datetime} | {%s | %s | %s | %d | %s} }}\",\n"
      "fillcolor=white,\n"
      "color=grey,\n"
      "style=\"filled\",\n"
      "];\n",
      debug.name,
      debug.file_name,
      debug.func_name,
      debug.line_num,
      datetime_str
  );
  fprintf(dump_file,
      "list [\n"
      "shape=record,\n"
      "label=\"{list | { {size | capacity | first_elem | last_elem} | {%lu | %lu | %d | %d} }}\",\n"
      "fillcolor=white,\n"
      "color=grey,\n"
      "style=\"filled\",\n"
      "];\n",
      list->size,
      list->capacity,
      iog_ListFirst(list),
      iog_ListLast(list)
  );

  for (int i = 0; i < list->capacity; i++) {
    fprintf(dump_file,
      "elem_%d [\n"
      "  shape=record,\n"
      "  label=\"{elem_%d | {{data | %lg} | {next | %d} | {prev | %d}}}\",\n"
      "  fillcolor=white,\n"
      "  color=black,\n"
      "  style=\"filled\"\n"
      "];\n",
      i,
      i,
      list->data[i],
      list->next[i],
      list->prev[i]
    );
  }

  fprintf(dump_file, "\n");

  if (list->size > 1) {
    for (IogListId_t elem_id = iog_ListFirst(list); ; elem_id = list->next[elem_id]) {
      fprintf(dump_file, 
          "elem_%d -> elem_%d [color=black, line=stigh];\n"
          "elem_%d -> elem_%d [color=blue];\n"
          "elem_%d [fillcolor=lightgrey];\n"
          "\n",
          elem_id, list->next[elem_id], elem_id, list->prev[elem_id], elem_id
      );

      if (elem_id == iog_ListLast(list))
        break;
    }
  }


  fprintf(dump_file, "}\n");

  fclose(dump_file);

  char dot_cmd[MAX_CMD_STR_LEN] = "";
  snprintf(dot_cmd, MAX_CMD_STR_LEN, "dot -Tpng %s -o %s", gv_filename, png_filename);

  system(dot_cmd);

  (*dumps_count)++;

  fprintf(stderr, GREEN("[DUMPED] ") BLACK("gv: %s\n"), gv_filename);
  fprintf(stderr,       "         "  BLACK("png: %s\n"), png_filename);

  return OK;
}
