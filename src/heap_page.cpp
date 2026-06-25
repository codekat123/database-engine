#include "../include/heap_page.h"



HeapPage::HeapPage(Page& page) : page_(page) {}


uint16_t HeapPage::free_space() const {
    PageHeader* h = page_.header();
    uint16_t slot_dir_start = Page::PAGE_SIZE - h->record_count * sizeof(Slot);
    return slot_dir_start - h->free_space_offset;
}

int32_t HeapPage::insert_record(const char* data, uint16_t length) {
    if (length + sizeof(Slot) > free_space()) {
        return -1;
    }

    PageHeader* h = page_.header();
    char* raw = page_.raw();

    uint16_t offset = h->free_space_offset;
    std::memcpy(raw + offset, data, length);
    h->free_space_offset += length;

    uint16_t slot_id = h->record_count;
    uint16_t slot_pos = Page::PAGE_SIZE - (slot_id + 1) * sizeof(Slot);
    Slot* slot = reinterpret_cast<Slot*>(raw + slot_pos);
    slot->offset = offset;
    slot->length = length;

    h->record_count++;
    return slot_id;
}

const char* HeapPage::get_record(uint16_t slot_id, uint16_t& length) const {
    PageHeader* h = page_.header();

    if (slot_id >= h->record_count) {
        length = 0;
        return nullptr;
    }

    const char* raw = page_.raw();
    uint16_t slot_pos = Page::PAGE_SIZE - (slot_id + 1) * sizeof(Slot);
    const Slot* slot = reinterpret_cast<const Slot*>(raw + slot_pos);

    length = slot->length;
    return raw + slot->offset;
}
