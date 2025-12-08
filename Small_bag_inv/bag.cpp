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
	if (index >= items.size()) {
		return false;

	}
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