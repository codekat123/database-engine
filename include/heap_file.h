#pragma once

#include "buffer_pool_manager.h"
#include "heap_page.h"
#include "rid.h"

class HeapFile {
public:
    explicit HeapFile(BufferPoolManager& bpm);

    RID insert_record(const char* data, uint16_t length);
    bool get_record(const RID& rid, char* buffer, uint16_t& length);

private:
    BufferPoolManager& bpm_;
    int32_t last_page_id_;
};
