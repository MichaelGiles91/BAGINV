#pragma once
#include "bagv5.h"


inline const char* ToString(BagResult r)
{
    //error message 
    switch (r)
    {
    case BagResult::Success: return "Success";
    case BagResult::InvalidInput: return "InvalidInput";
    case BagResult::NotFound: return "NotFound";
    case BagResult::StackOverflow: return "StackOverflow";
    case BagResult::FileOpenFailed: return "FileOpenFailed";
    case BagResult::ParseError: return "ParseError";
    case BagResult::BagFull: return "Bag is full";
    case BagResult::NonStackable: return "Item is not Stackable";
    case BagResult::AlreadyExists: return "This item already exists";
    case BagResult::Unknown: return "Unknown";
    default: return "Unknown";
    }
}

//item types
inline const char* ToString(ItemType t)
{
    switch (t)
    {
    case ItemType::Armor:      return "Armor";
    case ItemType::Consumable: return "Consumable";
    case ItemType::Material:   return "Material";
    case ItemType::Quest:      return "Quest";
    case ItemType::Misc:       return "Misc";
    default:                   return "UnknownType";
    }
}