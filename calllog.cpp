// Call logger
/******************************************
 * This software is released into the
 * Public Domain, or is licensed for free,
 * unrestricted use, modification, and
 * redistribution.
 * This program is not secure. Adding
 * password protection or otherwise
 * securing it is left as an exercise to
 * the reader.
 * This software comes with no warrantee of
 * any kind.
 ******************************************/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include "parrot.h"
/* void parrot(const char* filename) just reads a text file to stdout.
   Anyone should be able to reconstruct it, probably better than mine.
   This file is not included in this software. */
#include "calllog.h"
using std::cout;

int main() {
	callRecord c;
	char l;
	bool ready = false;
	
	bool goodEnt = false;
	bool goodNum = false;
	bool goodDate = false;
	bool goodTime = false;
	bool goodSub = false;
	bool goodRes = false; // Testing for good input is also left as an exercise.
	std::string dataString  = "";
	int contentLength = 0;
	int lenent = 0; int lenphon = 0; int lentime = 0; int lensub = 0; int lenres = 0; int lenpre = 0; int lennote  = 0;
	int debug = 6;

	parrot("calllog1.htf");
	cout << "<p class=\"debug\">Build (after first clean exit) #038</p>\n\n";
	std::fstream f("calllog.xml", std::ios::in | std::ios::out);
	f.seekg(0,std::ios::end);
	if (f.bad() || (f.tellg() < 0)) {
 		std::cout << "\n<p>Error opening file!</p>\n";
		parrot("foot.htf");
		return -1;
	}
	if (debug > 7) cout << "<p>EOF: " << f.tellg() << "</p>\n";
	f.seekg(-5,std::ios::end);
	while (ready != true) {
		if (debug > 8) cout << "Position: " << f.tellg() << "<br />\n";
		f.read(reinterpret_cast<char*>(&l),sizeof(char));
		if (f.bad()) {
			std::cout << "\nUgh while reading!\n";
			parrot("foot.htf");
			exit(-1);
		}
		if (debug > 8) cout << "Letter: " << l << "\n";
		if (l == '<') {
			if (debug > 6) std::cout << "Found &lt;";
			f.read(reinterpret_cast<char*>(&l),sizeof(char));
			if (l == '/') {
				if (debug > 6) std::cout << "/";
				f.read(reinterpret_cast<char*>(&l),sizeof(char));
				if (l == 'l') {
					if (debug > 6) std::cout << "l";
					f.read(reinterpret_cast<char*>(&l),sizeof(char));
					if (l == 'o') {
						if (debug > 6) std::cout << "o! Assuming in right place!\n";
						f.seekg(-2,std::ios::cur);
						f.read(reinterpret_cast<char*>(&l),sizeof(char));
						f.seekg(-1,std::ios::cur);
						if (debug > 8) std::cout << "Letter at current read position: " << l << "\n";
						ready = true;
					}
					f.seekg(-1,std::ios::cur);
				}
				f.seekg(-1,std::ios::cur);
			}
			f.seekg(-1,std::ios::cur);
		} else {
			if (debug > 8) cout << "Position: " << f.tellg() << "\n";
			f.seekg(-2,std::ios::cur);
		}
		if (f.tellg() <= 1) {
			std::cerr << "String not found in XML file!\n";
			cout << "<p>Closing tag not found for element &lt;log&gt;! Exiting!</p>";
			parrot("foot.htf");
			exit(-1);
		}
	}
//	std::cout << "Ready!";
	f.seekp(f.tellg());
	f.read(reinterpret_cast<char*>(&l),sizeof(char));
	if (debug > 5) cout << "Write position: " << f.tellp() << "\n";
	ready = false;
	// Processing data
	if (getenv("CONTENT_LENGTH")) {
		contentLength = atoi(getenv("CONTENT_LENGTH"));
		cout << "<p>Content Length Established: " << contentLength << "/2048</p>\n";	
		char query[2048] = "";
		dTxTx tmp;
		int shortBy = 0;
		std::cin.read(query, contentLength);
		cout << "<p class=\"debug\">Query: " << query << "</p>\n";
		dataString = query;
		int sLoc = dataString.find("entity=") + 7;
		int sEnd = dataString.find("&num");
		lenent = sEnd - sLoc;
		c.entity = dataString.substr(sLoc, sEnd - sLoc);
		if (debug > 8) {
			cout << "Entity: " << c.entity << "\n";
		}
		sLoc = sEnd + 5;
		sEnd = dataString.find("&time");
		lenphon = sEnd - sLoc;
		c.number = dataString.substr(sLoc, sEnd - sLoc);
		if (debug > 7) {
			cout << "Number: " << c.number << "\n";
		}
		time_t today;
		struct tm *now;
		char year[5];
		char mo[3];
		char day[3];
		time(&today);
		now = localtime(&today);
		strftime(year,5,"%Y",now);
		strftime(mo,3,"%m",now);
		strftime(day,3,"%d",now);
		c.year = year;
		c.mo = mo;
		c.day = day;
		if (debug > 7) {
			cout << "Date: " << c.year << "/" << c.mo << "/" << c.day << "\n";
		}
		sLoc = sEnd + 6;
		sEnd = dataString.find("&subject");
		lentime = sEnd - sLoc;
		c.time = dataString.substr(sLoc,sEnd - sLoc);
		if (debug > 7) {
			cout << "Time: " << c.time << "\n";
		}
		sLoc = sEnd + 9;
		sEnd = dataString.find("&result");
		lensub = sEnd - sLoc;
		c.subject = dataString.substr(sLoc, sEnd - sLoc);
		if (debug > 7) {
			cout << "Subject: " << c.subject << "\n";
		}
		sLoc = sEnd + 8;
		sEnd = dataString.find("&prenote");
		lenres = sEnd - sLoc;
		c.result = dataString.substr(sLoc, sEnd - sLoc);
		if (debug > 7) {
			cout << "Result: " << c.result << "\n";
		}
		sLoc = sEnd + 9;
		sEnd = dataString.find("&note");
		lenpre = sEnd - sLoc;
		c.prenote = dataString.substr(sLoc,sEnd - sLoc);
		if (debug > 7) {
			cout << "Prenote: " << c.prenote << "\n";
		}
		sLoc = sEnd + 6;
		sEnd = dataString.find("&end");
		lennote = sEnd - sLoc;
		c.note = dataString.substr(sLoc,sEnd - sLoc);
		if (debug > 7) {
			cout << "Note: " << c.note << "\n";
		}
// ==================== Now to unURL them all =================
		// Entity
		for (int i=0;i<lenent;i++) {
			tmp.sho[i] = c.entity[i];
		}
		tmp.sho[lenent] = '\0';
		shortBy = unURL(tmp,lenent);
		tmp.sho[lenent - shortBy] = '\0';
		lenent -= shortBy;
		tmp.sho[lenent] = '\0';
		c.entity = tmp.sho;
		// Phone
		for (int i=0;i<lenphon;i++) {
			tmp.sho[i] = c.number[i];
		}
		tmp.sho[lenphon] = '\0';
		shortBy = unURL(tmp,lenphon);
		lenphon -= shortBy;
		tmp.sho[lenphon] = '\0';
		c.number = tmp.sho;
		// Time
		for (int i=0;i<lentime;i++) {
			tmp.sho[i] = c.time[i];
		}
		tmp.sho[lentime] = '\0';
		shortBy = unURL(tmp,lentime);
		lentime -= shortBy;
		tmp.sho[lentime] = '\0';
		c.time = tmp.sho;
		// Subject
		for (int i=0;i<lensub;i++) {
			tmp.sho[i] = c.subject[i];
		}
		tmp.sho[lensub] = '\0';
		shortBy = unURL(tmp,lensub);
		lensub -= shortBy;
		tmp.sho[lensub] = '\0';
		c.subject = tmp.sho;
		// Result
		for (int i=0;i<lenres;i++) {
			tmp.sho[i] = c.result[i];
		}
		tmp.sho[lenres] = '\0';
		shortBy = unURL(tmp,lenres);
		lenres -= shortBy;
		tmp.sho[lenres] = '\0';
		c.result = tmp.sho;
		// Prenote
		for (int i=0;i<lenpre;i++) {
			tmp.sho[i] = c.prenote[i];
		}
		tmp.sho[lenpre] = '\0';
		shortBy = unURL(tmp,lenpre);
		lenpre -= shortBy;
		tmp.sho[lenpre] = '\0';
		c.prenote = tmp.sho;
		// Note
		for (int i=0;i<lennote;i++) {
			tmp.sho[i] = c.note[i];
		}
		tmp.sho[lennote] = '\0';
		shortBy = unURL(tmp,lennote);
		lennote -= shortBy;
		tmp.sho[lennote] = '\0';
		c.note = tmp.sho;
// ============== Now simulate (and write) XML output =====================
		cout << "<p>Simulated XML output:</p><pre>\n\t&lt;call id=\"" << today << "\"&gt;";
		f << "\t<call id=\"" << today << "\">";
		for (int i=0;i<lenent;i++) {
			tmp.sho[i] = c.entity[i];
		}
		tmp.sho[lenent] = '\0';
		cout << "\n\t\t&lt;entity&gt;";
		f << "\n\t\t<entity>";
		xmlSay(tmp,lenent);
		xmlWrite(tmp,lenent,f);
		cout << "&lt;/entity&gt;";
		cout << "</entity>";
		for (int i=0;i<lenphon;i++) {
			tmp.sho[i] = c.number[i];
		}
		tmp.sho[lenphon] = '\0';
		cout << "\n\t\t&lt;number&gt;";
		f << "\n\t\t<number>";
		xmlSay(tmp,lenphon);
		xmlWrite(tmp,lenphon,f);
		cout << "&lt;/number&gt;";
		cout << "</number>";
		cout << "\n\t\t&lt;date&gt;\n\t\t\t&lt;year&gt;" << c.year << "&lt;/year&gt;\n\t\t\t&lt;month&gt;" << c.mo << "&lt;/month&gt;\n\t\t\t&lt;day&gt;" << c.day << "&lt;/day&gt;\n\t\t&lt;/date&gt;";
		f << "\n\t\t<date>\n\t\t\t<year>" << c.year << "</year>\n\t\t\t<month>" << c.mo << "</month>\n\t\t\t<day>" << c.day << "</day>\n\t\t</date>";

		for (int i=0;i<lentime;i++) {
			tmp.sho[i] = c.time[i];
		}
		tmp.sho[lentime] = '\0';
		cout << "\n\t\t&lt;time&gt;";
		f << "\n\t\t<time>";
		xmlSay(tmp,lentime);
		xmlWrite(tmp,lentime,f);
		cout << "&lt;/time&gt;";
		f << "</time>";
		for (int i=0;i<lensub;i++) {
			tmp.sho[i] = c.subject[i];
		}
		tmp.sho[lensub] = '\0';
		cout << "\n\t\t&lt;subject&gt;";
		f << "\n\t\t<subject>";
		xmlSay(tmp,lensub);
		xmlWrite(tmp,lensub,f);
		cout << "&lt;/subject&gt;";
		f << "</subject>";
		for (int i=0;i<lenres;i++) {
			tmp.sho[i] = c.result[i];
		}
		tmp.sho[lenres] = '\0';
		cout << "\n\t\t&lt;result&gt;";
		f << "\n\t\t<result>";
		xmlSay(tmp,lenres);
		xmlWrite(tmp,lenres,f);
		cout << "&lt;/result&gt;";
		f << "</result>";
		for (int i=0;i<lenpre;i++) {
			tmp.sho[i] = c.prenote[i];
		}
		tmp.sho[lenpre] = '\0';
		cout << "\n\t\t&lt;prenote&gt;";
		f << "\n\t\t<prenote>";
		xmlSay(tmp,lenpre);
		xmlWrite(tmp,lenpre,f);
		cout << "&lt;/prenote&gt;";
		f << "</prenote>";
		for (int i=0;i<lennote;i++) {
			tmp.sho[i] = c.note[i];
		}
		tmp.sho[lennote] = '\0';
		cout << "\n\t\t&lt;note&gt;";
		f << "\n\t\t<note>";
		xmlSay(tmp,lennote);
		xmlWrite(tmp,lennote,f);
		cout << "&lt;/note&gt;";
		f << "</note>";
		cout << "\n\t\t&lt;hr /&gt;";
		f << "\n\t\t<hr />";
		cout << "\n\t&lt;/call&gt;</pre>\n";
		f << "\n\t</call>\n</log>\n";
		// Don't forget to add a HR at the end of the call.
		// Don't forget to add the closing </log> we overwrote.
		//  class=\"debug\"
// ==================== Now mark allGood so we can write this stuff to a file ================
		ready = true;
		f.close();
	} // CONTENT_LENGTH

	if (ready) {
		// Write information to the XML file.
		// Why don't I do this above and save space?
		// Don't forget to add a HR at the end of the call.
		// Don't forget to add the closing </log> we overwrote.
	}
	parrot("logform.htf");
// I'm leaving this here in case I some day decide to implement better error checking.

/*	if (!ready) { // Print JS to fill valid forms with previously typed data
		std::cout << "<SCRIPT  LANGUAGE=\"JavaScript\">\n<!-- Hide me from older browsers.\n";
		std::cout << "document.mkusr.uname.value = \"";
		for (int i = 0; uname[i] != '\0'; i++) {
			if (uname[i] != ' ') charJS(uname[i]);
		}
		if (nameExists) {
			charJS(char(rng(48,57,uname[0])));
			charJS(char(rng(48,57,uname[1])));
		}
		std::cout << "\"\n";
		std::cout << "document.mkusr.address.value = \"";
		for (int i = 0; address[i] != '\0'; i++) {
			charJS(address[i]);
		}
		std::cout << "\"\n";
		std::cout << "document.mkusr.mail.value = \"";
		for (int i = 0; mail[i] != '\0'; i++) {
			charJS(mail[i]);
		}
		std::cout << "\"\n";
		std::cout << "// -->\n</SCRIPT><BR>\n";
	} else if (allGood && !logged) {
		// Try to set a cookie.
		for (int i=0;i<21;i++) {
			txt.to[i] = uname[i];
		}
		sanityCook(txt);
		char ename[21];
		for (int i=0;i<20;i++) {
			ename[i] = txt.to[i];
		}
		ename[20] = '\0';
		// Start saving information.
		nr.seekg(0,std::ios::beg);
		nr.read(reinterpret_cast<char*>(&nexts),sizeof(dNxtRec));
		if (nr.bad()) {
			std::cout << "\nUgh while reading!\n";
			parrot("foot.htf");;
			exit(-1);
		}
		// Set UserID:
		ur.setid(findDeadU());
		nexts.user++;
		if (debug > 0) cSayC(3,"w\n");  //riting\n";
		nr.seekp(0,std::ios::beg);
		nr.write(reinterpret_cast<const char*>(&nexts),sizeof(dNxtRec));
		if (nr.bad()) {
			endwin();
			std::cout << "\nUgh while writing!\n";
			parrot("foot.htf");;
			exit(-1);
		}
		std::cout << "Saving data..";
		saveUser();
		for (int i=0;i<16;i++) { txt.st[i] = pass[i]; }
		std::cout << ".";
		ur.setpass(txt.st);
		std::cout << ".";
		for (int i=0;i<21;i++) {
			txt.to[i] = uname[i];
		}
		txt.to[20] = '\0';
		ur.setname(txt.to);
		std::cout << ".";
		ur.setemail(mail);
		std::cout << ".";
		ur.setupdate(time(0));
		std::cout << ".";
		ur.setaddy(address);
		std::cout << ".";
		saveUser();
		hexl(ur.id(), txt);
		std::cout << "<SCRIPT LANGUAGE=\"JavaScript\">\n<!-- Hide me from older browsers.\n";
		std::cout << "var num = \"\";\n";
		std::cout << "var un = \"\";\n";
		std::cout << "var sc = \"\";\n";
		std::cout << "function Cookie(document, name, num, un, sc, days)\n";
		std::cout << "{\n";
		std::cout << "	this.$document = document;\n";
		std::cout << "	this.$name = name;\n";
		std::cout << " if (days)\n	this.$expiration = new Date((new Date()).getTime() + days*86400000);\n";
		std::cout << "	else this.$expiration = null;";
		std::cout << " this.num = num;\n";
		std::cout << "	this.un = un;\n";
		std::cout << "	this.sc = sc;\n";
		std::cout << "}\n\n";
		std::cout << "function _Cookie_store()\n";
		std::cout << "{\n";
		std::cout << "	var cookieval = \"\";\n";
		std::cout << "	for (var prop in this) {\n";
		std::cout << "		if ((prop.charAt(0) == \'$\') || ((typeof this[prop]) == \'function\'))\n";
		std::cout << "			continue;\n";
		std::cout << "		if (cookieval != \"\") cookieval += \'&\';\n";
		std::cout << "		cookieval += prop + \':\' + escape(this[prop]);\n";
		std::cout << "	}\n";
		std::cout << "	var cookie = this.$name + \'=\' + cookieval;\n";
		std::cout << "	cookie += \'; expires=\' + this.$expiration.toGMTString();\n";
		std::cout << "	cookie += \'; num=\' + this.num;\n";
		std::cout << "	cookie += \'; un=\' + this.un;\n";
		std::cout << "	cookie += \'; sc=\' + this.sc;\n";
		std::cout << "	this.$document.cookie = cookie;\n";
		std::cout << "}\n\n";
		std::cout << "new Cookie();\n";
		std::cout << "Cookie.prototype.store = _Cookie_store;\n\n";
		std::cout << "var temp = new Cookie(document, \"DORA\", \"" << txt.st << "\", \"" << uname << "\", \"" << ename << "\", 30);\n";
		std::cout << "temp.store();\n";
		std::cout << "document.write(\'Cookie Saved.\');\n// -->\n</SCRIPT><BR>\n";
		std::cout << "<BR>\n<FORM method=\"get\" action=\"mydora.cgi\"><INPUT type=\"hidden\" name=\"show\" value=\"info\"><INPUT type=\"submit\" value=\"Continue\">";
	} else if (logged) {
		std::cout << "<BR>\nDon't I know you already? Proceed to the <A href=\"dora.cgi\">main page</A> to continue.\n<BR>";
	} else {
		std::cout << "There was an undefined error.  Please contact the site administrator!\n<BR>";
	}
// */
	parrot("foot.htf");
	return 0;
}
