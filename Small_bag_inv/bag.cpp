#include "bag.h"

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

BagResult bag::AddItem(const string& name, int quantitytoAdd, int& outItemId, ItemType type)
{
	std::string cleanName = Trim(name);

	if (cleanName.empty() || quantitytoAdd <= 0 || quantitytoAdd > MAX_STACK)
		return BagResult::InvalidInput;

	// If stackable, merge with existing item (same name, same type).
	if (isStackable(type))
	{
		for (auto& item : items)
		{
			if (item.type == type && NamesEqual(item.name, cleanName, true))
			{
				if (item.quantity + quantitytoAdd > MAX_STACK)
					return BagResult::StackOverflow;

				item.quantity += quantitytoAdd;
				outItemId = item.id;
				return BagResult::Success;
			}
		}
	}

	// Non-stackable (Armor/Quest) always creates a new entry,
	// even if the name matches.
	InventoryItem newItem;
	newItem.id = NextId++;
	newItem.name = cleanName;
	newItem.quantity = quantitytoAdd;
	newItem.type = type;

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
std::vector<size_t> bag::FindIndicesByName(const std::string& query, bool caseInsensitive) const
{
	std::vector<size_t> results;

	auto trimCopy = [](const std::string& s) -> std::string
		{
			size_t start = 0;
			while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
				++start;

			size_t end = s.size();
			while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
				--end;

			return s.substr(start, end - start);
		};

	auto lowerCopy = [](std::string s)
		{
			for (char& ch : s)
				ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
			return s;
		};

	std::string q = trimCopy(query);
	if (q.empty())
		return results;

	if (caseInsensitive)
		q = lowerCopy(q);

	for (size_t i = 0; i < items.size(); ++i)
	{
		std::string name = items[i].name;
		if (caseInsensitive)
			name = lowerCopy(name);

		if (name.find(q) != std::string::npos)
			results.push_back(i);
	}

	return results;
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
const vector<InventoryItem>& bag::GetItems()const {
	return items;
}
bool bag::SaveToFiles(const string& filename) const
{
	// reads file
	ofstream outFile(filename);
	if (!outFile.is_open())
	{
		return false;
	}
	// to be able to produce id, space ,name, space, quantity
	for (const auto& item : items) {

		outFile << item.id << ' '
			<< item.name << ' '
			<< item.quantity 
			<< static_cast<int>(item.type) << '\n';
	}
	return true;
}
bool bag::LoadFromFiles(const string& filename)
{
	// loads save file as long as it opens
	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		return false;
	}
	items.clear();
	int maxId = -1;

	string line;
	// parsing
	while (getline(inFile, line))
	{
		line = Trim(line);
		if (line.empty()) continue;

		size_t firstSpace = line.find(' ');
		if (firstSpace == string::npos) continue;

		// Try to parse 4-field format: id [name...] qty type
		size_t lastSpace = line.rfind(' ');
		if (lastSpace == string::npos || lastSpace <= firstSpace) continue;

		size_t secondLastSpace = line.rfind(' ', lastSpace - 1);

		string idStr = Trim(line.substr(0, firstSpace));
		string typeStr;
		string qtyStr;
		string nameStr;

		string lastTok = Trim(line.substr(lastSpace + 1)); // could be qty (old) or type (new)
		bool hasType = false;

		if (secondLastSpace != string::npos && secondLastSpace > firstSpace)
		{
			string secondLastTok = Trim(line.substr(secondLastSpace + 1, lastSpace - secondLastSpace - 1)); // could be qty (new)

			// New format only if BOTH last and second-last tokens are integers
			if (IsInteger(secondLastTok) && IsInteger(lastTok))
			{
				hasType = true;
				nameStr = Trim(line.substr(firstSpace + 1, secondLastSpace - firstSpace - 1));
				qtyStr = secondLastTok;
				typeStr = lastTok;
			}
		}

		if (!hasType)
		{
			// Old format: id [name...] qty
			nameStr = Trim(line.substr(firstSpace + 1, lastSpace - firstSpace - 1));
			qtyStr = lastTok;
			typeStr.clear();
		}
		else
		{
			// Fallback old format: id [name...] qty
			nameStr = Trim(line.substr(firstSpace + 1, lastSpace - firstSpace - 1));
			qtyStr = Trim(line.substr(lastSpace + 1));
			typeStr = "";
		}

		if (nameStr.empty()) continue;

		InventoryItem item;
		int typeInt = static_cast<int>(ItemType::Misc);

		try
		{
			item.id = stoi(idStr);
			item.quantity = stoi(qtyStr);

			if (!typeStr.empty())
				typeInt = stoi(typeStr);
		}
		catch (...)
		{
			continue;
		}

		if (item.id < 0) continue;
		if (item.quantity <= 0 || item.quantity > MAX_STACK) continue;

		// Clamp/validate type
		if (typeInt < 0 || typeInt > static_cast<int>(ItemType::Misc))
			typeInt = static_cast<int>(ItemType::Misc);

		item.name = nameStr;
		item.type = static_cast<ItemType>(typeInt);

		items.push_back(item);

		if (item.id > maxId) maxId = item.id;
	}
	// set NextId once, after reading everything
	NextId = (maxId >= 0) ? (maxId + 1) : 1;

		
	
	return true;
}

