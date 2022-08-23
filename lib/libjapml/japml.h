#ifndef JAPML_H_INCLUDED_
#define JAPML_H_INCLUDED_

#include "handle.h"
#include "log.h"
#include "db.h"
#include "list.h"
#include "package.h"
#include "error.h"
#include "exit.h"

/*
 * A handle is a instance of JAMPL, it should be created every time the program is run
 * */
typedef struct _japml_handle japml_handle_t;

typedef struct _japml_package japml_package_t;

typedef struct _japml_list japml_list_t;

typedef struct _japml_db_local japml_db_local_t;
typedef struct _japml_db_remote japml_db_remote_t;

typedef enum _japml_error japml_error_t;

typedef enum _japml_log_level japml_log_level_t;

#endif
