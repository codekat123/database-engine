#include "../include/disk_manager.h"
#include <iostream>


DiskManager::DiskManager(const std::string& db_file) {
    file_.open(db_file, std::ios::in | std::ios::out | std::ios::binary);

    if (!file_.is_open()) {
        file_.open(db_file, std::ios::out | std::ios::binary);
        file_.close();
        file_.open(db_file, std::ios::in | std::ios::out | std::ios::binary);
    }
}


void DiskManager::write_page(std::int32_t page_id, const Page& page) {
    std::size_t offset = static_cast<std::size_t>(page_id) * Page::PAGE_SIZE;

    file_.seekp(offset);
    file_.write(reinterpret_cast<const char*>(&page), Page::PAGE_SIZE);
    file_.flush();
}

void DiskManager::read_page(std::int32_t page_id, Page& page) {
    std::size_t offset = static_cast<std::size_t>(page_id) * Page::PAGE_SIZE;

    file_.seekg(offset);
    file_.read(reinterpret_cast<char*>(&page), Page::PAGE_SIZE);
}
