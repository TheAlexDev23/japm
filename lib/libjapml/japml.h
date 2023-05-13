#ifndef _JAPML_H_INCLUDED_
#define _JAPML_H_INCLUDED_

/* A handle is an instance of JAMPL, with all the information required for the library to work. */
typedef struct _japml_handle japml_handle_t;

/* Type of comand line argument */
typedef enum _japml_arg_type japml_arg_type_t;
/* Structure representing comand line parameters */
typedef struct _japml_parse_parameters japml_parse_parameters_t;

/* A linked list */
typedef struct _japml_list japml_list_t;

/* 
* Actions are used to represent packages that need to be installed/removed
* They are used by the JAPML API to perform package management.
* A pacakge can only either install or remove packages.
*/
typedef struct _japml_action japml_action_t;
typedef enum _japml_action_type japml_action_type_t;
typedef enum _japml_action_status japml_action_status_t;

/* Enum representing an action you can do with a package */
typedef enum _japml_package_action japml_package_action_t;
/* Represents a JAPML package */
typedef struct _japml_package japml_package_t;

/* A structure representing a remote databse */
typedef struct _japml_db_remote japml_db_remote_t;

/* JAPML errors */
typedef enum _japml_error japml_error_t;

/* A log level for logging */
typedef enum _japml_log_level japml_log_level_t;

/* Has all the information to write a log */
typedef struct _japml_log_message japml_log_message_t;

#endif
