#include "demo.h"
#include <assert.h>
#include <errno.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// In order to gain a better understanding of the order in which SQLite
// calls module functions, we print a message to standard error at the
// start of each function. The TRACE_FUNCTION() macro simplifies this.
#define TRACE_FUNCTION() fprintf(stderr, "TRACE: %s\n", __FUNCTION__)

// Our module will be named "demo".
const char *DEMO_MODULE_NAME = "demo";

// The schema for our demo table. For the demo, the table schema
// consists of a single column named 'value', which is an integer.
// The implementation of the virtual table informs the engine of
// the schema by calling 'sqlite3_declare_vtab'. The name of the
// table and any constraints are ignored. In this example, we
// declare the schema in the DemoConnect function, below.
const char *DEMO_TABLE_SCHEMA =
    "CREATE TABLE xx( "
    "  value INTEGER "
    ");";

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

// Implementation of the "xConnect" function. Implementations of this function
// are responsible for allocating a structure to track information about the
// table; this initial fields of this structure must share the same layout as
// the sqlite3_vtab structure, but may include additional fields as well. In
// this example, we allocate an instance of DemoTable using SQLite's malloc
// routine, and return it in ppVTab.
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
  DemoTable *table = (DemoTable *)(sqlite3_malloc64(sizeof(DemoTable)));
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
  *ppVTab = (sqlite3_vtab *)table;

  return SQLITE_OK;
}

// SQLite calls the xBestIndex function during the planning phase in order
// to determine the possible way to proceed with the query. The implementer
// of this function has a chance to examine aspects of the query (provided
// by the sqlite3_index_info* structure) and make recommendations of the
// cost back to the engine. The implementation also returns an integer
// identifier of the index number (also a field in sqlite3_index_info).
// The engine may call this function more than once. Ultimately, when it
// invokes the filter function to begin the "search" of the virtual table,
// it passes back the index identifier that it wants to use based on its
// cost determination.
int DemoBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *info) {
  TRACE_FUNCTION();

  // TODO(tdial): Implement

  return SQLITE_OK;
}

// Implementations of xDisconnect are responsible for cleaning up data
// structures used during the session.
int DemoDisconnect(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();

  // Just free the table structure.
  DemoTable *table = (DemoTable *)pVTab;
  sqlite3_free(table);

  return SQLITE_OK;
}

// Implementations of xDestroy are responsible for actually destroying any
// backing store. For example, if this implementation created files on disk,
// xDestroy would be expected to purge them. In our example, we have nothing
// to destroy, so we just return error. (Note: It may be more correct to
// return SQLITE_OK.)
int DemoDestroy(sqlite3_vtab *pVTab) {
  TRACE_FUNCTION();
  // TODO(tdial): Implement
  return SQLITE_ERROR;
}

// Routine the implements the opening of a cursor structure, which is really
// just an iterator over a result set. The cursor structure returned by the
// module must share the layout of sqlite3_vtab_cursor, but the struct
// may include additional fields for module-specific information. In this
// example, we allocate an instance of DemoCursor, and return a pointer to
// it in ppCursor after casting it to the generic engine type.
int DemoOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
  TRACE_FUNCTION();
  DemoCursor *cursor = (DemoCursor *)sqlite3_malloc64(sizeof(DemoCursor));
  if (!cursor) {
    return SQLITE_NOMEM;
  }

  memset(cursor, 0, sizeof(DemoCursor));
  cursor->base.pVtab = pVTab;
  cursor->n = 0;
  *ppCursor = (sqlite3_vtab_cursor *)cursor;

  return SQLITE_OK;
}

// The xClose function implements cursor cleanup.
int DemoClose(sqlite3_vtab_cursor *pCursor) {
  TRACE_FUNCTION();
  DemoCursor *cursor = (DemoCursor *)pCursor;
  sqlite3_free(cursor);
  return SQLITE_OK;
}

// The xFilter function is invoked by the engine during execution, and would
// be the appropriate place to arrange for the iteration of of the data set,
// make an external call to an API, etc.  The xFilter function is called with
// index parameters chosen by the engine from those provided by prior calls to
// the xBestIndex function.
int DemoFilter(sqlite3_vtab_cursor *cursor, int idxNum, const char *idxStr,
               int argc, sqlite3_value **argv) {
  TRACE_FUNCTION();
  // TODO(tdial): This method begins a search of a virtual table.
  return SQLITE_OK;
}

// Called to advance the cursor to the next position in the results.
int DemoNext(sqlite3_vtab_cursor *pCursor) {
  TRACE_FUNCTION();
  DemoCursor *cursor = (DemoCursor *)pCursor;
  ++cursor->n;
  return SQLITE_OK;
}

// Invoked to determine whether the cursor has reached the end of results.
int DemoEof(sqlite3_vtab_cursor *pCursor) {
  TRACE_FUNCTION();
  DemoCursor *cursor = (DemoCursor *)pCursor;
  return (cursor->n >= 10);
}

// Invoked by the engine to retrieve a column from the tuple that the cursor
// currently points to. Results are reported to the engine via, the:
// sqlite3_result_xxx family of functions. In this example, we're returning
// an integer value.
int DemoColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
  TRACE_FUNCTION();
  DemoCursor *cursor = (DemoCursor *)pCursor;
  // TODO(tdial): This implementation just returns an integer that's
  // stashed in the cursor. A real implementation would check the column
  // number specified 'i' and return a value that was appropriate for the
  // schema.
  sqlite3_result_int(ctx, cursor->n);
  return SQLITE_OK;
}

// Return a unique rowid for the row.
int DemoRowid(sqlite3_vtab_cursor *pCursor, sqlite_int64 *pRowid) {
  TRACE_FUNCTION();
  DemoCursor *cursor = (DemoCursor *)pCursor;
  // TODO(tdial): A real implementation might return a unique rowid
  // (a 64 bit integer.) Here, we just return the value of the integer
  // column.
  *pRowid = cursor->n;
  return SQLITE_OK;
}

//
// The following functions are intentionally not implemented for the demo.
//

int DemoUpdate(sqlite3_vtab *table, int n, sqlite3_value **val,
               sqlite_int64 *rowid) {
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
                     void (**pxFunc)(sqlite3_context *, int n,
                                     sqlite3_value **val),
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
int DemoSavepoint(sqlite3_vtab *pVTab, int n) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoRelease(sqlite3_vtab *pVTab, int n) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

int DemoRollbackTo(sqlite3_vtab *pVTab, int n) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

// The methods above are in versions 1 and 2 of the sqlite_module object.
// Those below are for version 3 and greater. */
int DemoShadowName(const char *name) {
  TRACE_FUNCTION();
  // Intentionally not implemented.
  return SQLITE_ERROR;
}

// Inform the engine of our module's functions. This example implements a
// simple read-only table with one column, so we don't provide any of the
// functions involved with writing to the database.
int RegisterDemo(sqlite3 *db) {
  TRACE_FUNCTION();
  static sqlite3_module module = {
      0,               // iVersion
      DemoConnect,     // xCreate
      DemoConnect,     // xConnect
      DemoBestIndex,   // xBestIndex
      DemoDisconnect,  // xDisconnect
      DemoDisconnect,  // xDestroy
      DemoOpen,        // xOpen
      DemoClose,       // xClose
      DemoFilter,      // xFilter
      DemoNext,        // xNext
      DemoEof,         // xEof
      DemoColumn,      // xColumn
      DemoRowid,       // xRowid
      0,               // xUpdate
      0,               // xBegin
      0,               // xSync
      0,               // xCommit
      0,               // xRollback
      0,               // xFindMethod
      0,               // xRename
      0,               // xSavepoint
      0,               // xRelease
      0,               // xRollbackTo
      0,               // xShadowName
  };

  return sqlite3_create_module(db, DEMO_MODULE_NAME, &module, 0);
}
