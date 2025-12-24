
#include <iostream>
#include "bagv5.h"
#include "bag_Utilsv5.h"
#include <limits>




// reads the cin input1 safely
bool ReadIntSafe(int& out)
{
	if (!(std::cin >> out))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
	return true;
}
//display for item type.
std::string ItemTypeToString(ItemType type)
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
	bool running = true;
	const std::string kSaveFile = "Inventory.txt";
	

	do {

		std::cout << "Welcome to my BAG!" << std::endl;
		std::cout << "1. Add  an item" << std::endl;
		std::cout << "2. Remove an item" << std::endl;
		std::cout << "3. View all items" << std::endl;
		std::cout << "4. Sort Items by name" << std::endl;
		std::cout << "5. Search Items" << std::endl;
		std::cout << "6. Save inventory" << std::endl;
		std::cout << "7. Load inventory" << std::endl;
		std::cout << "8. quit" << std::endl;

		int input1;
		if (!ReadIntSafe(input1)) {
			std::cout << "Invalid input.\n";
			continue;
		}
		
		switch (input1) {
		case 1: {
			
			
			std::string name;
			std::cout << "Enter item name: ";
			std::getline(std::cin >> std::ws, name);

			int qty;
			std::cout << "Enter quantity: ";
			if (!ReadIntSafe(qty)) { 
				std::cout << "Invalid input.\n";
				break;
			}

			int typeChoice = 5;
			std::cout << "Type (1=Armor, 2=Consumable, 3=Material, 4=Quest, 5=Misc): ";
			if (!ReadIntSafe(typeChoice)) { 
				std::cout << "Invalid input.\n"; 
				break;
			}

			ItemType type = ItemType::Misc;
			switch (typeChoice) {
			case 1: type = ItemType::Armor;
				break;
			case 2: type = ItemType::Consumable;
				break;
			case 3: type = ItemType::Material; 
				break;
			case 4: type = ItemType::Quest;
				break;
			default: type = ItemType::Misc; 
				break;
			}

			int rarity = 0;
			std::cout << "Rarity (0-10, default 0): ";
			if (!ReadIntSafe(rarity)) { 
				rarity = 0;
			}

			int value = 0;
			std::cout << "Value (>=0, default 0): ";
			if (!ReadIntSafe(value)) { 
				value = 0; 
			}

			int id = -1;
			BagResult res = myBag.AddItem(name, qty, id, type, rarity, value);

			if (res == BagResult::Success) {
				std::cout << "Added/updated item id: " << id << "\n";
			}
			else if (res == BagResult::StackOverflow) {
				std::cout << "Too many. Would exceed max stack.\n";
			}
			else {
				std::cout << "Invalid input.\n";
			}

			break;
		}
		case 2: {
			if (myBag.isEmpty()) {
				std::cout << "Bag is empty" << std::endl;
				break;
			}
			int id;
			std::cout << "Enter item id to remove: ";
			if (!ReadIntSafe(id)) {
				std::cout << "Invalid input.\n";
				break;
			}
		

			BagResult r = myBag.RemoveOneById(id);
			if (r == BagResult::Success) {
				std::
					cout << "Quantity reduced by 1.\n";
			}
			else if (r == BagResult::NotFound) {
				std::cout << "Item not found.\n";
			}
			else {
				std::cout << "Remove failed.\n";

			}
			
			break;
		}
		case 3: {

			const auto& items = myBag.GetItems();
			if (items.empty()) {
				std::cout << "Bag is empty" << std::endl;
			}

			else {
				std::cout << "View Items" << std::endl;
				for (const auto& item : items)
				{
					std::cout << item.name
						<< " x" << item.quantity
						<< " [" << ItemTypeToString(item.type) << "]"
						<< " (rarity: " << item.rarity
						<< ", value: " << item.value << ")"
						<< " [id: " << item.id << "]\n";
				}
			}

			break;
		}
		case 4: {
			std::cout << "Items Sorted" << "\n";
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
			std::cout << "Search for: ";
			std::getline(std::cin >> std::ws, query);

			auto ids = myBag.FindIdsByName(query, true);
			if (ids.empty()) { 
				std::cout << "No matches.\n"; break; 
			}

			for (int id : ids)
			{
				const InventoryItem* it = myBag.FindById(id);
				if (!it) continue;

				std::cout << it->name
					<< " x" << it->quantity
					<< " [id: " << it->id << "]\n";
			}

			break;
		}
		case 6: {
			BagResult r = myBag.SaveToFiles("Inventory.txt");
			std::cout << "Save: " << ToString(r) << "\n";
			if (r == BagResult::Success) {
				std::cout << "Inventory saved.\n";
			}
			else {
				std::cout << "Failed to save inventory.\n";
			}
			break;
		}
		case 7: {
			BagResult r = myBag.LoadFromFiles("Inventory.txt");
			std::cout << "Load: " << ToString(r) << "\n";
			if (r == BagResult::Success) {
				std::cout << "Inventory loaded.\n";
			}
			else if (r == BagResult::ParseError) {
				std::cout << "Inventory loaded, but some lines were invalid and got skipped.\n";
			}
			else {
				std::cout << "Failed to load inventory.\n";
			}
			break;
		}
		case 8: {
			std::cout << "Thank you for visiting!" << std::endl;
			running = false;
			break;
		}
		default: {
			std::cout << "invalid options. Pick a new option:" << std::endl;
			break;
		}
		}
	}
	while (running);
	
	
};

