# JPAM: Just-Another-Package-Manager 
[![CodeFactor](https://www.codefactor.io/repository/github/thealexdev23/japm/badge)](https://www.codefactor.io/repository/github/thealexdev23/japm)

I've seen many people trying to build their own package manager recently, so I also decided to try it out.

For now it's not finished and doesn't fully work. I'm only using GitHub to host the code.

## Dependencies:

```
libjson-c
```

# Bulding from source

### Debian or Debian based


- Installing dependencies:

```
sudo apt instal libjson-c-dev cmake
```

- Building:

```
git clone https://github.com/TheAlexDev23/japm.git
cd japm
make install
```
#### OR

```
git clone https://github.com/TheAlexDev23/japm.git
cd japm
mkdir build
cmake -S src -B build
cd build
make
```
Documentation:

- Coming soon
