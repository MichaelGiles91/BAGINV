#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "stdlib.h"
#include "fstream"
#include "sstream"




using namespace std;
struct InventoryItem {
	int id; // unique ids assigned by inv
	string name; // display item name
	int quantity; // displays the number of one unique item
};
class bag
{
public:

	bag();
	//adds item by name and returns IDs
	int AddItem(const string& name, int quantity);
	// Removes Items at the given index (0 based) returns true if removed.
	bool RemoveItemByIndex(size_t index);

	//read only acces for items
	const vector<InventoryItem>& GetItems()const;

	bool SaveToFiles(const string& filename) const;
	bool LoadFromFiles(const string& filename);

	// helpers
	bool isEmpty()const;
	size_t GetSize()const;

private:
	vector<InventoryItem> items;
	int NextId;
};

