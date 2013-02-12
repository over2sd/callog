#ifndef CALLLOG_H
#define CALLLOG_H
#include <string>
using std::string;

//#define DEBUG
class callRecord {
	public:
		string entity;
		string number;
		string year;
		string mo;
		string day;
		string time;
		string subject;
		string result;
		string prenote;
		string note;
};

struct dTxTx {
	char sho[601];
};


int unURL(dTxTx &a, int c) {
#ifdef DEBUG
	std::cout << "<p>unURL subbuild: t+017</p>\n";
#endif
	int b = 0;
	char buff[601];
	char buff2[601] = "Unchanged!!!!ERROR!";
	int len = 0;
	for (int i=0;i<=c;i++) {
		buff[i] = a.sho[i];
		if (buff[i] == '\0') {
			len = i; i = c;
//			std::cout << "<p>unURL Len: " << len << "/" << c << " </p>\n";
		}
	}
	if (len == 0) {
		len = c; // If no '\0' found, trust the length passed, since there's no premature terminator.
		std::cout << "\n<!-- No terminator -->\n"; // But complain about it in comments.
	}
	buff[600] = '\0';
	int y = -1;
	char hexstr[4];
#ifdef DEBUG
	std::cout << "<p>Input: " << a.sho << " Buffer1: " << buff << " Length: " << len << "/" << c << "</p>\n";
#endif
	for (int x = 0; x < len; x++) {
		y++;
#ifdef DEBUG
		std::cout << "<span>X: " << x << " (" << buff[x] << ") Y: " << y << "</span>\n";
#endif
		switch (buff[x]) {
			// Convert all + chars to space chars //
			case '+':
				buff2[y] = ' ';
				if (buff2[y] == ' ' && buff2[y - 1] == ' ') { y--; b++; }
#ifdef DEBUG
				std::cout << "*";
#endif
				break;
			// Convert all %xy hex codes into ASCII chars //
			case '%':
				// Copy the two bytes following the % //
				strncpy(hexstr, &buff[x + 1], 2);
				// Skip over the hex //
				x = x + 2; b += 2;
				// Convert "<" to HTML sequence for display of "<" //
				if( ((strcmp(hexstr,"3C")==0)) ) {
					buff2[y]='&';
					y++;
					buff2[y]='l';
					y++;
					buff2[y]='t';
					y++;
					buff2[y]=';';
					y++; x++;
					b -= 3;
#ifdef DEBUG
					std::cout << "&lt;";
#endif
					while (buff[x] == '%') { // I have no idea why this works, but it does. :(
						strncpy(hexstr, &buff[x + 1], 2);
						x = x + 2; b += 2;
						if( ((strcmp(hexstr,"3C")==0)) ) {
							buff2[y]='&';
							y++;
							buff2[y]='l';
							y++;
							buff2[y]='t';
							y++;
							buff2[y]=';';
							y++; x++;
							b -= 3;// std::cout << "&lt;";
						}
					}
				}
				// Convert the hex to ASCII //
				if( strcmp(hexstr,"3C")!=0 ) {
#ifdef DEBUG
				std::cout << "<!-- , -->";
#endif
					buff2[y] = (char)strtol(hexstr, NULL, 16);
					if (buff2[y] == 13 || buff2[y] == 10) {
						buff2[y] = ' ';
						if (buff2[y] == ' ' && buff2[y - 1] == ' ') { y--; b++; }
					}
					break;
				}
#ifdef MYNOTDEF
				// Prevent user from altering URL delimiter sequence //
				if( ((strcmp(hexstr,"26")==0)) || ((strcmp(hexstr,"3D")==0)) ) {
					buff2[y]='%'; std::cout << "%";
					y++;
					strcpy(buff2,hexstr);
					y=y+2; b -= 2;
					break;
				}
#endif				
			// Make an exact copy of anything else //
			default:
				buff2[y] = buff[x];
#ifdef DEBUG
				std::cout << buff[x];
#endif
				break;
		}
//		std::cout << buff2[y];
	}
#ifdef DEBUG
	std::cout << "\n<p>" << buff2 << "</p>\n";
#endif
/*	int b2 = b; int z = -1;
	std::cout << "\n<p>URL processing: ";
	for (int i3=0;i3<len - (b - 1);i3++) {
		z++;
		std::cout << buff2[i3];
		if (buff2[i3] == '<' || buff2[i3] == '>') {
			buff[z] = '&'; z++;
			if (buff2[i3] == '<') {
				buff[z] = 'l';
			} else {
				buff[z] = 'g';
			}
			z++;
			buff[z] = 't'; z++;
			buff[z] = ';';
			b2 -= 3;
		} else {
			buff[z] = buff2[i3];
		}
	}
	b = b2;
	std::cout << "</p>\n";
// */
	for (int i2=0;i2<len - (b - 1);i2++) {
		a.sho[i2] = buff2[i2];
	}
	for (int y=(len - (b - 1));y<=len;y++) {
		a.sho[y] = '*';
	}
	return b;
}


void xmlWrite(dTxTx s, int len, std::fstream &x) {
	for (int i=0;i<len;i++) {
	// Loop
		switch (s.sho[i]) {
		// switch
			case '<':
			// Check for <
				x << "&lt;"; break;
			case '&':
			// Check for &
				// If it's &lt; or &gt;, let it pass
				if (s.sho[i + 2] == 't' && s.sho[i + 3] == ';') {
					if (s.sho[i + 1] == 'l' || s.sho[i + 1] == 'g') {
						x << "&";
					} else {
						x << "&amp;";
					}
				}
				break;
			case '>':
			// Check for >
				x << "&gt;"; break;
			case '\'':
				x << "&apos;"; break;
			case '\"':
				x << "&quot;"; break;
			default:
			// Say unchanged
				x << s.sho[i];
		}
		//
	}
	//
}

void xmlSay(dTxTx s, int len) {
	for (int i=0;i<len;i++) {
	// Loop
		switch (s.sho[i]) {
		// switch
			case '<':
			// Check for <
				std::cout << "&lt;"; break;
			case '&':
			// Check for &
				// If it's &lt; or &gt;, let it pass
				if (s.sho[i + 2] == 't' && s.sho[i + 3] == ';') {
					if (s.sho[i + 1] == 'l' || s.sho[i + 1] == 'g') {
						std::cout << "&";
					} else {
						std::cout << "&amp;";
					}
				}
				break;
			case '>':
			// Check for >
				std::cout << "&gt;"; break;
			case '\'':
				std::cout << "&apos;"; break;
			case '\"':
				std::cout << "&quot;"; break;
			default:
			// Say unchanged
				std::cout << s.sho[i];
		}
		//
	}
	//
}


#endif
