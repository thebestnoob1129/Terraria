#include "saveMap.h"

bool saveBlockDataToFile(std::vector<Block> blocks, int w, int h, const char* fileName)
{
	
	std::ofstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	// Development
	permaAssertDevelopement(blocks.size() == w * h);
	permaAssertDevelopement(blocks.size() != 0);

	// Production
	if (blocks.size() != w * h) { return false; }
	if (blocks.empty()) { return false; }

	f.write((char*)&w, sizeof(w));
	f.write((char*)&h, sizeof(h));

	size_t blockCount = static_cast<size_t>(w) * h;
	f.write((char*)blocks.data(), sizeof(Block) * blockCount);

	f.close();

	return true;
}

bool loadBlockDataFromFile(std::vector<Block>& blocks, int& w, int& h, const char* fileName)
{
	blocks.clear();
	w = 0;
	h = 0;
	
	std::ifstream f(fileName, std::ios::binary);
	if (!f.is_open()) { return false; }

	// Read Dimensions
	f.read((char*)&w, sizeof(w));
	f.read((char*)&h, sizeof(h));

	if (!f || w <= 0 || h <= 0) return false;
	if (w > 10000 || h > 10000) return false;

	size_t blockCount = static_cast<size_t>(w) * h;
	blocks.resize(blockCount);

	f.read((char*)blocks.data(), sizeof(Block) * blockCount);

	if (!f)
	{
		blocks.clear();
		w = 0;
		h = 0;
		f.close();
		return false;
	}

	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks[i].sanatize();
	}

	std::cout << "W: " << w << " H: " << h << "\n";
	std::cout << "First block type: " << blocks[0].type << "\n";

	f.close();
	return true;
}

bool writeEntireFile(const char* fileName, const void* data, size_t size)
{
	std::ofstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	f.write((const char*)data, size);

	f.close();

	return true;
}



bool readEntireFile(const char* fileName, const void* data, size_t size)
{


	std::ifstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return  false; }

	f.seekg(0, std::ios_base::end);

	size_t readSize = f.tellg();
	f.seekg(0, std::ios_base::beg);


	f.read((char*)data, std::min(size, readSize));

	//auto* data_vec = static_cast<unsigned char*>(const_cast<void*>(data));

	//std::vector<unsigned char> vec(data_vec, data_vec + size);

	if (!f)
	{
		data = nullptr;
		return false;
	}
	
	std::cout << "Read Size: " << readSize << "\n";

	return true;
}