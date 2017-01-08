import os
import datetime
import json

import pandas as pd

from config import DATA_DIR, OUTPUT_DIR


"""Functions to easily return flat-file Elf data as pandas dataframes for visualisation.
   Plotting can be done with the `seaborn` package.
   Writing this has made me questioning why we're not using a database..."""


def _setwd_input():
    """Run before getting any inputs"""
    os.chdir(DATA_DIR)

def get_available_elves():
    _setwd_input()
    """Return list of available elf data directories"""
    elves = next(os.walk("."))[1]
    elves.remove("changes")
    return elves

def validate_elf_existance(elf):
    _setwd_input()
    """Make sure an elf data directory exists"""
    if elf not in get_available_elves():
        raise NameError("No elf named %s in %s" % (elf, DATA_DIR))

def validate_date_format(date):
    _setwd_input()
    """Make sure a date is in the YYYY-MM-DD format
    
       This format is set assuming that the elf text files use this date format
    """
    try:
        datetime.datetime.strptime(date, '%Y-%m-%d')
    except ValueError:
        raise ValueError("Incorrect data format, should be YYYY-MM-DD")

def get_dates_for_elf(elf):
    _setwd_input()
    """Return list of dates from filenames of data files for a given elf"""
    validate_elf_existance(elf)
    files = map(lambda x: x.split("_")[0], os.listdir(elf))
    return files

def construct_filename_from_id_and_date(elf, date):
    _setwd_input()
    """Return an elf filename from an elf name and a date

       Constructs the filename in the format: "<elf_name>_<date>"
    """

    validate_date_format(date)
    filename = "%s_%s.txt" % (date, elf)
    return filename

def load_elf_data(elf, date):
    _setwd_input()
    """Return a pandas dataframe from an elf data file"""
    validate_elf_existance(elf)

    filename = construct_filename_from_id_and_date(elf, date)
    filepath = os.path.join(DATA_DIR, elf, filename)

    with open(filepath) as f:
        frame = pd.read_json('[%s]' % ','.join(f.readlines()))

    frame['file_date'] = date 
    frame['elf_id'] = elf
    return frame

def try_load_elf_data(elf, date, die_on_missing):
    _setwd_input()
    """Return data for a given elf on a given date.
    
       die_on_missing boolean flag controls whether or not the program
       should raise an exception if the file cannot be opened.
    """
    try:
        frame = load_elf_data(elf, date)
    except IOError as e:
        if die_on_missing:
            raise e
        else:
            return None
    else:
        return frame

def load_elves_data(elves, date, die_on_missing=True):
    _setwd_input()
    """
        Return a concatenated pandas dataframe for a list of elves on one given date.
    """
    frames = []
    for elf in elves:
        frame = try_load_elf_data(elf, date, die_on_missing=die_on_missing)
        if frame is not None:
            frames.append(frame)
    if frames:
        return pd.concat(frames)
    return None

def load_elves_data_today(elves, die_on_missing=True, today_date_override=None):
    _setwd_input()
    """Return a pandas dataframe for a list of elves from today's data file(s)"""
    date = datetime.datetime.now().strftime("%Y-%m-%d")
    if today_datetime_override is not None:
        date = today_date_override

    return load_elves_data(elves, date, die_on_missing=die_on_missing)


def load_elves_data_last_n_days(elves, n_days, die_on_missing=True, today_date_override=None):
    _setwd_input()
    """Return a concatenated dataframe for multiple elves over the last n number of days
    (inclusive of today)
    """
    today = datetime.datetime.now()
    if today_date_override is not None:
        today = datetime.datetime.strptime(today_date_override, "%Y-%m-%d")

    dates = [(today - datetime.timedelta(days=x)).strftime("%Y-%m-%d") for x in range(0, n_days)]

    concat_date_frames = []
    for date in dates:
        concat_for_date = load_elves_data(elves, date, die_on_missing=die_on_missing)
        if concat_for_date is not None:
            concat_date_frames.append(concat_for_date)
    
    if concat_date_frames:
        return pd.concat(concat_date_frames)
    return None


if __name__ == "__main__":
    # Example
    elves = ['elf-denny', 'elf-hbn-asg']
    date = "2016-12-20"
    frame = load_elves_data_last_n_days(elves, 5, die_on_missing=False, today_date_override=date)
    print set(frame.file_date)

