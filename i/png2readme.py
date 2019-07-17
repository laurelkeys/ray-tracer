#!/usr/bin/env python3

# ref.: 
# https://stackoverflow.com/questions/168409/how-do-you-get-a-directory-listing-sorted-by-creation-date-in-python/539024#539024
# https://docs.python.org/2/library/stat.html#stat.ST_CTIME

from stat import S_ISREG, ST_CTIME, ST_MODE
import os, sys, time, math
import numpy as np

# path to the directory (relative or absolute)
dirpath = r'.'

# get all entries in the directory w/ stats
entries = (os.path.join(dirpath, fn) for fn in os.listdir(dirpath))
entries = ((os.stat(path), path) for path in entries)

# leave only .png files and insert creation date
entries = [(stat[ST_CTIME], path) for stat, path in entries if S_ISREG(stat[ST_MODE]) and path.endswith('.png')]

# NOTE: on Windows `ST_CTIME` is the creation date
entries = sorted(entries)

# the README table has 3 columns by default
cols = int(sys.argv[1]) if len(sys.argv) == 2 else 4

def readme_fname(path):
    return f"![]({os.path.basename(path)})"

table = np.array([readme_fname(path) for _, path in entries])
# print(f"{table.size} entries, {cols} columns, {math.ceil(table.size / cols)} rows")

table = np.append(table, np.tile([' '], cols - (table.size % cols)))
table = table.reshape((-1, cols))
print("| " * cols + '|')
print("| :---: " * cols + '|')
for row in table:
    print('| ' + ' | '.join(row) + ' |')