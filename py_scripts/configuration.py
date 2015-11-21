# Copyright (C) 2014, 2015 by Ali Baharev <ali.baharev@gmail.com>
# All rights reserved.
# BSD license.
# https://github.com/baharev/CSV_Test

from platform import system
from os.path import join

def get_root():
    from os.path import dirname, normcase, normpath
    return normcase(normpath(dirname(dirname(__file__))))

IS_WIN = system()=='Windows'

#-------------------------------------------------------------------------------
# Run the SG2PS executable in a debugger. If it crashes, we get a stack trace.
# Assumes that the stacktrace.gdb is in the etalon rgf folder.
# Useful if the sg2ps executable is compiled with -ggdb3 and -O0 or -Og.
RUN_IN_DEBUGGER = False

# This is the directory where the git repo (Eclipse project) of SG2PS lives
SG2PS_HOME = get_root()

# Assumes the default directory layout of Eclipse and project name SG2PS
SG2PS_EXE = join(SG2PS_HOME, 'Debug', 'SG2PS.exe' if IS_WIN else 'SG2PS')

FLAG =  '--debug'

INPUT_EXT = '.rgf'

# The .rgf files in this folder will be used to generate the CSV files.
RGF_FOLDER = join(SG2PS_HOME, 'etalon_rgf')

# The directory with the expected output (etalon CSV files).
ETALON_DIR = join(SG2PS_HOME, 'etalon')

# Extension of the input CSV files (both the etalon and the test files). All 
# other file types will be ignored in the comparison.
EXTENSION  = '.csv'

# Separator in the input CSV files.
SEP  = '\t'

# Root directory of the other temporary directories
TMP_ROOT = 'D:\\tmp' if IS_WIN else '/tmp' 

# The directory with the CSV files that should be compared to the etalon CSVs.
# WARNING: This directory will be *deleted* every time sg2ps_runner.py is run!
TOCOMP_DIR = join(TMP_ROOT, 'sg2ps_tests', 'to_compare')

# Save the console output of sg2ps into a <project name>.log file
LOG_EXT = '.log'

# The spreadsheets show where the errors were detected.
# Careful: all .xlsx files and the log file will be deleted on startup!
SPREADSHEETS_DIR = join(TMP_ROOT, 'sg2ps_tests', 'results')
LOGFILE = 'log.txt'

# If empty, all the RGF files in the RGF_FOLDER are considered. If not empty,
# only the listed projects will be tested. Only the project names should be 
# given, without the file extension.
TESTSET = [ ] # [ 'some_project', 'another_project' ]

# These projects will be ignored
IGNORE = [ ]

# The last character of the column name encodes the type. Here we map that 
# charater to a type.
TO_TYPE = { 's' : str,
            'i' : int,
            'd' : float } # NaN should be represented by the string NaN

# Thresholds in floating point comparison
REL_TOL = 1.0e-4
ABS_TOL = 1.0e-4
