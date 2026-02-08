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

#include <cstddef>

#include <rc4/rc4.hpp>

namespace slop::crypto {

void RC4::apply(std::span<std::byte> data) noexcept {
  for (std::byte& b : data) {
    m_i += 1;
    m_j += std::to_integer<std::uint8_t>(m_state[m_i]);

    std::swap(m_state[m_i], m_state[m_j]);

    const std::uint8_t key_byte_idx = std::to_integer<std::uint8_t>(m_state[m_i]) + std::to_integer<std::uint8_t>(m_state[m_j]);
    const std::byte key_byte = m_state[key_byte_idx];
    b = b ^ key_byte;
  }
}

}
