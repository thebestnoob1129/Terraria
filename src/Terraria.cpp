// Terraria.cpp : Defines the entry point for the application.
//

#include "Terraria.h"

using namespace std;

// const char *PlayerData = ResourcePath "./playerData:

int main()
{
	cout << "Hello CMake." << endl;

#if PRODUCITON_BUILD == 0
	std::cout << "Production Build \n";
#endif


	return 0;
}
