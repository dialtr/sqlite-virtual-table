#include <errno.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo.h"

const char* TEST_DB_PATH = "db/test.db";

int callback(void* v, int n, char** arg1, char** args2) {
  printf("got here\n");
  return 0;
}

int main(int argc, char* argv[]) {
  sqlite3* db = NULL;
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
  char* msg = NULL;
  status = sqlite3_exec(db, "SELECT * FROM demo;", callback, NULL, &msg);
  //status = sqlite3_exec(db, "CREATE VIRTUAL TABLE demotab using demo;",
  //                      callback, NULL, &msg);
  if (status == SQLITE_OK) {
    fprintf(stderr, "OK\n");
  } else {
    fprintf(stderr, "error %d returned from sqlite3_exec(): %s\n", status,
            msg ? msg : "(no message)");
  }
	*/
	sqlite3_stmt* stmt = NULL;
	status = sqlite3_prepare_v2(db, "SELECT value FROM demo;", -1, &stmt, NULL);
	if (status != SQLITE_OK) {
		fprintf(stderr, "error %d returned from sqlite3_prepare_v2(): %s\n",
				status, sqlite3_errmsg(db));
	} else {
		fprintf(stderr, "OK\n");
	}

  // Close the db instance.
  sqlite3_close(db);

  return 0;
}
