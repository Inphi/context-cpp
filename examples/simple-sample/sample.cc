#include <cassert>
#include <cinttypes>
#include <random>
#include <string>
#include "context/context.hpp"
#include "logger.h"

enum class context_id {
  req_id,
  argument,
  log,
};

auto generate_request_id() {
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist{
      1, std::numeric_limits<std::uint32_t>::max()};
  return dist(rng);
}

using sample_context = context::context<context_id>;

sample_context with_logger(sample_context& ctx, std::string tag) {
  std::vector<std::string> tags{tag};
  return ctx.with_args<simple::tagged_logger>(context_id::log, std::cerr,
                                              std::move(tags));
}

void log(const sample_context& ctx) {
  const auto logger =
      std::any_cast<simple::tagged_logger>(ctx.value(context_id::log));

  const auto arg = ctx.value(context_id::argument);

  logger.log("argument is \"" + std::any_cast<std::string>(arg) + '"');
}

int main(int ac, char** av) {
  enum class values {
    reqId,
    argument,
  };

  std::string arg{"default argument"};
  if (ac > 1) {
    arg = av[1];
  }

  const auto id = generate_request_id();

  sample_context ctx;
  auto new_ctx = ctx.with_value(context_id::req_id, id)
                     .with_value(context_id::argument, arg);

  auto logging_ctx = with_logger(new_ctx, std::to_string(id));

  log(logging_ctx);
}