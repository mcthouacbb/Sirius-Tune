import re

string = open("quiet-labeled.epd").read()
#print(string)
string = string.replace(";", "");
string = string.replace("\"", "");
string = string.replace("c9 ", "");
open("quiet-labeled.epd", "w").write(string);