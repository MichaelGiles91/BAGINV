
#include "bagv4.h"
#include "bag_Utils.h"

#include <iostream>

int main()
{
    bag inv;

    int newId = -1;

    // Add a few items
    {
        BagResult r = inv.AddItem("Health Potion", 5, newId, ItemType::Consumable, 1, 10);
        std::cout << "AddItem: " << ToString(r) << " id=" << newId << "\n";
    }
    {
        BagResult r = inv.AddItem("Iron Helm", 1, newId, ItemType::Armor, 2, 120);
        std::cout << "AddItem: " << ToString(r) << " id=" << newId << "\n";
    }

    // Save
    {
        BagResult r = inv.SaveToFiles("Inventory.txt");
        std::cout << "Save: " << ToString(r) << "\n";
    }

    // Load into a fresh bag
    bag loaded;
    {
        BagResult r = loaded.LoadFromFiles("Inventory.txt");
        std::cout << "Load: " << ToString(r) << "\n";
    }

    // Print results
    std::cout << "\nLoaded items:\n";
    for (const auto& it : loaded.GetItems())
    {
        std::cout << it.id << " " << it.name << " x" << it.quantity << " type=" << (int)it.type << "\n";
    }

    return 0;
}