#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <boost/multiprecision/cpp_int.hpp>

#define KEY_BYTES_SIZE 32

using BigInt = boost::multiprecision::int1024_t;

struct Point {
    int x;
    BigInt y;

    Point(int x, const BigInt& y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& stream, const Point& rhs);
};

std::string format_to_hex(const BigInt& num) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(KEY_BYTES_SIZE * 2) << num;

    return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const Point& rhs) {
    stream << rhs.x << " " << format_to_hex(rhs.y);

    return stream;
}

template <typename T>
T convert_from_hex(const std::string& hex_str) {
    std::stringstream ss;
    ss << std::hex << hex_str;

    T v;
    ss >> v;

    return v;
}

template <size_t bytes_count>
std::string generate_random_hex() {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (size_t i = 0; i < bytes_count; i++) {
        ss << std::setw(2) << rand() % 256;
    }

    return ss.str();
}

BigInt PRIME = convert_from_hex<BigInt>("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF43");

std::vector<BigInt> generate_random_vector(uint32_t size) {
    std::vector<BigInt> coefficients;
    for (uint32_t i = 0; i < size; i++) {
        BigInt random = convert_from_hex<BigInt>(generate_random_hex<KEY_BYTES_SIZE>());
        coefficients.push_back(random % PRIME);
    }

    return coefficients;
}

BigInt calculate_function(
    std::vector<BigInt>& coefficients,
    uint8_t argument
) {
    BigInt result = 0;
    size_t coefficients_count = coefficients.size();
    for (size_t i = 1; i <= coefficients_count; i++) {
        result += coefficients[i - 1] * pow(BigInt(argument), coefficients_count - i);
    }

    return result % PRIME;
}

std::vector<Point> split(
    uint8_t shares_count, 
    uint8_t entry_threshold, 
    const BigInt& secret
) {
    std::vector<BigInt> coefficients = generate_random_vector(entry_threshold - 1);
    coefficients.push_back(secret);

    std::vector<Point> result;
    for (uint8_t i = 1; i <= shares_count; i++) {
        result.push_back(Point(i, calculate_function(coefficients, i)));
    }

    return result;
}

BigInt recover(std::vector<Point>& shares) {
    auto l = [shares](int x, size_t i) -> BigInt {
        double result = 1;

        for (size_t j = 0; j < shares.size(); j++) {
            if (j != i) {
                result *= (
                    static_cast<double>(x) - 
                    static_cast<double>(shares[j].x)) / 
                    (static_cast<double>(shares[i].x) - 
                    static_cast<double>(shares[j].x)
                );
            }
        }
        
        return BigInt(result);
    };

    BigInt result = 0;
    for (size_t i = 0; i < shares.size(); i++) {
        result += shares[i].y * l(0, i);
    }

    return result % PRIME;
}

int main(int args_count, char** args_value) {
    srand(static_cast<unsigned int>(time(0)));

    if (args_count < 2) {
        return 1;
    }

    std::string first_argument(args_value[1]);

    if (first_argument == "recover") {
        int shares_count = 0;
        std::cin >> shares_count;

        std::vector<Point> shares;
        for (int i = 0; i < shares_count; i++) {
            int x = 0;
            std::string y;

            std::cin >> x >> y;
            shares.push_back(Point(x, convert_from_hex<BigInt>(y)));
        }

        BigInt result = recover(shares);

        std::cout << format_to_hex(result) << std::endl;
    } else if (first_argument == "split") {
        int shares_count = 0;
        int entry_threshold = 0;
        std::string string_secret;

        std::cin >> shares_count >> entry_threshold >> string_secret;

        if (shares_count < entry_threshold) {
            std::cout << "Shares count must be higher then entry threshold" << std::endl;
            return 1;
        }

        BigInt secret = convert_from_hex<BigInt>(string_secret);

        if (secret > PRIME) {
            std::cout << "Your secret must be less than prime number" << std::endl;
            return 1;
        }

        auto shares = split(shares_count, entry_threshold, secret);

        for (const auto& point : shares) {
            std::cout << point << std::endl;
        }
    } else {
        std::cout << "Unknow arguments" << std::endl;
    }
}