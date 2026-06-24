#include "../include/buffer_pool_manager.h"

BufferPoolManager::BufferPoolManager(size_t num_frames, DiskManager& disk_manager)
    : num_frames_(num_frames),
      disk_manager_(disk_manager),
      replacer_(num_frames)
{
    frames_.resize(num_frames_);
    pin_count_.resize(num_frames_, 0);
    dirty_.resize(num_frames_, false);

    for (size_t i = 0; i < num_frames_; i++) {
        free_frames_.push_back(i);
    }
}

Page* BufferPoolManager::fetch_page(int32_t page_id) {
    auto it = page_table_.find(page_id);
    if (it != page_table_.end()) {
        size_t frame_id = it->second;
        pin_count_[frame_id]++;
        replacer_.record_access(frame_id);
        return &frames_[frame_id];
    }

    size_t frame_id;

    if (!free_frames_.empty()) {
        frame_id = free_frames_.front();
        free_frames_.pop_front();
    } else {
        bool found = replacer_.evict(frame_id);
        if (!found) return nullptr;

        if (dirty_[frame_id]) {
            int32_t old_page_id = frames_[frame_id].header()->page_id;
            disk_manager_.write_page(old_page_id, frames_[frame_id]);
            dirty_[frame_id] = false;
        }

        int32_t old_page_id = frames_[frame_id].header()->page_id;
        page_table_.erase(old_page_id);
    }

    disk_manager_.read_page(page_id, frames_[frame_id]);
    page_table_[page_id] = frame_id;
    pin_count_[frame_id] = 1;
    dirty_[frame_id] = false;
    replacer_.record_access(frame_id);

    return &frames_[frame_id];
}

bool BufferPoolManager::unpin_page(int32_t page_id, bool is_dirty) {
    auto it = page_table_.find(page_id);
    if (it == page_table_.end()) return false;

    size_t frame_id = it->second;

    if (pin_count_[frame_id] == 0) return false;

    pin_count_[frame_id]--;

    if (pin_count_[frame_id] == 0) {
        replacer_.set_evictable(frame_id,true);
    }

    if (is_dirty) {
        dirty_[frame_id] = true;
    }

    return true;
}


bool BufferPoolManager::flush_page(int32_t page_id) {
    auto it = page_table_.find(page_id);
    if (it == page_table_.end()) return false;

    size_t frame_id = it->second;

    disk_manager_.write_page(page_id, frames_[frame_id]);
    dirty_[frame_id] = false;

    return true;
}
Page* BufferPoolManager::new_page(int32_t& page_id) {
    size_t frame_id;

    if (!free_frames_.empty()) {
        frame_id = free_frames_.front();
        free_frames_.pop_front();
    } else {
        bool found = replacer_.evict(frame_id);
        if (!found) return nullptr;

        if (dirty_[frame_id]) {
            int32_t old_page_id = frames_[frame_id].header()->page_id;
            disk_manager_.write_page(old_page_id, frames_[frame_id]);
            dirty_[frame_id] = false;
        }

        int32_t old_page_id = frames_[frame_id].header()->page_id;
        page_table_.erase(old_page_id);
    }

    page_id = next_page_id_++;

    frames_[frame_id] = Page();
    frames_[frame_id].header()->page_id = page_id;

    page_table_[page_id] = frame_id;
    pin_count_[frame_id] = 1;
    dirty_[frame_id] = true;
    replacer_.record_access(frame_id);

    return &frames_[frame_id];
}

BufferPoolManager::~BufferPoolManager() {
    for (size_t i = 0; i < num_frames_; i++) {
        if (dirty_[i]) {
            int32_t page_id = frames_[i].header()->page_id;
            disk_manager_.write_page(page_id, frames_[i]);
        }
    }
}
