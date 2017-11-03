#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <regex>
#include <cstdlib>
#include "soptions.hpp"
#include "serverfileop.hpp"


using namespace std;
int umain() {

ServerOptions options;
options.parseFile("config.txt");
FileOperator fop(options);
set<string> v;

char * data = "1234567890\n";

v = fop.BasketLS("qwertyui");
fop.putFile("hello", "qwertyui", reinterpret_cast<void*>(data), 8);

for (auto i: v)
	cout << i << endl;
return 0;
}
