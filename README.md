<div align="center">
  <div>
    <img src="/static/japmgif.gif"/>
  </div>
  <div>
    <h1>JAPM</h1>
    <b>Just-Another-Package-Manager</b>
  </div>
  <br/>
  <div>
    <a href="https://www.codefactor.io/repository/github/thealexdev23/japm"><img src="https://www.codefactor.io/repository/github/thealexdev23/japm/badge" alt="CodeFactor" /></a>
    <img src="https://img.shields.io/github/issues-raw/thealexdev23/japm"/>
    <img src="https://img.shields.io/github/v/release/thealexdev23/japm"/>
    <img src="https://img.shields.io/github/license/thealexdev23/japm" />
    <img src="https://img.shields.io/github/actions/workflow/status/thealexdev23/japm/cmake.yml"/>
  </div>
</div>
</br>

JAPM is not intended to be the *"next package manger"* nor a large and powerfull package manager like apt or pacman. It is, rather, a small and light-weight package manager that can allow you to install small programs. These programs are usually made by indie open source developers. Having a separate package manager allows managing packages in a more organized way. From one side you will have large software that is installed by your main package manager and from the other you have small tools you found on github that make your day-to-day easier. Having all those packages managed by a single package manager can be uncomfortable, and JAPM tries to bring a solution to this.

JAPM is also different from other package managers in the sense that it uses ncurses to create a TUI and represent information in a more organized and better looking way.

However if you want to use it without ncurses it can also work as a normal comand-line package manager.

A package is a .json file usually saved in a remote repository, by default it is [Japm Official Packages](https://github.com/TheAlexDev23/japm-official-packages). If you want to create a package, said repository has the instructions.

# JAPM necessary milestones
- [x] Logging system
- [x] Error system
- [x] Input parsing system
- [x] JSON parsing
- [x] Local DB CRUD
- [x] Remote DB reading
- [x] Actions system
- [x] Package file download
- [x] Package Installation
- [x] Package Removal
- [x] Package Update
- [x] Package Search
- [x] Progress bar 
- [x] Package list

# Dependencies:

```
libjson-c sqlite3 ncurses curl
```

Note, that even though JAPML has an option of not displaying curses mode, the ncurses library is still required for building from source.

# Bulding from source

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
cmake -S . -B build
cd build
make
cp ./japm /usr/bin
```

# Downloading a release
Download the [latest release](https://github.com/TheAlexDev23/japm/releases/latest) and move japm to /usr/bin

# How to use
## Installing packages

This will install all packages and it's dependencies

`sudo japm install package_name1 package_name2 package_name2`

## Removing packages

Will remove if no packages depend on these.

`sudo japm remove package_name1 package_name2 package_name3`

If you want to delete a package and all packages that depend on it run the following:

`sudo japm -rec remove package_name1 package_name2 package_name3`

## Updating packages

Will reinstall selected packages but not dependencies

`sudo japm update package_name1 package_name2 package_name3`

If you want to update **all** installed packages

`sudo japm -all update`

## Searching for packages

Will search and print general information about a package.
`sudo japm search package_name1 package_name2 package_name3`

## Running without ncurses
`sudo japm -nocurses ...`

## Providing log files
`sudo japm --log_files logf1 logf2 logf3 --error_log_files errlogf1 errlogf2 errlogf3`

## Other commands
Run `sudo japm --help` for a list of available commands and parameters.
