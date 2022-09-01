# JAPM: Just-Another-Package-Manager 
[![CodeFactor](https://www.codefactor.io/repository/github/thealexdev23/japm/badge)](https://www.codefactor.io/repository/github/thealexdev23/japm) 
![](https://tokei.rs/b1/github/thealexdev23/japm)

I've seen many people trying to build their own package manager recently, so I also decided to try it out.

The package manager worked before, until I decided to rewrite it in a different way. Now all JAPM operations would be done in a separate library named [JAPML](lib/libjapml/README.md)

# Milestones => Deprecated, will not use anymore. Package manager is only usabe if you tweak a bit the source code to work as before
- [x] Package Installation
- [x] Package Removal
- [x] Package Update
- [x] Package Search

# JAPML necessary milestones (milestones required for JAPML to work) => doing (36.27% done)
- [x] Logging system
- [x] Error system
- [ ] Input parsing system
- [x] JSON parsing
- [ ] Local DB reading
- [ ] Remote DB reading
- [ ] Actions system => doing (33% done)
- [ ] Package Installation => doing (66% done)
- [ ] Package Removal
- [ ] Package Update
- [ ] Package Search

# JAPML optional milestones
- [ ] Documentation => doing (about 7% done)

# Bulding from source

- Dependencies:

```
libjson-c sqlite3 ncurses
```

Note, that even though JAPML has an option of not displaying curses mode (still work in progress, the option is not fully implemented), the ncurses library is still required for building from source.

- Building:

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
chmod +x build.sh
./build.sh
```
Or

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
mkdir build
cmake -S src -B build
cd build
make
```

- Documentation:

    - Coming soon
