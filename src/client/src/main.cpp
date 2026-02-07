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

#include <print>
#include <boost/multiprecision/cpp_int.hpp>
#include <int/modpow.hpp>

// #include <asio.hpp>

// class AsyncClient {
//   asio::io_context& m_ctx;
//   asio::ip::tcp::socket m_socket;

// public:
//   AsyncClient(asio::io_context& ctx) : m_ctx(ctx), m_socket(ctx) {}

//   asio::awaitable<void> run(std::string host, std::string port) {
//     auto resolver = asio::ip::tcp::resolver(m_ctx);
//     auto endpoints = co_await resolver.async_resolve("127.0.0.1", "6767", asio::use_awaitable);

//     co_await asio::async_connect(m_socket, endpoints, asio::use_awaitable);
//     co_await read_loop();
//   }

// private:
//   asio::awaitable<void> read_loop() {
//     while (true) {

//     }
//   }
// };

int main(int argc, char** argv) {
  boost::multiprecision::int256_t a{-2};
  boost::multiprecision::int256_t b{3};
  boost::multiprecision::int256_t lsp{9};

  auto c = slop::math::mod_pow(a, b, lsp);

  std::println("Hello, {}!", c.convert_to<std::string>());

  return 0;
}
