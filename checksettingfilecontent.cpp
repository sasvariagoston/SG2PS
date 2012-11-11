// Copyright (C) 2012, Ágoston Sasvári
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#include "checksettingfilecontent.h"
#include "exceptions.hpp"

using namespace std;

void header () {

	cout << endl << endl << endl << endl << endl;

	cout << "+------------------------------------------------------------------+" << endl;
	cout << "|                                                                  |" << endl;
	cout << "|                             SG2PS                                |" << endl;
	cout << "|       Structural Geological data to PostScript converter         |" << endl;
	cout << "|                                                                  |" << endl;
	cout << "|                     Coded by Agoston Sasvari                     |" << endl;
	cout << "|                       All rights reserved                        |" << endl;
	cout << "|                                                                  |" << endl;
	cout << "|                       version: 0.2.0 beta                        |" << endl;
	cout << "+------------------------------------------------------------------+" << endl << endl << endl;
}

bool settingfilecorrect (string settingfilename) {


	ifstream settingfile;
	string filename = settingfilename + ".set";
	settingfile.open (filename.c_str());

	bool settingfilecorrect = true;
	string b, c;

	if (settingfile.eof()) settingfilecorrect = false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "R") || (c == "G")) && (b == "DATARULE:")))	settingfilecorrect = false;



	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "A") || (c == "H")) && (b == "PLOT:")))	settingfilecorrect=false;



	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "S") || (c == "W")) && (b == "PLOTTYPE:")))	settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}

	if (!(((c == "U")|| (c == "L")) && (b == "HEMISPHERE:"))) settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "B") || (c == "P") || (c == "A")) && (b == "TILTING:")))	settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "N") || (c == "Y")) && (b == "GROUP:")))	settingfilecorrect=false;


	if (settingfile.eof()) settingfilecorrect = false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "N") || (c == "A") || (c == "2") || (c == "3") || (c == "4") || (c == "5") || (c == "6") || (c == "7") || (c == "8") || (c == "9")) && (b == "CLUSTERNUMBER:")))	settingfilecorrect=false;


	if (settingfile.eof()) settingfilecorrect = false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "Y") || (c == "N")) && (b == "LABELING:"))) settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect = false;
	else {
		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "D") || (c == "A") || (c == "P") || (c == "M") || (c == "O") || (c == "S") || (c == "F") || (c == "N")) && (b == "INVERSION:"))) settingfilecorrect = false; //PTN!!!


	if (settingfile.eof()) settingfilecorrect = false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "Y") || (c == "N")) && (b == "VIRTUAL:"))) settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect = false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "Y") || (c == "N")) && (b == "IDEALMOVEMENT:"))) settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
	}
	if ((atof(c.c_str()) < 10.0) || (atof(c.c_str()) > 80.0) || (b != "STRESSANGLE:")) settingfilecorrect=false; //angle


	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "B")|| (c == "N")) && (b == "BINGHAM:")))	settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);

	}
	if (!((	(c == "0.0") || (c == "0.1") || (c == "0.2") ||
			(c == "0.3") || (c == "0.4") || (c == "0.5") ||
			(c == "0.6") || (c == "0.7") || (c == "0.8") ||
			(c == "0.9") || (c == "1.0")) && (b == "LINEWIDTH:"))) settingfilecorrect = false;


	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "S") || (c == "A")) && (b == "ROSETYPE:"))) settingfilecorrect = false;



	if (settingfile.eof()) settingfilecorrect=false;
	else  {

		getline (settingfile, b, '\t');
		getline (settingfile, c);
		b = capslock (b);
		c = capslock (c);
	}
	if (!(((c == "A") || (c == "B") || (c == "C")|| (c == "D")) && (b == "ROSEBINNING:"))) settingfilecorrect = false;

	settingfile.close();

	return settingfilecorrect;	
}

INPSET loadsettingsfromsettingfile (string settingfilename) {

	INPSET settings;
	string b, c;

	ifstream settingfile;
	string filename = settingfilename + ".set";
	settingfile.open (filename.c_str());

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.datarule = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.plot = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.plottype = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.hemisphere = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.tilting = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.group = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.clusternumber = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.labeling = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.inversion = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.virt_striae = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.idealmovement = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, settings.anglest);
	settings.angle = atof(settings.anglest.c_str());

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.fracture = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, settings.linewidthst);

	if (settings.linewidthst == "0.1") {settings.linewidth = 0.1;}
	if (settings.linewidthst == "0.2") {settings.linewidth = 0.2;}
	if (settings.linewidthst == "0.3") {settings.linewidth = 0.3;}
	if (settings.linewidthst == "0.4") {settings.linewidth = 0.4;}
	if (settings.linewidthst == "0.5") {settings.linewidth = 0.5;}
	if (settings.linewidthst == "0.6") {settings.linewidth = 0.6;}
	if (settings.linewidthst == "0.7") {settings.linewidth = 0.7;}
	if (settings.linewidthst == "0.8") {settings.linewidth = 0.8;}
	if (settings.linewidthst == "0.9") {settings.linewidth = 0.9;}
	if (settings.linewidthst == "1.0") {settings.linewidth = 1.0;}

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.rosetype = capslock (c);

	getline (settingfile, b, '\t');
	getline (settingfile, c);
	settings.rosebinning = capslock (c);

	settingfile.close();

	return settings;
}

void printsettingsonscreen (INPSET settings) {

	if (settings.datarule=="R")		cout<<"  - Data convention ............................: right-hand rule"					<< endl;
	if (settings.datarule=="G")		cout<<"  - Data convention.............................: german dipdir-dip"					<< endl;

	if (settings.plot=="H")			cout<<"  - Plot type ..................................: Hoeppener plot"					<< endl;
	if (settings.plot=="A")			cout<<"  - Plot type ..................................: Angelier plot"						<< endl;

	if (settings.plottype=="S")		cout<<"  - Net type....................................: Schmidt-net"						<< endl;
	if (settings.plottype=="W")		cout<<"  - Net type....................................: Wulff-net"							<< endl;

	if (settings.hemisphere=="U")	cout<<"  - Hemisphere..................................: upper hemisphere"					<< endl;
	if (settings.hemisphere=="L")	cout<<"  - Hemisphere..................................: lower hemisphere"					<< endl;

	if (settings.tilting=="B")		cout<<"  - Tilting by..................................: bedding"							<< endl;
	if (settings.tilting=="P")		cout<<"  - Tilting by..................................: paleo-north direction"				<< endl;
	if (settings.tilting=="A")		cout<<"  - Tilting by..................................: bedding and paleo-north"			<< endl;

	if (settings.group=="Y")		cout<<"  - Groupcode...................................: use"								<< endl;
	if (settings.group=="N")		cout<<"  - Groupcode...................................: do not use"						<< endl;

	if (settings.clusternumber=="N")cout<<"  - Clustering..................................: do not use"						<< endl;
	if (settings.clusternumber=="A")cout<<"  - Clustering..................................: iteration for ideal cluster number"<< endl;
	if (settings.clusternumber=="2")cout<<"  - Clustering..................................: 2 clusters"						<< endl;
	if (settings.clusternumber=="3")cout<<"  - Clustering..................................: 3 clusters"						<< endl;
	if (settings.clusternumber=="4")cout<<"  - Clustering..................................: 4 clusters"						<< endl;
	if (settings.clusternumber=="5")cout<<"  - Clustering..................................: 5 clusters"						<< endl;
	if (settings.clusternumber=="6")cout<<"  - Clustering..................................: 6 clusters"						<< endl;
	if (settings.clusternumber=="7")cout<<"  - Clustering..................................: 7 clusters"						<< endl;
	if (settings.clusternumber=="8")cout<<"  - Clustering..................................: 8 clusters"						<< endl;
	if (settings.clusternumber=="9")cout<<"  - Clustering..................................: 9 clusters"						<< endl;

	if (settings.labeling == "Y")		cout<<"  - Labeling....................................: yes"								<< endl;
	if (settings.labeling == "N")		cout<<"  - Labeling....................................: no labeling"						<< endl;

	if (settings.inversion == "D")		cout<<"  - Inversion...................................: using Sprang (1972) method"		<< endl;
	if (settings.inversion == "A")		cout<<"  - Inversion...................................: using Angelier's method"			<< endl;
	if (settings.inversion == "P")		cout<<"  - Inversion...................................: using Turner's (1953) method"		<< endl;
	if (settings.inversion == "S")		cout<<"  - Inversion...................................: using Shan et al's (2003) method"	<< endl;
	if (settings.inversion == "F")		cout<<"  - Inversion...................................: using Fry's (1999) method"			<< endl;
	if (settings.inversion == "O")		cout<<"  - Inversion...................................: using Mostafa's (2005) method"		<< endl;
	if (settings.inversion == "M")		cout<<"  - Inversion...................................: using using Michael (1984) method"	<< endl;
	if (settings.inversion == "N")		cout<<"  - Inversion...................................: none"								<< endl;

	if (settings.virt_striae == "Y")	cout<<"  - Virtual symmetrical striae set..............: use" 							<< endl;
	if (settings.virt_striae == "N")	cout<<"  - Virtual symmetrical striae set..............: do not use"								<< endl;

	if (settings.idealmovement == "Y")	cout<<"  - Ideal slickenside movement..................: display" 							<< endl;
	if (settings.idealmovement == "N")	cout<<"  - Ideal slickenside movement..................: not display"								<< endl;

	cout<<"  - Angle between s1 and fault plane (if needed): " << settings.angle << " degs" 	<<endl;

	if (settings.fracture == "B")		cout<<"  - Fracture statistics.........................: Bingham statistics"				<< endl;
	if (settings.fracture == "N")		cout<<"  - Fracture statistics.........................: no statistics"						<< endl;

	cout<<"  - Linewidth:..................................: "<<settings.linewidth<<" points"	<<endl;

	if (settings.rosetype=="S")		cout<<"  - Rose plot type..............................: symmetrical"						<< endl;
	if (settings.rosetype=="A")		cout<<"  - Rose plot type..............................: asymmetrical"						<< endl;
	if (settings.rosebinning=="A")	cout<<"  - Data bin size on rose plot..................: 2.5 deg"							<< endl;
	if (settings.rosebinning=="B")	cout<<"  - Data bin size on rose plot..................: 5 deg"								<< endl;
	if (settings.rosebinning=="C")	cout<<"  - Data bin size on rose plot..................: 10 deg"							<< endl;
	if (settings.rosebinning=="D")	cout<<"  - Data bin size on rose plot..................: 22.5 deg"							<< endl;

}


INPSET inputsettings_manually (string projectname) {

	string c;
	INPSET inputsettingsonscreen;

	cout << endl << "INPUT USER SETTINGS - to exit: press [X]" << endl;

	do {
		cout << "  - Data convention:" << endl;
		cout << "    - german with dip direction.............................[G],  " << endl;
		cout << "    - or right-hand rule with strike........................[r]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c=="G") || (c=="R")));
	inputsettingsonscreen.datarule = c;


	do {
		cout << "  - Plot type:" << endl;
		cout << "    - Angelier plot with planes.............................[A],  " << endl;
		cout << "    - or Hoeppener plot with poles..........................[h]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c=="A") || (c=="H")));
	inputsettingsonscreen.plot = c;



	do {
		cout << "  - Net type:" << endl;
		cout << "    - equal are Schmidt-net.................................[S],  " << endl;
		cout << "    - or equal angle Wulff-net..............................[w]?  " << flush;
		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c=="S") || (c=="W")));
	inputsettingsonscreen.plottype = c;


	do {
		cout << "  - Hemisphere:" << endl;
		cout << "    - upper hemisphere......................................[u],  " << endl;
		cout << "    - or lower hemisphere...................................[L]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c=="L") || (c=="U")));
	inputsettingsonscreen.hemisphere = c;


	do {
		cout << "  - Correction of measured data with:" << endl;
		cout << "    - bedding...............................................[b],  " << endl;
		cout << "    - paleo-north direction.................................[p],  " << endl;
		cout << "    - or bedding and paleo-north direction..................[A]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "B") || (c == "P") || (c == "A")));
	inputsettingsonscreen.tilting = c;


	do {
		cout << "  - Using group codes:" << endl;
		cout << "    - use and evaluate groups independently.................[Y],  " << endl;
		cout << "    - or do not use and evaluate groups together............[n]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "Y") || (c == "N")));
	inputsettingsonscreen.group = c;


	do {
		cout << "  - Clustering:" << endl;
		cout << "    - no clustering.........................................[N],  " << endl;
		cout << "    - automatic cluster number..............................[a],  " << endl;
		cout << "    - or 2..9 clusters...................................[2..9]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "N") || (c == "A") || (c == "2") || (c == "3") || (c == "4") || (c == "5") || (c == "6") || (c == "7") || (c == "8") || (c == "9")));
	inputsettingsonscreen.clusternumber = c;


	do {
		cout << "  - Labeling of stereonet:" << endl;
		cout << "    - labeling of measured data using data ID...............[y],  " << endl;
		cout << "    - or none...............................................[N]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "Y") || (c == "N")));
	inputsettingsonscreen.labeling = c;


	do {
		cout << "  - Inversion of slickenside data:" << endl;
		cout << "    - pseudo-inversion using Sprang's (1972) NDA method.....[D],  " << endl;
		cout << "    - pseudo-inversion using Turner's (1953) PTN method.....[p],  " << endl;
		cout << "    - regression using Fry's (1999) method..................[f],  " << endl;
		cout << "    - regression using Shan et al's (2003) method...........[s],  " << endl;
		cout << "    - regression using Michael's (1984) method..............[m],  " << endl;
		cout << "    - inversion using Angelier's (1984) method..............[a],  " << endl;
	    cout << "    - inversion Mostafa's (2005) method.....................[o],  " << endl;
		cout << "    - or none...............................................[n]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "D") || (c == "A") || (c == "P") || (c == "N") || (c == "M") || (c == "O") || (c == "S") || (c == "F") ));
	inputsettingsonscreen.inversion = c;


	do {
		cout << "  - Virtual symmetric striae set:" << endl;
		cout << "    - use virtual symmetric striae set......................[Y],  " << endl;
		cout << "    - or do not generate virtual symmetric set..............[n)],  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "Y") || (c == "N")));
	inputsettingsonscreen.virt_striae = c;


	do {
		cout << "  - Ideal movement display for slickensides:" << endl;
		cout << "    - display...............................................[Y],  " << endl;
		cout << "    - or do not display.....................................[n],  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "Y") || (c == "N")));
	inputsettingsonscreen.virt_striae = c;


	do {
		cout << "  - Angle between s1 and fault plane for pseudo-inversion: " << endl;
		cout << "    - 10 to 80 degrees.................................[10..80]?  " << flush;

		cin >> c;
		if (c == "X") throw exit_requested();
	}
	while ((atof(c.c_str()) < 10.0) || (atof(c.c_str()) > 80.0));
	inputsettingsonscreen.angle = atof(c.c_str());


	do {
		cout << "  - Fracture statistics:" << endl;
		cout << "    - Bingham statistics for weight point computation.......[B],  " << endl;
		cout << "    - or none...............................................[n]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "B") || (c == "N")));
	inputsettingsonscreen.fracture = c;


	do {
		cout << "  - Linewidth in points (1/72 inch):" << endl;
		cout << "    - 0.1 to 1.0.............................[1...9, 0 for 1.0]?  " << flush;

		cin >> c;
		if (c == "X") throw exit_requested();
	}
	while (!(
		  (c == "0") || (c == "1") || (c == "2") || (c == "3")|| (c == "4") ||
		  (c == "5") || (c == "6") || (c == "7") || (c == "8")|| (c == "9") || (c == "0")
		  ));
	inputsettingsonscreen.linewidth = 0.1;

	if (c == "2") inputsettingsonscreen.linewidth = 0.2;
	if (c == "3") inputsettingsonscreen.linewidth = 0.3;
	if (c == "4") inputsettingsonscreen.linewidth = 0.4;
	if (c == "5") inputsettingsonscreen.linewidth = 0.5;
	if (c == "6") inputsettingsonscreen.linewidth = 0.6;
	if (c == "7") inputsettingsonscreen.linewidth = 0.7;
	if (c == "8") inputsettingsonscreen.linewidth = 0.8;
	if (c == "9") inputsettingsonscreen.linewidth = 0.9;
	if (c == "0") inputsettingsonscreen.linewidth = 1.0;


	do {
		cout << "  - Rose plot for data sets:" << endl;
		cout << "    - symmetrical...........................................[S],  " << endl;
		cout << "    - or asymmetrical.......................................[a]?  " << flush;

		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "S") || (c == "A")));
	inputsettingsonscreen.rosetype = c;


	do {
		cout << "  - Bin size on rose plot:" << endl;
		cout << "    - 2.5 degrees...........................................[a],  " << endl;
		cout << "    - 5.0 degrees...........................................[b],  " << endl;
		cout << "    - 10.0 degrees..........................................[C],  " << endl;
		cout << "    - or 22.5 degrees.......................................[d]?  " << flush;
		cin >> c;
		c = capslock(c);
		if (c == "X") throw exit_requested();
	}
	while (!((c == "A") || (c == "B") || (c == "C") || (c == "D")));
	inputsettingsonscreen.rosebinning = c;

	cout << endl << endl;
	cout << "NEW SETTINGS FOR '" << capslock (projectname) << "' PROJECT"<< endl;

return inputsettingsonscreen;

}

bool outputsettingfile (INPSET outputsettingfile, string projectname) {

	ofstream settingfile;
	string filename = projectname + ".set";
	settingfile.open (filename.c_str());

	if ((settingfile.is_open())) {

		settingfile << "DATARULE:" << '\t' << flush;
		settingfile << outputsettingfile.datarule << endl;

		settingfile << "PLOT:" << '\t' << flush;
		settingfile << outputsettingfile.plot << endl;

		settingfile << "PLOTTYPE:" << '\t' << flush;
		settingfile << outputsettingfile.plottype << endl;

		settingfile << "HEMISPHERE:" << '\t' << flush;
		settingfile << outputsettingfile.hemisphere << endl;

		settingfile << "TILTING:" << '\t' << flush;
		settingfile << outputsettingfile.tilting << endl;

		settingfile << "GROUP:" << '\t' << flush;
		settingfile << outputsettingfile.group << endl;

		settingfile << "CLUSTERNUMBER:" << '\t' << flush;
		settingfile << outputsettingfile.clusternumber << endl;

		settingfile << "LABELING:" << '\t' << flush;
		settingfile << outputsettingfile.labeling << endl;

		settingfile << "INVERSION:" << '\t' << flush;
		settingfile << outputsettingfile.inversion << endl;

		settingfile << "VIRTUAL:" << '\t' << flush;
		settingfile << outputsettingfile.virt_striae << endl;

		settingfile << "IDEALMOVEMENT:" << '\t' << flush;
		settingfile << outputsettingfile.idealmovement << endl;

		settingfile << "STRESSANGLE:" << '\t' << flush;
		settingfile << outputsettingfile.angle << endl;

		settingfile << "BINGHAM:" << '\t' << flush;
		settingfile << outputsettingfile.fracture << endl;

		settingfile << "LINEWIDTH:" << '\t' << flush;
		settingfile << outputsettingfile.linewidth << endl;

		settingfile << "ROSETYPE:" << '\t' << flush;
		settingfile << outputsettingfile.rosetype << endl;

		settingfile << "ROSEBINNING:" << '\t' << flush;
		settingfile << outputsettingfile.rosebinning << endl;
		
		return true;
	}

	else return false;
}

INPSET input_hardcoded () {

	INPSET input_hardcoded;

	input_hardcoded.datarule		= "G";
	input_hardcoded.plot			= "A";
	input_hardcoded.plottype		= "S";
	input_hardcoded.hemisphere		= "L";
	input_hardcoded.tilting			= "A";
	input_hardcoded.group			= "Y";
	input_hardcoded.clusternumber	= "N";
	input_hardcoded.labeling		= "N";
	input_hardcoded.inversion		= "D";
	input_hardcoded.virt_striae		= "N";
	input_hardcoded.idealmovement	= "N";
	input_hardcoded.angle			= 30.0;
	input_hardcoded.fracture		= "B";
	input_hardcoded.linewidth		= 0.6;
	input_hardcoded.rosetype		= "S";
	input_hardcoded.rosebinning		= "C";
	input_hardcoded.filename		= "sg2ps.set";

	return input_hardcoded;
}



INPSET decide_setting_status (string projectname) {

	INPSET currect_settings;

	cout << endl << endl;
	cout << "CHECKING OF SETTINGS OF '" << capslock (projectname) << "' PROJECT"<< endl;

	if (settingfilecorrect (projectname)) {

		cout << "  - Using '" << capslock (projectname) << ".SET' setting file." << endl;
		currect_settings = loadsettingsfromsettingfile (projectname);
	}

	else {

		if (settingfilecorrect ("sg2ps")) {

			cout << "  - No project setting file, using 'SG2PS.SET' setting file." << endl;
			currect_settings = loadsettingsfromsettingfile ("sg2ps");
		}

		else {

			cout << "  - No any setting files, using hard coded settings." << endl;
			currect_settings = input_hardcoded ();
		}
	}

	return currect_settings;
}

INPSET manage_settings_batch (string projectname) {

	INPSET currect_settings = decide_setting_status (projectname);

	printsettingsonscreen (currect_settings);

	return currect_settings;
}

INPSET manage_settings_nobatch (string projectname) {

	INPSET currect_settings = decide_setting_status (projectname);

	string c;

	do {

		printsettingsonscreen (currect_settings);

		c = input_setting_decision ();

		if (c == "D") currect_settings = input_hardcoded ();
		if (c == "N") currect_settings = inputsettings_manually (projectname);

	} while (!(c == "S"));

	outputsettingfile (currect_settings, projectname);

	return currect_settings;
}

string input_setting_decision () {

	string c;

	while (!((c == "S") || (c == "D") || (c == "N"))) {

		cout << endl << endl;
		cout << "Do You want to use and save these settings.....[S]," 			<< endl;
		cout << "use default values.............................[D]," 			<< endl;
		cout << "input new ones.................................[N]," 			<< endl;
		cout << "or exit........................................[X]........?  " << flush;

		cin >> c;
		c = capslock (c);

		if (c == "X") throw exit_requested();
	}

	return c;
}
