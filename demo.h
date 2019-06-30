#ifndef DEMO_H_
#define DEMO_H_

#include <sqlite3.h>

int RegisterDemo(sqlite3 *db);

struct DemoTable {
  sqlite3_vtab base;
};

struct DemoCursor {
  sqlite3_vtab_cursor base;
};

#endif  // DEMO_H_
