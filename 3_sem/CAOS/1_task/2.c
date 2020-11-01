/* Реализуйте функцию с прототипом:

 * typedef enum {
 *     PlusZero      = 0x00,
 *     MinusZero     = 0x01,
 *     PlusInf       = 0xF0,
 *     MinusInf      = 0xF1,
 *     PlusRegular   = 0x10,
 *     MinusRegular  = 0x11,
 *     PlusDenormal  = 0x20,
 *     MinusDenormal = 0x21,
 *     SignalingNaN  = 0x30,
 *     QuietNaN      = 0x31
 * } float_class_t;

 * extern float_class_t
 * classify(double *value_ptr);
      
 * которая классифицирует специальные значения вещественного числа, на которое указывает value_ptr, в соответствии со стандартом IEEE 754.

 * При решении допускается использовать только побитовые операции, включая сдвиги.
 */
#include <stdint.h>

typedef enum {
    PlusZero      = 0x00,  // (0, 0...0, 0)*
    MinusZero     = 0x01,  // (1, 0...0, 0)*
    PlusInf       = 0xF0,  // (0, 1...1, 0...0) *
    MinusInf      = 0xF1,  // (1, 1...1, 0...0) *
    PlusRegular   = 0x10,  // (0, ?, ?...?) 
    MinusRegular  = 0x11,  // (1, ?, ?...?) 
    PlusDenormal  = 0x20,  // (0, 0...0, !0) 
    MinusDenormal = 0x21,  // (1, 0...0, !0) 
    SignalingNaN  = 0x30,  // (?, 1...1, 0?.(!0)..?) *
    QuietNaN      = 0x31  // (?, 1...1, 1?...?) *
} float_class_t;

enum {
    E_MASK = 2047,
    S_SIZE = 1,
    E_SIZE = 11,
    M_SIZE = 52
};
enum flag { 
    S0, S1,
    E11, E00, E10,
    M00, M0, M1
};
enum flag s_part(uint64_t bits) {
    enum flag ans = S0;
    if (bits >> (E_SIZE + M_SIZE)) {
        ans = S1;
    } 
    return ans;
} 
enum flag e_part(uint64_t bits) {
    enum flag ans = E00;
    bits <<= S_SIZE;
    if (bits >> (S_SIZE + M_SIZE) == E_MASK) {
        ans = E11;
    } else if (bits >> (S_SIZE + M_SIZE)) {
        ans = E10;
    } 
    return ans;
} 
enum flag m_part(uint64_t bits) {
    enum flag ans = M00;
    bits <<= (S_SIZE + E_SIZE);
    if (bits >> (E_SIZE + M_SIZE)) {
        ans = M1;
    } else if (bits) {
        ans = M0;
    } 
    return ans;
} 

extern float_class_t
classify(double *value_ptr) {
    void *void_ptr = value_ptr;
    uint64_t *uint_ptr = void_ptr;
    uint64_t bits = *uint_ptr;
    float_class_t ans;
    if (s_part(bits) == S0 && e_part(bits) == E00 && m_part(bits) == M00) {
        ans = PlusZero;
    } else if (s_part(bits) == S1 && e_part(bits) == E00 && m_part(bits) == M00) {
        ans = MinusZero;
    } else if (s_part(bits) == S0 && e_part(bits) == E11 && m_part(bits) == M00) {
        ans = PlusInf;
    } else if (s_part(bits) == S1 && e_part(bits) == E11 && m_part(bits) == M00) {
        ans = MinusInf;
    } else if (e_part(bits) == E11 && m_part(bits) == M0) {
        ans = SignalingNaN;
    } else if (e_part(bits) == E11 && m_part(bits) == M1) {
        ans = QuietNaN;
    } else if (s_part(bits) == S0 && e_part(bits) == E00) {
        ans = PlusDenormal;
    } else if (s_part(bits) == S1 && e_part(bits) == E00) {
        ans = MinusDenormal;
    } else if (s_part(bits) == S0) {
        ans = PlusRegular;
    } else {
        ans = MinusRegular;
    } 
    return ans;
} 

