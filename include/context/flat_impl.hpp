#pragma once

#include <any>
#include <functional>
#include <memory>
#include <vector>

namespace context {

namespace detail {

template <class, class>
class flat_impl;

template <class Key, class KeyEqual = std::equal_to<Key>>
class flat_impl {
  struct data {
    Key key;
    std::any value;
    std::int64_t index;
  };

  using shared_list = std::shared_ptr<std::vector<data>>;

  std::int64_t index_;
  shared_list list_;

 public:
  static flat_impl make_root() {
    flat_impl i;
    i.index_ = -1;
    i.list_ = std::make_shared<std::vector<data>>();
    return i;
  }

  static flat_impl make(Key key, std::any value, const flat_impl& parent) {
    flat_impl impl;

    data d{std::move(key), std::move(value), parent.index_};
    parent.list_->emplace_back(std::move(d));

    impl.list_ = parent.list_;
    impl.index_ = parent.list_->size() - 1;
    assert(impl.index_ > parent.index_);
    return impl;
  }

  std::any lookup(const Key& key) const {
    auto i = index_;

    while (i >= 0) {
      const auto& d = (*list_)[i];
      if (KeyEqual{}(d.key, key)) {
        return d.value;
      }

      i = d.index;
    }

    return std::any{};
  }
};

}  // namespace detail
}  // namespace context