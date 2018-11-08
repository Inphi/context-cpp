#include <cassert>
#include <cinttypes>
#include <random>
#include <string>
#include "context/context.hpp"

struct user_settings {
  std::string user;
  std::int32_t user_id;

  user_settings(std::string u, std::int32_t id)
      : user(std::move(u)), user_id(id) {}
};

bool operator==(const user_settings& a, const user_settings& b) {
  return a.user == b.user && a.user_id == b.user_id;
}

auto generate_request_id() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist{1, 6};
  return dist(rng);
}

template <template <class, class> class ContextImpl>
void test() {
  enum class values {
    reqId,
    env_argument,
    user_settings,
  };

  using Context = context::basic_context<ContextImpl, values>;

  const auto arg = "default argument";

  const auto id = generate_request_id();

  Context ctx;
  auto new_ctx =
      ctx.with_value(values::reqId, id).with_value(values::env_argument, arg);

  static std::int32_t user_id = 0;
  user_settings usr_settings{"userA", user_id++};
  auto new_ctx2 = ctx.with_value(values::user_settings, usr_settings);

  user_settings usr_settings2{"userB", user_id++};
  auto nested_new_ctx = new_ctx.with_args<user_settings>(
      values::user_settings, usr_settings2.user, usr_settings2.user_id);

  const auto stored_id = new_ctx.value(values::reqId);
  const auto nonexistent_id = new_ctx2.value(values::reqId);
  const auto stored_arg = nested_new_ctx.value(values::env_argument);
  const auto stored_usr_settings = new_ctx2.value(values::user_settings);
  const auto stored_usr_settings2 = nested_new_ctx.value(values::user_settings);

  assert(stored_id.has_value());
  assert(!nonexistent_id.has_value());
  assert(stored_arg.has_value());
  assert(stored_usr_settings.has_value());
  assert(stored_usr_settings2.has_value());

  assert(std::any_cast<decltype(id)>(stored_id) == id);
  assert(std::any_cast<decltype(arg)>(stored_arg) == arg);
  assert(std::any_cast<decltype(usr_settings)>(stored_usr_settings) ==
         usr_settings);
  assert(std::any_cast<decltype(usr_settings2)>(stored_usr_settings2) ==
         usr_settings2);
}

int main(int ac, char** av) {
  test<context::detail::link_impl>();
  test<context::detail::flat_impl>();
}