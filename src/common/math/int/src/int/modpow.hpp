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

#include <boost/multiprecision/integer.hpp>

#include <limits>
#include <stdexcept>
#include <type_traits>

#include <concepts/ints.hpp>

namespace mp = boost::multiprecision;

namespace slop::math {

static constexpr auto mod_pow(concepts::Integer auto const& base, concepts::Integer auto const& exp, concepts::Integer auto const& mod) {
    using ModType = std::remove_cvref_t<decltype(mod)>;

    if constexpr (std::numeric_limits<ModType>::is_signed) {
        if (mod <= 0) {
            throw std::invalid_argument("slop::math::int#modpow: provided modulus is non-positive.");
        }
    }

    auto result = mp::powm(base, exp, mod);

    if (result < 0) {
        result += mod;
    }

    return static_cast<ModType>(result);
}

}