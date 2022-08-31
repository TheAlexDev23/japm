# Initialazation

To initialize JAPML you need to call one of the initialazation methods. Those methods will return a handle that you will use throughout your program and it represents a instance of JAPML.

## Current implementation

The current way of doing things is the following:

There are 2 methods to initialize JAPML:

- `japml_init_default()`: to initialize JAPML with default settings (like it will be used normally not for development/debugging)
- `japml_init_devel()`: to initialize JAPML with development settings

Both functions take argc and argv from the main function.

## Implementation I'm aiming for

There will be a single method:

`japml_init()`: it will take argc and argv from the main function and using the argument parser (still not implemented that's why the current implementation has different functions to initialize JAPML) it will initialize and configure JAPML the way the arguments demand.