#pragma once


#include "page.h"
#include "rid.h"
#include <cstdint>
#include <cstring>



struct Slot {
    std::uint16_t offset;
    std::uint16_t length;
};

class HeapPage {
public:
    explicit HeapPage(Page& page);

    int32_t insert_record(const char* data, uint16_t length);
    const char* get_record(uint16_t slot_id, uint16_t& length) const;
    uint16_t free_space() const;

private:
    Page& page_;
};
