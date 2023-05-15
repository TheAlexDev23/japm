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
