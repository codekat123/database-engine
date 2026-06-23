#pragma once
#include <list>
#include <unordered_map>

class LRUReplacer {
public:
    explicit LRUReplacer(size_t num_frames);
    bool evict(size_t& frame_id);
    void record_access(size_t frame_id);
    void set_evictable(size_t frame_id, bool evictable);
    size_t size() const;

private:
    size_t num_frames_;
    std::list<size_t> lru_list_;
    std::unordered_map<size_t, std::list<size_t>::iterator> position_;
    std::unordered_map<size_t, bool> evictable_;
};
