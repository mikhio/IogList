#include "iog_list.h"
#include "iog_list_dumper.h"

#include "iog_return_codes.h"
#include "iog_assert.h"
#include "cli_colors.h"

#include <stdio.h>

int iog_ListGraphDump (const IogList_t *list, const IogDebugInfo_t debug) {
  IOG_ASSERT(list);

  FILE *dump_file = fopen("graph_dump.gv", "wb");

  if (dump_file == NULL) {
    fprintf(stderr, RED("DumpError: can't open dump file\n"));
    return ERR_CANT_OPEN_FILE;
  }
  
  fprintf(dump_file, "digraph G {\n");
  fprintf(dump_file,
      "debug_info [\n"
      "shape=record,\n"
      "label=\"{debug info | { {list name | file name | function name | line number} | {%s | %s | %s | %d} }}\",\n"
      "fillcolor=white,\n"
      "color=grey,\n"
      "style=\"filled\",\n"
      "];\n",
      debug.name,
      debug.file_name,
      debug.func_name,
      debug.line_num
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
      list->first_elem,
      list->last_elem
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
    for (IogListId_t elem_id = list->first_elem; ; elem_id = list->next[elem_id]) {
      fprintf(dump_file, 
          "elem_%d -> elem_%d [color=black, line=stigh];\n"
          "elem_%d -> elem_%d [color=blue];\n"
          "elem_%d [fillcolor=lightgrey];\n"
          "\n",
          elem_id, list->next[elem_id], elem_id, list->prev[elem_id], elem_id
      );

      if (elem_id == list->last_elem)
        break;
    }
  }


  fprintf(dump_file, "}\n");

  fclose(dump_file);

  system("dot -Tpng graph_dump.gv -o dump.png");

  return OK;
}
