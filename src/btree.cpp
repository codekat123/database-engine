#include "../include/btree.h"

BTree::BTree(BufferPoolManager& bpm)
    : bpm_(bpm), root_page_id_(INVALID_PAGE_ID) {}


int32_t BTree::new_leaf_page() {
    int32_t page_id;
    Page* page = bpm_.new_page(page_id);

    BTreeLeafPage* leaf = reinterpret_cast<BTreeLeafPage*>(page->raw());
    leaf->header.node_type = NodeType::LEAF;
    leaf->header.num_keys = 0;
    leaf->header.parent_page_id = INVALID_PAGE_ID;
    leaf->next_page_id = INVALID_PAGE_ID;

    bpm_.unpin_page(page_id, true);
    return page_id;
}

int32_t BTree::new_internal_page() {
    int32_t page_id;
    Page* page = bpm_.new_page(page_id);

    BTreeInternalPage* node = reinterpret_cast<BTreeInternalPage*>(page->raw());
    node->header.node_type = NodeType::INTERNAL;
    node->header.num_keys = 0;
    node->header.parent_page_id = INVALID_PAGE_ID;

    bpm_.unpin_page(page_id, true);
    return page_id;
}


bool BTree::search(int32_t key, RID& result) {
    if (root_page_id_ == INVALID_PAGE_ID) return false;

    int32_t current_page_id = root_page_id_;

    while (true) {
        Page* page = bpm_.fetch_page(current_page_id);
        BTreeHeader* header = reinterpret_cast<BTreeHeader*>(page->raw());

        if (header->node_type == NodeType::LEAF) {
            BTreeLeafPage* leaf = reinterpret_cast<BTreeLeafPage*>(page->raw());

            for (int32_t i = 0; i < leaf->header.num_keys; i++) {
                if (leaf->keys[i] == key) {
                    result = leaf->rids[i];
                    bpm_.unpin_page(current_page_id, false);
                    return true;
                }
            }

            bpm_.unpin_page(current_page_id, false);
            return false;
        }

        BTreeInternalPage* node = reinterpret_cast<BTreeInternalPage*>(page->raw());
        int32_t i = 0;
        while (i < node->header.num_keys && key >= node->keys[i]) {
            i++;
        }

        int32_t next_page_id = node->children[i];
        bpm_.unpin_page(current_page_id, false);
        current_page_id = next_page_id;
    }
}

bool BTree::insert(int32_t key, RID rid) {
    if (root_page_id_ == INVALID_PAGE_ID) {
        root_page_id_ = new_leaf_page();
    }

    int32_t current_page_id = root_page_id_;

    while (true) {
        Page* page = bpm_.fetch_page(current_page_id);
        BTreeHeader* header = reinterpret_cast<BTreeHeader*>(page->raw());

        if (header->node_type == NodeType::LEAF) {
            BTreeLeafPage* leaf = reinterpret_cast<BTreeLeafPage*>(page->raw());

            if (leaf->header.num_keys < LEAF_MAX_KEYS) {
                insert_into_leaf(leaf, current_page_id, key, rid);
                bpm_.unpin_page(current_page_id, true);
                return true;
            }

            bpm_.unpin_page(current_page_id, false);
            split_leaf(current_page_id);
            current_page_id = root_page_id_;
            continue;
        }

        BTreeInternalPage* node = reinterpret_cast<BTreeInternalPage*>(page->raw());
        int32_t i = 0;
        while (i < node->header.num_keys && key >= node->keys[i]) {
            i++;
        }
        int32_t next_page_id = node->children[i];
        bpm_.unpin_page(current_page_id, false);
        current_page_id = next_page_id;
    }
}

void BTree::insert_into_leaf(BTreeLeafPage* leaf, int32_t page_id,
                              int32_t key, RID rid) {
    int32_t i = leaf->header.num_keys - 1;

    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        leaf->rids[i + 1] = leaf->rids[i];
        i--;
    }

    leaf->keys[i + 1] = key;
    leaf->rids[i + 1] = rid;
    leaf->header.num_keys++;
}

void BTree::split_leaf(int32_t leaf_page_id) {
    Page* left_page = bpm_.fetch_page(leaf_page_id);
    BTreeLeafPage* left = reinterpret_cast<BTreeLeafPage*>(left_page->raw());

    int32_t right_page_id = new_leaf_page();
    Page* right_page = bpm_.fetch_page(right_page_id);
    BTreeLeafPage* right = reinterpret_cast<BTreeLeafPage*>(right_page->raw());

    int32_t split = left->header.num_keys / 2;

    for (int32_t i = split; i < left->header.num_keys; i++) {
        right->keys[i - split] = left->keys[i];
        right->rids[i - split] = left->rids[i];
    }
    right->header.num_keys = left->header.num_keys - split;
    left->header.num_keys = split;

    right->next_page_id = left->next_page_id;
    left->next_page_id = right_page_id;
    right->header.parent_page_id = left->header.parent_page_id;

    int32_t push_up_key = right->keys[0];

    bpm_.unpin_page(leaf_page_id, true);
    bpm_.unpin_page(right_page_id, true);

    insert_into_parent(leaf_page_id, push_up_key, right_page_id);
}
