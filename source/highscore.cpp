#include "highscore.h";
#include <rapidxml_utils.hpp>
#include <rapidxml.hpp>


highscore::highscore()  {

	std::ifstream f("./save.xml");

	if (!f.good()) {
		std::ofstream outfile("./save.xml");
		outfile << "<lastscore>0</lastscore>\n" 
			<< "<highscore>0</highscore>"; //HARDCODED BECAUSE IM A SIMP
		outfile.close();
		maxscore = 0;
		previousscore = 0;

	}
	else {
		rapidxml::file<> xmlFile("./save.xml");
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		maxscore = atoi(doc.first_node("highscore")->value());
		previousscore = atoi(doc.first_node("lastscore")->value());
		
	}
	f.close();
}
void highscore::update(int prev) {
	if (prev > maxscore) {
		maxscore = prev;
	}
	previousscore = prev;
	std::ofstream outfile("save.xml");
	outfile << "<lastscore>" << std::to_string(previousscore).c_str() << "</lastscore>\n"
		<< "<highscore>" << std::to_string(maxscore).c_str() << "</highscore>"; //HARDCODED BECAUSE IM A SIMP
	outfile.close();
}