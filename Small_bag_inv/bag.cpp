#include "bag.h"
#include <iostream>
bag::bag(): NextId(1) {

	//starting equipment
	items.push_back({ NextId++, "Rusty Sword" });
	items.push_back({ NextId++, "Quarter Staff" });
	items.push_back({ NextId++, "Short Bow" });
	items.push_back({ NextId++, "Hand Crossbow" });
	items.push_back({ NextId++, "Butcher's Knife" });
}
int bag::AddItem(const string& name)
{
	InventoryItem newItem;
	newItem.id = NextId++;
	newItem.name = name;

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