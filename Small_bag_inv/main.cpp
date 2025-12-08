
#include <iostream>
#include "bag.h"


using namespace std;

int main() {

	bag myBag;
	string name;
	bool running = true;
	int chosenIndex = 0;

	do {

		cout << "Welcome to my BAG! wanna see some shit?" << endl;
		cout << "1. Add  an item" << endl;
		cout << "2. Remove an item" << endl;
		cout << "3. View all items" << endl;
		cout << "4.quit" << endl;

		int input1;
		std::cin >> input1;
		switch (input1) {
		case 1: {

			cout << "Enter item name: ";
			getline(cin >> ws, name);

			cout << "Enter the quantity: ";
			int qty;
			cin >> qty;

			myBag.AddItem(name, qty);
			cout << "Added: " << qty <<" " << name << endl;
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
				cout << (i + 1) << ") " << items[i].name << " x"<< items[i].quantity << endl;

			}
			cout << "Enter index to remove: " << endl;
			std::cin >> chosenIndex;
			int zeroBase = chosenIndex - 1;

			if (zeroBase < 0 || zeroBase >= items.size()) {
				cout << "Invalid index" << endl;
				break;
			}
			size_t ChosenConIndex = static_cast<size_t>(zeroBase);

			bool removed = myBag.RemoveItemByIndex(ChosenConIndex);

			if (removed = true) {
				cout << "Item succesfully removed" << endl;
			}
			else {
				cout << "Something went wrong!" << endl;
			}
			break;
		}
		case 3: {
			cout << "View Items" << endl;
			const auto& items = myBag.GetItems();
			if (items.empty()) {
				cout << "Bag is empty" << endl;
			}
			else {
				for (size_t i = 0; i < items.size(); i++)
				{
					cout << (i + 1) << ") " << items[i].name << " x" << items[i].quantity << endl;
				}
			}

			break;
		}
		case 4: {
			cout << "Thank you for visiting!" << endl;
			running = false;
			break;
		}
		default: {
			cout << "invalid options. Pick a new option:" << endl;
			break;
		}

		}
	} while (running);


};
