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

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>

template <typename T>
concept ContiguousBytes = std::ranges::contiguous_range<T> && (sizeof(std::ranges::range_value_t<T>) == 1);

class Sha1 {
public:
  void update(const ContiguousBytes auto& data) {
    auto bytes = std::as_bytes(std::span{data});

    process_span(bytes);
  }

  [[nodiscard]] std::array<std::byte, 20> finalize();

private:
  void process_span(std::span<const std::byte> data);
  void transform(std::span<const std::byte, 64> block);

  std::uint32_t m_state[5]{0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
  std::uint64_t m_count{0};
  std::byte m_buffer[64]{};
};