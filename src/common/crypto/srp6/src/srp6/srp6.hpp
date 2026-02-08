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
#include <span>
#include <string>

#include <boost/multiprecision/cpp_int.hpp>

namespace slop::crypto {

class SRP6 {
public:
  explicit SRP6(const std::string& username, const std::string& password, const std::array<std::byte, 32>& salt) : m_username(username), m_password(password), m_salt(salt) {}

  std::array<std::byte, 20> calculate_x() const;
  std::array<std::byte, 32> calculate_password_verifier() const;

private:
  const boost::multiprecision::uint256_t LARGE_SAFE_PRIME{"0x894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7"};
  const boost::multiprecision::uint256_t GENERATOR{7};

  std::string m_username;
  std::string m_password;
  std::array<std::byte, 32> m_salt;
};

}
