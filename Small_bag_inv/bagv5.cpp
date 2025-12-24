#include "bagv5.h"




static size_t FindIndexByIdInternal(const std::vector<InventoryItem>& items, int id)
{
	for (size_t i = 0; i < items.size(); ++i)
	{
		if (items[i].id == id) return i;
	}
	return static_cast<size_t>(-1);
}
// for parsing
static bool IsInteger(const std::string& s)
{
	if (s.empty()) return false;
	size_t i = 0;
	if (s[0] == '-' || s[0] == '+') i = 1;
	if (i >= s.size()) return false;
	for (; i < s.size(); ++i)
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	return true;
}


bag::bag(): NextId(1) {

	//starting equipment
	//items.push_back({ NextId++, "Rusty Sword",1 });
	//items.push_back({ NextId++, "Quarter Staff" ,1});
	//items.push_back({ NextId++, "Short Bow" ,1});
	//items.push_back({ NextId++, "Hand Crossbow" ,1 });
	//items.push_back({ NextId++, "Butcher's Knife" ,1 });
}

BagResult bag::AddItem(const std::string& name, int quantityToAdd, int& outItemId, ItemType type, int rarity, int value)
{

	outItemId = -1;

	std::string cleanName = Trim(name);
	if (cleanName.empty()) return BagResult::InvalidInput;

	if (quantityToAdd <= 0) return BagResult::InvalidInput;
	if (type == ItemType::Armor && quantityToAdd != 1)
	{
		return BagResult::NonStackable;
	}
	if (quantityToAdd > MAX_STACK) return BagResult::StackOverflow;

	// clamp metadata
	if (rarity < 0) rarity = 0;
	if (rarity > 10) rarity = 10;
	if (value < 0) value = 0;

	const bool stackable = isStackable(type);

	// non-stackable items should be exactly 1 per entry
	if (!stackable && quantityToAdd != 1)
		return BagResult::InvalidInput;

	// merge only when identity matches
	if (stackable)
	{
		for (auto& item : items)
		{
			if (item.type == type &&
				item.rarity == rarity &&
				item.value == value &&
				NamesEqual(item.name, cleanName, true))
			{
				if (item.quantity + quantityToAdd > MAX_STACK)
					return BagResult::StackOverflow;

				item.quantity += quantityToAdd;
				outItemId = item.id;
				return BagResult::Success;
			}
		}
	}

	InventoryItem newItem;
	newItem.id = NextId++;
	newItem.name = cleanName;
	newItem.quantity = quantityToAdd;
	newItem.type = type;
	newItem.rarity = rarity;
	newItem.value = value;
	if (items.size() >= maxSlots)
		return BagResult::BagFull;

	items.push_back(newItem);
	outItemId = newItem.id;
	return BagResult::Success;
}
BagResult bag::AddItemByDefId(int defId, int quantityToAdd, int& outItemId)
{
	auto it = defs.find(defId);
	if (it == defs.end()) {
		return BagResult::NotFound; // or InvalidInput if you prefer
	}
	const ItemDef& d = it->second;
	return AddItem(d.name, quantityToAdd, outItemId, d.type, d.rarity, d.value);

}
BagResult bag::ConsumeById(int id)
{
	return RemoveOneById(id);
}
BagResult bag::RemoveEntryById(int id)
{
	return RemoveItemById(id);
}
BagResult bag::RemoveItemByIndex(size_t index)
{
	
	if (index >= items.size())
		return BagResult::NotFound;

	items.erase(items.begin() + index);
	return BagResult::Success;
	

	
}
BagResult bag::RemoveOneByIndex(size_t index)
{
	if (index >= items.size())
		return BagResult::NotFound;

	// if quantity is already weird, treat as remove entry
	if (items[index].quantity <= 1)
	{
		items.erase(items.begin() + index);

		return BagResult::Success;
	}

	items[index].quantity -= 1;
	return BagResult::Success;
}
bool bag::isEmpty()const {
	return items.empty();

}
size_t bag::GetSize() const {
	return items.size();
}
std::vector<InventoryItem> bag::GetItemsSortedByName(bool ascending, bool caseInsensitive) const
{
	{
		std::vector<InventoryItem> copy = items;

		auto lowerCopy = [](std::string s)
			{
				for (char& ch : s)
					ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
				return s;
			};

		std::sort(copy.begin(), copy.end(),
			[&](const InventoryItem& a, const InventoryItem& b)
			{
				std::string an = caseInsensitive ? lowerCopy(a.name) : a.name;
				std::string bn = caseInsensitive ? lowerCopy(b.name) : b.name;

				if (ascending) return an < bn;
				return an > bn;
			});

		return copy;
	}

}
std::vector<InventoryItem> bag::GetItemsSortedByQuantity(bool ascending) const
{

	std::vector<InventoryItem> copy = items;
	std::sort(copy.begin(), copy.end(),
		[&](const InventoryItem& a, const InventoryItem& b)
		{
			if (ascending) return a.quantity < b.quantity;
			return a.quantity > b.quantity;
		});

	return copy;
}
std::vector<InventoryItem> bag::GetItemsSortedById(bool ascending) const
{

	std::vector<InventoryItem> copy = items;

	std::sort(copy.begin(), copy.end(),
		[&](const InventoryItem& a, const InventoryItem& b)
		{
			if (ascending) return a.id < b.id;
			return a.id > b.id;
		});

	return copy;
}

void bag::SetMaxSlots(size_t slots)
{
	maxSlots = slots;
}

size_t bag::GetMaxSlots() const
{
	return maxSlots;
}

BagResult bag::RegisterItemDef(const ItemDef& def)
{
	if (def.id < 0) return BagResult::InvalidInput;
	if (def.name.empty()) return BagResult::InvalidInput;

	// prevent duplicates unless you want overwrite behavior
	if (defs.find(def.id) != defs.end())
		return BagResult::AlreadyExists; // if you have this, otherwise InvalidInput

	defs.emplace(def.id, def);
	return BagResult::Success;
}

const ItemDef* bag::GetItemDef(int id) const
{
	auto it = defs.find(id);
	if (it == defs.end())
	{ 
		return nullptr;
	}
	return &it->second;
}

std::string bag::Trim(const std::string& s)
{
	size_t start = 0;
	while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
		++start;
	}
	size_t end = s.size();
	while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
		--end;
	}
	return s.substr(start, end - start);
}
std::string bag::Tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) {return static_cast<char>(std::tolower(c)); });

	return s;
}
bool bag::NamesEqual(const std::string& a, const std::string& b, bool caseInsensitive) {
	if (!caseInsensitive) {
		return a == b;
		
	}
	return Tolower(a) == Tolower(b);
}
bool bag::isStackable(ItemType type)
{
	switch (type)
	{
	case ItemType::Consumable:
	case ItemType::Material:
	case ItemType::Misc:
		return true;
	case ItemType::Armor:
	case ItemType::Quest:
	default:
		return false;
	}
	
}
const std::vector<InventoryItem>& bag::GetItems()const {
	return items;
}
BagResult bag::SaveToFiles(const std::string& filename) const
{
	std::ofstream outFile(filename);
	if (!outFile.is_open())
		return BagResult::FileOpenFailed;

	for (const auto& item : items)
	{
		outFile << item.id << " " << item.quantity << "\n";
	}

	return BagResult::Success;
}

BagResult bag::LoadFromFiles(const std::string& filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open())
		return BagResult::FileOpenFailed;

	items.clear();

	int defId = 0;
	int qty = 0;

	while (inFile >> defId >> qty)
	{
		int outId = -1;
		BagResult r = AddItemByDefId(defId, qty, outId);
		if (r != BagResult::Success)
			return r;
	}
	return BagResult::Success;
}
const InventoryItem* bag::FindById(int id) const
{
	size_t idx = FindIndexByIdInternal(items, id);
	if (idx == static_cast<size_t>(-1)) return nullptr;
	return &items[idx];
}

std::vector<int> bag::FindIdsByName(const std::string& query, bool caseInsensitive) const
{
	std::vector<int> ids;

	std::string q = Trim(query);
	if (q.empty()) return ids;

	if (caseInsensitive) q = Tolower(q);

	for (const auto& item : items)
	{
		std::string n = item.name;
		if (caseInsensitive) n = Tolower(n);

		if (n.find(q) != std::string::npos)
			ids.push_back(item.id);
	}
	return ids;
}

BagResult bag::RemoveItemById(int id)
{
	size_t idx = FindIndexByIdInternal(items, id);
	if (idx == static_cast<size_t>(-1)) return BagResult::NotFound;
	return RemoveItemByIndex(idx);
}

BagResult bag::RemoveOneById(int id)
{
	size_t idx = FindIndexByIdInternal(items, id);
	if (idx == static_cast<size_t>(-1)) return BagResult::NotFound;
	return RemoveOneByIndex(idx);
}

BagResult bag::UpdateQuantityById(int id, int newQty)
{
	if (newQty < 0) {
		return BagResult::InvalidInput;
	}
	size_t idx = FindIndexByIdInternal(items, id);
	if (idx == static_cast<size_t>(-1)) return BagResult::NotFound;

	// zero means remove the entry (your chosen behavior)
	if (newQty == 0)
	{
		items.erase(items.begin() + idx);
		return BagResult::Success;
	}

	if (newQty > MAX_STACK) {
		return BagResult::StackOverflow;
	}
	// enforce non-stackable rule: must be exactly 1
	if (!isStackable(items[idx].type) && newQty != 1) {
		return BagResult::InvalidInput;
	}
	items[idx].quantity = newQty;
	return BagResult::Success;
}
