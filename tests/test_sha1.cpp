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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <array>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>

#include <sha1.hpp>

std::string to_hex(const std::array<std::byte, 20>& hash) {
    std::string out;
    for (auto b : hash) {
        out += std::format("{:02x}", std::to_integer<int>(b));
    }

    return out;
}

TEST_CASE("SHA-1 Golden Vectors") {
    Sha1 hasher;
    std::array<std::byte, 20> result;

    SUBCASE("Empty String") {
        hasher = Sha1();
        hasher.update(std::string_view{""}); 
        hasher.finalize(result);
        
        CHECK(to_hex(result) == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    }

    SUBCASE("ABC (The Classic)") {
        hasher = Sha1();
        hasher.update(std::string_view{"abc"});
        hasher.finalize(result);

        CHECK(to_hex(result) == "a9993e364706816aba3e25717850c26c9cd0d89d");
    }

    SUBCASE("The Quick Brown Fox") {
        hasher = Sha1();
        hasher.update(std::string_view{"The quick brown fox jumps over the lazy dog"});
        hasher.finalize(result);

        CHECK(to_hex(result) == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");
    }

    SUBCASE("Padding Logic Check (56 bytes)") {
        hasher = Sha1();
        hasher.update(std::string_view{"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"});
        hasher.finalize(result);

        CHECK(to_hex(result) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
    }
}