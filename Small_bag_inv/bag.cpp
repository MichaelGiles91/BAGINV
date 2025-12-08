#include "bag.h"
#include <iostream>
bag::bag(): NextId(1) {

	//starting equipment
	items.push_back({ NextId++, "Rusty Sword",1 });
	items.push_back({ NextId++, "Quarter Staff" ,1});
	items.push_back({ NextId++, "Short Bow" ,1});
	items.push_back({ NextId++, "Hand Crossbow" ,1 });
	items.push_back({ NextId++, "Butcher's Knife" ,1 });
}
int bag::AddItem(const string& name, int quantity)
{
	InventoryItem newItem;
	newItem.id = NextId++;
	newItem.name = name;
	newItem.quantity = quantity;

	items.push_back(newItem);
	return newItem.id;
}


bool bag::RemoveItemByIndex(size_t index)
{
	if (index >= items.size()) {
		return false;

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