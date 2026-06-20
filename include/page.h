#pragma once

#include <cstdint>
#include <cstring>

struct PageHeader {
    std::uint32_t page_id;
    std::uint32_t record_count;
    std::uint32_t free_space_offset;
};

class Page {
public:
    static constexpr int PAGE_SIZE = 4096;

    Page() {
        std::memset(data_, 0, PAGE_SIZE);
        header()->free_space_offset = sizeof(PageHeader);
    }

    PageHeader* header() {
        return reinterpret_cast<PageHeader*>(data_);
    }

private:
    char data_[PAGE_SIZE];
};
