# JAPM: Just-Another-Package-Manager 
[![CodeFactor](https://www.codefactor.io/repository/github/thealexdev23/japm/badge)](https://www.codefactor.io/repository/github/thealexdev23/japm) 
![](https://tokei.rs/b1/github/thealexdev23/japm)

I've seen many people trying to build their own package manager recently, so I also decided to try it out.

The package manager worked before, until I decided to rewrite it in a different way. Now all JAPM operations would be done in a separate library named [JAPML](lib/libjapml/README.md)

# Important

JAPM is undergoing heavy development, since I'm creating JAPML and without it JAPM doesn't work.

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
- [ ] Local DB reading
- [ ] Remote DB reading
- [ ] Actions system => doing (33% done)
- [ ] Package Installation => doing (66% done)
- [ ] Package Removal
- [ ] Package Update
- [ ] Package Search

# JAPML optional milestones
- [ ] Documentation => doing (about 7% done)

# When will JAPML be finished?

I'm not 100% sure but the progress bar for now is aproximately at 36.27% done.

Note that when it reaches 100%, it will only mean the the package manager has the basic features working (install, remove, update, search). I will still continue working on it to improve it's efficiency, bugs, security, tidiness, etc.

# Bulding from source

- Dependencies:

```
libjson-c sqlite3 ncurses curl
```

Note, that even though JAPML has an option of not displaying curses mode (still work in progress, the option is not fully implemented), the ncurses library is still required for building from source.

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
cmake -S . -B build
cd build
make
cp ./japm /usr/bin
```

# Documentation:

Also work in progress. Docs are found at docs/. I dont have a precise progress bar for that but it's about 7% done.
