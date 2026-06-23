#include "../include/lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_frames)
    : num_frames_(num_frames) {}


void LRUReplacer::record_access(size_t frame_id) {
    if (position_.count(frame_id)) {
        lru_list_.erase(position_[frame_id]);
    }
    lru_list_.push_front(frame_id);
    position_[frame_id] = lru_list_.begin();
}


void LRUReplacer::set_evictable(size_t frame_id, bool evictable) {
    if (!position_.count(frame_id)) return;
    evictable_[frame_id] = evictable;
}


bool LRUReplacer::evict(size_t& frame_id) {
    for (auto it = lru_list_.rbegin(); it != lru_list_.rend(); ++it) {
        if (evictable_.count(*it) && evictable_[*it]) {
            frame_id = *it;
            position_.erase(*it);
            evictable_.erase(*it);
            lru_list_.erase(std::next(it).base());
            return true;
        }
    }
    return false;
}


size_t LRUReplacer::size() const {
    size_t count = 0;
    for (auto& [frame_id, is_evictable] : evictable_) {
        if (is_evictable) count++;
    }
    return count;
}
