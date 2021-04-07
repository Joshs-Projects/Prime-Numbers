#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class TextFile {
protected:
	//std::vector<std::vector<std::string>> contentsOfFile;
	std::vector<std::string> contentsOfFile;
	std::string fileName;
	int wordCount = 0;

public:
	//default constructor
	TextFile() {  }

	TextFile(std::string fileName) {
		fileName = fileName + ".txt";
		wordCount = 0;
	}

	int SetFileName(std::string newFileName) {
		fileName = newFileName + ".txt";
		return 1;
	}

	int ReadTextFile() {
		contentsOfFile = {};
		wordCount = 0;
		//variables used temporarily
		std::string line;
		std::vector<std::string> splitLine;

		std::ifstream myFile(fileName);

		while (getline(myFile, line)) {
			std::istringstream ss(line);
			std::string token;

			//while (getline(ss, token, ' ')) {
				//splitLine.push_back(token);
			//}

			contentsOfFile.push_back(line);
			splitLine = {};

		}
		myFile.close();
		return 1;
	}

	int WriteTextFile() {
		std::ofstream myFile;
		myFile.open(fileName);

		for (int i = 0; i < contentsOfFile.size(); i++) {
			myFile << contentsOfFile[i] << "\n";
		}

		myFile.close();

		return 1;
	}

	int addLine(std::string line) {
		contentsOfFile.push_back(line);
		return 1;
	}

};