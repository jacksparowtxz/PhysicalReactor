
#pragma once
#include "CommonInclude.h"



#include <string>
#include <vector>

#define RADIX 10   
#define KEYNUM 10
namespace PRE
{
	struct DataBlob
	{
		char *Data;
		size_t dataSize;
		size_t dataPos;

		//DataBlob(const std::string& filename = "");
		~DataBlob() {}

		template<typename T>
		void read(T& data)
		{
			assert(Data != nullptr);
			assert(dataPos - (size_t)Data < dataSize);
			memcpy(&data, reinterpret_cast<void*>(dataPos), sizeof(T));
			dataPos += sizeof(T);
		}
	};



	template<uint64_t BitSize>
	struct BitSizeStruct
	{
		uint64_t data : BitSize;
	};

	std::string toupper(const std::string& s);

	bool readByteData(const std::string& filename, BYTE** data, size_t& datasize);

	//void messageBox(const std::string& msg, const std::string& caption = "Warning");

	//void screenshot(const std::string& name = "");

	//std::string getCurrentDataTimeString();

	//std::string GetApplicationDirection();

	//std::string GetOriginalWorkingDirectory();

	//std::string GetWorkingDirectory();

	//bool SetWorkingDirectory(const std::string& path);

	//void GetFileInDirectory(std::vector<std::string>&out, const std::string &directory);

	void SplitPath(const std::string& fullPath, std::string& dir, std::string& filename);

	//std::string GetFileNameFromPath(const std::string& fullPath);

	std::string GetDirectoryFromPath(const std::string& fullPath);

	std::string GetExtensionFromFileName(const std::string& filename);
	UINT AlignTo16(int Round,UINT size);
	//void RemoveExtensionFromFileName(std::string& filename);
	//void Sleep(float milliseconds);
	int GetDigitInPos(int num, int pos);
	
	template<typename T>
	 void RadixSort(T* unorderarray,int offset, int datanum);


	template<typename T>
	 void RadixSort(T* unorderarray, int offset,int datanum)
	{
		T* radixArrays[RADIX];
		for (int i = 0; i < 10; i++)
		{
			radixArrays[i] = (T*)malloc(sizeof(T)*(datanum + 1));
			radixArrays[i][0] = 0;
		}
		for (int pos=1;pos<=KEYNUM;pos++)
		{
			for (int i = 0; i < datanum; i++)
			{
				int num = GetDigitInPos(unorderarray[i + offset], pos);
				int index = ++radixArrays[num][0];
				radixArrays[num][index] = unorderarray[i + offset];
			}
			for (int i=0,j=0;i<RADIX;i++)
			{
				for (int k=1;k<=radixArrays[i][0];k++)
					unorderarray[offset + j++] = radixArrays[i][k];
				radixArrays[i][0] = 0;
			}
		}
	}

	template<typename T>
	void mergeBlocks(T* const pDataArray, int arraylen, const int blockNum, T* const ResultArray);

	template<typename T>
	void mergeBlocks(T* const pDataArray, int arraylen, const int blockNum, T* const ResultArray)
	{
		int blockLen = arraylen / blockNum;
		int blockIndex[blockNum];

		for (int i=0;i<blockNum;++i)
		{
			blockIndex[i] = i*blockLen;
		}
		T smallest = 0;
		for (int i=0;i<arraylen;++i)
		{
			for (int j=0;j<blockNum;++j)
			{
				if (blockIndex[j]<(j*blockLen+blockLen))
				{
					smallest = pDataArray[blockIndex[j]];
					break;
				}
			}
			for (int j=0;j<blockNum;++j)
			{
				if ((blockIndex[j] < (j*blockLen + blockLen)) && (pDataArray[blockIndex[j]] < smallest))
				{
					smallest = pDataArray[blockIndex[j]];
				}
			}
			for (int j=0;j<blockNum;++j)
			{
				if ((blockIndex[j]<(j*blockLen+blockLen))&&(pDataArray[blockLen[j]]==smallest))
				{
					++blockIndex[j];
					break;
				}
			}
			ResultArray[i] = smallest;
		}
	}

	template<typename TO, typename FROM>
	inline TO union_cast(FROM from)
	{
		static_assert(sizeof(FROM) == sizeof(TO), "Size mismatch. Cannot use a union_cast for types of different sizes.");

		union
		{
			FROM castFrom;
			TO castTo;
		};

		castFrom = from;

		return castTo;
	}

}