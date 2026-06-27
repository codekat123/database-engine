#include <iostream>
#include "../include/disk_manager.h"
#include "../include/buffer_pool_manager.h"
#include "../include/heap_file.h"

int main() {
    DiskManager dm("test.db");
    BufferPoolManager bpm(4, dm);
    HeapFile hf(bpm);

    const char* record = "hello from heapfile";
    RID rid = hf.insert_record(record, 19);
    std::cout << "inserted at page: " << rid.page_id
              << " slot: " << rid.slot_id << "\n";

    char buffer[64];
    uint16_t length = 0;
    bool ok = hf.get_record(rid, buffer, length);
    std::cout << "retrieved: " << std::string(buffer, length) << "\n";
    std::cout << "success: " << ok << "\n";

    return 0;
}
