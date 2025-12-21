
#include <iostream>
#include "bag.h"
#include <limits>


using namespace std;
//display for item type.
string ItemTypeToString(ItemType type)
{
	switch (type)
	{
	case ItemType::Armor: return "Armor";
	case ItemType::Consumable: return "Consumable";
	case ItemType::Material: return "Material";
	case ItemType::Quest: return "Quest";
	default: return "Misc";
	}
}

// need explaing on what this does, 
size_t FindIndexById(const bag& b, int id)
{
	const auto& items = b.GetItems();
	for (size_t i = 0; i < items.size(); ++i)
	{
		if (items[i].id == id)
			return i;
	}
	return static_cast<size_t>(-1);

}
int main() {

	bag myBag;
	string name;
	bool running = true;
	int chosenIndex = 0;

	do {

		cout << "Welcome to my BAG!" << endl;
		cout << "1. Add  an item" << endl;
		cout << "2. Remove an item" << endl;
		cout << "3. View all items" << endl;
		cout << "4. Sort Items by name" << endl;
		cout << "5. Search Items" << endl;
		cout << "6. Save inventory" << endl;
		cout << "7. Load inventory" << endl;
		cout << "8. quit" << endl;

		int input1;
		std::cin >> input1;
		switch (input1) {
		case 1: {

			std::string name;
			cout << "Enter item name: ";
			getline(cin >> ws, name);

			int qty = 0;
			cout << "Enter the quantity: ";
			cin >> qty;

			int id = -1;
			BagResult result = myBag.AddItem(name, qty, id);
			if (result == BagResult::Success)
			{
				cout << "Added/updated item id: " << id << "\n";
			}
			else if (result == BagResult::StackOverflow)
			{
				cout << "Too many. Would exceed max stack.\n";
			}
			else
			{
				cout << "Invalid name or quantity.\n";
			}
			break;
		}
		case 2: {
			if (myBag.isEmpty()) {
				cout << "Bag is empty" << endl;
				break;
			}
			int id;
			cout << "Enter item id to remove: ";
			cin >> id;

			size_t idx = FindIndexById(myBag, id);
			if (idx == static_cast<size_t>(-1))
			{
				cout << "Item not found.\n";
				break;
			}


			BagResult r = myBag.RemoveOneByIndex(idx);

			if (r == BagResult::Success) {
				cout << "Quantity reduced by 1" << endl;
			}
			else {
				cout << "Something went wrong!" << endl;
			}
			break;
		}
		case 3: {

			const auto& items = myBag.GetItems();
			if (items.empty()) {
				cout << "Bag is empty" << endl;
			}

			else {
				cout << "View Items" << endl;
				for (const auto& item : items)
				{
					cout << item.name << " x" << item.quantity << "["
						 << ItemTypeToString(item.type) << "]"
						 << " [id: " << item.id << "]\n";
				}
			}

			break;
		}
		case 4: {
			cout << "Items Sorted" << "\n";
			auto sorted = myBag.GetItemsSortedByName(true);
			for (size_t i = 0; i < sorted.size(); ++i)
			{
				std::cout << sorted[i].name
					<< " x" << sorted[i].quantity
					<< "[" << ItemTypeToString(sorted[i].type) << "]"
					<< " [id " << sorted[i].id << "]\n";
			}
			break;
		}
		case 5: {
			std::string query;
			cout << "Search for: ";
			getline(cin >> ws, query);

			auto matches = myBag.FindIndicesByName(query, true);
			const auto& items = myBag.GetItems();

			if (matches.empty())
			{
				cout << "No matches.\n";
			}
			else
			{
				for (size_t idx : matches)
				{
					cout << items[idx].name << " x" << items[idx].quantity << "[" << 
						ItemTypeToString(items[idx].type) << "]"
						<< " [id " << items[idx].id << "]\n";
				}
			}
			break;
		}
		case 6: {
			if (myBag.SaveToFiles("Inventory.txt")) {
				cout << "Inventory saved.\n";

			}
			else {
				cout << "Failed to save inventory.\n";
			}
			break;
		}
		case 7: {
			if (myBag.LoadFromFiles("Inventory.txt")) {
				cout << "Inventory loaded.\n";

			}
			else {
				cout << "Failed to load inventory.\n";
			}
			break;
		}
		case 8: {
			cout << "Thank you for visiting!" << endl;
			running = false;
			break;
		}
		default: {
			cout << "invalid options. Pick a new option:" << endl;
			break;
		}
		}
	}
	while (running);
	
	
};

