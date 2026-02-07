#include <doctest.h>

#include <filesystem>
#include <fstream>
#include <srp6/srp6.hpp>
#include <sstream>
#include <string>

#include <boost/algorithm/hex.hpp>

using namespace slop::crypto;

namespace fs = std::filesystem;

static std::string to_hex(const std::array<std::byte, 20>& hash) {
  std::string out;
  for (auto b : hash) {
    out += std::format("{:02X}", std::to_integer<int>(b));
  }

  return out;
}

TEST_CASE("SRP6") {
    SRP6 srp6 = SRP6();

    const fs::path data_dir = TEST_DATA_DIR;
    
    SUBCASE("Calculating X with provided username and password") {
        const fs::path x_salts_file = data_dir / "x_salts.txt";
        std::ifstream file(x_salts_file);
        std::ios::sync_with_stdio(false);

        std::string line, input, expected;
        int line_counter = 0;

        while (std::getline(file, line)) {
            line_counter++;
            if (line.empty()) continue;

            std::stringstream ss(line);
            if (ss >> input >> expected) {
                CAPTURE(line_counter);
                CAPTURE(input);

                std::array<std::byte, 32> salt;
                boost::algorithm::unhex(input.begin(), input.end(), reinterpret_cast<char*>(salt.data()));
                std::reverse(salt.begin(), salt.end());

                auto actual = srp6.calculate_x("USERNAME123", "PASSWORD123", salt);
                std::reverse(actual.begin(), actual.end());

                CHECK(to_hex(actual) == expected);
            }
        }
    }
}