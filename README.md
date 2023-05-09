# JAPM: Just-Another-Package-Manager 
[![CodeFactor](https://www.codefactor.io/repository/github/thealexdev23/japm/badge)](https://www.codefactor.io/repository/github/thealexdev23/japm) 
![](https://tokei.rs/b1/github/thealexdev23/japm)

I've seen many people trying to build their own package manager recently, so I also decided to try it out.

JAPM is different from other package managers in the sense that it uses ncurses to create a TUI and represent information in a more organized and better looking way.

A package is a .json file usually saved in a remote repository, by default it is [Japm Official Packages](https://github.com/TheAlexDev23/japm-official-packages)

# Important
Doesn't fully work in the current state.

If you want to use the package manager the way it worked before you can either tweak a bit the source code or install the v1.0.0 release (old JAPM without JAPML).

# JAPM necessary milestones
- [x] Logging system
- [x] Error system
- [ ] Input parsing system
- [x] JSON parsing
- [x] Local DB CRUD => requires testing
- [x] Remote DB reading => requires testing
- [ ] Actions system => doing
- [x] Package Installation
- [ ] Package Removal
- [ ] Package Update

# Dependencies:

```
libjson-c sqlite3 ncurses curl
```

Note, that even though JAPML has an option of not displaying curses mode (still work in progress, the option is not fully implemented), the ncurses library is still required for building from source.

# Bulding from source

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
cmake -S . -B build
cd build
make
cp ./japm /usr/bin
```
