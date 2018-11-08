#pragma once

#include <iostream>
#include <vector>

namespace simple {
class tagged_logger {
  std::ostream& os_;
  std::vector<std::string> tags_;

 public:
  tagged_logger(std::ostream& os = std::cerr) : os_(os) {}
  tagged_logger(std::ostream& os, std::vector<std::string> tags)
      : os_(os), tags_(std::move(tags)) {}

  tagged_logger(const tagged_logger&) = default;
  tagged_logger& operator=(const tagged_logger&) = default;

  void log(std::string_view msg) const {
    for (const auto& tag : tags_) {
      os_ << '|' << tag << '|';
    }
    os_ << " " << msg.data() << '\n';
  }
};
}  // namespace simple