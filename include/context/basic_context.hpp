#pragma once

#include <any>
#include <cinttypes>
#include <functional>
#include <memory>

namespace context {

/// The main interface to context. This class can be customized via ContextImpl
/// to provide alternative storage and lookup strategies. The ContextImpl is a
/// template that takes in a Key type and a key comparison function. The
/// ContextImpl must satify the following expressions that return ContextImpl:
/// ContextImpl::make_root()` - to create the parent context.
/// ContextImpl::make(Key, std::any, ContextImpl& parent) - to derive a new
/// context from another context.
template <template <class, class> class ContextImpl, class Key,
          class KeyEqual = std::equal_to<Key>>
class basic_context {
  using ContextImplType = ContextImpl<Key, KeyEqual>;
  ContextImplType impl_;

 public:
  basic_context() : impl_(ContextImplType::make_root()) {}

  basic_context(ContextImplType i) : impl_(std::move(i)) {}

  basic_context with_value(Key key, std::any value) {
    auto i = ContextImplType::make(std::move(key), std::move(value), impl_);
    return basic_context{std::move(i)};
  }

  template <class ValueType, class... Args>
  basic_context with_args(Key key, Args &&... args) {
    std::any value{std::in_place_type<ValueType>, std::forward<Args>(args)...};

    auto i = ContextImplType::make(std::move(key), std::move(value), impl_);
    return basic_context{std::move(i)};
  }

  std::any value(const Key &key) const { return impl_.lookup(key); }
};

}  // end namespace context