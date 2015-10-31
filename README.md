# rib
Ronen's Interactive BrainF**k Interpreter

## Introduction
RIB allows you to interactivly test and write BrainF**k programs.

Features include:
- Interactive interpretation of BrainF**k programs
- View the memory and pointer positions

RIB uses [BrainFuck.tk](http://www.brainfuck.tk) as a reference implementation.

## TODO
- Please note that this is a VERY VERY VERY quick implementation of a BF interpreter. There are many things that
don't look very good at all (like the memory view and debug output) or are not supported at all
(like spaces and comments). These will be added in the future.
- Currently, you can only write and interpret single-line BF programs. This will also be improved greatly,
as I plan to implement a Python-style program input mechanism.
- Debug mode must be enabled through compilation. This will be changed soon!

## Getting RIB
### System Requirements
- A C99-compliant compiler (tested only with GCC 4.8.4 on Ubuntu 14.04 TLS x86_64)
- GNU Make >= 2.69

### Installation

* Clone the repository using git:
```
$ git clone https://github.com/ronen25/rib
```
* Run the following commands:
```
$ ./configure
$ make all
```
* (Optional) install using the following command (root permissions may be required):
```
# make install
```
* Run the interpreter:
```
$ rib
```
