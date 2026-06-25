#include <iostream>
#include "../include/page.h"
#include "../include/heap_page.h"

int main() {
    Page page;
    HeapPage hp(page);

    const char* record = "hello";
    int32_t slot_id = hp.insert_record(record, 5);
    std::cout << "inserted at slot: " << slot_id << "\n";

    uint16_t len = 0;
    const char* data = hp.get_record(slot_id, len);
    std::cout << "retrieved: " << std::string(data, len) << "\n";
    std::cout << "free space: " << hp.free_space() << "\n";

    return 0;
}
