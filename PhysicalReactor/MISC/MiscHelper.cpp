#include "MiscHelper.h"
#include <locale>
#include <direct.h>
#include <chrono>
#include <iomanip>
#include <fstream>


using namespace std;
/*
Halo::DataBlob::DataBlob(const std::string& filename / *= ""* /)
{

}

Halo::DataBlob::~DataBlob()
{

}*/

std::string PRE::toupper(const std::string& s)
{
	std::string result;
	std::locale loc;
	for (unsigned int i = 0; i < s.length(); ++i)
	{
		result += std::toupper(s.at(i), loc);
	}
	return result;
}


bool PRE::readByteData(const std::string& filename, BYTE** data, size_t& datasize)
{
	ifstream file(filename, ios::binary | ios::ate);
	if (file.is_open()) {

		datasize = (size_t)file.tellg();
		file.seekg(0, file.beg);
		*data = new BYTE[datasize];
		file.read((char*)*data, datasize);
		file.close();

		return true;
	}
	stringstream ss("");
	//ss << "File not found: " << filename;
	//qDebug("File not found:");
	//messageBox(ss.str());
	return false;


}
/*
void Halo::messageBox(const std::string& msg, const std::string& caption / *= "Warning"* /)
{

}*/

std::string PRE::GetDirectoryFromPath(const std::string& fullPath)
{
	return "a";
}



/*
void Halo::screenshot(const std::string& name= "")
{

}*/

/*
std::string Halo::getCurrentDataTimeString()
{

}

std::string Halo::GetApplicationDirection()
{

}

std::string Halo::GetOriginalWorkingDirectory()
{

}

std::string Halo::GetWorkingDirectory()
{

}

bool Halo::SetWorkingDirectory(const std::string& path)
{

}

void Halo::GetFileInDirectory(std::vector<std::string>&out, const std::string &directory)
{

}*/

void PRE::SplitPath(const std::string& fullPath, std::string& dir, std::string& filename)
{
	size_t found;
	found = fullPath.find_last_of("/\\");
	dir = fullPath.substr(0, found + 1);
	filename = fullPath.substr(found + 1);
}

/*
std::string Halo::GetFileNameFromPath(const std::string& fullPath)
{

}*/



std::string PRE::GetExtensionFromFileName(const std::string& filename)
{
	size_t idx = filename.rfind('.');

	if (idx != std::string::npos)
	{
		std::string extension = filename.substr(idx + 1);
		return extension;
	}

	// No extension found
	return "";
}

UINT PRE::AlignTo16(int Round, UINT size)
{
	if (Round==0)
	{
		return std::move(0);
	}
	else
	{
		if (size<=256)
		{
			return std::move(16);
		}
		else
		{
			UINT i = size / 256;
			return std::move((i + 1) * 16);
		}
	}
}

int PRE::GetDigitInPos(int num, int pos)
{
	int temp = 1;
	for (int i=0;i<pos-1;i++)
		temp *= 10;
	return (num / temp) % 10;
}

void PRE::LoadSH(std::string filename, shcoeffs* cofs)
{
	ifstream ifs(filename);

	for (int i = 0; i < 9; i++)
	{
		ifs >> cofs[i].r >> cofs[i].g >> cofs[i].b;
	}
}

/*
void Halo::RemoveExtensionFromFileName(std::string& filename)
{

}

void Halo::Sleep(float milliseconds)
{

}
*/

