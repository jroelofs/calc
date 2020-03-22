[![Build Status](https://travis-ci.org/jroelofs/calc.svg?branch=master)](https://travis-ci.org/jroelofs/calc)
[![Coverage Status](https://coveralls.io/repos/github/jroelofs/calc/badge.svg)](https://coveralls.io/github/jroelofs/calc)

## Toy Calculator

Simple toy calcualtor that parses a command-line provided string, and
evaluates the resulting expression.

### Build

```sh
git clone git@github.com:jroelofs/calc.git
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ../calc
ninja
```

### Run

`crepl` can either be used with and expression as the command-line argument:
```sh
$ ./bin/crepl "1 + 1"
2
$ ./bin/crepl "1 + 2 * 3"
7
```

or with pipes:
```sh
$ echo "2*(1+2)" | ./bin/crepl -
6
```

or as a Read-Evaluate-Print-Loop, or REPL:
```
$ ./bin/crepl
> 1+2
3
> 3*4
12
> q
```

### Test

```sh
$ ninja check
```

