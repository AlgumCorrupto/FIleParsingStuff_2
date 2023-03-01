#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstddef>
#include <string>
#include <tuple>

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
	std::string decrFilenames;
};

struct DAT_directory {
	//criar uma struct que armazena informaçãp de cada arquivo
	std::vector <DAT_file> file;
};





class Utilities
{
public:


	std::string convertToString(char* a, int size)
	{
		std::string s = "";

		for (int i = 0; i < size; i++) {
			s = s + a[i];
		}
		return s;
	}

	std::string convertToString(char a)
	{
		std::string s = "";
		s = a;

		return s;
	}

	std::tuple<std::string, bool> decode(std::string const& encoded_string) {
		std::string filenameEncoding{ "+ #$()-./?0123456789_abcdefghijklmnopqrstuvwxyz~" };

		int stringLength = encoded_string.size();
		int in_ = 0;
		int j;
		int i = j = 0;

		unsigned char char_array_4[4];
		uint8_t int_array_5[4];
		std::string ret;
		int firstChar = encoded_string.at(0);
		int secondChar = encoded_string.at(1);
		auto index = (firstChar & 0x7) | ((firstChar & 0xC0) >> 3) | ((secondChar & 0x7) << 5);
		int count = 0;
		bool InsideFolder = false;

		while (stringLength != 0) {
			stringLength--; count++;
			char_array_4[i] = encoded_string[in_]; in_++, i++;
			if (i == 3 || stringLength < 2) {
				int_array_5[0] = (char_array_4[0] & 0x3f);
				int_array_5[1] = (char_array_4[0] >> 6) | ((char_array_4[1] & 0xf) << 2);
				int_array_5[2] = (char_array_4[1] >> 4) | ((char_array_4[2] & 0x3) << 4);
				int_array_5[3] = (char_array_4[2] >> 2);

				if (stringLength < 1)
				{
					for (i = 3; i > (stringLength); i--)
						int_array_5[i] = 0;
				}

				if ((encoded_string.at(0) & 0x3f) > 0x38 && count < 4)
				{
					for (i = 0; i < 2; i++)
						int_array_5[i] = 0;
					InsideFolder = true;
				}

				for (i = 0; (i < 4); i++)
				{
					if (int_array_5[i] != 0)
						ret.append(convertToString(filenameEncoding.at(int_array_5[i])));
				}
				i = 0;
			}
		}

		return  std::make_tuple(ret, InsideFolder);
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
		std::string folderName = "";
		for (int i = 0; i < header.numofFiles; i++)
		{
			std::string filename;
			bool isInsideFolder;


			ifs.seekg((filenameDirOffset + directory.file[i].fileHeader.filenameOffset), std::ios::beg);
			std::cout << ifs.tellg() << std::endl;
			std::getline(ifs, directory.file[i].filenameDir, '\0');
			tie(filename, isInsideFolder) = utilites.decode(directory.file[i].filenameDir);

			if (false == isInsideFolder )
			{
				directory.file[i].decrFilenames = folderName = filename;
			}
			else
				directory.file[i].decrFilenames = folderName + filename;




			std::cout << "extracting: " << directory.file[i].decrFilenames << std::endl;
		}

		std::cout << directory.file[0].filenameDir.size() << std::endl;

		std::cout << "filenameDirOffset" << filenameDirOffset << std::endl;

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