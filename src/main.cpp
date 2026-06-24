#include <iostream>
#include "../include/disk_manager.h"
#include "../include/buffer_pool_manager.h"

int main() {
    DiskManager dm("test.db");
    BufferPoolManager bpm(3, dm);

    int32_t page_id;
    Page* page = bpm.new_page(page_id);
    std::cout << "new page id: " << page_id << "\n";

    page->header()->record_count = 42;
    bpm.unpin_page(page_id, true);

    Page* fetched = bpm.fetch_page(page_id);
    std::cout << "record_count: " << fetched->header()->record_count << "\n";
    bpm.unpin_page(page_id, false);

    return 0;
}
