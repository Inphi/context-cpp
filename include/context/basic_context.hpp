#pragma once

#include <any>
#include <cinttypes>
#include <functional>
#include <memory>

namespace context {

template <template <class, class> class ContextImpl, class Key,
          class KeyEqual = std::equal_to<Key>>
class basic_context {
  using ContextImplType = ContextImpl<Key, KeyEqual>;
  ContextImplType impl_;

 public:
  basic_context() : impl_(ContextImplType::make_root()) {}

  basic_context(ContextImplType i) : impl_(std::move(i)) {}

  /// KeyType is hashable
  basic_context with_value(Key key, std::any value) {
    auto i = ContextImplType::make(std::move(key), std::move(value), impl_);
    return basic_context{std::move(i)};
  }

  /// KeyType is hashable
  template <class ValueType, class... Args>
  basic_context with_args(Key key, Args &&... args) {
    std::any value{std::in_place_type<ValueType>, std::forward<Args>(args)...};

    auto i = ContextImplType::make(std::move(key), std::move(value), impl_);
    return basic_context{std::move(i)};
  }

  std::any value(const Key &key) const { return impl_.lookup(key); }
};

}  // end namespace context