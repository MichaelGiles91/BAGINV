#include "bag.h"
bag::bag() {

}
void bag::AddItem()
{
	cout << " add a item to the bag: " << endl;
	getline(cin >> ws, userinput);
	items.push_back(userinput);
}

void bag::RemoveItem()
{
	if (items.empty()) {
		cout << "Bag is Empty" << endl;
		return;
	}
	for (size_t i = 0; i < items.size(); i++)
	{
		cout << (i + 1) << ". " << items[i] << endl;

	}
	cout << "What Item do you want removed: " << endl;
	cin >> NumChoice;
	index = NumChoice - 1;
	if (index >= 0 && index < static_cast<int>(items.size())) {
		items.erase(items.begin() + index);
	}
	else {
		cout << "Error" << endl;
	}
}

void bag::ViewItems()
{
	if (items.empty()) {
		cout << " Bag is empty" <<endl;
		return;
	}
	for (int i = 0; i < items.size(); i++) {
		cout << items[i] << endl;
	}
}

bool bag::QuitProgram()
{
	quit = true;
	return quit;
}
