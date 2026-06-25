#pragma once 

#include <cstdint>

struct RID {
    std::uint32_t page_id;
    std::uint32_t slot_id;
};
