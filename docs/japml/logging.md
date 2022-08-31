# Logging

JAPML has a logging system (implemented at log.c at lib/libjapml/log.c).

This system can log into log files (or log files for errors, if the log is a error) provided when the program is ran.

The logging system will also log the message in the curses window, if curses is enabled when running the program.

## Log levels

The following levels exist when logging:
- Debug
- Information
- Error
- Critical

## Levels JAPML will not log

If JAPML is asked to log but the level of the log is under the log level the handle has set, JAPML will not log.

For example:

If the log function is called but the level of the log is less important than the log level at handle.log_level, the message will not be logged.

### What does it mean for a log to be less important?

The order of logs by importance is the following:

- Debug
- Information
- Error
- Critical

Just as defined in code or above when explaining what a log level is.

Basically what this means is that Debug is less important than Error or Information.

Why is this needed you may ask? Well the normal user doesnt want to see hundreds of debug messages when installing a package, but a developer does. So if we set the log level to debug, it will be useful for developers and if we set it to information then the average person could use it wihout getting blasted with thousands of debug messages. Or if you only want to see errors / critical errors you can just set the log_level to Error and not a single Debug or Information log will bother you.

## How to log

To log a message `japml_log()` function needs to be called. This function takes as input:

- A handle
- A log level
- And a message

## How it works

When called japml_log will first check if the message can be logged (it's above the handle log_level). Then if the message is a normal log (Debug, Information) it will be logged into the normal log files (provided in handle) but if they are error logs (Error, Critical) they will be logged into the error log files. 

Then if the user had chosen to use curses will call `japml_ncurses_log()` to log the message into the curses log screen.