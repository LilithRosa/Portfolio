// euint.hpp
#pragma once

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring> // for memcpy

/*
*   The structure of these values is stored in little-endian order.
*   For the individual limbs, the least significant limb is stored
*   at the lowest index, i.e., v[0].
*/

template <size_t MEMS>
struct euint_t {
    std::array<uint64_t, MEMS> v{};
    bool LittleEndianLimbs{ true };

    euint_t() = default;

    explicit euint_t(uint64_t x) {
        v.fill(0);
        v[0] = x;
    }

    euint_t(uint64_t hi, uint64_t lo) {
        v.fill(0);
        v[0] = lo;
        if constexpr (MEMS > 1)
            v[1] = hi;
    }

    static constexpr size_t limbs() noexcept { return MEMS; }
};

//! Arithmetic helper methods.

// Add 64-bit inputs, carry-aware.
inline uint64_t ADC64(uint64_t a, uint64_t b, uint64_t c, uint64_t& r) noexcept {
    uint64_t s = a + b;
    uint64_t c1 = (s < a) ? 1u : 0u;
    uint64_t s2 = s + c;
    uint64_t c2 = (s2 < s) ? 1u : 0u;

    r = s2;
    return c1 | c2;
}

// Add 64-bit value to extended value, carry-aware.
template <size_t MEMS>
inline uint64_t EADC64(euint_t<MEMS>& a, uint64_t b) noexcept {
    uint64_t o = a.v[0];
    uint64_t s = o + b;
    a.v[0] = s;

    uint64_t c = (s < o) ? 1u : 0u;

    for (size_t i = 1; i < MEMS && c; ++i) {
        uint64_t t;
        c = ADC64(a.v[i], 0u, c, t);
        a.v[i] = t;
    }

    return c;
}

// Add extended values, carry-aware.
template <size_t MEMS>
inline uint64_t EADC(euint_t<MEMS>& a, const euint_t<MEMS>& b, uint64_t ci = 0u) noexcept {
    uint64_t c = ci;
    for (size_t i = 0; i < MEMS; ++i) {
        uint64_t t;
        c = ADC64(a.v[i], b.v[i], c, t);
        a.v[i] = t;
    }
    return c;
}

// Subtract 64-bit inputs, borrow-aware.
inline uint64_t SBB64(uint64_t a, uint64_t b, uint64_t c, uint64_t& r) noexcept {
    uint64_t s = a - b;
    uint64_t b1 = (a < b) ? 1u : 0u;
    uint64_t s2 = s - c;
    uint64_t b2 = (s < c) ? 1u : 0u;
    r = s2;
    return b1 | b2;
}

// Subtract 64-bit value from extended value, borrow-aware.
template <size_t MEMS>
inline uint64_t ESBB64(euint_t<MEMS>& a, uint64_t b) noexcept {
    uint64_t o = a.v[0];
    uint64_t s = o - b;
    a.v[0] = s;
    uint64_t c = (o < b) ? 1u : 0u;
    for (size_t i = 1; i < MEMS && c; ++i) {
        uint64_t t;
        c = SBB64(a.v[i], 0u, c, t);
        a.v[i] = t;
    }
    return c;
}

// Subtract extended values, borrow-aware.
template <size_t MEMS>
inline uint64_t ESBB(euint_t<MEMS>& a, const euint_t<MEMS>& b, uint64_t bi = 0u) noexcept {
    uint64_t c = bi;
    for (size_t i = 0; i < MEMS; ++i) {
        uint64_t t;
        c = SBB64(a.v[i], b.v[i], c, t);
        a.v[i] = t;
    }
    return c;
}

// Multiply 64-bit full length (low/high).
inline void MUL64(uint64_t a, uint64_t b, uint64_t& l, uint64_t& h) noexcept {
    uint64_t a0 = (uint32_t)a;
    uint64_t a1 = a >> 32;
    uint64_t b0 = (uint32_t)b;
    uint64_t b1 = b >> 32;

    uint64_t p0 = a0 * b0;
    uint64_t p1 = a0 * b1;
    uint64_t p2 = a1 * b0;
    uint64_t p3 = a1 * b1;

    uint64_t m = (p0 >> 32) + (uint32_t)p1 + (uint32_t)p2;
    l = (p0 & 0xffffffffULL) | (m << 32);
    h = p3 + (p1 >> 32) + (p2 >> 32) + (m >> 32);
}

// Multiply extended value by 64-bit, carry-aware.
template <size_t MEMS>
inline uint64_t EMUL64(euint_t<MEMS>& r, const euint_t<MEMS>& a, uint64_t b) noexcept {
    uint64_t c = 0;
    for (size_t i = 0; i < MEMS; ++i) {
        uint64_t l, h;
        MUL64(a.v[i], b, l, h);
        uint64_t t;
        c = ADC64(l, c, 0u, t);
        r.v[i] = t;
        c += h;
    }

    return c;
}

// Multiply extended values, carry-aware.
template <size_t MEMS>
inline uint64_t EMUL(euint_t<MEMS>& r, const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    r.v.fill(0);
    uint64_t of = 0;
    for (size_t i = 0; i < MEMS; ++i) {
        uint64_t c = 0;
        for (size_t j = 0; j + i < MEMS; ++j) {
            uint64_t l, h;
            MUL64(a.v[i], b.v[j], l, h);
            uint64_t t;
            c = ADC64(r.v[i + j], l, c, t);
            r.v[i + j] = t;
            c += h;
        }
        of |= c;
    }
    return of;
}

// Divide operation: q = a / b, r = a % b.
template <size_t MEMS>
inline void EDIVMOD(euint_t<MEMS>& q, euint_t<MEMS>& r, const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    q.v.fill(0);
    r.v.fill(0);

    for (int i = int(MEMS * 64) - 1; i >= 0; --i) {
        uint64_t c = 0;
        for (size_t k = 0; k < MEMS; ++k) {
            uint64_t nc = r.v[k] >> 63;
            r.v[k] = (r.v[k] << 1) | c;
            c = nc;
        }
        size_t limb = size_t(i) / 64;
        size_t bit  = size_t(i) % 64;
        uint64_t bv = (a.v[limb] >> bit) & 1u;
        r.v[0] |= bv;

        bool ge = true;
        for (int k = int(MEMS) - 1; k >= 0; --k) {
            if (r.v[size_t(k)] > b.v[size_t(k)]) { ge = true; break; }
            if (r.v[size_t(k)] < b.v[size_t(k)]) { ge = false; break; }
        }

        if (ge) {
            ESBB(r, b, 0u);
            q.v[limb] |= (1ULL << bit);
        }
    }
}

//! Equality operations.

template <size_t MEMS>
inline int ECMP(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    for (int i = int(MEMS) - 1; i >= 0; --i) {
        if (a.v[size_t(i)] < b.v[size_t(i)]) return -1;
        if (a.v[size_t(i)] > b.v[size_t(i)]) return 1;
    }
    return 0;
}

//! Bitwise shift helpers (by limbs).

template <size_t MEMS>
inline euint_t<MEMS> ESHLMOD64(const euint_t<MEMS>& a, size_t limb_shift) noexcept {
    euint_t<MEMS> r;
    r.v.fill(0);
    if (limb_shift >= MEMS) {
        return r;
    }
    for (size_t i = MEMS; i-- > limb_shift; ) {
        r.v[i] = a.v[i - limb_shift];
    }
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> ESHRMOD64(const euint_t<MEMS>& a, size_t limb_shift) noexcept {
    euint_t<MEMS> r;
    r.v.fill(0);
    if (limb_shift >= MEMS) {
        return r;
    }
    for (size_t i = 0; i + limb_shift < MEMS; ++i) {
        r.v[i] = a.v[i + limb_shift];
    }
    return r;
}

//! Bitwise shifts (by bits).

template <size_t MEMS>
inline euint_t<MEMS> ESHL(const euint_t<MEMS>& a, size_t bit_shift) noexcept {
    euint_t<MEMS> r;
    r.v.fill(0);
    if (bit_shift == 0) {
        r = a;
        return r;
    }
    size_t limb_shift  = bit_shift / 64;
    size_t intra_shift = bit_shift % 64;

    r = ESHLMOD64(a, limb_shift);
    if (intra_shift > 0) {
        for (size_t i = MEMS; i-- > 0; ) {
            uint64_t hi = r.v[i] << intra_shift;
            uint64_t lo = 0;
            if (i > 0) {
                lo = r.v[i - 1] >> (64 - intra_shift);
            }
            r.v[i] = hi | lo;
        }
    }
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> ESHR(const euint_t<MEMS>& a, size_t bit_shift) noexcept {
    euint_t<MEMS> r;
    r.v.fill(0);
    if (bit_shift == 0) {
        r = a;
        return r;
    }
    size_t limb_shift  = bit_shift / 64;
    size_t intra_shift = bit_shift % 64;

    r = ESHRMOD64(a, limb_shift);
    if (intra_shift > 0) {
        for (size_t i = 0; i < MEMS; ++i) {
            uint64_t lo = r.v[i] >> intra_shift;
            uint64_t hi = 0;
            if (i + 1 < MEMS) {
                hi = r.v[i + 1] << (64 - intra_shift);
            }
            r.v[i] = lo | hi;
        }
    }
    return r;
}

//! Rotations (logical, full width).

template <size_t MEMS>
inline euint_t<MEMS> EROL(const euint_t<MEMS>& a, size_t bit_shift) noexcept {
    const size_t total_bits = MEMS * 64;
    if (total_bits == 0) return a;
    bit_shift %= total_bits;
    if (bit_shift == 0) return a;
    return ESHL(a, bit_shift) | ESHR(a, total_bits - bit_shift);
}

template <size_t MEMS>
inline euint_t<MEMS> EROR(const euint_t<MEMS>& a, size_t bit_shift) noexcept {
    const size_t total_bits = MEMS * 64;
    if (total_bits == 0) return a;
    bit_shift %= total_bits;
    if (bit_shift == 0) return a;
    return ESHR(a, bit_shift) | ESHL(a, total_bits - bit_shift);
}

//! Free arithmetic operators.

template <size_t MEMS>
inline euint_t<MEMS>& operator+=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    (void)EADC(a, b, 0u);
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator+(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a += b;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS>& operator-=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    (void)ESBB(a, b, 0u);
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator-(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a -= b;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS>& operator*=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    euint_t<MEMS> r;
    (void)EMUL(r, a, b);
    a = r;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator*(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a *= b;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS>& operator/=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    euint_t<MEMS> q, r;
    EDIVMOD(q, r, a, b);
    a = q;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator/(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a /= b;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS>& operator%=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    euint_t<MEMS> q, r;
    EDIVMOD(q, r, a, b);
    a = r;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator%(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a %= b;
    return a;
}

//! Free iterative operators.

template <size_t MEMS>
inline euint_t<MEMS>& operator++(euint_t<MEMS>& a) noexcept {
    (void)EADC64(a, 1);
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator++(euint_t<MEMS>& a, int) noexcept {
    euint_t<MEMS> r = a;
    ++a;
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS>& operator--(euint_t<MEMS>& a) noexcept {
    (void)ESBB64(a, 1);
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator--(euint_t<MEMS>& a, int) noexcept {
    euint_t<MEMS> r = a;
    --a;
    return r;
}

//! Free comparison operators.

template <size_t MEMS>
inline bool operator==(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return ECMP(a, b) == 0;
}

template <size_t MEMS>
inline bool operator!=(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return !(a == b);
}

template <size_t MEMS>
inline bool operator<(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return ECMP(a, b) < 0;
}

template <size_t MEMS>
inline bool operator>(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return ECMP(a, b) > 0;
}

template <size_t MEMS>
inline bool operator<=(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return ECMP(a, b) <= 0;
}

template <size_t MEMS>
inline bool operator>=(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return ECMP(a, b) >= 0;
}

template <size_t MEMS>
inline bool operator!(const euint_t<MEMS>& a) noexcept {
    for (size_t i = 0; i < MEMS; ++i) {
        if (a.v[i] != 0)
            return false;
    }
    return true;
}

// Logical && and || on "non-zero" semantics.
// You may want to delete these to avoid confusion with bitwise ops.
template <size_t MEMS>
inline bool operator&&(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return !(!a || !b);
}

template <size_t MEMS>
inline bool operator||(const euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    return !(!a && !b);
}

//! Free bitwise operators.

template <size_t MEMS>
inline euint_t<MEMS>& operator&=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    for (size_t i = 0; i < MEMS; ++i)
        a.v[i] &= b.v[i];
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator&(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a &= b;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS>& operator|=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    for (size_t i = 0; i < MEMS; ++i)
        a.v[i] |= b.v[i];
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator|(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a |= b;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS>& operator^=(euint_t<MEMS>& a, const euint_t<MEMS>& b) noexcept {
    for (size_t i = 0; i < MEMS; ++i)
        a.v[i] ^= b.v[i];
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator^(euint_t<MEMS> a, const euint_t<MEMS>& b) noexcept {
    a ^= b;
    return a;
}

template <size_t MEMS>
inline euint_t<MEMS> operator~(const euint_t<MEMS>& a) noexcept {
    euint_t<MEMS> r;
    for (size_t i = 0; i < MEMS; ++i)
        r.v[i] = ~a.v[i];
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> operator<<(const euint_t<MEMS>& a, size_t bit_shift) noexcept {
    return ESHL(a, bit_shift);
}

template <size_t MEMS>
inline euint_t<MEMS> operator>>(const euint_t<MEMS>& a, size_t bit_shift) noexcept {
    return ESHR(a, bit_shift);
}

//! Endianness conversions.

template <size_t MEMS>
inline euint_t<MEMS> ELimbSwap(const euint_t<MEMS>& a) noexcept {
    euint_t<MEMS> r;
    for (size_t i = 0; i < MEMS; ++i) {
        r.v[(MEMS - 1) - i] = a.v[i];
    }
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> EByteSwap(const euint_t<MEMS>& a) noexcept {
    euint_t<MEMS> r;
    for (size_t i = 0; i < MEMS; ++i) {
        r.v[i] = std::byteswap(a.v[i]);
    }
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> EReadLELE(const unsigned char* p) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r;
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(&r.v[i], p + i * 8, 8);
    if (!isLittleNative)
        r = EByteSwap(r);
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> EReadLEBE(const unsigned char* p) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r;
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(&r.v[i], p + i * 8, 8);
    if (isLittleNative)
        r = EByteSwap(r);
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> EReadBELE(const unsigned char* p) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r;
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(&r.v[i], p + (MEMS - 1 - i) * 8, 8);
    if (!isLittleNative)
        r = EByteSwap(r);
    return r;
}

template <size_t MEMS>
inline euint_t<MEMS> EReadBEBE(const unsigned char* p) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r;
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(&r.v[i], p + (MEMS - 1 - i) * 8, 8);
    if (isLittleNative)
        r = EByteSwap(r);
    return r;
}

template <size_t MEMS>
inline void EWriteLELE(unsigned char* p, const euint_t<MEMS>& x) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r = x;
    if (!x.LittleEndianLimbs)
        r = ELimbSwap(r);
    if (!isLittleNative)
        r = EByteSwap(r);
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(p + i * 8, &r.v[i], 8);
}

template <size_t MEMS>
inline void EWriteLEBE(unsigned char* p, const euint_t<MEMS>& x) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r = x;
    if (!x.LittleEndianLimbs)
        r = ELimbSwap(r);
    if (isLittleNative)
        r = EByteSwap(r);
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(p + i * 8, &r.v[i], 8);
}

template <size_t MEMS>
inline void EWriteBELE(unsigned char* p, const euint_t<MEMS>& x) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r = x;
    if (x.LittleEndianLimbs)
        r = ELimbSwap(r);
    if (!isLittleNative)
        r = EByteSwap(r);
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(p + i * 8, &r.v[i], 8);
}

template <size_t MEMS>
inline void EWriteBEBE(unsigned char* p, const euint_t<MEMS>& x) noexcept {
    bool isLittleNative = (std::endian::native == std::endian::little);
    euint_t<MEMS> r = x;
    if (x.LittleEndianLimbs)
        r = ELimbSwap(r);
    if (isLittleNative)
        r = EByteSwap(r);
    for (size_t i = 0; i < MEMS; ++i)
        std::memcpy(p + i * 8, &r.v[i], 8);
}

// EOF