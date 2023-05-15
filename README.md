<div align="center">
  <div>
    <h1>JAPM</h1>
    <b>Just-Another-Package-Manager</b>
  </div>
  <br/>
  <div>
    <img src="https://www.codefactor.io/repository/github/thealexdev23/japm/badge"/>
    <img src="https://tokei.rs/b1/github/thealexdev23/japm"/>
  </div>
</div>
</br>


I've seen many people trying to build their own package manager recently, so I also decided to try it out.

JAPM is different from other package managers in the sense that it uses ncurses to create a TUI and represent information in a more organized and better looking way.

However if you want to use it without ncurses it can also work as a normal comand-line package manager.

A package is a .json file usually saved in a remote repository, by default it is [Japm Official Packages](https://github.com/TheAlexDev23/japm-official-packages)

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
