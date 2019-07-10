#ifndef DEMO_H_
#define DEMO_H_

#include <sqlite3.h>

int RegisterDemo(sqlite3 *db);

typedef struct DemoTable {
  sqlite3_vtab base;
} DemoTable;

typedef struct DemoCursor {
  sqlite3_vtab_cursor base;
  int n;
  int eof;
} DemoCursor;

#endif  // DEMO_H_
