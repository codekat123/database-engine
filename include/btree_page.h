#pragma once

#include <cstdint>
#include "rid.h"

static constexpr int32_t INVALID_PAGE_ID = -1;

enum class NodeType : uint8_t {
    INTERNAL = 0,
    LEAF = 1
};

struct BTreeHeader {
    NodeType node_type;
    int32_t  num_keys;
    int32_t  parent_page_id;
};

static constexpr int PAGE_SIZE = 4096;

static constexpr int INTERNAL_MAX_KEYS =
    (PAGE_SIZE - sizeof(BTreeHeader) - sizeof(int32_t))
    / (sizeof(int32_t) + sizeof(int32_t));

static constexpr int LEAF_MAX_KEYS =
    (PAGE_SIZE - sizeof(BTreeHeader) - sizeof(int32_t))
    / (sizeof(int32_t) + sizeof(RID));



struct BTreeInternalPage {
    BTreeHeader header;
    int32_t keys[INTERNAL_MAX_KEYS];
    int32_t children[INTERNAL_MAX_KEYS + 1];
};

struct BTreeLeafPage {
    BTreeHeader header;
    int32_t keys[LEAF_MAX_KEYS];
    RID     rids[LEAF_MAX_KEYS];
    int32_t next_page_id;
};

static_assert(sizeof(BTreeInternalPage) <= PAGE_SIZE,
    "BTreeInternalPage exceeds page size");

static_assert(sizeof(BTreeLeafPage) <= PAGE_SIZE,
    "BTreeLeafPage exceeds page size");
