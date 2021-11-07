#include "highscore.h"
#include <rapidxml_utils.hpp>
#include <rapidxml.hpp>

#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif

highscore::highscore()  {
	
	std::string prfx = filepath;
	std::ifstream ifile;
	ifile.open(prfx +"save.xml");

	if (ifile) {
		std::ofstream outfile(prfx + "save.xml");
		outfile << "<lastscore>0</lastscore>\n" 
			<< "<highscore>0</highscore>"; //HARDCODED BECAUSE IM A SIMP
		outfile.close();
		maxscore = 0;
		previousscore = 0;

	}
	else {
		rapidxml::file<> xmlFile(filepath "save.xml");
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		maxscore = atoi(doc.first_node("highscore")->value());
		previousscore = atoi(doc.first_node("lastscore")->value());
		
	}
	ifile.close();
	
}
void highscore::update(int prev) {
	
	std::string prfx = filepath;
	if (prev > maxscore) {
		maxscore = prev;
	}
	previousscore = prev;
	std::ofstream outfile(prfx + "save.xml");
	outfile << "<lastscore>" << std::to_string(previousscore).c_str() << "</lastscore>\n"
		<< "<highscore>" << std::to_string(maxscore).c_str() << "</highscore>"; //HARDCODED BECAUSE IM A SIMP
	outfile.close();
	
	
}
