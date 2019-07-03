#include "demo.h"
#include <assert.h>
#include <errno.h>
#include <sqlite3.h>
#include <string.h>
#include <cstdio>

#define TRACE_FUNCTION() \
	fprintf(stderr, "TRACE: %s\n", __FUNCTION__)

const char *DEMO_TABLE_NAME = "demo";

// The schema for our demo table. For the demo, the table schema
// consists of a single column named 'value', which is an integer.
// The implementation of the virtual table informs the engine of
// the schema by calling 'sqlite3_declare_vtab'. The name of the
// table and any constraints are ignored.
const char *DEMO_TABLE_SCHEMA = 
  "CREATE TABLE xx( "
	"  value INTEGER "
	");";

extern "C" {

// The "xCreate" function is intentionally not implemented because
// this demo implements an "eponymous virtual table", one that does
// not require explicit creation. To signal to the engine that we're
// an eponymous table implementation, we pass DemoConnect for both the
// xCreate and xConnect functions.
int DemoCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
               sqlite3_vtab **ppVTab, char **pzErr) {
  TRACE_FUNCTION();
  return SQLITE_ERROR;
}

int DemoConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                sqlite3_vtab **ppVTab, char **pzErr) {
  TRACE_FUNCTION();

	// Inform the SQLite engine of our virtual table's schema.
	int status = sqlite3_declare_vtab(db, DEMO_TABLE_SCHEMA);
	if (status != SQLITE_OK) {
		return status;
	}

  // Allocate a copy of our custom table structure.
  // See demo.h for details.
//DemoTable *table = new DemoTable();
  DemoTable *table = reinterpret_cast<DemoTable*>(
			sqlite3_malloc64(sizeof(DemoTable)));
  if (!table) {
    // In the unlikely event that we couldn't allocate memory for the
    // structure, return the following error:
    return SQLITE_NOMEM;
  }

  // Clear out all the members of the structure.
  memset(table, 0, sizeof(DemoTable));

  // TODO(tdial): In a real implementation, we'd potentially use information
  // from the arguments to populate private fields in the DemoTable structure.
  // Since there aren't any at this time, this is left as a TODO item.

  // Cast the table structure to the "base" type and return it to the engine.
  *ppVTab = reinterpret_cast<sqlite3_vtab *>(table);

  return SQLITE_OK;
}

int DemoBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoDisconnect(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();

	// Just delete the table structure.
  DemoTable *table = reinterpret_cast<DemoTable *>(pVTab);
  //delete table;
	sqlite3_free(table);

  return SQLITE_OK;
}

int DemoDestroy(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoClose(sqlite3_vtab_cursor *) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoFilter(sqlite3_vtab_cursor *, int idxNum, const char *idxStr, int argc,
               sqlite3_value **argv) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoNext(sqlite3_vtab_cursor *) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoEof(sqlite3_vtab_cursor *) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoColumn(sqlite3_vtab_cursor *, sqlite3_context *, int) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

int DemoRowid(sqlite3_vtab_cursor *, sqlite_int64 *pRowid) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

//
// The following functions are intentionally not implemented for the demo.
//

int DemoUpdate(sqlite3_vtab *, int, sqlite3_value **, sqlite_int64 *) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoBegin(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoSync(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoCommit(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoRollback(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                     void (**pxFunc)(sqlite3_context *, int, sqlite3_value **),
                     void **ppArg) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoRename(sqlite3_vtab *pVtab, const char *zNew) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

// The methods above are in version 1 of the sqlite_module object. Those
// below are for version 2 and greater.
int DemoSavepoint(sqlite3_vtab *pVTab, int) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoRelease(sqlite3_vtab *pVTab, int) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoRollbackTo(sqlite3_vtab *pVTab, int) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

// The methods above are in versions 1 and 2 of the sqlite_module object.
// Those below are for version 3 and greater. */
int DemoShadowName(const char *) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

}  // extern "C"


// Register the demo table.
int RegisterDemo(sqlite3 *db) {
  sqlite3_module module;
  memset(&module, 0, sizeof(module));

  module.iVersion = 0;

  // xCreate and xConnect are both passed "DemoConnect"
  // because this is an eponymous virtual table.
  module.xCreate = DemoConnect;
  module.xConnect = DemoConnect;
  module.xBestIndex = DemoBestIndex;

  // Disconnect and Destroy are both passed "DemoDisconnect" because
  // our table has no backing store. We just want to free the table
  // structure that happens.
  module.xDisconnect = DemoDisconnect;
  module.xDestroy = DemoDisconnect;

  module.xOpen = DemoOpen;
  module.xClose = DemoClose;
  module.xFilter = DemoFilter;
  module.xNext = DemoNext;
  module.xEof = DemoEof;
  module.xColumn = DemoColumn;
  module.xRowid = DemoRowid;

  // We don't support writing at this time in our demo.
  module.xUpdate = nullptr;        // DemoUpdate;
  module.xBegin = nullptr;         // DemoBegin;
  module.xSync = nullptr;          // DemoSync;
  module.xCommit = nullptr;        // DemoCommit;
  module.xRollback = nullptr;      // DemoRollback;
  module.xFindFunction = nullptr;  // DemoFindFunction;
  module.xRename = nullptr;        // DemoRename;
  module.xSavepoint = nullptr;     // DemoSavepoint;
  module.xRelease = nullptr;       // DemoRelease;
  module.xRollbackTo = nullptr;    // DemoRollbackTo;
  module.xShadowName = nullptr;    // DemoShadowName;

  return sqlite3_create_module(db, DEMO_TABLE_NAME, &module, 0);
}
