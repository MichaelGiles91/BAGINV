
#include "bagv5.h"
#include "bag_Utilsv5.h"
#include <iostream>

static void PrintInventory(const bag& inv)
{
    std::cout << "Inventory:\n";
    for (const auto& it : inv.GetItems())
    {
        std::cout << "  " << it.id << " " << it.name
            << " x" << it.quantity
            << " type=" << ToString(it.type)
            << " rarity=" << it.rarity
            << " value=" << it.value
            << "\n";
    }
}
   

    int main()
    {
        bag inv;

        // ----- Item definitions (pretend this is your game database) -----
        ItemDef potion{};
        potion.id = 1;
        potion.name = "Health Potion";
        potion.type = ItemType::Consumable;
        potion.rarity = 1;
        potion.value = 25;
        potion.stackable = true;

        ItemDef helm{};
        helm.id = 2;
        helm.name = "Iron Helm";
        helm.type = ItemType::Armor;
        helm.rarity = 2;
        helm.value = 60;
        helm.stackable = false;

        std::cout << "Register potion: " << ToString(inv.RegisterItemDef(potion)) << "\n";
        std::cout << "Register helm: " << ToString(inv.RegisterItemDef(helm)) << "\n";
        std::cout << "Register potion again: " << ToString(inv.RegisterItemDef(potion)) << "\n";

        // ----- Adds using your current AddItem signature -----
        int potionId = -1;
        std::cout << "Add potion def x5: " << ToString(inv.AddItemByDefId(1, 5, potionId))
            << " id=" << potionId << "\n";

        int helmId = -1;
        std::cout << "Add helm def x1: " << ToString(inv.AddItemByDefId(2, 1, helmId))
            << " id=" << helmId << "\n";

        std::cout << "Consume helm by id: " << ToString(inv.ConsumeById(helmId)) << "\n";
        std::cout << "Consume helm again: " << ToString(inv.ConsumeById(helmId)) << "\n";
        PrintInventory(inv);

        int junkId = -1;
        std::cout << "Add missing def: " << ToString(inv.AddItemByDefId(999, 1, junkId)) << "\n";
        
        PrintInventory(inv);
        
        std::cout << "Save: " << ToString(inv.SaveToFiles("Inventory.txt")) << "\n";
        
        bag loaded;

        loaded.RegisterItemDef(potion);
        loaded.RegisterItemDef(helm);

        std::cout << "Load: " << ToString(loaded.LoadFromFiles("Inventory.txt")) << "\n";
        PrintInventory(loaded);
        return 0;
    }

