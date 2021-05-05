#pragma once

#include <vector>
#include <utility>
#include <string>

#include "Reader.cpp"
#include "Parser.cpp"
#include "Decryptor.cpp"
//#include "KeyValue.cpp"
//#include "Dictionary.cpp"

using namespace std;

class Driver {
private:
	FileUtil fileUtil;
	Parser parser;
	Decryptor decryptor;
public:
	Driver() {}

	void decode_file(string in_name, string out_name) {
		std::vector<string> lines;
		fileUtil.readFile(in_name, lines);
		std::vector<int> file_nums = parser.parseFileLine(lines[0]);
		string out = decryptor.decrypt(file_nums.begin(), file_nums.end());
		cout << out << endl;
		fileUtil.writeFile(out_name, out);
	}
};

int main() {
	Driver driver;
	driver.decode_file("Encrypted.txt", "Decrypter.txt");
	return 0;
}

/*
	OUTPUT:
	Margaret Hamilton American Computer Scientist.  Margaret Hamilton, maiden name Margaret Heafield, (born August 17, 1936, Paoli, Indiana, U.S.), American computer scientist who was one of the first computer software programmers; she created the term software engineer to describe her work. She helped write the computer code for the command and lunar modules used on the Apollo missions to the Moon in the late 1960s and early '70s.  While studying mathematics and philosophy at Earlham College in Richmond, Indiana, she met James Hamilton, and they subsequently married. After graduating in 1958, she taught high school mathematics for a short time. The couple then moved to Boston. Although Margaret planned to study abstract mathematics at Brandeis University, she accepted a job at the Massachusetts Institute of Technology (MIT) while her husband attended Harvard Law School. At MIT she began programming software to predict the weather and did postgraduate work in meteorology.  In the early 1960s Hamilton joined MIT's Lincoln Laboratory, where she was involved in the Semi-Automatic Ground Environment (SAGE) project, the first U.S. air defense system. She notably wrote software for a program to identify enemy aircraft. Hamilton next worked at MIT's Instrumentation Laboratory (now the independent Charles Stark Draper Laboratory), which provided aeronautical technology for the National Aeronautics and Space Administration (NASA). She led a team that was tasked with developing the software for the guidance and control systems of the in-flight command and lunar modules of the Apollo missions. At the time, no schools taught software engineering, so the team members had to work out any problems on their own. Hamilton herself specifically concentrated on software to detect system errors and to recover information in a computer crash. Both those elements were crucial during the Apollo 11 mission (1969), which took astronauts Neil Armstrong and Buzz Aldrin to the Moon.  Hamilton left MIT in the mid-1970s to work in the private sector. She cofounded the company Higher Order Software in 1976 and established Hamilton Technologies 10 years later.  Hamilton was the recipient of various honours, including NASA's Exceptional Space Act Award (2003). Pres. Barack Obama presented her with the Presidential Medal of Freedom in 2016.

*/