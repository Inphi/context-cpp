#pragma once

#include <any>
#include <cinttypes>
#include <functional>
#include <memory>

namespace context {
namespace detail {

template <class Key, class KeyEqual = std::equal_to<Key>>
class link_impl {
  struct data;
  using data_ptr = std::shared_ptr<data>;

  struct data {
    Key key;
    data_ptr parent;
    std::any value;

    std::any lookup(const Key& key) const {
      if (!parent) return std::any{};
      if (KeyEqual{}(key, this->key)) return value;
      return parent->lookup(key);
    }
  };

  data_ptr data_;

 public:
  static link_impl make_root() {
    link_impl i;
    i.data_ = std::make_shared<data>();
    return i;
  }

  static link_impl make(Key key, std::any value, const link_impl& parent) {
    link_impl i;
    i.data_ = std::make_shared<data>();
    i.data_->key = std::move(key);
    i.data_->value = std::move(value);
    i.data_->parent = parent.data_;

    return i;
  }

  std::any lookup(const Key& key) const { return data_->lookup(key); }
};

}  // namespace detail
}  // namespace context
