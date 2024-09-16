#include "grid.h"
void NumberGrid::initializeClues() {
    functionMap[0] = [this]() -> bool {
        return validateNumbers(separateIntoDecimals(grid[0]), isPerfectSquare);
    };
    functionMap[1] = [this]() -> bool {
        vector<long long> nums = separateIntoDecimals(grid[1]);
        for(long long& num : nums) {
            num -= 1;
        }
        return validateNumbers(nums, isPalindrome);
    };
    functionMap[2] = [this]() -> bool {
        return validateNumbers(separateIntoDecimals(grid[2]), isPrimePowerPrime);
    };
    functionMap[3] = [this]() -> bool {
        return validateDigits(separateIntoDigits(grid[3]), isDigitSum7);
    };
    functionMap[4] = [this]() -> bool {
        return validateNumbers(separateIntoDecimals(grid[4]), isFibonacci);
    };
    functionMap[5] = [this]() -> bool {
        return validateNumbers(separateIntoDecimals(grid[5]), isPerfectSquare);
    };
    functionMap[6] = [this]() -> bool {
        return validateNumbers(separateIntoDecimals(grid[6]), [this](long long n) -> bool {
            return isMultipleOfNum(n, 37);
        });
    };
    functionMap[7] = [this]() -> bool {
        return validateNumbers(separateIntoDecimals(grid[7]), [this](long long n) -> bool {
            return isPalindrome(n) && isMultipleOfNum(n, 23);
        });
    };
    functionMap[8] = [this]() -> bool {
        return validateDigits(separateIntoDigits(grid[8]), isProductOfDigitsEnd1);
    };
    functionMap[9] = [this]()->bool {
        return validateNumbers(separateIntoDecimals(grid[9]), [this](long long n) -> bool {
            return isMultipleOfNum(n, 88);
        });
    };
    functionMap[10] = [this]() -> bool {
        vector<long long> nums = separateIntoDecimals(grid[10]);
        for(long long& num : nums) {
            num += 1;
        }
        return validateNumbers(nums, isPalindrome);
    };
}

vector<long long> NumberGrid::separateIntoDecimals(const vector<int>& row) {
    vector<long long> decimalNums;
    long long num = 0;
    bool inProgress = false;
    for (long long n : row) {
        if (n == -1) {
            if (inProgress) {
                decimalNums.push_back(num);
                inProgress = false;
            }
            num = 0;
        } else {
            if (!inProgress) {
                inProgress = true;
                num = n;
            } else {
                num = num * 10 + n;
            }
        }
    }
    if (inProgress) {
        decimalNums.push_back(num);
    }

    return decimalNums;
}

vector<vector<int>> NumberGrid::separateIntoDigits(const vector<int>& row) {
    vector<vector<int>> digitNums;
    vector<int> digits;
    bool inProgress = false;
    for (int n : row) {
        if (n == -1) {
            if(inProgress) {
                digitNums.push_back(digits);
                digits.clear();
                inProgress = false;
            }
            digits = {};
        } else {
            if(!inProgress) {
                inProgress = true;
            }
            digits.push_back(n);
        }
    }
    if (inProgress) {
        digitNums.push_back(digits);
    }
    return digitNums;
}

bool NumberGrid::isPerfectSquare(long long n) {
    auto root = static_cast<long long>(sqrt(n));
    if(root * root != n) {
        return false;
    }
    return true;
}

bool NumberGrid::isPalindrome(long long n) {
    if(n % 10 == 0 && n != 0) return false;
    if(n < 0) return false;
    long long reversed = 0;
    long long original = n;
    while(n > 0) {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }
    return original == reversed;
}

vector<long long> sieveOfEratosthenes(long long max) {
    vector<bool> is_prime(max + 1, true);
    vector<long long> primes;
    is_prime[0] = is_prime[1] = false;
    for (long long i = 2; i * i <= max; ++i) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= max; j += i)
                is_prime[j] = false;
        }
    }
    for (long long i = 2; i <= max; ++i) {
        if (is_prime[i])
            primes.push_back(i);
    }
    return primes;
}

long long intPow(long long base, long long exp) {
    if (exp < 0) {
        throw invalid_argument("Exponent must be non-negative");
    }
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

bool NumberGrid::isPrimePowerPrime(long long n) {
    if(n < 4) return false;
    vector<long long> primes = sieveOfEratosthenes(static_cast<long long>(sqrt(n)));
    for (long long base : primes) {
        for (long long exp : primes) {
            if (intPow(base, exp) > n) break;
            if (intPow(base, exp) == n) return true;
        }
    }
    return false;
}

bool NumberGrid::isDigitSum7(const vector<int>& digits) {
    int sum = 0;
    for(int digit : digits) {
        sum += digit;
    }
    return sum == 7;
}

bool NumberGrid::isFibonacci(long long n) {
    if(n > 2147482647) {
        return (n == 2971215073 || n == 4807526976 || n == 7778742049 || n == 12586269025 || n == 20365011074 ||
        n == 32951280099 || n == 53316291173 || n == 86267571272);
    }
    if(n < 0) return false;
    return isPerfectSquare(5*n*n + 4) || isPerfectSquare(5*n*n - 4);
}

bool NumberGrid::isMultipleOfNum(long long n, int num) {
    return n % num == 0;
}

bool NumberGrid::isProductOfDigitsEnd1(const vector<int>& digits) {
    int product = 1;
    for(int digit : digits) {
        product = (product * digit) % 10;
    }
    return product == 1;
}

bool NumberGrid::validateNumbers(const vector<long long>& nums, const function<bool(long long)>& condition) {
    return ranges::all_of(nums, condition);
}

bool NumberGrid::validateDigits(const vector<vector<int>>& nums, const function<bool(vector<int>)>& condition) {
    return ranges::all_of(nums, condition);
}

void NumberGrid::initializeGroups() {
    vector<Coord> group1 = {Coord(5, 0), Coord(4, 0), Coord(3, 0), Coord(2, 0), Coord(1, 0),
                            Coord(0, 0), Coord(0, 1), Coord(0, 2)};
    groupNumToCoords[1] = group1;
    for (const auto& coord : group1) {
        coordToGroupNum[coord] = 1;
    }

    vector<Coord> group2 = {Coord(0, 3), Coord(0, 4), Coord(0, 5), Coord(1, 4), Coord(1, 5),
                            Coord(2, 3), Coord(2, 4), Coord(2, 5), Coord(3, 3), Coord(3, 4),
                            Coord(4, 2), Coord(4, 3)};
    groupNumToCoords[2] = group2;
    for (const auto& coord : group2) {
        coordToGroupNum[coord] = 2;
    }

    vector<Coord> group3 = {Coord(0, 6), Coord(0, 7), Coord(1, 6), Coord(2, 6)};
    groupNumToCoords[3] = group3;
    for (const auto& coord : group3) {
        coordToGroupNum[coord] = 3;
    }

    vector<Coord> group4 = {
            Coord(0, 8), Coord(0, 9), Coord(0, 10),
            Coord(1, 7), Coord(1, 8), Coord(1, 9),
            Coord(2, 7), Coord(2, 8), Coord(2, 9),
            Coord(3, 7),
            Coord(4, 4), Coord(4, 5), Coord(4, 7),
            Coord(5, 1), Coord(5, 2), Coord(5, 3), Coord(5, 4), Coord(5, 5), Coord(5, 6), Coord(5, 7),
            Coord(6, 1), Coord(6, 2), Coord(6, 3), Coord(6, 4), Coord(6, 7),
            Coord(7, 3), Coord(7, 7), Coord(7, 8), Coord(7, 10),
            Coord(8, 7), Coord(8, 8), Coord(8, 9), Coord(8, 10),
            Coord(9, 7), Coord(9, 8), Coord(9, 9),
            Coord(10, 8), Coord(10, 9)
    };;
    groupNumToCoords[4] = group4;
    for (const auto& coord : group4) {
        coordToGroupNum[coord] = 4;
    }

    vector<Coord> group5 = {Coord(1, 1), Coord(1, 2), Coord(1, 3), Coord(2, 1), Coord(2, 2),
                            Coord(3, 1), Coord(3, 2), Coord(4, 1)};
    groupNumToCoords[5] = group5;
    for (const auto& coord : group5) {
        coordToGroupNum[coord] = 5;
    }

    vector<Coord> group6 = {Coord(1, 10), Coord(2, 10), Coord(3, 8), Coord(3, 9), Coord(3, 10),
                            Coord(4, 8), Coord(4, 10)};
    groupNumToCoords[6] = group6;
    for (const auto& coord : group6) {
        coordToGroupNum[coord] = 6;
    }

    vector<Coord> group7 = {Coord(3, 5), Coord(3, 6), Coord(4, 6)};
    groupNumToCoords[7] = group7;
    for (const auto& coord : group7) {
        coordToGroupNum[coord] = 7;
    }

    vector<Coord> group8 = {Coord(4, 9), Coord(5, 8), Coord(5, 9), Coord(6, 8), Coord(6, 9),
                            Coord(6, 10), Coord(7, 9)};
    groupNumToCoords[8] = group8;
    for (const auto& coord : group8) {
        coordToGroupNum[coord] = 8;
    }

    vector<Coord> group9 = {Coord(5, 10)};
    groupNumToCoords[9] = group9;
    for (const auto& coord : group9) {
        coordToGroupNum[coord] = 9;
    }

    vector<Coord> group10 = {Coord(6, 0), Coord(7, 0), Coord(7, 1), Coord(8, 0), Coord(8, 1),
                            Coord(9, 0), Coord(9, 3), Coord(9, 4), Coord(9, 5), Coord(10, 0), Coord(10, 1), Coord(10, 2),
                            Coord(10, 3), Coord(10, 4)};
    groupNumToCoords[10] = group10;
    for (const auto& coord : group10) {
        coordToGroupNum[coord] = 10;
    }

    vector<Coord> group11 = {Coord(6, 5), Coord(6, 6), Coord(7, 5), Coord(7, 6), Coord(8, 5),
                             Coord(8, 6), Coord(9, 6), Coord(10, 5), Coord(10, 6), Coord(10, 7)};
    groupNumToCoords[11] = group11;
    for (const auto& coord : group11) {
        coordToGroupNum[coord] = 11;
    }

    vector<Coord> group12 = {Coord(7, 2), Coord(7, 4), Coord(8, 2), Coord(8, 3), Coord(8, 4),
                             Coord(9, 1), Coord(9, 2)};
    groupNumToCoords[12] = group12;
    for (const auto& coord : group12) {
        coordToGroupNum[coord] = 12;
    }

    vector<Coord> group13 = {Coord(9, 10), Coord(10, 10)};
    groupNumToCoords[13] = group13;
    for (const auto& coord : group13) {
        coordToGroupNum[coord] = 13;
    }
}