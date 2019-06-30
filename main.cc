#include <errno.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include "demo.h"

const char* TEST_DB_PATH = "db/test.db";

int main(int argc, char* argv[]) {
  sqlite3* db = nullptr;
  int status = 0;

  // Open a DB instance.
  status = sqlite3_open(TEST_DB_PATH, &db);
  if (status != SQLITE_OK) {
    fprintf(stderr, "failed to open database: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  // Register the "demo" module.
  status = RegisterDemo(db);
  if (status != 0) {
    fprintf(stderr, "failed to register module: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  /*
// Try a select.
char* msg = nullptr;
status = sqlite3_exec(db, "SELECT * FROM demo", nullptr, nullptr, &msg);
if (status == SQLITE_OK) {
fprintf(stderr, "OK\n");
} else {
fprintf(stderr, "error %d returned from sqlite3_exec()\n", status);
}
  */

  // Close the db instance.
  sqlite3_close(db);

  return 0;
}
