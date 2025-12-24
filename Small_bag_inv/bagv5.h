#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <unordered_map>






enum class ItemType
{
	Armor,
	Consumable,
	Material,
	Quest,
	Misc
};
enum class BagResult
{
	Success,
	InvalidInput,
	NotFound,
	StackOverflow,
	FileOpenFailed,
	ParseError,
	BagFull,
	NonStackable,
	AlreadyExists
};
struct InventoryItem {
	int id = -1; // unique ids assigned by inv
	std::string name; // display item name
	int quantity = 0; // displays the number of one unique item
	ItemType type = ItemType::Misc;// displays item type
	int value = 0;
	int rarity = 0;
};
struct ItemDef {
	int id = -1;
	std::string name;
	ItemType type;
	int rarity = 0;
	int value = 0;
	bool stackable = true;
};


class bag
{
public:
	


	bag();


	//adds item by name and returns IDs
	BagResult AddItem(const std::string& name, int quantityToAdd, int& outItemId, ItemType type = ItemType::Misc, int rarity = 0, int value = 0);
	BagResult AddItemByDefId(int defId, int quantityToAdd, int& outItemId);

	BagResult ConsumeById(int id);
	BagResult RemoveEntryById(int id);
	
	BagResult RemoveItemById(int id);
	BagResult RemoveOneById(int id);
	// Removes Items at the given index (0 based) returns true if removed.
	BagResult RemoveItemByIndex(std::size_t index);
	BagResult RemoveOneByIndex(std::size_t index);
	BagResult UpdateQuantityById(int id, int newQty);

	const InventoryItem* FindById(int id) const;
	std::vector<int> FindIdsByName(const std::string& query, bool caseInsensitive = true) const;

	

	//read only acces for items
	const std::vector<InventoryItem>& GetItems()const;


	// save/load
	BagResult SaveToFiles(const std::string& filename) const;
	BagResult LoadFromFiles(const std::string& filename);

	// helpers
	bool isEmpty()const;
	std::size_t GetSize()const;

	static constexpr int MAX_STACK = 999;

	//sorting
	std::vector<InventoryItem> GetItemsSortedByName(bool ascending = true, bool caseInsensitive = true) const;
	std::vector<InventoryItem> GetItemsSortedByQuantity(bool ascending = true) const;
	std::vector<InventoryItem> GetItemsSortedById(bool ascending = true) const;
	

	// max slots
	void SetMaxSlots(size_t slots);
	size_t GetMaxSlots() const;

	// Register each item with unordered map Def
	BagResult RegisterItemDef(const ItemDef& def);
	const ItemDef* GetItemDef(int id) const;
private:
	static std::string Trim(const std::string& s);
	static std::string Tolower(std::string s);
	bool NamesEqual(const std::string& a, const std::string& b, bool caseInsensitive);
	static bool isStackable(ItemType type);
	
	std::vector<InventoryItem> items;
	int NextId;

	size_t maxSlots = 30; 
	std::unordered_map<int, ItemDef> defs;
	
};

