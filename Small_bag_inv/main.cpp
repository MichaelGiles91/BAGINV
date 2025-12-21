
#include <iostream>
#include "bag.h"
#include <limits>


using namespace std;

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
			const auto& items = myBag.GetItems();
			for (int i = 0; i < items.size(); i++)
			{
				cout << (i + 1) << ") " << items[i].name << " x" << items[i].quantity << endl;

			}
			cout << "Enter index to remove one from the stack: " << endl;
			std::cin >> chosenIndex;
			int zeroBase = chosenIndex - 1;

			if (zeroBase < 0 || zeroBase >= items.size()) {
				cout << "Invalid index" << endl;
				break;
			}
			size_t ChosenConIndex = static_cast<size_t>(zeroBase);

			BagResult r = myBag.RemoveOneByIndex(ChosenConIndex);

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
				for (size_t i = 0; i < items.size(); i++)
				{
					cout << (i + 1) << ") " << items[i].name << " x" << items[i].quantity << '\n';
				}
			}

			break;
		}
		case 4: {
			cout << "Items Sorted" << "\n";
			auto sorted = myBag.GetItemsSortedByName(true);
			for (size_t i = 0; i < sorted.size(); ++i)
			{
				std::cout << i << ") " << sorted[i].name
					<< " x" << sorted[i].quantity
					<< " (id " << sorted[i].id << ")\n";
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
					cout << items[idx].name << " x" << items[idx].quantity
						<< " (id " << items[idx].id << ")\n";
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
