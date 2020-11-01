//
//  bigint_binary_version.h
//  Speedwaagonnnnnnnm
//
//  Created by Александр Фреик on 24.03.2020.
//  Copyright © 2020 Freik. All rights reserved.
//

#ifndef biginteger_h
#define biginteger_h

#include<iostream>
#include<vector>
#include<string>
#include <cassert>

class BigInteger {
friend bool operator==(BigInteger const &num1, BigInteger const &num2);
friend bool operator<(BigInteger const &num1, BigInteger const &num2);
friend BigInteger operator*(BigInteger const &num1, BigInteger const &num2);
    static const uint8_t KARATSUBA_START_LENGTH{0};
    static const uint64_t BASE{static_cast<uint64_t>(1) << 32};
    static const uint8_t BASE_DEGREE{32};
    static const uint32_t BASE_MASK{4294967295};
    /*
     0 - ""
     sign = -1 for num<0, 0 for num = 0, 1 for num >0
     */
    int sign{0};
    std::vector<uint32_t> body;
    bool abs_lesser_than(BigInteger const *other) const;  //
    BigInteger& add_module(BigInteger const *other);  //
                // sign will be same for this or 1 if this->sign == 0
    BigInteger& subtract_smaller_module(BigInteger const *other);  //
                // sign will be same for this or zero
    BigInteger digits_before_inclusively(size_t n) const;
          // if n == 0, ans == 0, if n > body_size, ans = this
    BigInteger digits_after_noninclusively(size_t n) const;
                              // if n >= body_size, ans = 0
    BigInteger& right_shift(size_t n);
    BigInteger& left_shift(size_t n);  //
    BigInteger& multiplication_in_column(BigInteger const *other);  //
    BigInteger& uint32_plus_eq(uint32_t num);  //
    BigInteger& uint32_mult_eq(uint32_t num);  //
    BigInteger uint32_mult(uint32_t num) const;
    bool check() const;
    void make_octal_body(std::vector<uint16_t> &octal_body) const;
  public:
    explicit BigInteger(std::string const &s_num);
    BigInteger() = default;
    explicit BigInteger(const char* len) :
                BigInteger(static_cast<std::string>(len)) {}
    BigInteger(int32_t i32_num);
    BigInteger(int64_t i64_num);
    BigInteger(uint32_t u32_num);
    BigInteger(uint64_t u64_num);
    BigInteger(BigInteger const &other) :
                sign(other.sign), body(other.body) {}
    bool is_null() const { return !sign; }
    bool is_negative() const {
        assert(this->check());
        return this->sign == -1;
    }
    bool is_positive() const {
        assert(this->check());
        return this->sign == 1;
    }
    std::string toString() const;
    BigInteger abs() const;
    BigInteger operator+() const;  //
    BigInteger operator-() const;  //
    BigInteger& operator++();
    BigInteger& operator--();
    BigInteger operator++(int);
    BigInteger operator--(int);
    BigInteger& operator=(BigInteger const &other);  //
    BigInteger& operator&=(BigInteger const &other);  //
    BigInteger& operator|=(BigInteger const &other);  //
    BigInteger& operator^=(BigInteger const &other);  //
    BigInteger& operator<<=(size_t n);  //
    BigInteger& operator>>=(size_t n);
    BigInteger& operator+=(BigInteger const &other);  //
    BigInteger& operator-=(BigInteger const &other);  //
    BigInteger& operator*=(BigInteger const &other);  //
    BigInteger& operator/=(BigInteger const &other);  //
    BigInteger& operator%=(BigInteger const &other);  //
    explicit operator bool() const {  // there can be mistake with
      return !this->is_null();  // convet to int
    }  //
    void print_num_body() const {
        for (auto num : body)
            std::cout << num << " ";
        if (sign == -1) std::cout << '-';
    }
};
// ---------------------funk definitions----------------------------
bool operator==(BigInteger const &num1, BigInteger const &num2);
bool operator!=(BigInteger const &num1, BigInteger const &num2);
bool operator<(BigInteger const &num1, BigInteger const &num2);
bool operator>(BigInteger const &num1, BigInteger const &num2);
bool operator<=(BigInteger const &num1, BigInteger const &num2);
bool operator>=(BigInteger const &num1, BigInteger const &num2);
BigInteger operator+(BigInteger const &num1, BigInteger const &num2);
BigInteger operator-(BigInteger const &num1, BigInteger const &num2);
BigInteger operator*(BigInteger const &num1, BigInteger const &num2);
BigInteger operator/(BigInteger const &num1, BigInteger const &num2);
BigInteger operator%(BigInteger const &num1, BigInteger const &num2);
BigInteger operator<<(BigInteger const &num1, size_t n);
BigInteger operator>>(BigInteger const &num1, size_t n);

// ---------------------class constructors--------------------------
BigInteger::BigInteger(std::string const &s_num) {
    assert(s_num != "" && s_num != "+" && s_num != "-");
    //change it
    int8_t num_sign{1};
    size_t cursor{0};
    if (*s_num.begin() == '+') {
        ++cursor;
        num_sign = 1;
    }
    if (*s_num.begin() == '-') {
        ++cursor;
        num_sign = -1;
    }
    // to work with numbers zeros in begin like -0003, +0987
    for (; cursor != s_num.size() && s_num[cursor] == '0'; ++cursor);
    for (; cursor != s_num.size(); ++cursor) {
        int c_i = s_num[cursor] - '0';
        assert((c_i >= 0 && c_i <= 9) || std::cerr << s_num << " "
               << s_num.size() << " " << static_cast<int>(s_num[cursor])
               << ' ' << static_cast<int>('0') << ' ' << static_cast<int>(' ')
               << ' ' << cursor << std::endl);  // num_body assigment
        assert(c_i >= 0 && c_i <= 9);  // num_body assigment
        this->uint32_mult_eq(10U);
        assert(this->check());
        this->uint32_plus_eq(c_i);
        assert(this->check());
    }
    if (body.empty())
        this->sign = 0;
    else
        this->sign = num_sign;
}
BigInteger::BigInteger(int32_t i32_num) :
        sign(i32_num == 0 ? 0 : (i32_num > 0 ? 1 : -1)) {
    if (sign) body.push_back(i32_num * sign);
}
BigInteger::BigInteger(int64_t i64_num) :
        sign(i64_num == 0 ? 0 : (i64_num > 0 ? 1 : -1)) {
    i64_num *= sign;
    assert(i64_num > 0);
    uint64_t u64_num = static_cast<uint64_t>(i64_num);
    if (u64_num) body.push_back(u64_num & BASE_MASK);
    u64_num >>= BASE_DEGREE;
    if (u64_num & BASE_MASK) body.push_back(u64_num & BASE_MASK);
}
BigInteger::BigInteger(uint32_t u32_num) : sign(u32_num != 0) {
    if (sign) body.push_back(u32_num * sign);
}
BigInteger::BigInteger(uint64_t u64_num) : sign(u64_num != 0) {
    if (u64_num) body.push_back(u64_num & BASE_MASK);
    u64_num >>= BASE_DEGREE;
    if (u64_num & BASE_MASK) body.push_back(u64_num & BASE_MASK);
}

// -----------------------class operators-------------------
BigInteger BigInteger::operator-() const {
    assert(this->check());
    BigInteger ans(*this);
    ans.sign *= -1;
    return ans;
}  // little check
BigInteger BigInteger::operator+() const {
    return *this;
}  // little check

BigInteger& BigInteger::operator++() {
    assert(this->check());
    return this->uint32_plus_eq(1);
}
BigInteger& BigInteger::operator--() {  // little check
    assert(this->check());
    this->sign *= -1;
    ++(*this);
    this->sign *= -1;
    return *this;
}  // little check
BigInteger BigInteger::operator++(int) {
    assert(this->check());
    BigInteger ans(*this);
    ++(*this);
    return ans;
}  // little check
BigInteger BigInteger::operator--(int) {
    assert(this->check());
    BigInteger ans(*this);
    --(*this);
    return ans;
}  // little check
BigInteger& BigInteger::operator=(BigInteger const &other) {
    assert(this->check() && other.check());
    this->body = other.body;
    this->sign = other.sign;
    return *this;
}
BigInteger& BigInteger::operator&=(BigInteger const &other) {
    assert(this->check() && other.check());
    int min_size_num{this->body.size() <= other.body.size() ? 1 : 2};
    size_t min_size{min_size_num == 1 ? this->body.size() : other.body.size()};
    for (size_t i = 0; i < min_size; ++i)
        this->body[i] &= other.body[i];
    for (; min_size > 0 && this->body[min_size - 1] == 0; --min_size);
    this->body.resize(min_size);
    return *this;
}
BigInteger& BigInteger::operator|=(BigInteger const &other) {
    assert(this->check() && other.check());
    int min_size_num{this->body.size() <= other.body.size() ? 1 : 2};
    size_t min_size{min_size_num == 1 ? this->body.size() : other.body.size()};
    for (size_t i = 0; i < min_size; ++i)
        this->body[i] |= other.body[i];
    for (; min_size > 0 && this->body[min_size - 1] == 0; --min_size);
    this->body.resize(min_size);
    return *this;
}
BigInteger& BigInteger::operator^=(BigInteger const &other) {
    assert(this->check() && other.check());
    int min_size_num{this->body.size() <= other.body.size() ? 1 : 2};
    size_t min_size{min_size_num == 1 ? this->body.size() : other.body.size()};
    for (size_t i = 0; i < min_size; ++i)
        this->body[i] ^= other.body[i];
    for (; min_size > 0 && this->body[min_size - 1] == 0; --min_size);
    this->body.resize(min_size);
    return *this;
}
BigInteger& BigInteger::operator<<=(size_t n) {
    assert(this->check());
    return this->left_shift(n);
}
BigInteger& BigInteger::operator>>=(size_t n) {
    assert(this->check());
    return this->right_shift(n);
}
BigInteger& BigInteger::operator+=(BigInteger const &other) {
    assert(this->check() && other.check());
    // for numbers with same sign or one zero
    if (!this->sign) {
        *this = other;
    } else if(!other.sign) {
        return *this;
    } else if (this->sign == other.sign) {
        this->add_module(&other);
    } else {
        int max_abs_num = this->abs_lesser_than(&other) ? 2 : 1;
        if (max_abs_num == 1) {
            this->subtract_smaller_module(&other);
        } else {
            //change it
            //this->sign = other.sign;
            BigInteger ans_sum(other);
            ans_sum.subtract_smaller_module(this);
            *this = ans_sum;
        }
    }
    return *this;
}
BigInteger& BigInteger::operator-=(BigInteger const &other) {
    assert(this->check() && other.check());
    this->sign *= -1;
    *this += other;
    this->sign *= -1;
    return *this;
}
BigInteger& BigInteger::operator*=(BigInteger const &other) {
    assert(this->check() && other.check());
    /*
    // change to decrise coping
    //change it
    // to make end recursion better
    if (this->is_null() || other.is_null()) {
        *this = BigInteger();
        return *this;
    } else if ((this->body.size() < KARATSUBA_START_LENGTH &&
        other.body.size() < KARATSUBA_START_LENGTH) ||
        (this->body.size() < 2 || other.body.size() < 2)) {
        this->multiplication_in_column(&other);
        return *this;
    }
    BigInteger ans(*this);
    this->sign *= other.sign;
    auto max_size = (this->body.size() > other.body.size()) ?
                        this->body.size() : other.body.size();
    auto half_size = max_size / 2;
    //  half_size == hs
    //  num1 = a1*base^hs + a0; num2 = b1*base^hs + b0
    //  num1 * num2 = a1*b1*hs^2 + ((a1+a0)(b1+b0) - a1*b1 - a0*b0)*hs + a0*b0
    BigInteger a0(this->digits_before_inclusively(half_size));
    BigInteger a1(*this >> (half_size * BASE_DEGREE));
    BigInteger b0(other.digits_before_inclusively(half_size)),
                b1(other >> (half_size * BASE_DEGREE));
    BigInteger a10b10((a1 + a0) *= (b1 + b0));
    a1 *= b1;
    a0 *= b0;
    ((*this = (a1 << (half_size * 2 * BASE_DEGREE))) +=
            ((a10b10 - a1 - a0) << (half_size * BASE_DEGREE))) +=
                a0;
    return *this;*/
    return *this = *this * other;
}
BigInteger& BigInteger::operator/=(BigInteger const &other) {
    assert(this->check() && other.check());
    assert(!other.is_null());
    int64_t length_diff = this->body.size();
    length_diff -= other.body.size();
    if (length_diff < 0) {
        this->body.clear();
        this->sign = 0;
        return *this;
    }
    BigInteger curr_summ;
    std::vector<uint32_t> ans_body;
    uint32_t up_edge = BASE - 1, down_edge = 0, mid_edge, diff;
    for (auto it = this->body.rbegin(); it != this->body.rend(); ++it) {
        curr_summ.left_shift(BASE_DEGREE).uint32_plus_eq(*it);
        up_edge = BASE - 1;
        down_edge = 0;
        while (up_edge > down_edge) {
            diff = up_edge - down_edge;
            if (diff == 1) {
                if (other.abs().uint32_mult_eq(up_edge) > curr_summ)
                    up_edge = down_edge;
                else
                    down_edge = up_edge;
                curr_summ -= other.abs().uint32_mult_eq(down_edge);
                break;
            }
            diff /= 2;
            mid_edge = down_edge + diff;
            if (other.abs().uint32_mult_eq(mid_edge) > curr_summ)
                up_edge = mid_edge;
            else
                down_edge = mid_edge;
        }
        ans_body.push_back(down_edge);
    }
    // to erase first zeros in ansvek
    size_t first_zeros_num = 0;
    for (auto it = ans_body.begin(); it != ans_body.end() && *it == 0;
         ++it, ++first_zeros_num);
    this->body.clear();
    if (ans_body.size() - first_zeros_num == 0) {
        this->sign = 0;
        return *this;
    }
    for (auto it = ans_body.rbegin(); it != ans_body.rend() - first_zeros_num;
         ++it) {
        this->body.push_back(*it);
        
    }
    this->sign *= other.sign;
    return *this;
}
BigInteger& BigInteger::operator%=(BigInteger const &other) {
    assert(this->check() && other.check());
    assert(!other.is_null());
    BigInteger div(*this / other);
    *this -= div * other;
    return *this;
}
//---------------------class inner funk---------------------
bool BigInteger::check() const {
    // check that sign is ok, and last digit isn't 0
    if (sign == 0 || body.empty()) return !sign && body.empty();
    bool ans{true};
    ans = ans && (sign == -1 || sign == 1);
    ans = ans && *body.rbegin();
    return ans;
}
std::string BigInteger::toString() const {  // little check
    assert(this->check());
    if (!*this) return "0";
    std::vector<uint16_t> octal_body;
    this->make_octal_body(octal_body);
    std::vector<uint16_t> diff_dec_body(octal_body.size() + 1, 0);
    for (size_t i = octal_body.size(); i > 1; --i) {
        // double first
        // k = octal_body.size() - i + 1;
        uint16_t remainder = 0;
        for (size_t j = i - 1; j != octal_body.size(); ++j) {
            remainder += octal_body[j] * 2;
            diff_dec_body[(j + 1) - i] = remainder % 10;
            remainder /= 10;
        }
        // then diff
        diff_dec_body[octal_body.size() - i + 1] = remainder;
        remainder = 0;
        int64_t diff_remainder = 0;
        for (size_t j = i - 2; j != octal_body.size(); ++j) {
            (diff_remainder += octal_body[j]) -= diff_dec_body[(j + 2) - i];
            octal_body[j] = (diff_remainder + 10) % 10;
            if (diff_remainder < 0)  diff_remainder = -1;
            else                     diff_remainder /= 10;
        }
    }
    // now octal body transformed to dec body
    std::string ans = (sign == -1 ? "-" : "");
    // to work with first zeros
    size_t dig_begin = octal_body.size();
    for (; dig_begin > 0 && octal_body[dig_begin - 1] == 0; --dig_begin);
    for (;dig_begin > 0; --dig_begin)
        ans += static_cast<char>(octal_body[dig_begin - 1] + '0');
    return ans;
}
void BigInteger::
make_octal_body(std::vector<uint16_t> &octal_body) const {
    assert(this->check());
    octal_body.clear();
    int bit_cnt = 0;
    uint32_t octal_mask = 7U;
    uint16_t remainder = 0;
    for (size_t i = 0; i < this->body.size(); ++i) {
        octal_mask = (7U << bit_cnt);
        if (bit_cnt) {
            remainder += (this->body[i] & ((1U << bit_cnt) - 1)) << (3 - bit_cnt);
            octal_body.push_back(remainder);
        }
        remainder = 0;
        for (size_t j = bit_cnt; j < 30 ; j += 3) {
           octal_body.push_back((body[i] & octal_mask) >> j);
           octal_mask <<= 3;
        }
        ++bit_cnt;
        bit_cnt %= 3;
        if (bit_cnt)
            remainder += (body[i] & octal_mask) >> (29 + bit_cnt);
    }
    bit_cnt %= 3;
    if (bit_cnt)
        octal_body.push_back(remainder);
}
BigInteger& BigInteger::uint32_plus_eq(uint32_t num) {
    assert(this->check());
    if (num == 0) return *this;
    if (this->sign > 0) {
        uint64_t remainder = num;
        for (size_t i = 0; i < this->body.size() && remainder; ++i) {
            remainder += this->body[i];
            this->body[i] = remainder & BASE_MASK;
            remainder >>= BASE_DEGREE;
        }
        assert(remainder < BASE);
        if (remainder)
            this->body.push_back(remainder & BASE_MASK);
    } else {
        int64_t remainder = num;
        // now remainder != 0
        if (this->body.empty()) {
            this->body.push_back(num);
            this->sign = 1;
            return *this;
        }
        if (this->body.size() == 1) {
            remainder -= this->body[0];
            if (remainder < 0) {
                this->body[0] = static_cast<uint32_t>(-remainder);
            } else if (remainder > 0) {
                this->body[0] = static_cast<uint32_t>(remainder);
                this->sign = 1;
            } else {
                this->body.clear();
                this->sign = 0;
            }
            remainder = 0;
        }
        for (size_t i = 0; i < this->body.size() && remainder; ++i) {
            remainder -= this->body[i];
            if (remainder <= 0) {
                this->body[i] = static_cast<uint32_t>(-remainder);
                remainder = 0;
            } else {
                this->body[i] = static_cast<uint32_t>(BASE - remainder);
                remainder = 1;
            }
        }
        assert(remainder == 0);
        //assert(bo);
        for (auto it = this->body.rbegin();
             it != this->body.rend() && *it == 0; ++it)
            this->body.pop_back();
        if (this->body.empty()) this->sign = 0;
    }
    return *this;
}
BigInteger& BigInteger::uint32_mult_eq(uint32_t num) {
    //assert(BASE > num);
    assert(this->check());
    if (num == 0 || this->is_null()) {
        this->body.clear();
        this->sign = 0;
        return *this;
    }
    // now ans != 0 for sure
    uint64_t mult_remainder = 0;
    for (size_t i = 0; i < this->body.size(); ++i) {
        mult_remainder += this->body[i] * static_cast<uint64_t>(num);
        this->body[i] = mult_remainder & BASE_MASK;
        mult_remainder >>= BASE_DEGREE;
    }
    assert(mult_remainder < BASE);
    if (mult_remainder > 0) this->body.push_back(mult_remainder & BASE_MASK);
    return *this;
}
BigInteger BigInteger::uint32_mult(uint32_t num) const {
    assert(this->check());
    BigInteger ans_mult;
    if (num == 0 || this->is_null()) {
        return ans_mult;
    }
    // now ans != 0 for sure
    ans_mult.sign = this->sign;
    uint64_t mult_remainder = 0;
    for (size_t i = 0; i < this->body.size(); ++i) {
        mult_remainder += this->body[i] * static_cast<uint64_t>(num);
        ans_mult.body.push_back(mult_remainder & BASE_MASK);
        mult_remainder >>= BASE_DEGREE;
    }
    assert(mult_remainder < BASE);
    if (mult_remainder > 0) ans_mult.body.push_back(mult_remainder & BASE_MASK);
    return ans_mult;
}

BigInteger BigInteger::abs() const {
    BigInteger ans(*this);
    ans.sign = ans.sign * ans.sign;
    return ans;
}
BigInteger& BigInteger::add_module(BigInteger const *other) {
    /* if this == 0, after operation this->sign = 1 */
    assert(this->check() && other->check());
    int max_size_num = (this->body.size() > other->body.size()) ? 1 : 2;
    size_t min_size = (max_size_num == 2) ?
                                this->body.size() : other->body.size();
    size_t max_size = (max_size_num == 1) ?
                                this->body.size() : other->body.size();
    uint64_t remainder = 0;
    for (size_t i = 0; i < min_size; ++i) {
        (remainder += this->body[i]) += other->body[i];
        this->body[i] = remainder & BASE_MASK;
        remainder >>= BASE_DEGREE;
    }
    if (max_size_num == 1) {
        for (size_t i = min_size; i < max_size; ++i) {
            remainder += this->body[i];
            this->body[i] = remainder & BASE_MASK;
            remainder >>= BASE_DEGREE;
        }
    } else {
        for (size_t i = min_size; i < max_size; ++i) {
            remainder += other->body[i];
            this->body.push_back(remainder & BASE_MASK);
            remainder >>= BASE_DEGREE;
        }
    }
    if (remainder != 0)
        this->body.push_back(remainder & BASE_MASK);
    if (this->sign == 0 && !this->body.empty())
        this->sign = 1;
    return *this;
}
BigInteger& BigInteger::subtract_smaller_module(BigInteger const *other) {
    assert(this->check() && other->check());
    int max_size_num = (this->body.size() >= other->body.size()) ? 1 : 2;
    assert(max_size_num == 1);
    size_t min_size = other->body.size();
    size_t max_size = this->body.size();
    int64_t remainder = 0;
    for (size_t i = 0; i < min_size; ++i) {
        (remainder += this->body[i]) -= other->body[i];
        if (remainder >= 0) {
            this->body[i] = static_cast<uint32_t>(remainder);
            remainder = 0;
        } else {
            this->body[i] = static_cast<uint32_t>(BASE + remainder);
            remainder = -1;
        }
    }
    for (size_t i = min_size; i < max_size; ++i) {
        remainder += this->body[i];
        if (remainder >= 0) {
            this->body[i] = static_cast<uint32_t>(remainder);
            break;
        } else {
            this->body[i] = static_cast<uint32_t>(BASE + remainder);
            remainder = -1;
        }
    }
    for (size_t i = max_size ; i > 0 && this->body[i - 1] == 0; --i)
        this->body.pop_back();
    if (this->body.empty()) this->sign = 0;
    return *this;
}
bool BigInteger::abs_lesser_than(BigInteger const *other) const {
    assert(this->check() && other->check());
    int max_size_num = (this->body.size() > other->body.size()) ? 1 : 2;
    size_t min_size = (max_size_num == 2) ?
                                this->body.size() : other->body.size();
    size_t max_size = (max_size_num == 1) ?
                                this->body.size() : other->body.size();
    if (max_size > min_size)
        return (max_size_num == 2);
    for (size_t i = min_size; i > 0; --i) {
        if (this->body[i - 1] <  other->body[i - 1]) return true;
        else if (this->body[i - 1] >  other->body[i - 1]) return false;
    }
    return false;
}
BigInteger& BigInteger::left_shift(size_t n) {
    // chenge funk length
    assert(this->check());
    if (this->is_null() || n == 0) return *this;
    size_t dig_num{n / BASE_DEGREE};
    size_t old_body_size = this->body.size();
    uint32_t bits_num(n & (BASE_DEGREE - 1));
    if (!bits_num) {
        if (dig_num >= old_body_size) {
            for (size_t added_zeros = 0; added_zeros < dig_num - old_body_size; ++added_zeros)
                this->body.push_back(0);
            for (size_t i = 0; i < old_body_size; ++i) {
                this->body.push_back(this->body[i]);
                this->body[i] = 0;
            }
        } else {
            for (size_t i = 0; i < dig_num; ++i)
                this->body.push_back(this->body[i + old_body_size - dig_num]);
            assert(old_body_size > 0 && dig_num > 0);
            for (size_t i = old_body_size - 1; i >= dig_num; --i)
                this->body[i] = this->body[i - dig_num];
            for (size_t i = dig_num; i > 0; --i)
                this->body[i - 1] = 0;
        }
    }/* else {
        uint32_t mask = (1 << bits_num) - 1;
        uint32_t remainder1{0}, remainder2{0};
        if (dig_num >= old_body_size) {
            for (size_t added_zeros = 0;
                 added_zeros < dig_num - old_body_size; ++added_zeros)
                this->body.push_back(0);
            for (size_t i = 0; i < old_body_size; ++i) {
                remainder1 = this->body[i] >> bits_num;
                this->body.push_back(
                    ((this->body[i] & mask) << (BASE_DEGREE - bits_num))
                                     + remainder2);
                remainder2 = remainder1;
                this->body[i] = 0;
            }
            if (remainder2) this->body.push_back(remainder2);
        } else {
            // change to one line instead of two line
            for (size_t i = 0; i < dig_num; ++i)
                this->body.push_back(this->body[i + old_body_size - dig_num]);
            assert(old_body_size > 0 && dig_num > 0);
            for (size_t i = old_body_size - 1; i >= dig_num; --i)
                this->body[i] = this->body[i - dig_num];
            for (size_t i = dig_num; i > 0; --i)
                this->body[i - 1] = 0;
            //
            
            for (size_t i = dig_num; i < this->body.size(); ++i) {
                remainder1 = this->body[i] >> bits_num;
                this->body[i] =
                ((this->body[i] & mask) << (BASE_DEGREE - bits_num))
                                     + remainder2;
                remainder2 = remainder1;
                this->body[i] = 0;
            }
            if (remainder2) this->body.push_back(remainder2);
        }
    }*/
    else assert(false);
    return *this;
}
BigInteger& BigInteger::right_shift(size_t n) {
    assert(this->check());
    // chenge funk length
    size_t dig_num{n / BASE_DEGREE};
    size_t old_body_size = this->body.size();
    if (this->is_null() || dig_num == 0 || dig_num >= this->body.size())
        return *this = BigInteger();
    uint32_t bits_num(n & (BASE_DEGREE - 1));
    if (! bits_num) {
        for (size_t i = dig_num; i < old_body_size; ++i)
            this->body[i - dig_num] = body[i];
        this->body.resize(old_body_size - dig_num);
    } /*else {
        uint32_t mask = (1 << bits_num) - 1;
        uint32_t remainder1{0}, remainder2{0};
        if (dig_num >= old_body_size) {
            for (size_t added_zeros = 0;
                 added_zeros < dig_num - old_body_size; ++added_zeros)
                this->body.push_back(0);
            for (size_t i = 0; i < old_body_size; ++i) {
                remainder1 = this->body[i] >> bits_num;
                this->body.push_back(
                    ((this->body[i] & mask) << (BASE_DEGREE - bits_num))
                                     + remainder2);
                remainder2 = remainder1;
                this->body[i] = 0;
            }
            if (remainder2) this->body.push_back(remainder2);
        } else {
            // change to one line instead of two line
            for (size_t i = 0; i < dig_num; ++i)
                this->body.push_back(this->body[i + old_body_size - dig_num]);
            assert(old_body_size > 0 && dig_num > 0);
            for (size_t i = old_body_size - 1; i >= dig_num; --i)
                this->body[i] = this->body[i - dig_num];
            for (size_t i = dig_num; i > 0; --i)
                this->body[i - 1] = 0;
            //
            
            for (size_t i = dig_num; i < this->body.size(); ++i) {
                remainder1 = this->body[i] >> bits_num;
                this->body[i] =
                ((this->body[i] & mask) << (BASE_DEGREE - bits_num))
                                     + remainder2;
                remainder2 = remainder1;
                this->body[i] = 0;
            }
            if (remainder2) this->body.push_back(remainder2);
        }
    }*/
    else assert(false);
    return *this;
}
BigInteger BigInteger::digits_before_inclusively(size_t n) const {
    assert(this->check());
    BigInteger ans;
    if (n == 0 || this->is_null())
        return ans;
    n = (n < this->body.size()) ? n : this->body.size();
    // to work with zeros in mid
    for (; n > 0 && this->body[n - 1] == 0; --n);
    for (size_t i = 0; i < n; ++i)
        ans.body.push_back(this->body[i]);
    if (!ans.body.empty())
        ans.sign = this->sign;
    return ans;
}  // very lit check

BigInteger& BigInteger::multiplication_in_column(BigInteger const *other) {
    assert(this->check() && other->check());
    BigInteger ans;
    for (size_t i = 0; i < other->body.size(); ++i) {
        ans += this->uint32_mult(other->body[i]).left_shift(i * BASE_DEGREE);
    }
    this->body = ans.body;
    this->sign *= other->sign;
    return *this;
}

//-------------------operators with class elem---------------------
BigInteger operator&(BigInteger const &num1, BigInteger const &num2) {
    return BigInteger(num1) &= num2;
}
BigInteger operator|(BigInteger const &num1, BigInteger const &num2) {
    return BigInteger(num1) |= num2;
}
BigInteger operator^(BigInteger const &num1, BigInteger const &num2) {
    return BigInteger(num1) ^= num2;
}
BigInteger operator<<(BigInteger const &num1, size_t n) {
    return BigInteger(num1) <<= n;
}
BigInteger operator>>(BigInteger const &num1, size_t n) {
    return BigInteger(num1) >>= n;
}
BigInteger operator+(BigInteger const &num1, BigInteger const &num2) {
    return BigInteger(num1) += num2;
}
BigInteger operator-(BigInteger const &num1, BigInteger const &num2) {
    return BigInteger(num1) -= num2;
}
BigInteger operator*(BigInteger const &num1, BigInteger const &num2) {
    assert(num1.check() && num2.check());
    // change to decrise coping
    //change it
    // to make end recursion better
    BigInteger ans;
    if (num1.is_null() || num2.is_null()) {
        return ans;
    } else if ((num1.body.size() < num1.KARATSUBA_START_LENGTH &&
        num2.body.size() < num1.KARATSUBA_START_LENGTH) ||
        (num1.body.size() < 2 || num2.body.size() < 2)) {
        ans = num1;
        ans.multiplication_in_column(&num2);
        return ans;
    }
    auto max_size = (num1.body.size() > num2.body.size()) ?
                        num1.body.size() : num1.body.size();
    auto half_size = max_size / 2;
    //  half_size == hs
    //  num1 = a1*base^hs + a0; num2 = b1*base^hs + b0
    //  num1 * num2 = a1*b1*hs^2 + ((a1+a0)(b1+b0) - a1*b1 - a0*b0)*hs + a0*b0
    BigInteger a0(num1.digits_before_inclusively(half_size));
    BigInteger a1(num1 >> (half_size * num1.BASE_DEGREE));
    BigInteger b0(num2.digits_before_inclusively(half_size)),
                b1(num2 >> (half_size * num2.BASE_DEGREE));
    BigInteger a10b10((a1 + a0) *= (b1 + b0));
    a1 *= b1;
    a0 *= b0;
    ((ans = (a1 << (half_size * 2 * num2.BASE_DEGREE))) +=
            ((a10b10 - a1 - a0) << (half_size * num2.BASE_DEGREE))) +=
                a0;
    ans.sign = num1.sign * num2.sign;
    return ans;
}
BigInteger operator/(BigInteger const &num1, BigInteger const &num2) {
    return BigInteger(num1) /= num2;
}
BigInteger operator%(BigInteger const &num1, BigInteger const &num2) {
    return BigInteger(num1) %= num2;
}
//------------------------compare operators---------------------
bool operator==(BigInteger const &num1, BigInteger const &num2) {
    //change
    return (num1 - num2).is_null();
}  // little check, t check
bool operator!=(BigInteger const &num1, BigInteger const &num2) {
    return !(num1 == num2);
}
bool operator<(BigInteger const &num1, BigInteger const &num2) {
    //change
    return (num1 - num2).is_negative();
}  // little check, t check
bool operator>(BigInteger const &num1, BigInteger const &num2) {
    return !(num1 < num2) && num1 != num2;
}
bool operator<=(BigInteger const &num1, BigInteger const &num2) {
    return num1 == num2 || num1 < num2;
}
bool operator>=(BigInteger const &num1, BigInteger const &num2) {
    return num1 == num2 || num1 > num2;
}
std::ostream &operator<<(std::ostream &os, const BigInteger &num) {
    os << num.toString();
    return os;
}
std::istream &operator>>(std::istream &is, BigInteger &num) {
    std::string s_num;
    is >> s_num;
    if (is) num = static_cast<BigInteger>(s_num);
    else    num = BigInteger();
    return is;
}
//--------------end of bigint---------------------




class Rational {
friend bool operator==(Rational const &num1, Rational const &num2);  //
friend bool operator<(Rational const &num1, Rational const &num2);  //
    BigInteger numerator, denominator;  // den always > 0
    bool quick_check() const;
    bool full_check() const;
    void make_inter_prime();

  public:
    Rational() : numerator(0U), denominator(1U) {}
    Rational(int i_num) : numerator(BigInteger(i_num)), denominator(1U) {}
    Rational(uint32_t ui32_num) : numerator(BigInteger(ui32_num)),
                                                        denominator(1U) {}
    Rational(BigInteger const &bi_num) : numerator(bi_num), denominator(1U) {}
    Rational(Rational const &num) : numerator(num.numerator),
                                                denominator(num.denominator) {}
    Rational(BigInteger bi_num1, BigInteger bi_num2);
    Rational(std::string s_bi_num1, std::string s_bi_num2) :
            Rational(BigInteger(s_bi_num1), BigInteger(s_bi_num2)) {}
    BigInteger gcd() const;  // numer and denumer gcd
    std::string toString() const;  //
    std::string asDecimal(size_t precision) const;  //
    Rational operator+() const;  //
    Rational operator-() const;  //
    Rational& operator=(Rational const &other);  //
    Rational& operator+=(Rational const &other);  //
    Rational& operator-=(Rational const &other);  //
    Rational& operator*=(Rational const &other);  //
    Rational& operator/=(Rational const &other);  //
    explicit operator bool() const {
        // there can be mistake with convet to int
        return !this->numerator.is_null();
    }  //
    explicit operator double() const;
};
// ---------------------class constructors--------------------------
Rational::Rational(BigInteger bi_num1, BigInteger bi_num2) {
    assert(!bi_num2.is_null());
    if (bi_num2 < 0) {
        numerator = -bi_num1;
        denominator = -bi_num2;
    } else {
        numerator = bi_num1;
        denominator = bi_num2;
    }
    this->make_inter_prime();
    assert(denominator.is_positive());
}  // little check
// -----------------------class operators-------------------
Rational::operator double() const {
    double ans = 0;
    std::string s_num(this->asDecimal(7));
    assert(!s_num.empty());
    bool negative = (s_num[0] == '-');
    int dig_cnt = -7;
    assert(s_num.size() > 2 || !negative);
    for (auto it = s_num.rbegin(); it != s_num.rend() - negative;
                                                    ++it, ++dig_cnt) {
        if (*it == '.') {
            --dig_cnt;
            continue;
        }
        double add_to_ans = *it - '0';
        if (dig_cnt < 0) {
            for (int i = 0; i > dig_cnt; --i)
                add_to_ans /= 10;
        } else {
            for (int i = 0; i < dig_cnt; ++i)
            add_to_ans *= 10;
        }
        ans += add_to_ans;
    }
    if (negative) ans = -ans;
    return ans;
}
Rational Rational::operator+() const {
    assert(this->quick_check());
    return *this;
}  // little check
Rational Rational::operator-() const {
    assert(this->quick_check());
    Rational ans(*this);
    ans.numerator *= -1;
    return ans;
}  // little check
Rational& Rational::operator=(Rational const &other) {
    assert(this->quick_check() && other.quick_check());
    this->numerator = other.numerator;
    this->denominator = other.denominator;
    return *this;
}  // little check
Rational& Rational::operator+=(Rational const &other) {
    assert(this->quick_check() && other.quick_check());
    (this->numerator *= other.denominator) +=
                    other.numerator * this->denominator;
    this->denominator *= other.denominator;
    this->make_inter_prime();
    return *this;
}  // little check
Rational& Rational::operator-=(Rational const &other) {
    assert(this->quick_check() && other.quick_check());
    (this->numerator *= other.denominator) -=
                    other.numerator * this->denominator;
    this->denominator *= other.denominator;
    this->make_inter_prime();
    return *this;
}  // little check
Rational& Rational::operator*=(Rational const &other) {
    assert(this->quick_check() && other.quick_check());
    this->numerator *= other.numerator;
    this->denominator *= other.denominator;
    this->make_inter_prime();
    return *this;
}
Rational& Rational::operator/=(Rational const &other) {
    assert(this->quick_check() && other.quick_check());
    assert(other.numerator != 0);
    this->numerator *= other.denominator;
    this->denominator *= other.numerator;
    if (this->denominator.is_negative()) {
        this->numerator *= -1;
        this->denominator *= -1;
    }
    this->make_inter_prime();
    return *this;
}
//---------------------class inner funk---------------------
BigInteger Rational::gcd() const {
    assert(this->quick_check());
    BigInteger numer = numerator.abs(), den = denominator;
    // to avoid problems with negativ numerator
    while (!numer.is_null() && !den.is_null()) {
        if (numer > den)
            numer %= den;
        else
            den %= numer;
    }
    return numer + den;
}  // little check
void Rational::make_inter_prime() {
    assert(this->quick_check());
    BigInteger gcd(this->gcd());
    assert(gcd.is_positive());
    numerator /= gcd;
    denominator /= gcd;
    assert(denominator > 0);
}  // little check
bool Rational::quick_check() const {  // work O(1)
    return denominator.is_positive();
}  // little check
bool Rational::full_check() const {
    bool ans;
    ans = denominator.is_positive() && (this->gcd() == 1);
    return ans;
}  // little check
std::string Rational::toString() const {
    assert(this->quick_check());
    std::string ans_s;
    if  (denominator == 1)
        ans_s = numerator.toString();
    else
        ans_s = numerator.toString() + "/" + denominator.toString();
    return ans_s;
}  // little check
std::string Rational::asDecimal(size_t precision = 0) const {
    assert(this->quick_check());
    std::string tst( 7 + 1, '0');
    BigInteger number = numerator *
        BigInteger('1' + std::string(precision + 1, '0'));
    number /= denominator;
    if (number % 10 >= 5)
        number += 10;
    number /= 10;
    BigInteger integer_part(number /
                        BigInteger('1' + std::string(precision, '0')));
    BigInteger non_integer_part(number.abs() %
                    BigInteger('1' + std::string(precision, '0')));
    std::string ans;
    if (non_integer_part.is_null() && precision == 0) {
        ans = integer_part.toString();
    } else {
        std::string non_integer_part_zeros(precision -
                                    non_integer_part.toString().size(), '0');
        ans = integer_part.toString() + '.' +
                        non_integer_part_zeros + non_integer_part.toString();
    }
    if (integer_part.is_null() && numerator < 0 && !non_integer_part.is_null())
        ans = '-' + ans;
    return ans;
}  // little check
//------------------------compare operators---------------------
Rational operator+(Rational const &num1, Rational const &num2) {
    Rational ans(num1);
    ans += num2;
    return ans;
}  // little check
Rational operator-(Rational const &num1, Rational const &num2) {
    Rational ans(num1);
    ans -= num2;
    return ans;}  // little check
Rational operator*(Rational const &num1, Rational const &num2) {
    Rational ans(num1);
    ans *= num2;
    return ans;
}  // little check
Rational operator/(Rational const &num1, Rational const &num2) {
    Rational ans(num1);
    ans /= num2;
    return ans;
}  // little check

//-------------------operators with class elem---------------------
bool operator==(Rational const &num1, Rational const &num2) {
    assert(num1.quick_check() && num2.quick_check());
    return (num1.numerator == num2.numerator) &&
                (num1.denominator == num2.denominator);
}  // little check
bool operator!=(Rational const &num1, Rational const &num2) {
    return !(num1 == num2);
}  // little check
bool operator<(Rational const &num1, Rational const &num2) {
    assert(num1.quick_check() && num2.quick_check());
    return num1.numerator * num2.denominator <
                            num2.numerator * num1.denominator;
}  // little check
bool operator>(Rational const &num1, Rational const &num2) {
    return !(num1 < num2) && num1 != num2;
}  // little check
bool operator<=(Rational const &num1, Rational const &num2) {
    return num1 == num2 || num1 < num2;
}  // little check
bool operator>=(Rational const &num1, Rational const &num2)  {
    return num1 == num2 || num1 > num2;
}  // little check
//--------------end of Rational---------------------



#endif /* biginteger_h */
