#!/usr/bin/env python
# update-data-viz.py
#
# script to rewrite output/data, an html tree that visualises WaterElf data
# from ../../wegrow-data
#
# dependencies:
# sudo pip install prettytable
#
# data files that have changed are listed in 
#   ../../wegrow-data/changes/88888/elf-today-88888.txt
# for each of these we read the corresponding data from
#   ../../wegrow-data/88888/elf-today-88888.txt
# and write .htmls to output/data
# (these are then served via iframes in the Pelican-generated html tree)

import os
import json
import prettytable
import errno

# Data Handling

def load_data_file(path):
    with open(path, "r") as f:
        lines = f.readlines()
    data = map(lambda x: json.loads(x), lines)
    return data

def make_table(data, headers=None):
    if headers is None:
        headers = data[0].keys()
    rows = _convert_data_to_rows(data, headers)
    table = prettytable.PrettyTable(headers)
    for row in rows:
        table.add_row(row)
    return table
        
def _convert_data_to_rows(data, headers):
    rows = []
    for point in data:
        row = []
        for header in headers:
            val = point[header]
            row.append(val)
        rows.append(row)
    return rows

# Flag Stuff

def get_changed_paths(changes_dir, flags=False):
    """Return the paths of changed files or the paths of flags"""
    changed_files = []
    for root, subs, files in os.walk(changes_dir):
        paths = map(lambda x: os.path.join(root, x), files)
        if not flags:
            paths = map(lambda x: x.replace("changes%s" % os.path.sep, ""), paths)
        changed_files += paths
    return changed_files

def yield_changed_file_objects(changes_dir):
    """Generator to iterate through files that have changed"""
    changed = get_changed_paths(changes_dir)
    for cf in changed:
        data = load_data_file(cf)
        table = make_table(data)
        yield cf, data, table

def wipe_flags():
    flags = get_changed_paths(changes_dir, flags=True)
    for flag in flags:
        os.remove(flag)

# HTML Schemes

def create_data_table_htmls(changes_dir):
    """Create HTML file tables from all of the raw data files"""
    for original_path, data, table in yield_changed_file_objects(changes_dir):
        path = _make_html_path(original_path)
        path = _add_filename_prefix(path, "table%stable_" % os.path.sep)
        _create_path(path)

        html = table.get_html_string()
        write_html_file(path, html)

# HTML Writing

def write_html_file(path, page_html, use_base=True):
    # Input any base HTML here!
    base_html = ""

    html = ""
    if use_base:
        html += base_html
    html += page_html
    html += "\n" 
     
    print "Writing HTML to %s" % path
    with open(path, "w") as f:
        f.write(html)

def _add_filename_prefix(path, text):
    """Add text to the start of a filename in a path"""
    split = os.path.split(path)
    name = split[1]
    new_name = text + name
    path = os.path.join(split[0], new_name) 
    return path

def _make_html_path(input_path):
    """Return a .html path from the data file path"""
    filename = os.path.basename(input_path)
    name = os.path.splitext(filename)[0]
    new_name = name + ".html"
    new_path = os.path.join(html_output_dir, new_name)    
    return new_path

def _create_path(filename):
    if not os.path.exists(os.path.dirname(filename)):
        try:
            os.makedirs(os.path.dirname(filename))
        except OSError as exc: # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise

def main():
    create_data_table_htmls(changes_dir)
    wipe_flags()

if __name__ == "__main__":
    changes_dir = os.path.join("..", "..", "wegrow-data", "changes")
    data_dir = os.path.join("..", "..", "wegrow-data")
    html_output_dir = os.path.join("output", "data")
    main()

