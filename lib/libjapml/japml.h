#ifndef _JAPML_H_INCLUDED_
#define _JAPML_H_INCLUDED_

/* A handle is a instance of JAMPL */
typedef struct _japml_handle japml_handle_t;

typedef enum _japml_arg_type japml_arg_type_t;
typedef struct _japml_parse_parameters japml_parse_parameters_t;

/* A linked list */
typedef struct _japml_list japml_list_t;

/* 
An action is a way that the frontend can use to make JAPML install/remove packages.
An action is a struct that stores the packages JAPML will remove/install.
An action can only either remove or install packages.
*/
typedef struct _japml_action japml_action_t;
typedef enum _japml_action_type japml_action_type_t;
typedef enum _japml_action_status japml_action_status_t;

/* Enum representing an action you can do with a package */
typedef enum _japml_package_action japml_package_action_t;
/* Represents a JAPML package */
typedef struct _japml_package japml_package_t;

/* A structure representin a remote databse */
typedef struct _japml_db_remote japml_db_remote_t;

/* JAPML errors */
typedef enum _japml_error japml_error_t;

/* A log level for logging */
typedef enum _japml_log_level japml_log_level_t;

/* Has all the information to write a log */
typedef struct _japml_log_message japml_log_message_t;

#endif
