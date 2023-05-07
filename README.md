# JAPM: Just-Another-Package-Manager 
[![CodeFactor](https://www.codefactor.io/repository/github/thealexdev23/japm/badge)](https://www.codefactor.io/repository/github/thealexdev23/japm) 
![](https://tokei.rs/b1/github/thealexdev23/japm)

I've seen many people trying to build their own package manager recently, so I also decided to try it out.

# Important
Doesn't fully work in the current state.

If you want to use the package manager the way it worked before you can either tweak a bit the source code or install the v1.0.0 release (old JAPM without JAPML).

# Milestones => Deprecated
- [x] Package Installation
- [x] Package Removal
- [x] Package Update
- [x] Package Search

# JAPML necessary milestones
- [x] Logging system
- [x] Error system
- [ ] Input parsing system
- [x] JSON parsing
- [ ] Local DB reading => doing 
- [x] Remote DB reading
- [ ] Actions system => doing
- [ ] Package Installation => doing 
- [ ] Package Removal

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
