# Abstract

Test runner script.

# Dependencies

Python 3.1 or newer.

Tested on:

    $ uname -a
    Linux debian 3.16.0-4-amd64 #1 SMP Debian 3.16.36-1+deb8u2 (2016-10-19) x86_64 GNU/Linux
    $ python --version
    Python 2.7.9
    $ python3 --version
    Python 3.4.2

# Change Log

## Version 0.1

Tests must reside in the same directory as this script:

    .
    |-- ...
    |-- libtap.so
    |-- testrunner/
    \---.
        |-- test_base_api.c
        \-- ...

Tests get compiled against libtap:

    # You *must* provide CFLAGS, TAPDIR, and libtap.so must
    # be in your LD_LIBRARY_PATH.
    $ export CFLAGS='-m64 -W -Wall -std=c11 -pedantic -O2 -s'
    $ export TAPDIR='/home/user/Code/libtap/'
    $ make 
    $ LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TAPDIR:. python testrunner

## Version 0.2

Now takes arguments like a standard shell program:

    $ python testrunner --help

A test is anything without a '.' in its base file name:

    .
    |-- is
    |-- is.c
    |-- is.o
    |-- like
    |-- like.c
    |-- like.o
    |-- simple
    |-- simple.c
    |-- simple.o
    |-- skip
    |-- skip.c
    |-- skip.o
    |-- todo.c
    |-- todo.o
    \-- todo

Only `is`, `like`, `simple`, `skip`, and `todo`, would get picked up.

## Version 0.3

GPLv3 licenced, full license in COPYING.

Usage instructions.

# License

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
