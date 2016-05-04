#!/usr/bin/env python
#
# script to rewrite output/data, an html tree that visualises WaterElf data
# from ../../wegrow-data
#
# data files that have changed are listed in 
#   ../../wegrow-data/changes/88888/elf-today-88888.txt
# for each of these we read the corresponding data from
#   ../../wegrow-data/88888/elf-today-88888.txt
# and write .htmls to output/data
# (these are then served via iframes in the Pelican-generated html tree)
