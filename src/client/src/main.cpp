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

#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <print>

#include <sha1.hpp>
#include <string_view>

int main(int argc, char** argv) {
  std::println("Hello, {}!", "world");

  std::string_view input{"abc"};
  auto hasher = Sha1{};
  hasher.update(input);

  std::array<std::byte, 20> result;
  hasher.finalize(result);

  for (auto b : result) {
    std::cout << std::format("{:02x}", (std::uint8_t)b);
  }

  std::println();

  return 0;
}
