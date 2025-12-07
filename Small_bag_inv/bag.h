#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "stdlib.h"



using namespace std;

class bag
{
public:

	bag();

	void AddItem();
	void RemoveItem();
	void ViewItems();
	bool QuitProgram();
private:
	string item;
	int NumChoice = 0;
	int index = -1;

	vector<string> items = { "Penis", "vagina", "taint" ,"sword" };

	bool quit = false;
	string userinput;
};

