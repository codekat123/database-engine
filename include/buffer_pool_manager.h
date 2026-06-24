#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include "page.h"
#include "disk_manager.h"
#include "lru_replacer.h"

class BufferPoolManager {
public:
    BufferPoolManager(size_t num_frames, DiskManager& disk_manager);
    ~BufferPoolManager();

    Page* fetch_page(int32_t page_id);
    bool unpin_page(int32_t page_id, bool is_dirty);
    bool flush_page(int32_t page_id);
    Page* new_page(int32_t& page_id);

private:
    size_t num_frames_;
    int32_t next_page_id_ = 0;
    DiskManager& disk_manager_;
    LRUReplacer replacer_;

    std::vector<Page> frames_;
    std::vector<int> pin_count_;
    std::vector<bool> dirty_;
    std::unordered_map<int32_t, size_t> page_table_;
    std::list<size_t> free_frames_;
};
