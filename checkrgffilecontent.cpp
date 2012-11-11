// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include <map>
#include <stdexcept>

#include "checkrgffilecontent.h"
#include "exceptions.hpp"

using namespace std;

string inputfilename () {

	string filename;
	string projectname;
	ifstream rgffile;
	
	do {

		cout << endl << "Enter RGF file (*.rgf) name without extension........:  " << flush;

		cin  >> projectname;

		if ((projectname == "X") || (projectname == "x")) throw exit_requested();

		filename = projectname + ".rgf";

		rgffile.open (filename.c_str());
		
		if (rgffile.is_open())cout << "  - Input " << capslock(projectname)<< ".RGF file opened." << endl << endl;
		else cout << "    - ERROR, cannot open " << capslock(projectname)<< ".RGF geodatabase file. Please try again." << endl << endl;
	}
	
	while (!(rgffile.is_open()));

	rgffile.close();

	return projectname;
}

bool rgf_file_existence (string projectname) {

	ifstream rgffile;

	string filename = projectname + ".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	else {

		cout <<"    - Input file opened successfully." << endl;
		return true;
	}

}


bool tabcheck (string projectname) {

	ifstream rgffile;
	string RECORD;	char  RECORDchar;
	string temp;

	size_t errorcounter = 0;
	size_t i = 0;

	int j = 1;
	int tabnumber;
	
	string filename = projectname + ".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	if (rgffile.eof()) {

		cout << "    - ERROR, missing header line." << endl;
		return false;
	}

	getline (rgffile, temp);

	while  (!(rgffile.eof())) {

		j++;
		i = 0;		
		tabnumber = 0;

		getline (rgffile, RECORD);

			if (RECORD.length() == 0) {

				errorcounter++;

				if (errorcounter == 1) cout << "    - ERROR: Missing TAB character(s) in following rows:  " << j << flush;
				
				if (errorcounter > 1) cout << ", " << j << flush;
			}
			
			if ((RECORD.length() < 15) && (RECORD.length() > 0)) {

				errorcounter++;

				if (errorcounter == 1) cout <<"    - ERROR: Missing TAB character(s) in following record(s):  " << j << flush;

				if (errorcounter > 1)  cout << ", " << j << flush;
			}
			
			while  (i < RECORD.length()) {

				RECORDchar = RECORD.at(i);
				
				if (RECORDchar == '\t') tabnumber++;
				
				if ((i+1 == RECORD.length()) && (tabnumber < 14) && (RECORD.length() > 14)) {

					errorcounter++;
					
					if (errorcounter == 1) cout << "    - ERROR: Missing TAB character(s) in following record(s):  " << j << flush;

					if (errorcounter > 1)  cout << ", " << j << flush;
			}

			i++;
		}
	}

	rgffile.close();
	
	if ((errorcounter > 0) && (j > 0)) return false;

	if ((j == 1) && (RECORD.length() == 0)) return false;

	else {

		cout <<"    - Correct file structure in all of "<< j-1 << " records." << endl;
		return true;
	}
}

bool IDcheck (string projectname) {

	ifstream rgffile;
	string ID;
	string temp;

	int i=0;
	bool errorlevel = false;

	map <string, int> record_to_check;
	
	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		getline (rgffile, ID, '\t');
		getline (rgffile, temp);

		pair<string, int> ID_and_counter(ID, i);

		pair<map<string, int>::iterator, bool> p = record_to_check.insert(ID_and_counter);

		if (!(p.second)) {

			cout << "    - ERROR: ID '" << ID << "' used in line " << i << " is already used at line '" << (*(p.first)).second << "' ." <<endl;
			errorlevel = true;
		}
	}

	rgffile.close();

	if (!(errorlevel)) {

		cout << "    - Correct ID's in all of " << i << " records." << endl;
		return true;
	}

	else return false;
}

bool GCcheck (string projectname) {

	ifstream rgffile;
	string GC;
	string temp;

	int i = 0;
	int errorcounter = 0;
	string ID;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		getline (rgffile, ID, '\t');
		getline (rgffile, GC, '\t');
		getline (rgffile, temp);

		GC = capslock (GC);

		if  (!(
			(GC=="A") || (GC=="B") || (GC=="C") || (GC=="D") || (GC=="E") ||
			(GC=="F") || (GC=="G") || (GC=="H") || (GC=="I") || (GC=="X") ||
			(GC=="")
			 ))
		
		{
			errorcounter++;

			if (errorcounter == 1) cout <<"    - ERROR: incorrect group codes(s) in following record(s):  " << ID << flush;
			
			if (errorcounter > 1)  cout << ", " << ID << flush;
		}

		i++;
	}
	
	rgffile.close();

	if ((errorcounter > 0) && (i>0)) cout << "." <<endl;

	if (errorcounter==0) {

		cout <<"    - Correct group codes in all of " << i << " records." << endl;
		return true;
	}

	else return false;
}


bool LOCcheck (string projectname) {

	ifstream rgffile;
	string LOC;
	string temp;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	getline (rgffile, temp, '\t');
	getline (rgffile, temp, '\t');
	getline (rgffile, temp, '\t');
	getline (rgffile, LOC, '\t');
	getline (rgffile, temp);

	if  (LOC == "") {
		
		cout << "    - ERROR: no location ID in the first record. Please try again." << endl;
		return false;
	}
	
	else {

		cout << "    - Correct location data in the first record." << endl;
		return true;
	}
}

bool XYcheck (string projectname) {

	ifstream rgffile;

	string ID;
	string coordX;
	string coordY;
	string temp;

	int i=0;
	int errorcounter=0;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		getline (rgffile, ID, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, coordX, '\t');
		getline (rgffile, coordX, '\t');
		getline (rgffile, temp);

		if 	(((coordX != "") && (coordX != "0") && (coordX != "0.0") && (coordX != "0.0") && (atof(coordX.c_str()) <= 0.0)) ||
			 ((coordY != "") && (coordY != "0") && (coordY != "0.0") && (coordY != "0.0") && (atof(coordY.c_str()) <= 0.0))) {

			errorcounter++;

			if (errorcounter == 1) {

				cout <<"    - ERROR: incorrect coordinate(s) in the following record(s):  " << ID << flush;
			}

			if (errorcounter > 1) {

				cout << ", " << ID << flush;
			}
		}
	}

	rgffile.close();

	if ((errorcounter > 0) && (i > 0)) {

		cout << "." <<endl;
	}

	if (errorcounter == 0) {

		cout <<"    - Correct X and Y coordinates in all of " << i << " records." << endl;

		return true;
	}

	else return false;
}

bool COLORcheck (string projectname) {

	ifstream rgffile;

	string ID;
	string COLOR;
	string temp;
	int i = 0;
	int errorcounter = 0;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		getline (rgffile, ID, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, COLOR, '\t');
		getline (rgffile, temp);


		COLOR = capslock (COLOR);

		if  (!(
			(COLOR=="0") || (COLOR=="1") || (COLOR=="2") || (COLOR=="3") || (COLOR=="4") ||
			(COLOR=="5") || (COLOR=="6") || (COLOR=="7") || (COLOR=="8") || (COLOR=="9") ||
			(COLOR=="A") || (COLOR=="B") || (COLOR=="C") || (COLOR=="D") || (COLOR=="E") ||
			(COLOR=="F") || (COLOR=="G") || (COLOR=="H") || (COLOR=="I") || (COLOR=="J") ||
			(COLOR=="")
			))
		
		{
			errorcounter++;

			if (errorcounter == 1) {

				cout <<"    - ERROR: incorrect color code(s) in following record(s):  " << ID;
			}

			if (errorcounter > 1) cout << ", " << ID  << flush;

		}
	}
	
	rgffile.close();

	if ((errorcounter > 0) && (i>0)) {

		cout << "." <<endl;
	}

	if (errorcounter==0) {

		cout <<"    - Correct color codes in all of " << i << " records." << endl;
		return true;
	}

	else return false;
}


bool DATATYPEcheck (string projectname) {

	ifstream rgffile;

	string ID;
	string DATATYPE;
	string temp;
	string previous = "";

	int i = 0;
	int errorcounter = 0;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());

	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		getline (rgffile, ID, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, DATATYPE, '\t');
		getline (rgffile, temp);

		DATATYPE = capslock (DATATYPE);

		if ((i==1) && (DATATYPE == "")) {

			cout << "    - ERROR: incorrect or missing data type in first record." << ID << endl;

		}

		if  (!(		
			(DATATYPE == "BOUDAIN") ||			(DATATYPE == "CONTACT") ||			(DATATYPE == "FOLDAXIS") ||
			(DATATYPE == "FOLDPLANE") || 		(DATATYPE == "KINK") || 			(DATATYPE == "LINEATION") ||
			(DATATYPE == "LITHOLOGY") || 		(DATATYPE == "LITHOCLASE") ||		(DATATYPE == "SC") ||
			(DATATYPE == "BEDDING") ||			(DATATYPE == "S1") ||				(DATATYPE == "S2") ||
			(DATATYPE == "S3") ||				(DATATYPE == "S4") ||				(DATATYPE == "S5") ||
			(DATATYPE == "FRACTURE") ||			(DATATYPE == "STRIAE") ||			(DATATYPE == "CROSSBEDDING") ||
			(DATATYPE == "VEIN") ||				(DATATYPE == "FOLDSURFACE") ||		(DATATYPE == "USERPLANE4") ||
			(DATATYPE == "USERPLANE5") ||		(DATATYPE == "USERLINEATION1") ||	(DATATYPE == "USERLINEATION2") ||
			(DATATYPE == "USERLINEATION3") ||	(DATATYPE == "USERLINEATION4") ||	(DATATYPE == "USERLINEATION5") ||
			(DATATYPE == "")
			))
		
		{
			errorcounter++;

			if (errorcounter == 1) {

				cout <<"    - ERROR: incorrect data type(s) in following record(s):  " << ID << flush;
			}
			if (errorcounter > 1) {

				cout<<", " << ID << flush;
			}
		}

		previous = DATATYPE;
	}

	rgffile.close();

	if ((errorcounter > 0) && (i > 0)) {

		cout << "." <<endl;
	}
	
	if (errorcounter == 0) {

		cout <<"    - Correct data types in all of " << i << " records." << endl;
		return true;
	}

	else return false;
}


bool DIPDIRcheck (string projectname) {

	ifstream rgffile;

	string ID;
	string DIPDIR;	double numDIPDIR;
	string DATATYPE;
	bool LITHOLOGY = false;
	string temp;
	string previous;

	int i=0;
	int errorcounter=0;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());
	
	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		getline (rgffile, ID, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, DATATYPE, '\t');
		getline (rgffile, DIPDIR, '\t');
		getline (rgffile, temp);

		
		if ((DATATYPE == "LITHOLOGY") || ((DATATYPE == "") && (previous == "LITHOLOGY"))) LITHOLOGY = true;

		else LITHOLOGY = false;

		if ((DIPDIR != "") && (LITHOLOGY)) {

			errorcounter++;	

			if (errorcounter == 1) 	cout <<"    - ERROR: incorrect dip direction(s) in following record(s):  " << ID << flush;

			if (errorcounter > 1) 	cout << ", " << ID << flush;
		}


		if ((DIPDIR == "") && (!(LITHOLOGY))) {

			errorcounter++;	

			if (errorcounter == 1) 	cout <<"    - ERROR: incorrect dip direction(s) in following record(s):  " << ID << flush;

			if (errorcounter > 1) 	cout << ", " << ID << flush;
		}


		if (!((DIPDIR == "0") || (DIPDIR == "0.0") || (DIPDIR == "00")|| (DIPDIR == "000") || (DIPDIR == "0.00")  || (DIPDIR == ""))) {

			numDIPDIR = atof(DIPDIR.c_str());

			if (numDIPDIR == 0.0) {

				errorcounter++;

				if (errorcounter == 1) 	cout <<"    - ERROR: incorrect dip direction(s) in following record(s):  " << ID << flush;

				if (errorcounter > 1) 	cout << ", " << ID << flush;
			}

			if ((numDIPDIR < 0.0) || (numDIPDIR > 360.0)) {

				errorcounter++;	

				if (errorcounter == 1) 	cout <<"    - ERROR: incorrect dip direction(s) in following record(s):  " << ID << flush;

				if (errorcounter > 1)  	cout << ", " << ID << flush;

			}
		}

		previous = DATATYPE;
	}

	rgffile.close();
	
	if ((errorcounter > 0) && (i > 0)) 	cout << "." <<endl;

	if (errorcounter == 0) {

		cout <<"    - Correct dip directions in all of " << i << " records." << endl;
		return true;
	}	

	else return false;
}

bool DIPcheck (string projectname) {

	ifstream rgffile;

	string ID;
	string DIP;		double numDIP;
	string DATATYPE;
	string temp;
	string previous;

	bool LITHOLOGY = false;

	int i=0;
	int errorcounter=0;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());
	
	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		getline (rgffile, ID, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, DATATYPE, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, DIP, '\t');
		getline (rgffile, temp);

		LITHOLOGY = false;

		if ((DATATYPE == "LITHOLOGY") || ((DATATYPE == "") && (previous == "LITHOLOGY"))) LITHOLOGY = true;

		if ((DIP != "") && (LITHOLOGY))	{

			errorcounter++;	

				if (errorcounter == 1) 	cout <<"    - ERROR: incorrect dip(s) in following record(s):  " << ID << flush;

				if (errorcounter > 1) 	cout << ", " << ID;
		}


		if ((DIP == "") && (!(LITHOLOGY))) {

			errorcounter++;	

				if (errorcounter == 1) {

					cout <<"    - bERROR: incorrect dip(s) in following record(s):  " << ID << flush;
				}	

				if (errorcounter > 1) {

					cout<<", "<<ID;
				}
		}

		if (!((DIP == "0") || (DIP == "0.0")|| (DIP == "000") || (DIP == "00") || (DIP == "0.00") || (DIP == "") )) {

			numDIP = atof(DIP.c_str());

			if (numDIP == 0.0) {

				errorcounter++;
				
				if (errorcounter == 1) {

					cout << "    - ERROR: incorrect dip(s) in following record(s):  " << ID << flush;
				}

				if (errorcounter > 1) {

					cout << ", " << ID;
				}
			}

			if ((numDIP < 0.0) || (numDIP > 90.0)) {

				errorcounter++;

				if (errorcounter == 1) {

					cout << "    - ERROR: incorrect dip(s) in following record(s):  " << ID << flush;
				}

				if (errorcounter > 1) {

					cout << ", " << ID;
				}
			}
		}

		previous = DATATYPE;
	}

	rgffile.close();
	
	if ((errorcounter > 0) && (i>0)) {

		cout << "." <<endl;
	}

	if (errorcounter==0) {

		cout << "    - Correct dips in all of " << i << " records." << endl;
		return true;
	}

	else return false;
}

bool STRIAE_MISFIT_check (string dipdir, string dip, string ldir, string ldip) {

	DIPDIR_DIP plane, lineation;
	VCTR PN, SN;

	double misfit = 0.0;

	if (ldip != "") {

		plane.DIPDIR = atof(dipdir.c_str());
		plane.DIP = atof(dip.c_str());
		lineation.DIPDIR = atof(ldir.c_str());
		lineation.DIP = atof(ldip.c_str());

		PN = DXDYDZ_from_dipdir_dip (plane);
		SN = DXDYDZ_from_dipdir_dip (lineation);

		misfit = ASIN (dotproduct(PN, SN, false));
	}

	if (misfit > 90.0) return false;

	return true;
}

bool STRIAE_SC_check (string projectname) {

	ifstream rgffile;

	string ID;
	string DATATYPE;	
	string previous;
	string DIPDIR;
	string DIP;
	string LDIR;
	string LDIP;
	string SENSE;

	bool SCcorrect = false;
	bool LINEATIONcorrect = false;
	bool PITCHcorrect = false;
	bool OTHERcorrect = false;
	bool BEDDINGcorrect = false;

	bool STRIAE = false;
	bool SC = false;
	bool BEDDING = false;

	string temp;
	int i = 0;
	int errorcounter = 0;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());
	
	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		SCcorrect = false;
		LINEATIONcorrect = false;
		PITCHcorrect = false;
		OTHERcorrect = false;
		BEDDINGcorrect = false;

		STRIAE = false;
		SC = false;
		BEDDING = false;


		getline (rgffile, ID, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, DATATYPE, '\t');
		getline (rgffile, DIPDIR, '\t');
		getline (rgffile, DIP, '\t');
		getline (rgffile, LDIR, '\t');
		getline (rgffile, LDIP, '\t');
		getline (rgffile, SENSE, '\t');
		getline (rgffile, temp);

		DATATYPE = capslock (DATATYPE);
		SENSE = capslock (SENSE);

		if ((DATATYPE=="STRIAE") || ((DATATYPE=="") && (previous == "STRIAE"))) STRIAE = true;

		else if ((DATATYPE=="BEDDING") || ((DATATYPE=="") && (previous == "BEDDING"))) BEDDING = true;

		else if ((DATATYPE=="SC") || ((DATATYPE=="") && (previous == "SC"))) 		SC = true;

		else {};



		if (
				((!SC) && (!STRIAE ) && (!BEDDING)) &&
				(LDIP == "") &&
				(LDIR == "") &&
				(SENSE == "")
		) {
			OTHERcorrect = true;
		}

		else if (
				(SC) &&
				((LDIR == "0") || (LDIR == "0.0")|| (LDIR == "000")|| (LDIR == "00") || (LDIR == "0.00") || ((atof(LDIR.c_str()) > 0.0) && (atof(LDIR.c_str()) <= 360.0))) &&
				((LDIP == "0") || (LDIP == "0.0")|| (LDIP == "000")|| (LDIP == "00") || (LDIP == "0.00") || ((atof(LDIP.c_str()) > 0.0) && (atof(LDIP.c_str()) <= 90.0))) &&
				(SENSE == "")
		) {
			SCcorrect = true;
		}

		else if (
				(STRIAE) &&
				((LDIR == "0") || (LDIR == "0.0")|| (LDIR == "00")|| (LDIR == "000") || (LDIR == "0.00") || ((atof(LDIR.c_str()) > 0.0) && (atof(LDIR.c_str()) <= 360.0))) &&
				((LDIP == "0") || (LDIP == "0.0")|| (LDIP == "00")|| (LDIP == "000") || (LDIP == "0.00") || ((atof(LDIP.c_str()) > 0.0) && (atof(LDIP.c_str()) <= 90.0))) &&
				((SENSE == "+") 		|| (SENSE == "THRUST")	|| (SENSE == "UP") 		|| (SENSE == "INVERSE")	|| (SENSE == "U") 		|| (SENSE == "I") ||
				 (SENSE == "-")   		|| (SENSE == "NORMAL")	|| (SENSE == "FAULT") 	|| (SENSE == "DOWN")	|| (SENSE == "DOWNWARD")|| (SENSE == "N")  ||
				 (SENSE == "DEXTRAL")  	|| 	(SENSE == "DX")  	|| (SENSE == "D")  		||
				 (SENSE == "SINISTRAL") || 	(SENSE == "SN")		|| 	(SENSE == "S")		||
				 (SENSE == "X")	||	(SENSE == "NONE"))
				 &&
				(STRIAE_MISFIT_check (DIPDIR, DIP, LDIR, LDIP) )
		) {
			LINEATIONcorrect = true;
		}

		else if (
				(BEDDING) &&
				((DIPDIR == "0") || (DIPDIR == "0.0")|| (DIPDIR == "00")|| (DIPDIR == "000") || (DIPDIR == "0.00") || ((atof(DIPDIR.c_str()) > 0.0) && (atof(DIPDIR.c_str()) <= 360.0))) &&
				((DIP == "0") || (DIP == "0.0")|| (DIP == "00")|| (DIP == "000") || (DIP == "0.00") || ((atof(DIP.c_str()) > 0.0) && (atof(DIP.c_str()) <= 90.0))) &&
				((SENSE == "O") 		|| (SENSE == "N")	||  (SENSE == "") ||
				 (SENSE == "OVERTURNED")|| (SENSE == "NORMAL")
						)

		) {
			BEDDINGcorrect = true;
		}

		else if (
				(STRIAE) &&
				((LDIR == "0") || (LDIR == "0.0")|| (LDIR == "00")|| (LDIR == "000") || (LDIR == "0.00") || ((atof(LDIR.c_str()) > 0.0) && (atof(LDIR.c_str()) <= 90.0))) &&
				((capslock(LDIP) == "N") || (capslock(LDIP) == "NNE") || (capslock(LDIP) == "NE") || (capslock(LDIP) == "ENE") ||
				 (capslock(LDIP) == "E") || (capslock(LDIP) == "ESE") || (capslock(LDIP) == "SE") || (capslock(LDIP) == "SSE") ||
				 (capslock(LDIP) == "S") || (capslock(LDIP) == "SSW") || (capslock(LDIP) == "SW") || (capslock(LDIP) == "WSW") ||
				 (capslock(LDIP) == "W") || (capslock(LDIP) == "WNW") || (capslock(LDIP) == "NW") || (capslock(LDIP) == "NNW")) &&
				((SENSE == "+") 			|| (SENSE == "THRUST")	|| (SENSE == "UP") 		|| (SENSE == "INVERSE") 	|| (SENSE == "U") 			|| (SENSE == "I") ||
				 (SENSE == "-")   			|| (SENSE == "NORMAL")	|| (SENSE == "FAULT") 	|| (SENSE == "DOWN")	|| (SENSE == "DOWNWARD")	|| (SENSE == "N") ||
				 (SENSE == "DEXTRAL")  		|| 	(SENSE == "DX")  	|| (SENSE == "D")  		||
				 (SENSE == "SINISTRAL")  	|| 	(SENSE == "SN")		|| 	(SENSE == "S")		||
				 (SENSE == "X")			||	(SENSE == "NONE"))
		) {
			PITCHcorrect = true;
		}

		else {}

		if((!(SCcorrect)) && (!(LINEATIONcorrect)) && (!(PITCHcorrect)) && (!(BEDDINGcorrect)) && (!(OTHERcorrect))) {

			errorcounter++;
			if (errorcounter == 1) cout <<"  ERROR: incorrect striae/SC data in following record(s): " << ID;
			if (errorcounter > 1) cout << ", " << ID;
			
		}

		if ((DATATYPE=="STRIAE") || ((DATATYPE=="") && (previous == "STRIAE"))) previous = "STRIAE";
		else if  ((DATATYPE=="SC") || ((DATATYPE=="") && (previous == "SC"))) 	previous = "SC";
		else if  ((DATATYPE=="BEDDING") || ((DATATYPE=="") && (previous == "BEDDING"))) 	previous = "BEDDING";

		else previous = "";
	}
	
	rgffile.close();
	
	if ((errorcounter > 0) && (i > 0)) cout << "." <<endl;
	if (errorcounter == 0) {

		cout <<"    - Correct striae/SC data in all of " << i << " records." << endl;
		return true;
	}

	else return false;
}

bool PALEONcheck (string projectname) {

	ifstream rgffile;

	string ID;
	string PALEON;
	double numPALEON;
	string temp;
	int i=0;
	int errorcounter=0;

	string filename = projectname+".rgf";
	rgffile.open (filename.c_str());
	
	if (!(rgffile.is_open())) {

		cout << "    - ERROR, cannot open input datafile." << endl;
		return false;
	}

	getline (rgffile, temp);

	while (!(rgffile.eof())) {

		i++;

		getline (rgffile, ID, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, temp, '\t');
		getline (rgffile, PALEON, '\t');
		getline (rgffile, temp);


		if (!((PALEON == "0") || (PALEON == "0.0") || (PALEON == "0.00")  || (PALEON == ""))) {

			numPALEON=atof(PALEON.c_str());

			if (numPALEON == 0.0) {

				errorcounter++;
				
				if (errorcounter == 1) 	cout << "    - ERROR: incorrect paleo North data in following record(s):  " << ID << flush;

				if (errorcounter > 1) 	cout << ", " << ID;

			}

			if ((numPALEON < 0.0) || (numPALEON > 360.0)) {

				errorcounter++;	

				if (errorcounter == 1) 	cout << "    - ERROR: incorrect paleo North data in following record(s):  " << ID << flush;

				if (errorcounter > 1) 	cout << ", " << ID;

			}
		}
	}

	rgffile.close();
	
	if ((errorcounter > 0) && (i > 0)) 	cout << "." <<endl;

	if (errorcounter==0) {

		cout <<"    - Correct paleo North data in all of " << i << " records." << endl;
		return true;
	}

	else return false;

}

vector <string> check_rgf_inputs (vector <string> inputfilename_vector, bool batch) {

	size_t j = 1;

	cout << "CHECKING INPUT FILE(S) INTEGRITY" << endl;

	do {

		if (batch) {

			if (rgffile_correct(inputfilename_vector.at(j))) cout << "  - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is correct." << endl;

			else {

				inputfilename_vector.erase(inputfilename_vector.begin() + j);
			}

			if (inputfilename_vector.size() < 2) throw runtime_error ("No file to process");
		}

		else {

			while (!(rgffile_correct (inputfilename_vector.at(j)))) {

				cout << "  - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is incorrect; please enter file name again, or press 'X' to exit." << endl;
				inputfilename_vector.at(j) = inputfilename();
			}

			cout << "  - Input " << capslock(inputfilename_vector.at(j)) << ".RGF file structure is correct." << endl;
		}

		j++;

	} while (j < inputfilename_vector.size());

	return inputfilename_vector;
}

vector <string> create_inputfilename_vector (int argc, char *argv[]) {

	int j = 0;
	vector <string> out;

	do  {

		out.push_back(argv[j]);
		j++;

	} while (j < argc);

	return out;
}

bool rgffile_correct (string projectname) {

	if  (   rgf_file_existence (projectname) &&
			tabcheck (projectname) &&
			IDcheck (projectname) &&
			GCcheck (projectname) &&
			COLORcheck (projectname) &&
			LOCcheck (projectname) &&
			DATATYPEcheck (projectname) &&
			DIPDIRcheck (projectname) &&
			DIPcheck (projectname) &&
			STRIAE_SC_check	(projectname) &&
			PALEONcheck (projectname)		)	return true;

	else return false;
}


