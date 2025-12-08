
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
		cout << "1. add item" << endl;
		cout << "2. remove an item" << endl;
		cout << "3. view all items" << endl;
		cout << "4.quit bitch" << endl;

		int input1;
		std::cin >> input1;
		switch (input1) {
		case 1: {

			cout << "Enter item name: " << endl;
			getline(cin >> ws, name);

			myBag.AddItem(name);
			cout << "Added: " << name << endl;
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
				cout << (i + 1) << ") " << items[i].name << endl;

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
				cout << "Something w" << endl;
			}
			break;
		}
		case 3: {
			cout << "VIEW SHIT" << endl;
			const auto& items = myBag.GetItems();
			if (items.empty()) {
				cout << "Bag is empty" << endl;
			}
			else {
				for (size_t i = 0; i < items.size(); i++)
				{
					cout << (i + 1) << ") " << items[i].name << endl;
				}
			}

			break;
		}
		case 4: {
			cout << "WELL THEN GOODBYE BITCH" << endl;
			running = false;
			break;
		}
		default: {
			cout << "invalid options fuck face get it RIGHT!" << endl;
			break;
		}

		}
	} while (running);


};
