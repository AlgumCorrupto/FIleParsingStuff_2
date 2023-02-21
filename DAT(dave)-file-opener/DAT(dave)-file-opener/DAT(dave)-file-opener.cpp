#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstddef>
#include <string>

struct DAT_header {
	char fileID[4];
	uint32_t numofFiles;
	uint32_t filenameDirOffset;
	uint32_t firstFileDataOffset;
	char garbage[2032];
};

struct DAT_fileHeader {

	uint32_t filenameOffset;
	uint32_t fileOffset;
	uint32_t decomprFileLnght;
	uint32_t comprFileLnght;
};


struct DAT_fileData {
	std::vector<std::byte> fileData;
	std::vector<std::byte> nullByte;
};

struct DAT_file {
	DAT_fileHeader fileHeader;
	std::string filenameDir;
	DAT_fileData fileData;
};

struct DAT_directory {
	//criar uma struct que armazena informaçãp de cada arquivo
	std::vector <DAT_file> file;
};





class Utilities
{
public:


	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}


	std::string convertToString(char* a, int size)
	{
		std::string s = "";
		for (int i = 0; i < size; i++) {
			s = s + a[i];
		}
		return s;
	}

	std::vector<unsigned char> decode(std::string const& encoded_string) {
		std::string filenameEncoding{ "+ #$()-./?0123456789_abcdefghijklmnopqrstuvwxyz~" };

		int in_len = encoded_string.size();
		int in_ = 0;
		int j;
		int i = j = 0;

		unsigned char char_array_3[3], char_array_4[4];
		std::vector<unsigned char> ret;

		while (in_len-- && (encoded_string[in_] != '+')) {
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++)
					char_array_4[i] = filenameEncoding.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for ( i = 0; (i < 3); i++)
					ret.push_back(char_array_3[i]);
				i = 0;
			}
		}

		if (i) {
			for ( j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = filenameEncoding.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
		}

		return ret;
	}
};

class DatFile
{
public:
	DatFile()
	{
	}

	DatFile(const std::string& sFileName)
	{
		ParseFile(sFileName);
	}

	

	bool ParseFile(const std::string& sFileName)
	{

		//open DAT file as a stream
		std::ifstream ifs;
		ifs.open(sFileName, std::fstream::in | std::ios::binary);
		if (!ifs.is_open())
			return false;

		ifs.seekg(0, std::ios::end);

		ifs.seekg(0, std::ios::beg);

		//read DAT header 
		DAT_header header;
		ifs.read((char*)&header, sizeof(DAT_header));
		uint32_t filenameDirOffset = header.filenameDirOffset + sizeof(header);


		//verify if the file is valid
		Utilities utilites;
		std::string fileID = utilites.convertToString((char*)header.fileID, 4);
		if (fileID != "Dave" && fileID != "DAVE")
			ifs.close();

		DAT_directory directory;


		//goes to the offset end of the header and start reading it
		ifs.seekg(sizeof(DAT_header), std::ios::beg);
		for (int i = 0; i < header.numofFiles; i++)
		{
			//read header of file
			DAT_fileHeader Header;
			ifs.read((char*)&Header, sizeof(DAT_fileHeader));
			directory.file.push_back({ Header });
		}

		//goes to the filename offset and starts reading it
		ifs.seekg(filenameDirOffset, std::ios::beg);
		for (int i = 0; i < header.numofFiles; i++)
		{
			ifs.seekg((filenameDirOffset + directory.file[i].fileHeader.filenameOffset), std::ios::beg);
			std::getline(ifs, directory.file[i].filenameDir, '\0');
		}

		std::cout << directory.file[0].filenameDir.size() << std::endl;

		std::vector<unsigned char> hue = utilites.decode(directory.file[1000].filenameDir);

		std::cout << "filenameDirOffset" << filenameDirOffset << std::endl;

		for (int i = 0; i < hue.size(); i++)
		{
			std::cout << hue[i];
		}

		std::cout << "hue size: " << hue.size() << std::endl;

		std::cout << "first filename offset " << filenameDirOffset + directory.file[0].fileHeader.filenameOffset << std::endl;
		std::cout << directory.file.size() << std::endl;
		std::cout << directory.file[0].filenameDir << std::endl;



//		std::cout << "filenameOffset:" << directory.filenameOffset << "\n fileOffset: " << directory.fileOffset <<
//			"\n compressedFile: " << directory.compressedFileLength << "\n decompressedFile: " << directory.decompressedFileLength << std::endl;

		//jump to the file data offset and start reading it


		17;
		296960;

		ifs.close();
		return true;
	}
};

int main()
{ 
	DatFile datfile;
	datfile.ParseFile("ASSETS.DAT");
	std::string hel = "hel";
	return 0;
}