// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <map>
#include <iostream>
#include <stdexcept>

#include "checkxycontent.h"
#include "exceptions.hpp"


using namespace std;

bool needxyfile () {

	string need_xy_file;

	cout << endl << endl;

	cout << "COORDINATE FILE" << endl << endl;

	do {

		cout << "Do You want to use own coordinates in XY data format....[Y]," 			<< endl;
		cout << "or use coordinate data in RGF file......................[N]........?  " << flush;

		cin >> need_xy_file;
		need_xy_file = capslock(need_xy_file);
		if (need_xy_file == "X") throw exit_requested();
	}

	while (!((need_xy_file == "Y") || (need_xy_file == "N")));

	if (need_xy_file == "Y") return true;
	else return false;
}

string inputxyfilename () {

	string xyfilename;

	cout << endl << "  - Enter coordinate file (*.xy) name without extension, or press 'X' to skip: " << endl;

	cin  >> xyfilename;

	return xyfilename;
}

bool xyEXISTENCEcheck (string xyname) {

	ifstream xyfile;

	string filename = xyname + ".xy";
	xyfile.open (filename.c_str());

	if (!(xyfile.is_open())) {

		cout << "  - ERROR, cannot open " << capslock(xyname)<< ".XY coordinate file." << endl;
		return false;
	}

	else {

		cout << "  - " << capslock(xyname)<< ".XY coordinate file opened successfully" << endl;
		return true;
	}
}

bool xyIDcheck (string xyname)

{
	ifstream xyfile;
	string ID;
	string temp;

	int i=0;
	bool errorlevel = false;

	map <string, int> record_to_check;

	string filename = xyname + ".xy";	xyfile.open (filename.c_str());

	getline (xyfile, temp);

	while (!(xyfile.eof())) {

		i++;

		getline (xyfile, ID, '\t');
		getline (xyfile, temp);

		pair<string, int> ID_and_counter(ID, i);
		pair<map<string, int>::iterator, bool> p = record_to_check.insert(ID_and_counter);

		if (!(p.second)) {

			cout << "    - ERROR: ID '" << ID << "' used in line " << i << " is already used at line '" << (*(p.first)).second << "' ." <<endl;
			errorlevel = true;
		}
	}

	xyfile.close();

		if (!(errorlevel)) {

			cout << "    - Correct data point ID's in all of " << i << " records." << endl;
			return true;
		}

		else return false;
}

bool xyTABcheck (string xyname) {

	ifstream xyfile;
	string ID;
	string temp;
	char  IDchar;
	size_t errorcounter = 0;

	size_t i = 0;
	int j = 1;
	int tabnumber;

	string filename = xyname + ".xy";

	xyfile.open (filename.c_str());

	if (!(xyfile.is_open())) {

		cout << "    - ERROR, cannot open input coordinate file." << endl;
		return false;
	}

	if (xyfile.eof()) {

		cout << "    - ERROR, missing header line in coordinate file." << endl;
		return false;
	}

	getline (xyfile, temp);

	while  (!(xyfile.eof())) {

		j++;
		i = 0;
		tabnumber = 0;

		getline (xyfile, ID);

			if (ID.length() == 0) {

				errorcounter++;

				if (errorcounter == 1) {

					cout << "    - ERROR: Missing TAB character(s) in following record(s):  " << j;
				}

				if (errorcounter > 1) cout << ", " << ID;
			}

			if ((ID.length() < 4)&& (ID.length() > 0)) {  // 3 VOLT

				errorcounter++;

				if (errorcounter == 1) {

					cout <<"    - ERROR: Missing TAB character(s) in following record(s):  " << j;
									}
				if (errorcounter > 1) cout << ", " << ID;
			}

			while  (i < ID.length()) {

				IDchar = ID.at(i);

				if (IDchar == '\t') tabnumber++;

				if ((i+1 == ID.length()) && (tabnumber < 3) && (ID.length() > 3)) {  // 2 - 2 VOLT

					errorcounter++;

					if (errorcounter == 1) {

						cout << "    - ERROR: Missing TAB character(s) in following record(s):  " << j;
					}

					if (errorcounter > 1) cout << ", " << j;
			}

			i++;
		}
	}

	xyfile.close();

	if ((errorcounter > 0) && (j > 0)) return false;

	if ((j == 1) && (ID.length() == 0)) return false;

	else {

		cout <<"    - Correct xy coordinate file structure in all of "<< j-1 << " records." << endl;

		return true;
	}
}


bool xyCOORDcheck (string xyname) {

	ifstream xyfile;

	string ID, coordX, coordY, temp, formation;

	int i = 0;
	int errorcounter = 0;

	string filename = xyname + ".xy";
	xyfile.open (filename.c_str());

	if (!(xyfile.is_open())) {

		cout << "    - ERROR, cannot open input coordinate file." << endl;
		return false;
	}

	getline (xyfile, temp);

	while (!(xyfile.eof())) {

		i++;

		getline (xyfile, ID, '\t');
		getline (xyfile, coordX, '\t');
		getline (xyfile, coordY, '\t');
		getline (xyfile, formation);

		if 	(((coordX != "0") && (coordX != "0.0") && (coordX != "0.0") && (atof(coordX.c_str()) == 0.0)) ||
			 ((coordY != "0") && (coordY != "0.0") && (coordY != "0.0") && (atof(coordY.c_str()) == 0.0))) {

			errorcounter++;

			if (errorcounter == 1) cout <<"    - ERROR: incorrect coordinate(s) in following record(s):  " << ID;

			if (errorcounter > 1) cout << ", " << ID;
		}
	}

	xyfile.close();

	if ((errorcounter > 0) && (i > 0)) cout << "." <<endl;

	if (errorcounter == 0) {

		cout <<"    - Correct X and Y coordinates in all of " << i << " records." << endl;

		return true;
	}

	else return false;
}

GDB insertxy (GDB inGDB, string xyfilename) {

	GDB outGDB = inGDB;

	string LOC, tempX, tempY, formation;

	size_t i = 0;

	LOC_X_Y_FRM buffer;
	vector < LOC_X_Y_FRM > XYDB;

	ifstream xyfile;

	string filename = xyfilename + ".xy";

	xyfile.open (filename.c_str());

	while (!(xyfile.eof())) {

		getline (xyfile, LOC, '\t');
		getline (xyfile, tempX, '\t');
		getline (xyfile, tempY, '\t');
		getline (xyfile, formation);

		buffer.LOC = LOC;
		buffer.X = atof(tempX.c_str());
		buffer.Y = atof(tempY.c_str());
		buffer.FORMATION = formation;

		XYDB.push_back(buffer);
	}

	outGDB.LOCX = 999.99;
	outGDB.LOCY = 999.99;

	while (i < XYDB.size())  {

		if (outGDB.LOC == XYDB.at(i).LOC) {

			outGDB.LOCX = XYDB.at(i).X;
			outGDB.LOCY = XYDB.at(i).Y;
			outGDB.FORMATION = XYDB.at(i).FORMATION;
		}

		i++;
	}

	return outGDB;
}

bool xyfile_correct (string projectname) {

	if (	xyEXISTENCEcheck 	(projectname) &&
			xyTABcheck 			(projectname) &&
			xyIDcheck 			(projectname) &&
			xyCOORDcheck  		(projectname)	) return true;

	else return false;

}

string check_xy_inputs (string inputfilename, bool batch) {

	cout << "CHECKING OF '" << capslock (inputfilename) << ".XY' FILE INTEGRITY" << endl;

		if (batch)

			if (xyfile_correct(inputfilename)) cout << "  - Input " << capslock(inputfilename) << ".XY file structure is correct." << endl;

			else {

				cout << "  - Input " << capslock(inputfilename) << ".XY file structure is incorrect, file will not be used." << endl;

				return "";
			}

		else {


			while (!(xyfile_correct(inputfilename))  || (inputfilename == "X") || (inputfilename == "x")) {

				cout << "  - Input " << capslock(inputfilename) << ".XY file structure is incorrect." << endl;

				inputfilename = inputxyfilename();

				if (capslock(inputfilename) == "X") return "";
			}
			cout << "  - Input " << capslock(inputfilename) << ".RGF file structure is correct." << endl;
		}

	return inputfilename;
}
