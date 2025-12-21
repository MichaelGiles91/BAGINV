#pragma once
#include <vector>
#include <string>
#include "fstream"
#include "sstream"
#include <algorithm>
#include <cctype>




using namespace std;

enum class ItemType
{
	Armor,
	Consumable,
	Material,
	Quest,
	Misc
};
struct InventoryItem {
	int id = -1; // unique ids assigned by inv
	string name; // display item name
	int quantity = 0; // displays the number of one unique item
	ItemType type = ItemType::Misc;
};
enum class BagResult
{
	Success,
	InvalidInput,
	NotFound,
	StackOverflow
};

class bag
{
public:
	


	bag();


	//adds item by name and returns IDs
	BagResult AddItem(const std::string& name, int quantityToAdd, int& outItemId, ItemType type = ItemType::Misc);

	// Removes Items at the given index (0 based) returns true if removed.
	BagResult RemoveItemByIndex(size_t index);
	BagResult RemoveOneByIndex(size_t index);
	

	//read only acces for items
	const vector<InventoryItem>& GetItems()const;

	bool SaveToFiles(const string& filename) const;
	bool LoadFromFiles(const string& filename);

	// helpers
	bool isEmpty()const;
	size_t GetSize()const;

	static constexpr int MAX_STACK = 999;

	//sorting
	std::vector<InventoryItem> GetItemsSortedByName(bool ascending = true, bool caseInsensitive = true) const;
	std::vector<InventoryItem> GetItemsSortedByQuantity(bool ascending = true) const;
	std::vector<InventoryItem> GetItemsSortedById(bool ascending = true) const;
	std::vector<size_t> FindIndicesByName(const std::string& query, bool caseInsensitive = true) const;
private:
	static std::string Trim(const std::string& s);
	static std::string Tolower(std::string s);
	bool NamesEqual(const std::string& a, const std::string& b, bool caseInsensitive);
	static bool isStackable(ItemType type);
	
	vector<InventoryItem> items;
	int NextId;
	
};

