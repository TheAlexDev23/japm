# JAPM: Just-Another-Package-Manager 
[![CodeFactor](https://www.codefactor.io/repository/github/thealexdev23/japm/badge)](https://www.codefactor.io/repository/github/thealexdev23/japm) 
![](https://tokei.rs/b1/github/thealexdev23/japm)

I've seen many people trying to build their own package manager recently, so I also decided to try it out.

# Milestones
- [ ] Creating japml, a library that handles all operations where japm would just be it's implementation
- [x] Package Installation
- [x] Package Removal
- [x] Package Update
- [x] Package Search

# Bulding from source

## Dependencies:

```
libjson-c sqlite3
```


### Debian or Debian based


- Installing dependencies:

```bash
sudo apt instal libjson-c-dev sqlite3 cmake
```

- Building:

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
chmod +x build.sh
./build.sh
```
#### OR

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
mkdir build
cmake -S src -B build
cd build
make
```

### Arch or Arch Base

- Installing dependencies:

```bash
sudo pacman -S lib32-json-c sqlite3 cmake
```

- Building:

```bash
git clone https://github.com/TheAlexDev23/japm.git
cd japm
chmod +x build.sh
./build.sh
```
#### OR

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
