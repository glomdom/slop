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

#include <sha1/sha1.hpp>

#include <bit>
#include <cstdint>
#include <cstring>
#include <span>

namespace slop::crypto {

void Sha1::process_span(std::span<const std::byte> data) {
  auto current = data.data();
  auto remaining = data.size();

  std::size_t fill = m_count % 64;

  if (fill > 0) {
    std::size_t space_left = 64 - fill;

    if (remaining < space_left) {
      std::memcpy(m_buffer + fill, current, remaining);
      m_count += remaining;
      
      return;
    }

    std::memcpy(m_buffer + fill, current, space_left);
    transform(m_buffer);

    m_count += space_left;
    current += space_left;
    remaining -= space_left;
  }

  while (remaining >= 64) {
    transform(std::span<const std::byte, 64>(current, 64));

    m_count += 64;
    current += 64;
    remaining -= 64;
  }

  if (remaining > 0) {
    std::memcpy(m_buffer, current, remaining);
    m_count += remaining;
  }
}

[[nodiscard]] std::array<std::byte, 20> Sha1::finalize() {
  std::array<std::byte, 20> result;

  std::uint64_t total_bits = m_count * 8;
  total_bits = std::byteswap(total_bits);
  
  std::byte end_mark{0x80};
  update(std::span(&end_mark, 1));
  
  std::uint64_t current_mod = m_count % 64;
  std::uint64_t zeros_needed;

  if (current_mod < 56) {
    zeros_needed = 56 - current_mod;
  } else {
    zeros_needed = (64 - current_mod) + 56;
  }

  static const std::byte zero_pad[64]{}; 
  while (zeros_needed > 0) {
    std::size_t chunk = std::min(zeros_needed, sizeof(zero_pad));
    update(std::span(zero_pad, chunk));

    zeros_needed -= chunk;
  }

  auto length_bytes = std::as_bytes(std::span(&total_bits, 1));
  update(length_bytes);

  for (int i = 0; i < 5; ++i) {
    std::uint32_t val = std::byteswap(m_state[i]);
    std::memcpy(&result[i * 4], &val, 4);
  }

  return result;
}

inline static void swap_with_temp(std::uint32_t& a, std::uint32_t& b, std::uint32_t& c, std::uint32_t& d, std::uint32_t& e, std::uint32_t& temp) {
  e = d;
  d = c;
  c = std::rotl(b, 30);
  b = a;
  a = temp;
}

void Sha1::transform(std::span<const std::byte, 64> block) {
  std::uint32_t W[80];
  std::uint32_t a{m_state[0]}, b{m_state[1]}, c{m_state[2]}, d{m_state[3]}, e{m_state[4]};

  for (int i = 0; i < 16; ++i) {
    std::uint32_t raw;
    std::memcpy(&raw, block.data() + (i * 4), sizeof(uint32_t));

    W[i] = std::byteswap(raw);
  }

  for (int i = 16; i < 80; i++) {
    std::uint32_t w1{W[i - 3]}, w2{W[i - 8]}, w3{W[i - 14]}, w4{W[i - 16]};
    std::uint32_t xored = w1 ^ w2 ^ w3 ^ w4;

    W[i] = std::rotl(xored, 1);
  }

  // iter 1
  for (int i = 0; i < 20; i++) {
    std::uint32_t temp = std::rotl(a, 5) + ((b & c) | (~b & d)) + e + 0x5A827999 + W[i];

    swap_with_temp(a, b, c, d, e, temp);
  }

  // iter 2
  for (int i = 20; i < 40; i++) {
    std::uint32_t temp = std::rotl(a, 5) + (b ^ c ^ d) + e + 0x6ED9EBA1 + W[i];

    swap_with_temp(a, b, c, d, e, temp);
  }

  // iter 3
  for (int i = 40; i < 60; i++) {
    std::uint32_t temp = std::rotl(a, 5) + ((b & c) | (b & d) | (c & d)) + e + 0x8F1BBCDC + W[i];

    swap_with_temp(a, b, c, d, e, temp);
  }

  // iter 4
  for (int i = 60; i < 80; i++) {
    std::uint32_t temp = std::rotl(a, 5) + (b ^ c ^ d) + e + 0xCA62C1D6 + W[i];

    swap_with_temp(a, b, c, d, e, temp);
  }

  m_state[0] += a;
  m_state[1] += b;
  m_state[2] += c;
  m_state[3] += d;
  m_state[4] += e;
}

}