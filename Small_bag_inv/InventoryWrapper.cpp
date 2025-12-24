#include "InventoryWrapper.h"
BagResult InventoryWrapper::RegisterDef(const ItemDef& def)
{
    return inv.RegisterItemDef(def);
}

InvOpResult InventoryWrapper::AddByDef(int defId, int qty)
{
    InvOpResult out{};
    out.result = inv.AddItemByDefId(defId, qty, out.itemId);
    return out;
}

BagResult InventoryWrapper::ConsumeByItemId(int itemId)
{
    return inv.ConsumeById(itemId);
}

BagResult InventoryWrapper::RemoveEntryByItemId(int itemId)
{
    return inv.RemoveEntryById(itemId);
}

BagResult InventoryWrapper::Save(const std::string& path) const
{
    return inv.SaveToFiles(path);
}

BagResult InventoryWrapper::Load(const std::string& path)
{
    return inv.LoadFromFiles(path);
}

std::vector<InvItemView> InventoryWrapper::GetItems() const
{
    std::vector<InvItemView> out;
    out.reserve(inv.GetItems().size());

    for (const auto& it : inv.GetItems())
    {
        InvItemView v{};
        v.id = it.id;
        v.name = it.name;
        v.quantity = it.quantity;
        v.type = it.type;
        v.rarity = it.rarity;
        v.value = it.value;
        out.push_back(std::move(v));
    }
    return out;
}

void InventoryWrapper::SetMaxSlots(size_t slots)
{
    inv.SetMaxSlots(slots);
}