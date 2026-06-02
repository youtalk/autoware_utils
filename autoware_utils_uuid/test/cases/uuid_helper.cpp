// Copyright 2024 TIER IV, Inc.
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

#include "autoware_utils_uuid/uuid_helper.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include <gtest/gtest.h>

#include <string>

TEST(UUIDHelperTest, generate_uuid)
{
  // Generate two UUIDs and ensure they are all different

  unique_identifier_msgs::msg::UUID uuid1 = autoware_utils_uuid::generate_uuid();
  unique_identifier_msgs::msg::UUID uuid2 = autoware_utils_uuid::generate_uuid();

  EXPECT_FALSE(uuid1 == uuid2) << "Duplicate UUID generated: "
                               << autoware_utils_uuid::to_hex_string(uuid2);
}

TEST(UUIDHelperTest, generate_default_uuid)
{
  // Generate two UUIDs and ensure they are all different

  unique_identifier_msgs::msg::UUID default_uuid = autoware_utils_uuid::generate_default_uuid();
  unique_identifier_msgs::msg::UUID zero_uuid;
  std::fill(zero_uuid.uuid.begin(), zero_uuid.uuid.end(), 0x00);

  EXPECT_EQ(default_uuid, zero_uuid);
}

TEST(UUIDHelperTest, to_hex_string)
{
  unique_identifier_msgs::msg::UUID uuid;
  // Populate the UUID with some values
  std::fill(uuid.uuid.begin(), uuid.uuid.end(), 0x42);

  std::string hex_string = autoware_utils_uuid::to_hex_string(uuid);

  // Check if the generated hex string is correct
  EXPECT_EQ(hex_string, "42424242424242424242424242424242");

  // Sequential bytes pin zero-padding of values < 0x10 (e.g. 0x01 -> "01").
  unique_identifier_msgs::msg::UUID sequential_uuid;
  for (size_t i = 0; i < sequential_uuid.uuid.size(); ++i) {
    sequential_uuid.uuid[i] = static_cast<uint8_t>(i);
  }
  EXPECT_EQ(
    autoware_utils_uuid::to_hex_string(sequential_uuid), "000102030405060708090a0b0c0d0e0f");

  // All 0xAB pins lowercase hex digits.
  unique_identifier_msgs::msg::UUID lowercase_uuid;
  std::fill(lowercase_uuid.uuid.begin(), lowercase_uuid.uuid.end(), 0xAB);
  // cspell:disable-next-line
  EXPECT_EQ(autoware_utils_uuid::to_hex_string(lowercase_uuid), "abababababababababababababababab");

  // Mixed bytes with a leading 0x0A pin zero-padding combined with lowercase ("0a").
  unique_identifier_msgs::msg::UUID mixed_uuid;
  mixed_uuid.uuid = {0x0a, 0xff, 0x10, 0x00, 0x9c, 0x07, 0xde, 0xad,
                     0xbe, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xa0};
  EXPECT_EQ(autoware_utils_uuid::to_hex_string(mixed_uuid), "0aff10009c07deadbeef0123456789a0");
}

TEST(UUIDHelperTest, to_boost_uuid)
{
  unique_identifier_msgs::msg::UUID uuid;
  // Populate the UUID with some values
  std::fill(uuid.uuid.begin(), uuid.uuid.end(), 0x42);

  boost::uuids::uuid boost_uuid{};
  std::fill(boost_uuid.begin(), boost_uuid.end(), 0x42);

  // Check if the conversion from ROS UUID to Boost UUID is correct
  EXPECT_TRUE(boost_uuid == autoware_utils_uuid::to_boost_uuid(uuid));
}

TEST(UUIDHelperTest, to_uuid_msg)
{
  boost::uuids::random_generator generator;
  boost::uuids::uuid boost_uuid = generator();
  unique_identifier_msgs::msg::UUID ros_uuid = autoware_utils_uuid::to_uuid_msg(boost_uuid);

  // Check if the conversion from Boost UUID to ROS UUID is correct
  EXPECT_TRUE(std::equal(boost_uuid.begin(), boost_uuid.end(), ros_uuid.uuid.begin()));
}
