# Logging

JAPML has a logging system (implemented at log.c at lib/libjapml/log.c).

This system can log into log files (or log files for errors, if the log is a error) provided when the program is ran.

The logging system will also log the message in the curses window, if curses is enabled when running the program.

## Log levels

The following levels exist when logging:
- Debug = 0
- Information = 1
- Error = 2
- Critical = 3

Logs smaller than handle.log_level will not be logged. 

## How to log

To log a message: `japml_log(handle, log_level, message)` (log.h), note that it will also log with curses is necesary.

To log a curses only message: `japml_ncurses_log(handle, log_level, message)` (japmlcurses.h), note that it might throw an error if curses is not initialized (for now that is, since i'll forget to add security checks as always).