#!/usr/bin/env python
# Copyright (C) 2014, 2015 by Ali Baharev <ali.baharev@gmail.com>
# All rights reserved.
# BSD license.
# https://github.com/baharev/CSV_Test
from __future__ import print_function
from contextlib import closing
from cStringIO import StringIO
from itertools import izip, izip_longest
from math import isnan
from os import listdir, makedirs, remove
from os.path import basename, dirname, isdir, isfile, join, splitext
import sys
from xlsxwriter import Workbook

# A hackish way to import the configuration
sys.path.append(dirname(__file__))
from configuration import ETALON_DIR, TOCOMP_DIR, EXTENSION, SPREADSHEETS_DIR, \
                          LOGFILE, SEP, TO_TYPE, ABS_TOL, REL_TOL, TESTSET, \
                          IGNORE

#-------------------------------------------------------------------------------

_errors = { }

def main(extra_msg = ''):
    _errors.clear()
    setup_spreadsheets_dir()
    passed = [ fname for fname in files_to_check() if check(fname) ]
    show_summary(passed, extra_msg)

def setup_spreadsheets_dir():
    if not isdir(SPREADSHEETS_DIR):
        makedirs(SPREADSHEETS_DIR)
        return
    # clean up .xlsx files from the previous run and the log file, if any
    to_del = sorted(f for f in listdir(SPREADSHEETS_DIR) if f.endswith('.xlsx'))
    if isfile(join(SPREADSHEETS_DIR, LOGFILE)):
        to_del.append(LOGFILE)
    for f in to_del:
        remove(join(SPREADSHEETS_DIR,f))
    if to_del:
        print('Deleted',len(to_del),'files in', SPREADSHEETS_DIR)

def files_to_check():
    etalons = { f for f in listdir(ETALON_DIR) if f.endswith(EXTENSION) }
    tocomps = { f for f in listdir(TOCOMP_DIR) if f.endswith(EXTENSION) }
    testset = { f+EXTENSION for f in TESTSET }
    if testset:
        etalon_missing = testset - etalons
        for e in etalon_missing:
            log_error(e, 'no etalon found')
        etalons &= testset
        testfile_missing = testset - tocomps
        for t in testfile_missing:
            log_error(t, 'no test file found ')
        tocomps &= testset
        return sorted(etalons & tocomps)
    #
    ignore = { f+EXTENSION for f in IGNORE }
    etalons -= ignore
    tocomps -= ignore
    etalon_only = etalons - tocomps
    tocomp_only = tocomps - etalons
    for e in etalon_only:
        log_error(e, 'only etalon found but the test file is missing')
    for t in tocomp_only:
        log_error(t, 'the etalon is missing for this test file')
    return sorted(etalons & tocomps)

def check(filename):
    etalon = get_content(ETALON_DIR, filename, kind='etalon')
    if etalon is None:
        return
    tocomp = get_content(TOCOMP_DIR, filename, kind='test')
    if tocomp is None:
        return
    mismatch = compare_headers(etalon, tocomp)
    if mismatch:
        log_error(filename, 'mismatch in headers, excel sheet written')
        write_mismatch(filename, etalon, tocomp, mismatch)
        return
    etalon_len, tocomp_len = number_of_rows(etalon, tocomp)
    if etalon_len!=tocomp_len:
        msg = 'number of rows: {}!={}'.format(etalon_len, tocomp_len)
        log_error(filename, msg)
        return
    mismatch = compare_values(etalon, tocomp)
    if mismatch:
        log_error(filename, 'mismatch, excel sheet written')
        write_mismatch(filename, etalon, tocomp, mismatch)
        return
    return True

def log_error(filename, msg):
    assert filename not in _errors, (filename, _errors[filename])
    _errors[filename] = msg

def get_content(directory, filename, kind):
    header, lines = read_csv( join(directory,filename) )
    col_types, error_msg = get_col_types(header)
    if error_msg:
        log_error(filename, '{}, header: {}'.format(kind, error_msg))
        return
    error_msg = check_rowlength(lines, expected_len=len(col_types))
    if error_msg:
        log_error(filename, '{}, {}'.format(kind, error_msg))
        return
    table, type_errors = convert(col_types, lines)
    if type_errors:
        msg = '{}, type conversion errors, excel sheet written'.format(kind)
        log_error(filename, msg)
        xlsxname = get_filebase(filename) + '_'+kind+'_type_error.xlsx'
        write_cell_errors(xlsxname, header, lines, type_errors)
        return
    return header, table

def read_csv(filename):
    print('Trying to read file "{}"'.format(filename))
    with open(filename, 'r') as f:
        header = extract_first_line(f)
        lines = [ split(line) for line in f ]
    print('Read {} lines'.format( bool(header) + len(lines) ))
    return header, lines

def extract_first_line(f):
    header = next(f, None)
    return split(header) if header else [ ]

def split(line):
    return line.rstrip('\r\n').split(SEP)

def get_col_types(header):
    # Returns ([type converters], error message). Exactly one of them is None.
    # Only the first error is logged.
    if len(header)==0:
        return None, 'missing'
    col_types = [ TO_TYPE.get(col[-1:], None) for col in header ]
    for i, typ in enumerate(col_types):
        if typ is None:
            msg = 'unrecognized type in column {}: "{}"'.format(i+1, header[i])
            return None, msg
    assert len(col_types)==len(header)
    return col_types, None

def check_rowlength(lines, expected_len):
    # Returns error message on error, None otherwise.
    # The first row is the header, and error messages use base 1 indices
    indices = [i for i, row in enumerate(lines, 2) if len(row)!=expected_len]
    if indices:
        return 'row length error in rows (header is row 1): {}'.format(indices)
    return None

def convert(col_types, lines):
    # Returns the tuple of: lines converted to a 2D table with proper types, and
    # the cell indices where type conversion error occured.
    table, type_errors = [ ], [ ]
    for i, line in enumerate(lines,1):
        row = [ ]
        for j, col in enumerate(line):
            try:
                row.append( col_types[j](col) )
            except:
                row.append( None )
                type_errors.append((i,j))
        assert len(row)==len(col_types)
        table.append(row)
    return table if len(type_errors)==0 else [ ], type_errors 

def get_filebase(path):
    return splitext(basename(path))[0]

def write_cell_errors(xlsxname, header, lines, cells_to_mark):
    workbook  = Workbook(join(SPREADSHEETS_DIR, xlsxname))
    cell_fmt  = workbook.add_format()
    cell_fmt.set_bg_color('cyan')
    worksheet = workbook.add_worksheet()
    write_sheet(worksheet, cell_fmt, header, lines, cells_to_mark)
    workbook.close()

def write_mismatch(filename, etalon, tocomp, mismatch):
    workbook  = Workbook(join(SPREADSHEETS_DIR, get_filebase(filename)+'.xlsx'))
    cell_fmt  = workbook.add_format()
    cell_fmt.set_bg_color('cyan')
    worksheet = workbook.add_worksheet(name='test')
    write_sheet(worksheet, cell_fmt, *tocomp, cells_to_mark=mismatch)
    worksheet = workbook.add_worksheet(name='etalon')
    write_sheet(worksheet, cell_fmt, *etalon)
    workbook.close()

def write_sheet(worksheet, cell_fmt, header, lines, cells_to_mark=[]):
    formatter = { cell : cell_fmt for cell in cells_to_mark }
    for j, col_header in enumerate(header):
        worksheet.write(0, j, col_header, formatter.get((0,j), None))
    for i, line in enumerate(lines, 1):
        for j, item in enumerate(line):
            worksheet.write(i,j, replace_nan(item), formatter.get((i,j),None))

def replace_nan(item):
    return 'NaN' if isinstance(item, float) and isnan(item) else item

def compare_headers(etalon, tocomp):
    mismatch = [ ]
    e_head, _ = etalon
    t_head, _ = tocomp
    for i, (eh, th) in enumerate(izip_longest(e_head, t_head, fillvalue='')):
        if eh!=th:
            mismatch.append((0,i))
    return mismatch

def number_of_rows(etalon, tocomp):
    return len(etalon[1]), len(tocomp[1])

def compare_values(etalon, tocomp):
    mismatch = [ ]
    _, e_table = etalon
    _, t_table = tocomp
    for i, (e_row, t_row) in enumerate(izip(e_table, t_table), 1):
        for j, (e_item, t_item) in enumerate(izip(e_row, t_row)):
            if not equals(e_item, t_item):
                mismatch.append((i,j))
    return mismatch

def equals(e, t):
    return compare_floats(e, t) if isinstance(e, float) else e==t 

def compare_floats(e, t):
    e_nan, t_nan = isnan(e), isnan(t)
    if e_nan and t_nan:
        return True
    elif e_nan or t_nan:
        return False
    else:
        assert not e_nan and not t_nan
        diff = abs(e-t)
        return diff < ABS_TOL or diff < REL_TOL*abs(e)

def show_summary(passed, extra_msg):
    header = create_header(passed, extra_msg)
    print()
    print(header)
    if passed:
        print('Passed: {} files'.format(len(passed)))
    if _errors:
        log = create_error_log()
        print(log)
        write_errors(header, log)
        print('Tests FAILED! Check "{}"'.format(SPREADSHEETS_DIR))
    elif passed:
        print('Tests PASSED!')
    else:
        print('Something is strange: Are the directories empty?')
    if ETALON_DIR==TOCOMP_DIR:
        print('WARNING: The etalon directory has been compared to itself!')
    if TESTSET:
        print('WARNING: Only the given test set has been checked!')
    elif IGNORE:
        print('WARNING: There were ignored files!')

def create_header(passed, extra_msg):
    with closing(StringIO()) as out:
        out.write('Passed:\n')
        for p in passed:
            out.write(p)
            out.write('\n')
        out.write('=========================================================\n')
        if extra_msg:
            out.write( extra_msg + '\n' )
        out.write('Etalon directory: "{}"\n'.format(ETALON_DIR))
        out.write('Compared against: "{}"\n'.format(TOCOMP_DIR))
        return out.getvalue()  

def create_error_log():
    with closing(StringIO()) as out:
        out.write('There were errors:\n')
        for fname, msg in sorted(_errors.iteritems()):
            out.write('  {} {}\n'.format(fname,msg))
        return out.getvalue()      

def write_errors(header, log):
    logfile_name = join(SPREADSHEETS_DIR, 'log.txt') 
    with open(logfile_name, 'w') as f:
        f.write(header)
        f.write(log)

if __name__ == '__main__':
    main()
