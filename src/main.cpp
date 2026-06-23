#include <iostream>
#include "../include/page.h"
#include "../include/disk_manager.h"
#include "../include/tokenizer.h"
#include "../include/lru_replacer.h"

int main() {
    DiskManager dm("test.db");

    Page write_page;
    write_page.header()->page_id = 1;
    write_page.header()->record_count = 5;

    LRUReplacer replacer(3);
    replacer.record_access(0);
    replacer.record_access(1);
    replacer.record_access(2);
    replacer.set_evictable(0, true);
    replacer.set_evictable(1, true);
    replacer.set_evictable(2, true);

    size_t victim;
    replacer.evict(victim);
    std::cout << "evicted frame: " << victim << "\n";

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
