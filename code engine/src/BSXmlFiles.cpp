#include "BSXmlFiles.h"

void BSXmlFiles::getTheFileAsString(std::string fileName, std::string &theString)
{
	std::ifstream fin(fileName.c_str());

	theString = "";
	std::string readingLine;

	while(getline(fin, readingLine))
	{
		theString += readingLine;
	}
}


bool BSXmlFiles::checkElementNames(std::string el1, std::string el2)
{
	if(el1 != el2)
	{
		return false;
	}
	return true;
}


bool BSXmlFiles::checkIfNodeIsWanted(std::string node, std::vector<std::string> theNodes)
{
	for(int i = 0; i < theNodes.size(); i++)
	{
		if(node == theNodes[i])
		{
			return true;
		}
	}
	return false;
}

bool BSXmlFiles::readAnyXML(std::string fileName, std::string headNode, std::string secondNode, std::vector<std::string> theNodes,
							std::vector<std::string> theAttributes, std::vector<ListaDeCaractere*> &theNodeValues, std::vector<std::string> &headAttributesValues)
{
	std::string moFoLongString;
	getTheFileAsString(fileName, moFoLongString);

	bool encoding = false;
	std::string sEncoding = "";
	int lastPosition = 0;

	bool readingBlank = false;

	for(int i = 0; i < moFoLongString.size(); i++)
	{
		lastPosition = i + 1;
		sEncoding += moFoLongString[i];
		if(moFoLongString[i] == '>')
		{
			encoding  = true;
			break;
		}
	}
	if(sEncoding != "<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
	{
		std::cout << "The encoding is not good buddy!!" << '\n' << "It should be: " << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << '\n';
		return false;
	}
//    std::cout<<sEncoding<<'\n';

	std::vector<BS_xml_node> listOfActualNodes;

	std::string elementName1 =  "", elementName2 =  "", elementValue =  "", attributeName =  "", attributeValue =  "";
	bool readsAnElementName1 = false, readsAnElementName2 = false, readsAnElementValue = false, readsAnAttributeName = false, readsAnAttributeValue = false;

	for(int i = lastPosition; i < moFoLongString.size(); i++)
	{
		if(moFoLongString[i] == '<')
		{
			readingBlank = false;
			readsAnElementName1 = true;
			readsAnElementValue = false;
			readsAnAttributeName = false;
			readsAnAttributeValue = false;
			if(moFoLongString[i + 1] == '/')
			{
				readsAnElementName1 = false;
				readsAnElementName2 = true;
				i++;
			}
			else
			{
				BS_xml_node newNode;
				listOfActualNodes.push_back(newNode);
			}
			continue;
		}
		else
			if(moFoLongString[i] == ' ' && !readsAnAttributeValue && !readsAnElementValue)
			{
				if(attributeValue != "" && attributeName != "")
				{
					BS_xml_attribute atribut;
					atribut.name = attributeName;
					atribut.value = attributeValue;


					headAttributesValues.push_back(attributeValue);
				}
				attributeName = "";
				attributeValue = "";

				readsAnElementName1 = false;
				readsAnElementName2 = false;
				readsAnElementValue = false;
				readsAnAttributeName = true;
				readsAnAttributeValue = false;
				continue;
			}
			else
				if(moFoLongString[i] == '=' && moFoLongString[i + 1] == '"' && !readsAnElementValue)
				{
					readingBlank = false;
					readsAnElementName1 = false;
					readsAnElementName2 = false;
					readsAnElementValue = false;
					readsAnAttributeName = false;
					readsAnAttributeValue = true;
					i++;
					continue;
				}
				else
					if(moFoLongString[i] == '"' && !readsAnElementValue)
					{
						readingBlank = false;
						if(attributeValue != "" && attributeName != "")
						{
							BS_xml_attribute atribut;
							atribut.name = attributeName;
							atribut.value = attributeValue;

							headAttributesValues.push_back(attributeValue);
						}
						attributeName = "";
						attributeValue = "";

						readsAnElementName1 = false;
						readsAnElementName2 = false;
						readsAnElementValue = false;
						readsAnAttributeName = false;
						readsAnAttributeValue = false;
						continue;
					}
					else
						if(moFoLongString[i] == '>')
						{
							readingBlank = false;
							readsAnElementValue = true;
							if(readsAnElementName2)
							{
								int index = 0;
								for(int j = listOfActualNodes.size() - 1; j >= 0; j--)
								{
									if(listOfActualNodes[j].checked != true)
									{
										index = j;
										break;
									}
								}
								readsAnElementValue = false;
								if(!checkElementNames(listOfActualNodes[index].name, elementName2))
								{
									std::cout << "The element names do not match: " << listOfActualNodes.back().name << ' ' << elementName2 << ". Position: " << i << '\n';
									return false;
								}
								else
								{
									if(elementName2 == secondNode)
									{
//                                        std::cout<<"    "<<elementName2<<'\n'<<'\n';
									}
									else
										if(elementName2 == headNode)
										{
//                                            std::cout<<elementName2<<'\n';
										}
										else
										{
											if(checkIfNodeIsWanted(elementName2, theNodes))
											{
												theNodeValues.back()->theCharArray2D.push_back(elementValue);
											}
										}
								}
								listOfActualNodes[index].checked = true;
								elementName1 = "";
								elementName2 = "";
								elementValue = "";
								attributeName = "";
								attributeValue = "";
								readingBlank = true;
							}
							else
							{
								readingBlank = false;
								if(elementName1 == secondNode)
								{
									ListaDeCaractere *newNode = new ListaDeCaractere;
									theNodeValues.push_back(newNode);

//                                    std::cout<<"    "<<elementName1<<'\n';
								}
								else
									if(elementName1 == headNode)
									{
//                                        std::cout<<elementName1<<'\n';
									}
								listOfActualNodes.back().name = elementName1;
								elementName1 = "";
							}

							readsAnElementName1 = false;
							readsAnElementName2 = false;
							readsAnAttributeName = false;
							readsAnAttributeValue = false;

							continue;
						}
		if(readingBlank)
		{
			continue;
		}

		///write the name, attribute or the value of an element
		if(readsAnElementName1)
		{
			elementName1 += moFoLongString[i];
		}
		else
			if(readsAnElementName2)
			{
				elementName2 += moFoLongString[i];
			}
			else
				if(readsAnAttributeName)
				{
					attributeName += moFoLongString[i];
				}
				else
					if(readsAnAttributeValue)
					{
						attributeValue += moFoLongString[i];
					}
					else
						if(readsAnElementValue)
						{
							elementValue += moFoLongString[i];
						}
	}
	return true;
}
