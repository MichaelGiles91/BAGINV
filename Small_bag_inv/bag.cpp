#include "bag.h"
#include <iostream>
#include <limits>
bag::bag(): NextId(1) {

	//starting equipment
	//items.push_back({ NextId++, "Rusty Sword",1 });
	//items.push_back({ NextId++, "Quarter Staff" ,1});
	//items.push_back({ NextId++, "Short Bow" ,1});
	//items.push_back({ NextId++, "Hand Crossbow" ,1 });
	//items.push_back({ NextId++, "Butcher's Knife" ,1 });
}
void bag::Additem()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	string name;
	int qty = 0;

	while (true) {
		cout << "Enter item name: ";
		getline(cin, name);

		if (!name.empty()) {
			break;
		}
		cout << "Name cannot be empty.\n";
	}

	while (true) {
		cout << "enter quantity (-1" << MAX_STACK << "): ";
		if (!(cin >> qty)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid number.\n";
			continue;
		}
		if (qty <= 0 || qty > MAX_STACK) {
			cout << "quantity must be between 1 and " << MAX_STACK << ".\n";
			continue;
		}
		break;
	}
	int resultId = AddItem(name, qty);
	if (resultId == -1)
	{
		std::cout << "Add failed (stack limit or invalid input).\n";
	}
	else
	{
		std::cout << "Added/updated item id: " << resultId << "\n";
	}
}
int bag::AddItem(const string& name, int quantitytoAdd)
{
	// core rules
	if (name.empty()) return -1;
	if (quantitytoAdd <= 0) return -1;
	if (quantitytoAdd > MAX_STACK) return -1;

	// stack if name already exists
	for (auto& item : items)
	{
		if (item.name == name)
		{
			if (item.quantity + quantitytoAdd > MAX_STACK)
				return -1;

			item.quantity += quantitytoAdd;
			return item.id;
		}
	}

	// otherwise create new item
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
	items.erase(items.begin() + static_cast<long long>(index));
	return true;
	
}
bool bag::RemoveOneByIndex(size_t index)
{

	if (index >= items.size())
		return false;

	if (items[index].quantity > 1)
	{
		items[index].quantity--;
		return true;
	}

	// quantity is 1 (or weirdly <= 0), remove the entry
	items.erase(items.begin() + static_cast<long long>(index));
	return true;
}
void bag::ViewItems()const {
	
	if (items.empty())
	{
		std::cout << "(empty)\n";
		return;
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		std::cout << i << ": " << items[i].name << " x" << items[i].quantity
				<< " (id " << items[i].id << ")\n";
	}
	
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

		catch (...) {
			continue; // bad numbers, skip line
		}

		item.name = nameStr;
		items.push_back(item);


		if (item.id > maxId) {
			maxId = item.id;
		}

	}
	// set NextId once, after reading everything
	NextId = (maxId >= 0) ? (maxId + 1) : 1;

		
	
	return true;
}

