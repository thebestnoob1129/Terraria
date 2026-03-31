#include "saveMap.h"

const int VERSION = 1;

struct BlockSave1{
	std::uint16_t type = 0;
	std::uint16_t variation = 0;

	Block toBlock() const
	{
		Block b;
		b.type = type;
		b.variation = variation;
		return b;
	}

};

BlockSave1 ToBlockSave1(const Block& b)
{
	BlockSave1 bs;
	bs.type = b.type;
	bs.variation = b.variation;
	return bs;
}

bool saveBlockDataToFile(std::vector<Block> &blocks, int w, int h, const char* fileName)
{
	std::cout << "Saving Block Data To File: " << fileName << "\n";
	std::ofstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	// Development
	permaAssertDevelopement(blocks.size() == w * h);
	permaAssertDevelopement(blocks.size() != 0);

	// Production
	if (blocks.size() != w * h) { return false; }
	if (blocks.empty()) { return false; }

	f.write((char*)&VERSION, sizeof(VERSION));
	f.write((char*)&w, sizeof(w));
	f.write((char*)&h, sizeof(h));

	size_t blockCount = static_cast<size_t>(w) * h;
	f.write((char*)blocks.data(), sizeof(Block) * blockCount);

	for (size_t i = 0; i < blocks.size(); i++)
	{
		std::cout << "Block " << i << ": Type: " << blocks[i].type << " Variation: " << blocks[i].variation << "\n";
		auto b = ToBlockSave1(blocks[i]);
		f.write((char*)&b, sizeof(b));
	}

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

	int readVersion = 0;

	// Read Dimensions
	f.read((char*)&readVersion, sizeof(readVersion));
	f.read((char*)&w, sizeof(w));
	f.read((char*)&h, sizeof(h));

	if (!f || w <= 0 || h <= 0) return false;
	if (w > 10000 || h > 10000) return false;

	switch (readVersion)
	{
		case 1:
		{
			size_t blockCount = static_cast<size_t>(w) * h;
			blocks.resize(blockCount);

			for (size_t i = 0; i < blockCount; i++)
			{
				BlockSave1 bs;
				f.read((char*)&bs, sizeof(bs));
				if (!f)
				{
					blocks.clear();
					w = 0;
					h = 0;
					f.close();
					return false;
				}
				blocks[i] = bs.toBlock();
			}
			break;
		}
		default:
		{
			blocks.clear();
			w = 0;
			h = 0;
			f.close();
			return false;
			break;
		}
	}

	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks[i].sanatize();
	}

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