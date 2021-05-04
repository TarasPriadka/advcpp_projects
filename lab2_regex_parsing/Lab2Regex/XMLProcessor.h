#pragma once

#include <vector>
#include <string>
#include <memory>
#include <regex>
#include <iostream>
#include <stdexcept>
#include "Files.h"
#include "Structs.h"

using namespace std;

class XMLProcessor {
	regex tag;
	regex tagOpen;
	regex tagClose;
public:
	XMLProcessor() {
		tag = regex("<\/?[A-Za-z0-9]*>");
		tagOpen = regex("<[A-Za-z0-9]*>");
		tagClose = regex("<\/[A-Za-z0-9]*>");
	}
	vector<shared_ptr<XMLElement>> processXMLFile(string path);
	shared_ptr<XMLElement> parseStringToProduct(string &joinedLines);
};

vector<shared_ptr<XMLElement>> XMLProcessor::processXMLFile(string path) {
	vector<shared_ptr<XMLElement>> extractedTags;
	FileUtil files;
	vector<string> lines;
	files.readFile(path, lines);
	string meshedLines;
	for (string line : lines)
		meshedLines += line + '\n';
	// Start -> find the first matching open tag -> extract name into a new xmltag
	// continue until reach closing tag
	// capture all of the other tags in between open and closed tag

	while (!meshedLines.empty()) { // algorithm will move through the string, when no string left, you are done
		try {
			shared_ptr<XMLElement> newElem = parseStringToProduct(meshedLines);
			if (newElem)
				extractedTags.push_back(newElem);
		}
		catch (const std::exception& e) {
			cout << "Encountered an error while parsing XML." << endl;
			cout << e.what() << endl;
			return vector<shared_ptr<XMLElement>>{};
		}
	}
	return extractedTags;
}

// Alg:
// Open new tag-> find next tag -> check if it is a closing tag or a new tag
// if new tag recurse substring starting from the new tag
// Assumption, node which includes other nodes(not a leaf node), will not include any data value
shared_ptr<XMLElement> XMLProcessor::parseStringToProduct(string &lines) {
	shared_ptr<XMLElement> elem(new XMLElement);
	smatch match;
	regex_search(lines, match, this->tag);
	string tag_str = match.str();
	
	// if the found text an open tag
	if (regex_search(tag_str.begin(), tag_str.end(), this->tagOpen)) {
		elem->name = tag_str.substr(1, tag_str.length() - 2);
		shared_ptr<XMLElement> child;
		bool didLoop = false; //if you already looped, need to not cut the tag since it wasn't yet processed
		while(true) {
			regex_search(lines, match, this->tag);
			string next_section = lines.substr(didLoop ? match.position() : match.position() + match.length());
			child = parseStringToProduct(next_section);
			if (child->name == "/" + elem->name) { // current node is a leaf node
				if (elem->children.empty()) {
					elem->value = child->value;
				}
				lines = next_section;
				return elem;
			} else if (regex_search(child->name.begin(), child->name.end(), regex("\/.*"))) {
				throw std::invalid_argument("Improperly formated XML file. Tag " + tag_str + " doesn't match with <" + child->name + ">");
			} else {
				elem->children.push_back(move(child));
				lines = next_section;
				didLoop = true;
			}
		}
	}
	else if (regex_search(tag_str.begin(), tag_str.end(), this->tagClose)) {
		elem->name = tag_str.substr(1, tag_str.length() - 2);
		elem->value = lines.substr(0, match.position());
		//elem->name = "END";
		lines = lines.substr(match.position() + match.length(), lines.length());
		return elem;
	} else {
		//didn't find any tags left, you are done
		lines = string();
		return NULL;
	}
	return elem;
}
