// Copyright 2022 Tier IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef AUTOWARE_UTILS_UUID__UUID_HELPER_HPP_
#define AUTOWARE_UTILS_UUID__UUID_HELPER_HPP_

#include <unique_identifier_msgs/msg/uuid.hpp>

#include <boost/uuid/uuid.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>
#include <string>

namespace autoware_utils_uuid
{

inline unique_identifier_msgs::msg::UUID generate_uuid()
{
  // Generate random number
  unique_identifier_msgs::msg::UUID uuid;
  std::mt19937 gen(std::random_device{}());
  std::independent_bits_engine<std::mt19937, 8, uint8_t> bit_eng(gen);
  std::generate(uuid.uuid.begin(), uuid.uuid.end(), bit_eng);

  return uuid;
}
inline unique_identifier_msgs::msg::UUID generate_default_uuid()
{
  // Generate UUID with all zeros
  unique_identifier_msgs::msg::UUID default_uuid;
  // Use std::generate to fill the UUID with zeros
  std::generate(default_uuid.uuid.begin(), default_uuid.uuid.end(), []() { return 0; });

  return default_uuid;
}
inline std::string to_hex_string(const unique_identifier_msgs::msg::UUID & id)
{
  static constexpr char hex_chars[] = "0123456789abcdef";
  std::string s(2 * id.uuid.size(), '0');
  for (size_t i = 0; i < id.uuid.size(); ++i) {
    s[2 * i] = hex_chars[(id.uuid[i] >> 4) & 0x0F];
    s[2 * i + 1] = hex_chars[id.uuid[i] & 0x0F];
  }
  return s;
}

inline boost::uuids::uuid to_boost_uuid(const unique_identifier_msgs::msg::UUID & id)
{
  boost::uuids::uuid boost_uuid{};
  std::copy(id.uuid.begin(), id.uuid.end(), boost_uuid.begin());
  return boost_uuid;
}

inline unique_identifier_msgs::msg::UUID to_uuid_msg(const boost::uuids::uuid & id)
{
  unique_identifier_msgs::msg::UUID ros_uuid{};
  std::copy(id.begin(), id.end(), ros_uuid.uuid.begin());
  return ros_uuid;
}

}  // namespace autoware_utils_uuid

#endif  // AUTOWARE_UTILS_UUID__UUID_HELPER_HPP_
