# rib
Ronen's Interactive BrainF**k Interpreter

## Introduction
RIB allows you to interactivly test and write BrainF**k programs.

Features include:
- Interactive interpretation of BrainF**k programs
- View the memory and pointer positions

RIB uses [BrainFuck.tk](http://www.brainfuck.tk) as a reference implementation.

## Getting RIB
### System Requirements
- A C99-compliant compiler (tested only with GCC 4.8.4 on Ubuntu 14.04 LTS AMD64)
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
