#include "bag.h"
#include <iostream>
bag::bag(): NextId(1) {

	//starting equipment
	//items.push_back({ NextId++, "Rusty Sword",1 });
	//items.push_back({ NextId++, "Quarter Staff" ,1});
	//items.push_back({ NextId++, "Short Bow" ,1});
	//items.push_back({ NextId++, "Hand Crossbow" ,1 });
	//items.push_back({ NextId++, "Butcher's Knife" ,1 });
}
int bag::AddItem(const string& name, int quantitytoAdd = 1)
{
	// if item is found add to quantity
	for (auto& item : items) {
		if (item.name == name) {
			item.quantity += quantitytoAdd;
			return item.id;
		}
	}
	// if item is not found create item
	InventoryItem newItem;
	newItem.id = NextId++;
	newItem.name = name;
	newItem.quantity = quantitytoAdd;

	items.push_back(newItem);
	return newItem.id;


}


bool bag::RemoveItemByIndex(size_t index)
{
	// searches threw items and if cant find it returns false
	if (index >= items.size()) {
		return false;

	}
	// finds the item index and decreases the quantity if more then 1, else it gets rid of the item.
	auto& item = items[index];
	if (item.quantity > 1) {
		item.quantity--;
		return true;
	}
	else {
		items.erase(items.begin() + index);
		return true;

	}

	items.erase(items.begin() + static_cast<long long>(index));
	return true;
}

bool bag::isEmpty()const {
	return items.empty();

}
size_t bag::GetSize() const {
	return items.size();
}
const vector<InventoryItem>& bag::GetItems()const {
	return items;
}

bool bag::SaveToFiles(const string& filename) const
{
	// reads file
	ofstream outFile(filename);
	if (!outFile.is_open())
	{
		return false;
	}
	// to be able to produce id, space ,name, space, quantity
	for (const auto& item : items) {

		outFile << item.id << ' '
			<< item.name << ' '
			<< item.quantity << '\n';
	}
	return true;
}

bool bag::LoadFromFiles(const string& filename)
{
	// loads save file as long as it opens
	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		return false;
	}
	items.clear();
	int maxId = -1;

	string line;
	// parsing
	while (getline(inFile, line)) {
		if (line.empty()) {
		
			continue; // skips blank lines
		}
		size_t firstSpace = line.find(' ');
		//??
		if (firstSpace == string::npos) {
			
			continue; // malformed line
		}

		size_t lastSpace = line.rfind(' ');

		if (lastSpace == string::npos || lastSpace <= firstSpace) {
		
			continue; // malformed line
		}
		string idStr = line.substr(0, firstSpace);
		string nameStr = line.substr(firstSpace + 1, lastSpace - firstSpace - 1);
		string qtyStr = line.substr(lastSpace + 1);

		

		InventoryItem item;

		try {
			item.id = stoi(idStr);
			item.quantity = stoi(qtyStr);
		}
		//??
		catch (...) {
			continue; // bad numbers, skip line
		}
		item.name = nameStr;

	

		items.push_back(item);


		if (item.id > maxId) {
			maxId = item.id;
		}
		if (maxId >= 0) {
			NextId = maxId + 1;
		}
		else {
			NextId = 1;
		}
		
	}
	return true;
}

