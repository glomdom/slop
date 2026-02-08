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

#include "srp6/srp6.hpp"

#include <sha1/sha1.hpp>
#include <int/modpow.hpp>

namespace mp = boost::multiprecision;

namespace slop::crypto {

std::array<std::byte, 20> SRP6::calculate_x() const {
  auto interim_hasher = Sha1();
  interim_hasher.update(m_username + ":" + m_password);
  const auto interim = interim_hasher.finalize();

  auto result_hasher = Sha1();
  result_hasher.update(m_salt);
  result_hasher.update(interim);
  const auto result = result_hasher.finalize();

  return result;
}

std::array<std::byte, 32> SRP6::calculate_password_verifier() const {
  const auto x = calculate_x();

  mp::uint256_t x_int;
  mp::import_bits(x_int, x.begin(), x.end(), 8, false);

  const auto result_int = math::mod_pow(GENERATOR, x_int, LARGE_SAFE_PRIME);

  std::array<std::byte, 32> result{};
  mp::export_bits(result_int, reinterpret_cast<unsigned char*>(result.data()), 8, false);

  return result;
}

}
