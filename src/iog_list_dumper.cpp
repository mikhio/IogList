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
      "list [\n"
      "shape=record,\n"
      "label=\"{list | {size | %lu} | {capacity | %lu}}\",\n"
      "fillcolor=lightgrey,\n"
      "color=black,\n"
      "style=\"filled\",\n"
      "];\n",
      list->size,
      list->capacity
  );

  for (int i = 0; i < list->capacity; i++) {
    fprintf(dump_file,
      "elem_%d [\n"
      "  shape=record,\n"
      "  label=\"{elem_%d | {{data | %lg} | {<next_%d> next | %d} | {<prev_%d> prev | %d}}}\",\n"
      "  fillcolor=lightgrey,\n"
      "  color=black,\n"
      "  style=\"filled\"\n"
      "];\n",
      i,
      i,
      list->data[i],
      i,
      list->next[i],
      i,
      list->prev[i]
    );
  }

  fprintf(dump_file, "\n");

  for (int i = 0; i < list->size; i++) {
    fprintf(dump_file, 
        "next_%d -> elem_%d [color=black];\n"
        "prev_%d -> elem_%d [color=blue];\n",
        i, list->next[i], i, list->prev[i]
    );
  }


  fprintf(dump_file, "}\n");

  fclose(dump_file);

  return OK;
}
