#pragma once
#include <string>
#include <vector>
#include "bagv5.h"

struct InvOpResult
{
    BagResult result = BagResult::Unknown;
    int itemId = -1;          // for adds
};

struct InvItemView
{
    int id = -1;
    std::string name;
    int quantity = 0;
    ItemType type = ItemType::Misc;
    int rarity = 0;
    int value = 0;
};

class InventoryWrapper
{
public:
    // Database
    BagResult RegisterDef(const ItemDef& def);

    // Gameplay
    InvOpResult AddByDef(int defId, int qty);
    BagResult ConsumeByItemId(int itemId);
    BagResult RemoveEntryByItemId(int itemId);

    // Save/Load (caller supplies path)
    BagResult Save(const std::string& path) const;
    BagResult Load(const std::string& path);

    // UI readback
    std::vector<InvItemView> GetItems() const;

    // Config
    void SetMaxSlots(size_t slots);

private:
    bag inv;
};