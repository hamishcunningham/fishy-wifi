#!/usr/bin/env python
# test-data-viz.py

import os
import json
import sys
import datetime
import time
import random
from collections import OrderedDict

def _create_path(filename):
    if not os.path.exists(os.path.dirname(filename)):
        try:
            os.makedirs(os.path.dirname(filename))
        except OSError as exc: # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise

def generate_data_point():
    point = OrderedDict()
    now = datetime.datetime.now()
    timestamp = time.mktime(now.timetuple())
    lux = random.randint(1000, 2000)
    airTemp = random.randint(10, 20)
    waterTemp = airTemp + 3
    pH = round(random.uniform(6.8, 7.2), 1)
    humidity = 42
    point['timestamp'] = timestamp
    point['airTemp'] = airTemp
    point['humidity'] = humidity
    point['lux'] = lux
    point['pH'] = pH
    point['waterTemp'] = waterTemp
    return point 


def add_data(elf_id, dd, cd):
    point = generate_data_point()
    dpath, cpath = _get_paths(elf_id, dd, cd)
    _write_data_line(dpath, json.dumps(point))
    print "Wrote data to %s" % dpath
    write_flag(cpath)

def write_flag(cpath):
    _touch_file(cpath)
    if not os.path.exists(cpath.replace("changes/", "")):
        print "Warning: no data file matching flag"
    print "Touched file %s" % cpath

def remove_file(path):
    try:
        os.remove(path)
    except OSError:
        print "Cannot remove %s" % path
        return
    print "Removed file %s" % path

def _get_paths(elf_id, dd, cd):
    filename = "%s_%s.txt" % (_get_today(), elf_id)
    dpath = os.path.join(dd, elf_id, filename)
    cpath = os.path.join(cd, elf_id, filename)
    return dpath, cpath

def _get_today():
    return datetime.datetime.now().date().strftime("%Y-%m-%d")

def _write_data_line(path, line):
    _create_path(path)
    with open(path, "a") as f:
        f.write(line + "\n")

def _touch_file(path):
    _create_path(path)
    with open(path, "w") as f:
        f.write("")

def usage():
    print "Usage: %s [COMMAND]" % sys.argv[0]
    print
    print "Simulate writing & flagging data for testing."
    print "Writes data to a test file '$DATE_$ELIFID.txt'"
    print "where $DATE is today's date and $ELFID is '%s'." % elf_id
    print
    print "Commands: "
    print "  add    -- add a data point to the test file (adds a flag)"
    print "  del    -- deletes all the data in the test file & any flags"
    print "  flag   -- create the 'changes' file flag"
    print "  unflag -- remove the 'changes' file flag"

def aperture():
    ap = """
              .,-:;//;:=,
          . :H@@@MM@M#H/.,+%;,
       ,/X+ +M@@M@MM%=,-%HMMM@X/,
     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-
    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.
  ,%MM@@MH ,@%=             .---=-=:=,.
  =@#@@@MX.,                -%HX$$%%%:;
 =-./@M@M$                   .;@MMMM@MM:
 X@/ -$MM/      Aperture      . +MM@@@M$
,@M@H: :@:      Science       . =X#@@@@-
,@@@MMX, .       Testing      /H- ;@M@M=
.H@@@@M@+,        Suite       %MM+..%#$.
 /MMMM@MMH/.                  XM@MH; =;
  /%+%$XHH@$=              , .H@@@@MX,
   .=--------.           -%H.,@@@@@MX,
   .%MM@@@HHHXX$$$%+- .:$MMX =M@@MM%.
     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=
       =%@M@M#@$-.=$@MM@@@M; %M%=
         ,:+$+-,/H#MMMMMMM@= =,
               =++%%%%+/:-.
"""
    for line in ap.split("\n"):
        for char in line:
            sys.stdout.write(char)
            sys.stdout.flush()
            if char is not " ":
                time.sleep(0.005)
        print

if __name__ == "__main__":
    changes_dir = os.path.join("..", "..", "wegrow-data", "changes")
    data_dir = os.path.join("..", "..", "wegrow-data")
    elf_id = "test-elf1"
    
    args = sys.argv[1:]
    try:
        mode = args[0]
    except IndexError:
        usage()
        sys.exit(1)
    dpath, cpath = _get_paths(elf_id, data_dir, changes_dir)
    if mode in ['add']:
        add_data(elf_id, data_dir, changes_dir)
    elif mode in ['del']:
        remove_file(dpath)
        remove_file(cpath)
    elif mode in ['flag']:
        write_flag(cpath)
    elif mode in ['unflag']:
        remove_file(cpath)
    elif mode in ['testing']:
        aperture()

