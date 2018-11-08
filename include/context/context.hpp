#pragma once

#include "context/basic_context.hpp"
#include "context/flat_impl.hpp"
#include "context/link_impl.hpp"

namespace context {
template <class Key, class KeyEqual = std::equal_to<Key>>
using context = basic_context<detail::link_impl, Key, KeyEqual>;
}  // namespace context