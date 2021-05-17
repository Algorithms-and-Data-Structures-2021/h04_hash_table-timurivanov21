#include "hash_table.hpp"

#include <stdexcept>

namespace itis {

  int HashTable::hash(int key) const {
    return utils::hash(key, static_cast<int>(buckets_.size()));
  }

  HashTable::HashTable(int capacity, double load_factor) : load_factor_{load_factor} {
    if (capacity <= 0) {
      throw std::logic_error("hash table capacity must be greater than zero");
    }

    if (load_factor <= 0.0 || load_factor > 1.0) {
      throw std::logic_error("hash table load factor must be in range [0...1]");
    }

    // Tip: allocate hash-table buckets
    buckets_.resize(capacity);
  }

  std::optional<std::string> HashTable::Search(int key) const {
    // Tip: compute hash code (index) and use linear search
    int index = hash(key);
    auto bucket = buckets_[index];
    for (const auto &pair : bucket) {
      if (pair.first == key) {
        return pair.second;
      }
    }
    return std::nullopt;
  }

  void HashTable::Put(int key, const std::string &value) {
    // Tip 1: compute hash code (index) to determine which bucket to use
    // Tip 2: consider the case when the key exists (read the docs in the header file)

    int index = hash(key);
    bool flag = false;

    for (auto &pair : buckets_[index]) {
      if (pair.first == key) {
        pair.second = value;
        flag = true;
        break;
      }
    }

    if (!flag) {
      buckets_[index].emplace_back(key, value);
    }
    num_keys_++;
    if (static_cast<double>(num_keys_) / capacity() >= load_factor_) {
      std::vector<Bucket> new_buckets(capacity() * kGrowthCoefficient);
      std::list<std::pair<int, std::string>> empty_list;
      for (auto list : buckets_) {
        if (!list.empty()) {
          int new_index = utils::hash(list.front().first, new_buckets.size());
          new_buckets[new_index] = list;
        }
      }
      buckets_ = new_buckets;
    }
  }

  std::optional<std::string> HashTable::Remove(int key) {
    // Tip 1: compute hash code (index) to determine which bucket to use
    // TIp 2: find the key-value pair to remove and make a copy of value to return
    int index = hash(key);
    std::pair<int, std::string> pair_to_remove;
    bool flag = false;
    for (auto pair : buckets_[index]) {
      if (pair.first == key) {
        pair_to_remove = pair;
        flag = true;
        break;
      }
    }
    if (flag) {
      buckets_[index].remove(pair_to_remove);
      return pair_to_remove.second;
    }
    return std::nullopt;
  }

  bool HashTable::ContainsKey(int key) const {
    // Note: uses Search(key) which is not initially implemented
    return Search(key).has_value();
  }

  bool HashTable::empty() const {
    return size() == 0;
  }

  int HashTable::size() const {
    return num_keys_;
  }

  int HashTable::capacity() const {
    return static_cast<int>(buckets_.size());
  }

  double HashTable::load_factor() const {
    return load_factor_;
  }

  std::unordered_set<int> HashTable::keys() const {
    std::unordered_set<int> keys(num_keys_);
    for (const auto &bucket : buckets_) {
      for (const auto &[key, _] : bucket) {
        keys.insert(key);
      }
    }
    return keys;
  }

  std::vector<std::string> HashTable::values() const {
    std::vector<std::string> values;
    for (const auto &bucket : buckets_) {
      for (const auto &[_, value] : bucket) {
        values.push_back(value);
      }
    }
    return values;
  }

}  // namespace itis