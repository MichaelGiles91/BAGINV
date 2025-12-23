#pragma once
#include "bagv4.h"


inline const char* ToString(BagResult r)
{
    switch (r)
    {
    case BagResult::Success: return "Success";
    case BagResult::InvalidInput: return "InvalidInput";
    case BagResult::NotFound: return "NotFound";
    case BagResult::StackOverflow: return "StackOverflow";
    case BagResult::FileOpenFailed: return "FileOpenFailed";
    case BagResult::ParseError: return "ParseError";
    default: return "Unknown";
    }
}