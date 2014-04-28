#pragma once
#ifndef XMLFILES_H_INCLUDED
#define XMLFILES_H_INCLUDED

#include <iostream>
#include "BSValues.h"
#include <fstream>

struct BS_xml_attribute
{
	std::string name, value;
};

struct BS_xml_node
{
	std::string name;
	bool checked;
};


class BSXmlFiles
{
public:
	void getTheFileAsString(std::string fileName, std::string &theString);

	bool checkElementNames(std::string el1, std::string el2);

	bool checkIfNodeIsWanted(std::string node, std::vector<std::string> theNodes);

	bool readAnyXML(std::string fileName, std::string headNode, std::string secondNode, std::vector<std::string> theNodes,
					std::vector<std::string> theAttributes, std::vector<ListaDeCaractere*> &theNodeValues, std::vector<std::string> &headAttributesValues);
};

#endif // XMLFILES_H_INCLUDED
