#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <doctest.h>
#include <boost/algorithm/hex.hpp>

#include <srp6/srp6.hpp>
#include <concepts/bytes.hpp>

using namespace slop::crypto;

namespace fs = std::filesystem;

static std::string to_hex(const slop::concepts::ContiguousBytes auto& hash) {
  std::string out;
  for (auto b : hash) {
    out += std::format("{:02X}", static_cast<std::uint8_t>(b));
  }

  return out;
}

template <std::size_t Size>
static slop::concepts::ContiguousBytes auto from_be_hex(const std::string& hex) {
  std::array<std::byte, Size> bytes;

  boost::algorithm::unhex(hex.begin(), hex.end(), reinterpret_cast<char*>(bytes.data()));
  std::ranges::reverse(bytes);

  return bytes;
}

template <std::size_t Size>
static slop::concepts::ContiguousBytes auto from_le_hex(const std::string& hex) {
  std::array<std::byte, Size> bytes;

  boost::algorithm::unhex(hex.begin(), hex.end(), reinterpret_cast<char*>(bytes.data()));

  return bytes;
}

template<typename Func>
void foreach_test_case(const fs::path& path, Func&& func) {
  std::ifstream file(path);
  std::string line;
  int line_num = 0;

  while (std::getline(file, line)) {
    line_num++;
    if (line.empty()) continue;

    std::stringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;

    while (ss >> token) {
      tokens.push_back(token);
    }

    if (!tokens.empty()) {
      func(tokens, line_num);
    }
  }
}

TEST_CASE("SRP6") {
  const fs::path data_dir = TEST_DATA_DIR;

  SUBCASE("Calculating X with static username and password, and dynamic salt") {
    foreach_test_case(data_dir / "x_salts.txt", [&](const auto& row, const int line) {
      CAPTURE(line);

      const auto salt = from_be_hex<32>(row[0]);
      const auto srp6 = SRP6("USERNAME123", "PASSWORD123", salt);

      const auto actual = srp6.calculate_x();
      const auto expected = from_be_hex<20>(row[1]);

      CHECK(to_hex(actual) == to_hex(expected));
    });
  }

  SUBCASE("Calculating X with static salt, and dynamic username and password") {
    foreach_test_case(data_dir / "x_user_pass.txt", [&](const auto& row, const int line) {
      CAPTURE(line);

      const auto username = row[0];
      const auto password = row[1];
      const auto salt = from_be_hex<32>("CAC94AF32D817BA64B13F18FDEDEF92AD4ED7EF7AB0E19E9F2AE13C828AEAF57");
      const auto srp6 = SRP6(username, password, salt);

      const auto actual = srp6.calculate_x();
      const auto expected = from_be_hex<20>(row[2]);

      CHECK(to_hex(actual) == to_hex(expected));
    });
  }

  SUBCASE("Calculating password verifier with dynamic username, password and salt") {
    foreach_test_case(data_dir / "v_values.txt", [&](const auto& row, const int line) {
      CAPTURE(line);

      const auto username = row[0];
      const auto password = row[1];
      const auto salt = from_be_hex<32>(row[2]);
      const auto srp6 = SRP6(username, password, salt);

      const auto actual = srp6.calculate_password_verifier();
      const auto expected = from_be_hex<32>(row[3]);

      CHECK(to_hex(actual) == to_hex(expected));
    });
  }
}