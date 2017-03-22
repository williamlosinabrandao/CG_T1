#include "csvReader.h"
#include <fstream>	//ifstream
#include <sstream>	//istringstream
#include <iomanip>	//getline

csvReader::csvReader(std::string fileName)
{
	InputFileStream = new std::ifstream(fileName);
}

std::vector<int>
csvReader::getNextLine(void)
{
	std::vector<int> thisTokenizedStream;

	std::string thisLine;
	if (std::getline(*InputFileStream, thisLine))
	{
		std::istringstream thisStream(thisLine);
		
		while (thisStream)
		{
			std::string thisToken;
			if (!getline(thisStream, thisToken, ','))
			{
				break;
			}
			else
			{
				thisTokenizedStream.push_back(std::stoi(thisToken));
			}
		}
	}

	return thisTokenizedStream;
}

std::vector<std::vector<int>>
csvReader::getAllLines(void)
{
	std::vector<std::vector<int>> thisTokenizedFile;

	while (InputFileStream)
	{
		std::string thisLine;
		if (std::getline(*InputFileStream, thisLine))
		{
			std::istringstream thisStream(thisLine);
			std::vector<int> thisTokenizedStream;
			while (thisStream)
			{
				std::string thisToken;
				if (!getline(thisStream, thisToken, ','))
				{
					break;
				}
				else
				{
					thisTokenizedStream.push_back(std::stoi(thisToken));
				}
			}
			thisTokenizedFile.push_back(thisTokenizedStream);
		}
		else
		{
			break;
		}
	}

	return thisTokenizedFile;
}