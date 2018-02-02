#!/bin/sh
#    MIT License
#
#    Copyright (c) 2018, Alexey Dynda
#
#    Permission is hereby granted, free of charge, to any person obtaining a copy
#    of this software and associated documentation files (the "Software"), to deal
#    in the Software without restriction, including without limitation the rights
#    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#    copies of the Software, and to permit persons to whom the Software is
#    furnished to do so, subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in all
#    copies or substantial portions of the Software.
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#    SOFTWARE.
#

platform=avr
extra_args=
mcu=
flash_target=


print_help_and_exit()
{
    echo "Usage: ./build_and_run.sh [options] example"
    echo "options:"
    echo "        -p      platform to compile for: avr, linux"
    echo "        -m      mcu to compiler for: atmega328p, attiny85, etc.."
    echo "        -f      flash controller after build or run executable for Linux"
    exit 1
}

while getopts ":p::m::f" opt; do
  case $opt in
    p) platform=$OPTARG;;
    m) mcu=$OPTARG;;
    f) flash_target=flash;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      print_help_and_exit
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      print_help_and_exit
      ;;
  esac
done

shift $((OPTIND-1))
if [ $# = 0 ]; then
    print_help_and_exit
fi

project=$1
shift

if [ "$mcu" != "" ]; then
    extra_args="MCU=$mcu $extra_args"
fi

make -C ../examples -f Makefile.${platform} PROJECT=${project} ${extra_args} ${flash_target} $@
