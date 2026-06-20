#pragma once

#include <fstream>
#include <string>
#include "page.h"

class DiskManager {
public:
    explicit DiskManager(const std::string& db_file);
    
    void write_page(std::int32_t page_id,const Page& page);
    void read_page(std::int32_t page_id, Page& page);

private:
    std::fstream file_;
};
