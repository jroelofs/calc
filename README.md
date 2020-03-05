## Toy Calculator

Simple toy calcualtor that parses a command-line provided string, and
evaluates the resulting expression.

### Build

```sh
git clone git@github.com:jroelofs/calc.git
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug calc
cd build
ninja
```

### Run

```sh
$ ./bin/crepl "1 + 1"
2
$ ./bin/crepl "1 + 2 * 3"
7
```