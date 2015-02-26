#!/usr/bin/env python
# Copyright (C) 2014, 2015 by Ali Baharev <ali.baharev@gmail.com>
# All rights reserved.
# BSD license.
# https://github.com/baharev/CSV_Test
from __future__ import print_function
from subprocess import call
import sys
from os import access, listdir, makedirs, remove, X_OK
from os.path import dirname, isdir, isfile, join, normcase, normpath
from shutil import rmtree, copy

# A hackish way to import the configuration
sys.path.append(dirname(__file__))
from configuration import RGF_FOLDER, INPUT_EXT, ETALON_DIR, TOCOMP_DIR, \
                          EXTENSION, SG2PS_EXE, FLAG, TESTSET, IGNORE, LOG_EXT
from csv_test import main as csvtest_main

def main():
    
    # Check whether we need to invoke CSV test after generating the input
    NO_TEST = '--notest'
    argc = len(sys.argv)
    if argc > 2 or (argc==2 and sys.argv[1] != NO_TEST):
        print('Only the optional', NO_TEST, 'argument is supported')
        return
    RUN_CSVTEST = argc < 2
    
    if is_there_path_error():
        print('Exiting...')
        return
    print('All paths seem sane')
    
    # Delete the TOCOMP_DIR as it may contain files from a previous run
    if isdir(TOCOMP_DIR):
        rmtree(TOCOMP_DIR)
    print('Creating the test folder "{}"'.format(TOCOMP_DIR))
    makedirs(TOCOMP_DIR)
    
    # Copy the input files from the RGF folder to the test directory TOCOMP_DIR
    to_cp = sorted(f for f in listdir(RGF_FOLDER) if isfile(join(RGF_FOLDER,f)))
    for f in to_cp:
        copy(join(RGF_FOLDER, f), TOCOMP_DIR)
    print('Copied', len(to_cp), 'files ', end='')
    print('from "{}" to "{}"'.format(RGF_FOLDER, TOCOMP_DIR))
    
    projects = collect_project_names(to_cp)
    if projects is None:
        return # the error message has been logged already
    if not projects:
        print('Something is wrong, no projects found...')
        return
    
    # Run the sg2ps executable on the projects in TOCOMP_DIR 
    for f in projects:
        cmd = [SG2PS_EXE, FLAG, f]
        print('Executing command: {} {} {}'.format(*cmd))
        with open(join(TOCOMP_DIR, f+LOG_EXT), 'w') as logfile:
            ret = call(cmd, cwd=TOCOMP_DIR, stdout=logfile)
            # FIXME Simply log and ignore all errors? Otherwise test failures
            #       cannot be handled, which return non-zero return codes.
            if ret:
                print('Fatal error when calling {}, exiting'.format(SG2PS_EXE))
                return
    print('Test file generation finished')
    
    # Keep only the result and the log files
    allcontent = [ join(TOCOMP_DIR, f) for f in listdir(TOCOMP_DIR) ]
    files = [ f for f in allcontent if isfile(f) ]
    keep_csv = [ f for f in files if f.endswith(EXTENSION) ]
    keep = { f for f in files if f.endswith(LOG_EXT) } # keep the logs too
    keep.update(keep_csv)
    for f in allcontent:
        if f in keep:
            continue
        elif isfile(f):
            remove(f)
        else:
            rmtree(f)
    
    # Each project should generate exactly one CSV file
    if len(keep_csv) != len(projects):
        print('Something is strange:',len(projects),'projects but ',end='')
        print(len(keep_csv),'CSV files, exiting...')
        return

    if RUN_CSVTEST:
        print('Invoking CSV test now\n')
        csvtest_main('RGF files are in: "{}"'.format(RGF_FOLDER))
    else:
        print('Not running tests as requested; we are done!')


def collect_project_names(to_cp):
    # TODO Keep in sync with csv_test which also does something similar
    projects = { f[:-len(INPUT_EXT)] for f in to_cp if f.endswith(INPUT_EXT) }
    testset = set(TESTSET)
    if testset:
        missing = sorted(testset - projects)
        if missing:
            print('The following files in the test set are missing:')
            print(missing)
            return None
        return sorted(projects & testset)
    #
    projects.difference_update(IGNORE)
    return sorted(projects)


def is_there_path_error():
    # Consider replacing this long if - elif with a loop
    if not isfile(SG2PS_EXE) or not access(SG2PS_EXE, X_OK):
        print('SG2PS is not executable, check: "{}"'.format(SG2PS_EXE))
    elif not isdir(ETALON_DIR):
        print('ETALON_DIR: not a valid directory path "{}"'.format(ETALON_DIR))
    elif not isdir(RGF_FOLDER):
        print('RGF_FOLDER: not a valid directory path "{}"'.format(RGF_FOLDER))
    elif not isdir(TOCOMP_DIR):
        print('TOCOMP_DIR "{}" will be created'.format(TOCOMP_DIR))
        return False
    # TOCOMP_DIR exists and will be deleted: Check if that can cause data loss        
    elif samefile_or_dir(TOCOMP_DIR, ETALON_DIR):
        print('Etalon and test directory are the same "{}"'.format(ETALON_DIR))
    elif samefile_or_dir(TOCOMP_DIR, RGF_FOLDER):
        print('RGF and test directory are the same: "{}"'.format(RGF_FOLDER))
    elif samefile_or_dir(TOCOMP_DIR, dirname(TOCOMP_DIR)):
        print('Give a non-root TOCOMP_DIR directory: "{}"'.format(TOCOMP_DIR))
    else:
        return False
    return True


def samefile_or_dir(f1, f2):
    try:
        from os.path import samefile
    except ImportError:
        return normcase(normpath(f1)) == normcase(normpath(f2))
    return samefile(f1, f2)


if __name__=='__main__':
    main()
