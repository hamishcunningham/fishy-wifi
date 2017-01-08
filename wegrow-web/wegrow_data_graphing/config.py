import os
from ConfigParser import SafeConfigParser

config = SafeConfigParser()
config.read("config.ini")
DATA_DIR = config.get("input", "data_dir_path")
OUTPUT_DIR = config.get("output", "output_dir_path")
