/*

  Copyright (C) 2026 glomdom

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <doctest.h>

#include <cstddef>
#include <format>
#include <span>
#include <string_view>
#include <vector>

#include <rc4/rc4.hpp>

using namespace slop::crypto;

std::string to_hex(const std::vector<std::byte>& bytes) {
  std::string out;
  for (auto b : bytes) {
    out += std::format("{:02x}", std::to_integer<int>(b));
  }

  return out;
}

TEST_CASE("RC4 Test Vectors") {
    SUBCASE("10 bytes 0x00 with `Key`") {
        RC4 rc4 = RC4{std::string_view{"Key"}};
        std::vector<std::byte> input(10, std::byte{0});

        rc4.apply(input);

        CHECK(to_hex(input) == "eb9f7781b734ca72a719");
    }

    SUBCASE("6 bytes 0x00 with `Wiki`") {
        RC4 rc4 = RC4{std::string_view{"Wiki"}};
        std::vector<std::byte> input(6, std::byte{0});

        rc4.apply(input);

        CHECK(to_hex(input) == "6044db6d41b7");
    }

    SUBCASE("8 bytes 0x00 with `Secret`") {
        RC4 rc4 = RC4{std::string_view{"Secret"}};
        std::vector<std::byte> input(8, std::byte{0});

        rc4.apply(input);

        CHECK(to_hex(input) == "04d46b053ca87b59");
    }
}