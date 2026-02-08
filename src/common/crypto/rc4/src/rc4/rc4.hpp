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

#include <cstddef>
#include <span>
#include <utility>
#include <stdexcept>

#include <concepts/bytes.hpp>

namespace slop::crypto {

class RC4 {
public:
  explicit RC4(const concepts::ContiguousBytes auto& key) {
    if (key.size() == 0) {
      throw std::runtime_error("provided key to RC4 is 0-length.");
    }

    std::uint8_t j{};

    for (int i = 0; i < 256; i++) {
      j += std::to_integer<std::uint8_t>(m_state[i]);
      j += static_cast<std::uint8_t>(key[i % key.size()]);

      std::swap(m_state[i], m_state[j]);
    }
  }

  void apply(std::span<std::byte> data) noexcept;

private:
  template <typename T, std::size_t... IndexSequence>
  static constexpr std::array<T, sizeof...(IndexSequence)> make_sequenced_array(std::index_sequence<IndexSequence...>) {
    return { static_cast<T>(IndexSequence)... };
  }

  std::array<std::byte, 256> m_state = make_sequenced_array<std::byte>(std::make_index_sequence<256>{});
  std::uint8_t m_i{0}, m_j{0};
};

}
