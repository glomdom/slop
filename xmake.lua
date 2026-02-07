--[[

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

]]--

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", { outputdir="build" })

set_languages("c++26")
set_toolchains("clang")

add_requires("boost", { configs = { all = true } })

if is_mode("release") then
    set_warnings("all", "error")
    set_policy("build.optimization.lto", true)
end

includes("src/common")
includes("src/client")
includes("tests")
