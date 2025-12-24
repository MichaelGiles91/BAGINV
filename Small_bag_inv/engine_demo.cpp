#include "InventoryWrapper.h"
#include "bag_Utilsv5.h"
#include <iostream>

int main()
{
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

    InventoryWrapper iw;

    std::cout << "Register potion: " << ToString(iw.RegisterDef(potion)) << "\n";
    std::cout << "Register helm: " << ToString(iw.RegisterDef(helm)) << "\n";

    auto r1 = iw.AddByDef(1, 5);
    std::cout << "Add potion: " << ToString(r1.result) << " id=" << r1.itemId << "\n";

    auto r2 = iw.AddByDef(2, 1);
    std::cout << "Add helm: " << ToString(r2.result) << " id=" << r2.itemId << "\n";

    std::cout << "Consume helm: " << ToString(iw.ConsumeByItemId(r2.itemId)) << "\n";

    std::cout << "Save: " << ToString(iw.Save("Inventory.txt")) << "\n";
    std::cout << "Load: " << ToString(iw.Load("Inventory.txt")) << "\n";

    std::cout << "Inventory:\n";
    for (const auto& item : iw.GetItems())
    {
        std::cout << "  " << item.id << " " << item.name
            << " x" << item.quantity
            << " type=" << ToString(item.type)
            << " rarity=" << item.rarity
            << " value=" << item.value
            << "\n";
    }

    return 0;
}






