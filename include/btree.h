#pragma once

#include <cstdint>
#include "btree_page.h"
#include "buffer_pool_manager.h"
#include "rid.h"

class BTree {
public:
    BTree(BufferPoolManager& bpm);

    bool insert(int32_t key, RID rid);
    bool search(int32_t key, RID& result);

private:
    BufferPoolManager& bpm_;
    int32_t root_page_id_;

    int32_t new_leaf_page();
    int32_t new_internal_page();

    void insert_into_leaf(BTreeLeafPage* leaf, int32_t page_id,
                          int32_t key, RID rid);

    void split_leaf(int32_t leaf_page_id);
    void insert_into_parent(int32_t left_page_id,
                            int32_t key, int32_t right_page_id);
};
