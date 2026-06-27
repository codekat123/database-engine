#include "../include/heap_file.h"

HeapFile::HeapFile(BufferPoolManager& bpm)
    : bpm_(bpm), last_page_id_(-1) {}


RID HeapFile::insert_record(const char* data, uint16_t length) {
    if (last_page_id_ != -1) {
        Page* page = bpm_.fetch_page(last_page_id_);
        if (page != nullptr) {
            HeapPage hp(*page);
            if (hp.free_space() >= length + sizeof(Slot)) {
                int32_t slot_id = hp.insert_record(data, length);
                bpm_.unpin_page(last_page_id_, true);
                return RID{static_cast<uint32_t>(last_page_id_),
                           static_cast<uint32_t>(slot_id)};
            }
            bpm_.unpin_page(last_page_id_, false);
        }
    }

    int32_t new_page_id;
    Page* page = bpm_.new_page(new_page_id);
    last_page_id_ = new_page_id;

    HeapPage hp(*page);
    int32_t slot_id = hp.insert_record(data, length);
    bpm_.unpin_page(new_page_id, true);

    return RID{static_cast<uint32_t>(new_page_id),
               static_cast<uint32_t>(slot_id)};
}


bool HeapFile::get_record(const RID& rid, char* buffer, uint16_t& length) {
    Page* page = bpm_.fetch_page(rid.page_id);
    if (page == nullptr) {
        length = 0;
        return false;
    }

    HeapPage hp(*page);
    const char* data = hp.get_record(rid.slot_id, length);

    if (data == nullptr) {
        bpm_.unpin_page(rid.page_id, false);
        length = 0;
        return false;
    }

    std::memcpy(buffer, data, length);
    bpm_.unpin_page(rid.page_id, false);
    return true;
}
