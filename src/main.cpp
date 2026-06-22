#include <iostream>
#include "../include/page.h"
#include "../include/disk_manager.h"
#include "../include/tokenizer.h"

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

    Tokenizer tok("SELECT name FROM users WHERE age = 42");
    auto tokens = tok.tokenize();
    for (auto& t : tokens) {
        if (t.type == TokenType::END_OF_INPUT) break;
        std::cout << t.text << "\n";
    }

   return 0;
}
