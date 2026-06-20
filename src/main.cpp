#include <iostream>
#include "../include/page.h"
#include "../include/disk_manager.h"

int main() {
    DiskManager dm("test.db");

    Page write_page;
    write_page.header()->page_id = 1;
    write_page.header()->record_count = 5;

    dm.write_page(0, write_page);

    Page read_page;
    dm.read_page(0, read_page);

    std::cout << "page_id: " << read_page.header()->page_id << "\n";
    std::cout << "record_count: " << read_page.header()->record_count << "\n";

    return 0;
}
