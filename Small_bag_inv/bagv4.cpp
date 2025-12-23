#include "bagv4.h"




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

	items.push_back(newItem);
	outItemId = newItem.id;
	return BagResult::Success;
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
		outFile
			<< item.id << ' '
			<< item.name << ' '
			<< item.quantity << ' '
			<< static_cast<int>(item.type) << ' '
			<< item.rarity << ' '
			<< item.value << '\n';
	}

	return BagResult::Success;
}

BagResult bag::LoadFromFiles(const std::string& filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open())
		return BagResult::FileOpenFailed;

	items.clear();
	int maxId = -1;

	std::string line;
	while (std::getline(inFile, line))
	{
		line = Trim(line);
		if (line.empty()) continue;

		size_t firstSpace = line.find(' ');
		if (firstSpace == std::string::npos) continue;

		std::string idStr = Trim(line.substr(0, firstSpace));
		std::string working = Trim(line.substr(firstSpace + 1));
		if (working.empty()) continue;

		auto popLastToken = [&](std::string& s) -> std::string
			{
				s = Trim(s);
				size_t p = s.rfind(' ');
				if (p == std::string::npos)
				{
					std::string t = s;
					s.clear();
					return t;
				}
				std::string t = Trim(s.substr(p + 1));
				s = Trim(s.substr(0, p));
				return t;
			};

		std::string t4 = popLastToken(working);
		std::string t3 = popLastToken(working);
		std::string t2 = popLastToken(working);
		std::string t1 = popLastToken(working);

		std::string nameStr;
		std::string qtyStr;
		std::string typeStr;
		std::string rarityStr;
		std::string valueStr;

		if (IsInteger(t1) && IsInteger(t2) && IsInteger(t3) && IsInteger(t4))
		{
			nameStr = working;
			qtyStr = t1;
			typeStr = t2;
			rarityStr = t3;
			valueStr = t4;
		}
		else if (IsInteger(t1) && IsInteger(t2))
		{
			nameStr = working;
			qtyStr = t1;
			typeStr = t2;
			rarityStr = "0";
			valueStr = "0";
		}
		else
		{
			continue;
		}

		InventoryItem item{};
		try
		{
			item.id = std::stoi(idStr);
			item.quantity = std::stoi(qtyStr);
			item.type = static_cast<ItemType>(std::stoi(typeStr));
			item.rarity = std::stoi(rarityStr);
			item.value = std::stoi(valueStr);
		}
		catch (...)
		{
			continue;
		}

		if (item.id < 0) continue;
		if (item.quantity <= 0 || item.quantity > MAX_STACK) continue;
		if (item.rarity < 0) item.rarity = 0;
		if (item.rarity > 10) item.rarity = 10;
		if (item.value < 0) item.value = 0;

		item.name = Trim(nameStr);
		if (item.name.empty()) continue;

		items.push_back(item);
		if (item.id > maxId) maxId = item.id;
	}

	NextId = (maxId >= 0) ? maxId + 1 : 1;
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
