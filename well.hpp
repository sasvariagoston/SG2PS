// Copyright (C) 2012 - 2015 Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifndef WELL_HPP_
#define WELL_HPP_

bool is_WELL_PROCESSING_CALLS ();

vector <vector <WELL_INTERVAL> > RETURN_INTERVAL ();
vector <vector <WELL_FREQUENCY> > RETURN_FREQUENCY ();

vector <vector <WELL_INTERVAL> > MERGE_INTERVAL (const vector < vector <vector <vector <WELL_INTERVAL> > > >& IN);
vector <vector <WELL_FREQUENCY> > MERGE_FREQUENCY (const vector < vector <vector <vector <WELL_FREQUENCY> > > >& IN);

vector <GDB> filter_too_small_distances (const vector <GDB>& IN);

vector <WELL_FREQUENCY> FREQUENCY (const vector <GDB>& inGDB);

double record_derivate (const bool DIPDIR, const WELL_INTERVAL& ACT, const WELL_INTERVAL& NXT);

vector <WELL_INTERVAL> FIRST_DERIVATE (const vector <WELL_INTERVAL>& IN);

double calculate_interval_depth (const vector <GDB>& inGDB);//ok

vector <GDB> return_GDB_for_data_interval (const vector <GDB>& inGDB, const double MIN, const double MAX); //ok

double stdev_for_interval (const vector <GDB>& inGDB, const bool DIPDIR, const size_t RUN);//ok
WELL_INTERVAL interval_average (const vector <GDB>& inGDB, const size_t RUN);

vector <WELL_INTERVAL> WELL_AVERAGE_M (const vector <GDB>& p_GDB);
vector <WELL_INTERVAL> WELL_AVERAGE_D (const vector <GDB>& p_GDB);

void PROCESS_WELL_GROUPS (const vector <vector <GDB> >& inGDB_G);

#endif /* WELL_HPP_ */
