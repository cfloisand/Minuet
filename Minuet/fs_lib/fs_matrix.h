/* fs_matrix.h - Flying Sand Matrix Library
 * v. 0.1
 */

#pragma once
#include <cmath>
#include <type_traits>


// NOTE(christian): Matrices are stored in row-major order.

template <class T, int C, int R>
union fsMat {
    static_assert(C > 1, "fsMat must contain more than 1 columns.");
    static_assert(R > 1, "fsMat must contain more than 1 rows.");
    static_assert(std::is_arithmetic<T>::value, "fsMat value type must be arithmetic.");
    
    T v[C * R];
};

template <class T>
union fsMat<T, 2, 2> {
    struct { T a, b, c, d; };
    struct { T v[4]; };
};

template <class T>
union fsMat<T, 3, 3> {
    struct { T a, b, c, d, e, f, g, h, i; };
    struct { T v[9]; };
};

template <class T>
union fsMat<T, 4, 4> {
    struct { T a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p; };
    struct { T v[16]; };
};

#include <iostream>
template <class T, int C, int R>
std::ostream& operator<< (std::ostream& os, const fsMat<T, C, R>& rhs)
{
    os << "fsMat<" << C << "," << R << ">[" << rhs.v[0];
    for (int i = 1, j = 1; i < C * R; ++i, ++j) {
        os << (j == C ? " | " : " ") << rhs.v[i];
        j = (j == C ? 0 : j);
    }
    os << "]";
    return os;
}

template <class T, int C, int R, class... Args>
fsMat<T, C, R> fs_matrix_create(Args... args)
{
    const int length = sizeof...(args);
    static_assert(length == C * R, "Number of arguments in creator function does not match template argument.");
    fsMat<T, C, R> result = {static_cast<T>(args)...};
    return result;
}

template <class T>
fsMat<T, 2, 2> fs_matrix_create(T val)
{
    fsMat<T, 2, 2> result = {val, val, val, val};
    return result;
}

template <class T>
fsMat<T, 3, 3> fs_matrix_create(T val)
{
    fsMat<T, 3, 3> result = {val, val, val, val, val, val, val, val, val};
    return result;
}

template <class T>
fsMat<T, 4, 4> fs_matrix_create(T val)
{
    fsMat<T, 4, 4> result = {val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val};
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R> fs_matrix_create(T val)
{
    fsMat<T, C, R> result;
    for (int i = 0; i < C * R; ++i) {
        result.v[i] = val;
    }
    
    return result;
}

template <class T>
fsMat<T, 2, 2> fs_matrix_identity()
{
    fsMat<T, 2, 2> result = {1, 0, 0, 1};
    return result;
}

template <class T>
fsMat<T, 3, 3> fs_matrix_identity()
{
    fsMat<T, 3, 3> result = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    return result;
}

template <class T>
fsMat<T, 4, 4> fs_matrix_identity()
{
    fsMat<T, 4, 4> result = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    return result;
}

template <class T, int V>
fsMat<T, V, V> fs_matrix_identity()
{
    fsMat<T, V, V> result = {};
    int stride = V + 1;
    for (int i = 0; i < V * V; i+=stride) {
        result.v[i] = (T)1;
    }
    
    return result;
}

template <class T, class U>
U fs_matrix_convert_default_cast(T v)
{
    return (U)v;
}

/*! Converts the vector with V elements of type T to a vector with V elements of type U.
    The default converts using a simple cast, otherwise a function can be specified
    to perform a more customized conversion; e.g. rounding. */
template <class T, int C, int R, class U>
fsMat<U, C, R> fs_matrix_convert(fsMat<T, C, R> m, U(*F)(T) = fs_vconvert_default_cast)
{
    fsMat<U, C, R> result;
    for (int i = 0; i < C * R; ++i) {
        result.v[i] = F(m.v[i]);
    }

    return result;
}


#pragma mark - Operators
// ==================================================================================
//                          Operators
// ==================================================================================

template <class T, int C, int R>
fsMat<T, C, R>& operator+=(fsMat<T, C, R>& m, const fsMat<T, C, R> rhs)
{
    m = m + rhs;
    return m;
}

template <class T>
fsMat<T, 2, 2> operator+(fsMat<T, 2, 2> m1, const fsMat<T, 2, 2> m2)
{
    fsMat<T, 2, 2> result = {m1.a + m2.a, m1.b + m2.b, m1.c + m2.c, m1.d + m2.d};
    return result;
}

template <class T>
fsMat<T, 3, 3> operator+(fsMat<T, 3, 3> m1, const fsMat<T, 3, 3> m2)
{
    fsMat<T, 3, 3> result = {
        m1.a + m2.a, m1.b + m2.b, m1.c + m2.c,
        m1.d + m2.d, m1.e + m2.e, m1.f + m2.f,
        m1.g + m2.g, m1.h + m2.h, m1.i + m2.i};
    return result;
}

template <class T>
fsMat<T, 4, 4> operator+(fsMat<T, 4, 4> m1, const fsMat<T, 4, 4> m2)
{
    fsMat<T, 4, 4> result = {
        m1.a + m2.a, m1.b + m2.b, m1.c + m2.c, m1.d + m2.d,
        m1.e + m2.e, m1.f + m2.f, m1.g + m2.g, m1.h + m2.h,
        m1.i + m2.i, m1.j + m2.j, m1.k + m2.k, m1.l + m2.l,
        m1.m + m2.m, m1.n + m2.n, m1.o + m2.o, m1.p + m2.p};
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R> operator+(fsMat<T, C, R> m1, const fsMat<T, C, R> m2)
{
    fsMat<T, C, R> result;
    for (int i = 0; i < C * R; ++i) {
        result.v[i] = m1.v[i] + m2.v[i];
    }
    
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R>& operator-=(fsMat<T, C, R>& m, const fsMat<T, C, R> rhs)
{
    m = m - rhs;
    return m;
}

template <class T>
fsMat<T, 2, 2> operator-(fsMat<T, 2, 2> m1, const fsMat<T, 2, 2> m2)
{
    fsMat<T, 2, 2> result = {m1.a - m2.a, m1.b - m2.b, m1.c - m2.c, m1.d - m2.d};
    return result;
}

template <class T>
fsMat<T, 3, 3> operator-(fsMat<T, 3, 3> m1, const fsMat<T, 3, 3> m2)
{
    fsMat<T, 3, 3> result = {
        m1.a - m2.a, m1.b - m2.b, m1.c - m2.c,
        m1.d - m2.d, m1.e - m2.e, m1.f - m2.f,
        m1.g - m2.g, m1.h - m2.h, m1.i - m2.i};
    return result;
}

template <class T>
fsMat<T, 4, 4> operator-(fsMat<T, 4, 4> m1, const fsMat<T, 4, 4> m2)
{
    fsMat<T, 4, 4> result = {
        m1.a - m2.a, m1.b - m2.b, m1.c - m2.c, m1.d - m2.d,
        m1.e - m2.e, m1.f - m2.f, m1.g - m2.g, m1.h - m2.h,
        m1.i - m2.i, m1.j - m2.j, m1.k - m2.k, m1.l - m2.l,
        m1.m - m2.m, m1.n - m2.n, m1.o - m2.o, m1.p - m2.p};
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R> operator-(fsMat<T, C, R> m1, const fsMat<T, C, R> m2)
{
    fsMat<T, C, R> result;
    for (int i = 0; i < C * R; ++i) {
        result.v[i] = m1.v[i] - m2.v[i];
    }
    
    return result;
}

template <class T>
fsMat<T, 2, 2> operator*(const fsMat<T, 2, 2> m, const T scalar)
{
    fsMat<T, 2, 2> result = {m.a * scalar, m.b * scalar, m.c * scalar, m.d * scalar};
    return result;
}

template <class T>
fsMat<T, 3, 3> operator*(const fsMat<T, 3, 3> m, const T scalar)
{
    fsMat<T, 3, 3> result = {
        m.a * scalar, m.b * scalar, m.c * scalar,
        m.d * scalar, m.e * scalar, m.f * scalar,
        m.g * scalar, m.h * scalar, m.i * scalar};
    return result;
}

template <class T>
fsMat<T, 4, 4> operator*(const fsMat<T, 4, 4> m, const T scalar)
{
    fsMat<T, 4, 4> result = {
        m.a * scalar, m.b * scalar, m.c * scalar, m.d * scalar,
        m.e * scalar, m.f * scalar, m.g * scalar, m.h * scalar,
        m.i * scalar, m.j * scalar, m.k * scalar, m.l * scalar,
        m.m * scalar, m.n * scalar, m.o * scalar, m.p * scalar};
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R> operator*(const fsMat<T, C, R> m, const T scalar)
{
    fsMat<T, C, R> result;
    for (int i = 0; i < C * R; ++i) {
        result.v[i] = m.v[i] * scalar;
    }
    
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R> operator*(const T scalar, const fsMat<T, C, R> m)
{
    fsMat<T, C, R> result = m * scalar;
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R>& operator*=(fsMat<T, C, R>& m, const T scalar)
{
    m = m * scalar;
    return m;
}

template <class T, int C, int R>
fsMat<T, C, R> operator/(const fsMat<T, C, R> m, const T scalar)
{
    fsMat<T, C, R> result = m * (T)(1.0/scalar);
    return result;
}

template <class T>
fsMat<T, 2, 2> operator/(const T scalar, const fsMat<T, 2, 2> m)
{
    fsMat<T, 2, 2> result = {scalar / m.a, scalar / m.b, scalar / m.c, scalar / m.d};
    return result;
}

template <class T>
fsMat<T, 3, 3> operator/(const T scalar, const fsMat<T, 3, 3> m)
{
    fsMat<T, 3, 3> result = {
        scalar / m.a, scalar / m.b, scalar / m.c,
        scalar / m.d, scalar / m.e, scalar / m.f,
        scalar / m.g, scalar / m.h, scalar / m.i};
    return result;
}

template <class T>
fsMat<T, 4, 4> operator/(const T scalar, const fsMat<T, 4, 4> m)
{
    fsMat<T, 4, 4> result = {
        scalar / m.a, scalar / m.b, scalar / m.c, scalar / m.d,
        scalar / m.e, scalar / m.f, scalar / m.g, scalar / m.h,
        scalar / m.i, scalar / m.j, scalar / m.k, scalar / m.l,
        scalar / m.m, scalar / m.n, scalar / m.o, scalar / m.p};
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R> operator/(const T scalar, const fsMat<T, C, R> m)
{
    fsMat<T, C, R> result;
    for (int i = 0; i < C * R; ++i) {
        result.v[i] = scalar / m.v[i];
    }
    
    return result;
}

template <class T, int C, int R>
fsMat<T, C, R>& operator/=(fsMat<T, C, R>& m, const T scalar)
{
    m = m / scalar;
    return m;
}

template <class T>
fsMat<T, 2, 2> operator*(const fsMat<T, 2, 2> m1, const fsMat<T, 2, 2> m2)
{
    fsMat<T, 2, 2> result = {
        m1.a * m2.a + m1.b * m2.c, m1.a * m2.b + m1.b * m2.d,
        m1.c * m2.a + m1.d * m2.c, m1.c * m2.b + m1.d * m2.d};
    return result;
}

template <class T>
fsMat<T, 3, 3> operator*(const fsMat<T, 3, 3> m1, const fsMat<T, 3, 3> m2)
{
    fsMat<T, 3, 3> result = {
        m1.a * m2.a + m1.b * m2.d + m1.c * m2.g, m1.a * m2.b + m1.b * m2.e + m1.c * m2.h, m1.a * m2.c + m1.b * m2.f + m1.c * m2.i,
        m1.d * m2.a + m1.e * m2.d + m1.f * m2.g, m1.d * m2.b + m1.e * m2.e + m1.f * m2.h, m1.d * m2.c + m1.e * m2.f + m1.f * m2.i,
        m1.g * m2.a + m1.h * m2.d + m1.i * m2.g, m1.g * m2.b + m1.h * m2.e + m1.i * m2.h, m1.g * m2.c + m1.h * m2.f + m1.i * m2.i};
    return result;
}

template <class T>
fsMat<T, 4, 4> operator*(const fsMat<T, 4, 4> m1, const fsMat<T, 4, 4> m2)
{
    T r00 = m1.a * m2.a + m1.b * m2.e + m1.c * m2.i + m1.d * m2.m;
    T r01 = m1.a * m2.b + m1.b * m2.f + m1.c * m2.j + m1.d * m2.n;
    T r02 = m1.a * m2.c + m1.b * m2.g + m1.c * m2.k + m1.d * m2.o;
    T r03 = m1.a * m2.d + m1.b * m2.h + m1.c * m2.l + m1.d * m2.p;
    
    T r10 = m1.e * m2.a + m1.f * m2.e + m1.g * m2.i + m1.h * m2.m;
    T r11 = m1.e * m2.b + m1.f * m2.f + m1.g * m2.j + m1.h * m2.n;
    T r12 = m1.e * m2.c + m1.f * m2.g + m1.g * m2.k + m1.h * m2.o;
    T r13 = m1.e * m2.d + m1.f * m2.h + m1.g * m2.l + m1.h * m2.p;
    
    T r20 = m1.i * m2.a + m1.j * m2.e + m1.k * m2.i + m1.l * m2.m;
    T r21 = m1.i * m2.b + m1.j * m2.f + m1.k * m2.j + m1.l * m2.n;
    T r22 = m1.i * m2.c + m1.j * m2.g + m1.k * m2.k + m1.l * m2.o;
    T r23 = m1.i * m2.d + m1.j * m2.h + m1.k * m2.l + m1.l * m2.p;
    
    T r30 = m1.m * m2.a + m1.n * m2.e + m1.o * m2.i + m1.p * m2.m;
    T r31 = m1.m * m2.b + m1.n * m2.f + m1.o * m2.j + m1.p * m2.n;
    T r32 = m1.m * m2.c + m1.n * m2.g + m1.o * m2.k + m1.p * m2.o;
    T r33 = m1.m * m2.d + m1.n * m2.h + m1.o * m2.l + m1.p * m2.p;
    
    fsMat<T, 4, 4> result = {
        r00, r01, r02, r03,
        r10, r11, r12, r13,
        r20, r21, r22, r23,
        r30, r31, r32, r33
    };
    return result;
}

template <class T, int C, int R, int P>
fsMat<T, P, R> operator*(const fsMat<T, C, R> m1, const fsMat<T, P, C> m2)
{
    fsMat<T, P, R> result = {};
    
    int i = 0;
    for (int r = 0; r < R; ++r) {
        int offset = r * C;
        for (int p = 0; p < P; ++p) {
            for (int c = 0; c < C; ++c) {
                result.v[i] += m1.v[offset + c] * m2.v[c * P + p];
            }
            ++i;
        }
    }
    
    return result;
}

template <class T>
T fs_matrix_determinant(const fsMat<T, 2, 2> m)
{
    T det = m.a * m.d - m.b * m.c;
    return det;
}

template <class T>
T fs_matrix_determinant(const fsMat<T, 3, 3> m)
{
    T aei = m.a * m.e * m.i;
    T bfg = m.b * m.f * m.g;
    T cdh = m.c * m.d * m.h;
    T ceg = m.c * m.e * m.g;
    T bdi = m.b * m.d * m.i;
    T afh = m.a * m.f * m.h;
    T det = aei + bfg + cdh - ceg - bdi - afh;
    return det;
}

template <class T>
T fs_matrix_determinant(const fsMat<T, 4, 4> m)
{
    T det0 = m.a * fs_matrix_determinant((fsMat<T, 3, 3>){m.f, m.g, m.h, m.j, m.k, m.l, m.n, m.o, m.p});
    T det1 = m.e * fs_matrix_determinant((fsMat<T, 3, 3>){m.b, m.c, m.d, m.j, m.k, m.l, m.n, m.o, m.p});
    T det2 = m.i * fs_matrix_determinant((fsMat<T, 3, 3>){m.b, m.c, m.d, m.f, m.g, m.h, m.n, m.o, m.p});
    T det3 = m.m * fs_matrix_determinant((fsMat<T, 3, 3>){m.b, m.c, m.d, m.f, m.g, m.h, m.j, m.k, m.l});
    T det = det0 - det1 + det2 - det3;
    return det;
}

template <class T>
fsMat<T, 2, 2> fs_matrix_inverse(const fsMat<T, 2, 2> m)
{
    fsMat<T, 2, 2> result = {m.d, -m.b, -m.c, m.a};
    T det = fs_matrix_determinant<T>(m);
    if (det != 0) {
        return (result * (T)(1.0/det));
    } else {
        return m;
    }
}

template <class T>
fsMat<T, 3, 3> fs_matrix_inverse(const fsMat<T, 3, 3> m)
{
    fsMat<T, 3, 3> minors;
    minors.a = m.e * m.i - m.f * m.h;
    minors.b = -(m.b * m.i - m.c * m.h);
    minors.c = m.b * m.f - m.c * m.e;
    minors.d = -(m.d * m.i - m.f * m.g);
    minors.e = m.a * m.i - m.c * m.g;
    minors.f = -(m.a * m.f - m.c * m.d);
    minors.g = m.d * m.h - m.e * m.g;
    minors.h = -(m.a * m.h - m.b * m.g);
    minors.i = m.a * m.e - m.b * m.d;
    
    // NOTE(christian): Rule of Sarrus.
    T det = m.a * minors.a + m.b * minors.d + m.c * minors.g;
    if (det != 0) {
        return (minors * (T)(1.0/det));
    } else {
        return m;
    }
}

template <class T>
fsMat<T, 4, 4> fs_matrix_inverse(const fsMat<T, 4, 4> m)
{
    fsMat<T, 4, 4> adjugate;
    adjugate.a = (m.f * m.k * m.p) + (m.g * m.l * m.n) + (m.h * m.j * m.o) - (m.h * m.k * m.n) - (m.g * m.j * m.p) - (m.f * m.l * m.o);
    adjugate.b = -(m.b * m.k * m.p) - (m.c * m.l * m.n) - (m.d * m.j * m.o) + (m.d * m.k * m.n) + (m.c * m.j * m.p) + (m.b * m.l * m.o);
    adjugate.c = (m.b * m.g * m.p) + (m.c * m.h * m.n) + (m.d * m.f * m.o) - (m.d * m.g * m.n) - (m.c * m.f * m.p) - (m.b * m.h * m.o);
    adjugate.d = -(m.b * m.g * m.l) - (m.c * m.h * m.j) - (m.d * m.f * m.k) + (m.d * m.g * m.j) + (m.c * m.f * m.l) + (m.b * m.h * m.k);
    
    adjugate.e = -(m.e * m.k * m.p) - (m.g * m.l * m.m) - (m.h * m.i * m.o) + (m.h * m.k * m.m) + (m.g * m.i * m.p) + (m.e * m.l * m.o);
    adjugate.f = (m.a * m.k * m.p) + (m.c * m.l * m.m) + (m.d * m.i * m.o) - (m.d * m.k * m.m) - (m.c * m.i * m.p) - (m.a * m.l * m.o);
    adjugate.g = -(m.a * m.g * m.p) - (m.c * m.h * m.m) - (m.d * m.e * m.o) + (m.d * m.g * m.m) + (m.c * m.e * m.p) + (m.a * m.h * m.o);
    adjugate.h = (m.a * m.g * m.l) + (m.c * m.h * m.i) + (m.d * m.e * m.k) - (m.d * m.g * m.i) - (m.c * m.e * m.l) - (m.a * m.h * m.k);
    
    adjugate.i = (m.e * m.j * m.p) + (m.f * m.l * m.m) + (m.h * m.i * m.n) - (m.h * m.j * m.m) - (m.f * m.i * m.p) - (m.e * m.l * m.n);
    adjugate.j = -(m.a * m.i * m.p) - (m.b * m.l * m.m) - (m.d * m.i * m.n) + (m.d * m.i * m.m) + (m.b * m.i * m.p) + (m.a * m.l * m.n);
    adjugate.k = (m.a * m.f * m.p) + (m.b * m.h * m.m) + (m.d * m.e * m.n) - (m.d * m.f * m.m) - (m.b * m.e * m.p) - (m.a * m.h * m.n);
    adjugate.l = -(m.a * m.f * m.l) - (m.b * m.h * m.i) - (m.d * m.e * m.j) + (m.d * m.f * m.i) + (m.b * m.e * m.l) + (m.a * m.h * m.j);
    
    adjugate.m = -(m.e * m.i * m.o) - (m.f * m.k * m.m) - (m.g * m.i * m.n) + (m.g * m.i * m.m) + (m.f * m.i * m.o) + (m.e * m.k * m.n);
    adjugate.n = (m.a * m.j * m.o) + (m.b * m.k * m.m) + (m.c * m.i * m.n) - (m.c * m.j * m.m) - (m.b * m.i * m.o) - (m.a * m.k * m.n);
    adjugate.o = -(m.a * m.f * m.o) - (m.b * m.g * m.m) - (m.c * m.e * m.n) + (m.c * m.f * m.m) + (m.b * m.e * m.o) + (m.a * m.g * m.n);
    adjugate.p = (m.a * m.f * m.k) + (m.b * m.g * m.i) + (m.c * m.e * m.j) - (m.c * m.f * m.i) - (m.b * m.e * m.k) - (m.a * m.g * m.j);
    
    T det = fs_matrix_determinant(m);
    if (det != 0) {
        return ((T)1/det) * adjugate;
    } else {
        return m;
    }
}

template <class T>
fsMat<T, 2, 2> fs_matrix_transpose(const fsMat<T, 2, 2> m)
{
    fsMat<T, 2, 2> result = {m.a, m.c, m.b, m.d};
    return result;
}

template <class T>
fsMat<T, 3, 3> fs_matrix_transpose(const fsMat<T, 3, 3> m)
{
    fsMat<T, 3, 3> result = {m.a, m.d, m.g, m.b, m.e, m.h, m.c, m.f, m.i};
    return result;
}

template <class T>
fsMat<T, 4, 4> fs_matrix_transpose(const fsMat<T, 4, 4> m)
{
    fsMat<T, 4, 4> result = {m.a, m.e, m.i, m.m, m.b, m.f, m.j, m.n, m.c, m.g, m.k, m.o, m.d, m.h, m.l, m.p};
    return result;
}

template <class T, int C, int R>
fsMat<T, R, C> fs_matrix_transpose(const fsMat<T, C, R> m)
{
    fsMat<T, R, C> result;
    for (int i = 0; i < C * R; ++i) {
        result.v[i] = m.v[C * (i%R) + (i/R)];
    }
    return result;
}

template <class T>
fsMat<T, 4, 4> fs_matrix_look_at(const fsVec<T, 3>& from, const fsVec<T, 3>& to, const fsVec<T, 3>& up)
{
    fsVec<T, 3> forward = fs_vnormalize(from - to);
    fsVec<T, 3> right = fs_vnormalize(fs_vcross(up, forward));
    fsVec<T, 3> newUp = fs_vcross(forward, right);
    
    fsMat<T, 4, 4> result = {
        right.x, right.y, right.z, 0.f,
        newUp.x, newUp.y, newUp.z, 0.f,
        forward.x, forward.y, forward.z, 0.f,
        -fs_vdot(right, from), -fs_vdot(newUp, from), -fs_vdot(forward, from), 1.f
    };
    
    return result;
}

template <class T>
fsMat<T, 4, 4> fs_matrix_perspective_fov(fsr32 fovRad, fsr32 width, fsr32 height, fsr32 near, fsr32 far)
{
    fsr32 yScale = 1.f / tanf(fovRad * 0.5f);
    fsr32 xScale = yScale * (height / width);
    fsMat<T, 4, 4> result = {
        xScale, 0.f, 0.f, 0.f,
        0.f, yScale, 0.f, 0.f,
        0.f, 0.f, -(far + near) / (far - near), -1.f,
        0.f, 0.f, -(2.f * far * near) / (far - near), 0.f
    };
    
    return result;
}


#pragma mark - Matrix/Vector Operators
// ==================================================================================
//                          Matrix/Vector Operators
// ==================================================================================

template <class T>
fsVec<T, 2> operator*(fsMat<T, 2, 2> m, fsVec<T, 2> v)
{
    fsVec<T, 2> result = {m.a * v.x + m.b * v.y, m.c * v.x + m.d * v.y};
    return result;
}

template <class T>
fsVec<T, 2> operator*(fsMat<T, 3, 2> m, fsVec<T, 3> v)
{
    fsVec<T, 2> result = {m.a * v.x + m.b * v.y + m.c * v.z, m.d * v.x + m.e * v.y + m.f * v.z};
    return result;
}

template <class T>
fsVec<T, 3> operator*(fsMat<T, 2, 3> m, fsVec<T, 2> v)
{
    fsVec<T, 3> result = {m.a * v.x + m.b * v.y, m.c * v.x + m.d * v.y, m.e * v.x + m.f * v.y};
    return result;
}

template <class T>
fsVec<T, 3> operator*(fsMat<T, 3, 3> m, fsVec<T, 3> v)
{
    fsVec<T, 3> result = {m.a * v.x + m.b * v.y + m.c * v.z, m.d * v.x + m.e * v.y + m.f * v.z, m.g * v.x + m.h * v.y + m.i * v.z};
    return result;
}

template <class T>
fsVec<T, 4> operator*(fsMat<T, 2, 4> m, fsVec<T, 2> v)
{
    fsVec<T, 4> result = {m.a * v.x + m.b * v.y, m.c * v.x + m.d * v.y, m.e * v.x + m.f * v.y, m.g * v.x + m.h * v.y};
    return result;
}

template <class T>
fsVec<T, 4> operator*(fsMat<T, 3, 4> m, fsVec<T, 3> v)
{
    fsVec<T, 4> result = {
        m.a * v.x + m.b * v.y + m.c * v.z,
        m.d * v.x + m.e * v.y + m.f * v.z,
        m.g * v.x + m.h * v.y + m.i * v.z,
        m.j * v.x + m.k * v.y + m.l * v.z};
    return result;
}

template <class T>
fsVec<T, 4> operator*(fsMat<T, 4, 4> m, fsVec<T, 4> v)
{
    fsVec<T, 4> result = {
        m.a * v.x + m.b * v.y + m.c * v.z + m.d * v.w,
        m.e * v.x + m.f * v.y + m.g * v.z + m.h * v.w,
        m.i * v.x + m.j * v.y + m.k * v.z + m.l * v.w,
        m.m * v.x + m.n * v.y + m.o * v.z + m.p * v.w};
    return result;
}

template <class T, int C>
fsVec<T, 2> operator*(fsMat<T, C, 2> m, fsVec<T, C> v)
{
    fsVec<T, 2> result = {};
    
    for (int c = 0; c < C; ++c) {
        result.x += m.v[c] * v.e[c];
    }
    
    for (int c = 0; c < C; ++c) {
        result.y += m.v[c + C] * v.e[c];
    }
    
    return result;
}

template <class T, int C, int R>
fsVec<T, R> operator*(fsMat<T, C, R> m, fsVec<T, C> v)
{
    fsVec<T, R> result = {};
    
    for (int r = 0; r < R; ++r) {
        int offset = r * C;
        for (int c = 0; c < C; ++c) {
            result.e[r] += m.v[offset + c] * v.e[c];
        }
    }
    
    return result;
}

template <class T>
fsVec<T, 2> operator*(fsVec<T, 2> v, fsMat<T, 2, 2> m)
{
    fsVec<T, 2> result = {v.x * m.a + v.y * m.c, v.x * m.b + v.y * m.d};
    return result;
}

template <class T>
fsVec<T, 2> operator*(fsVec<T, 3> v, fsMat<T, 2, 3> m)
{
    fsVec<T, 2> result = {v.x * m.a + v.y * m.c + v.z * m.e, v.x * m.b + v.y * m.d + v.z * m.f};
    return result;
}

template <class T>
fsVec<T, 2> operator*(fsVec<T, 4> v, fsMat<T, 2, 4> m)
{
    fsVec<T, 2> result = {v.x * m.a + v.y * m.c + v.z * m.e + v.w * m.g, v.x * m.b + v.y * m.d + v.z * m.f + v.w * m.h};
    return result;
}

template <class T, int R>
fsVec<T, 2> operator*(fsVec<T, R> v, fsMat<T, 2, R> m)
{
    fsVec<T, 2> result = {};
    
    for (int r = 0; r < R; ++r) {
        result.x += v.e[r] * m.v[r * 2];
    }
    
    for (int r = 0; r < R; ++r) {
        result.y += v.e[r] * m.v[r * 2 + 1];
    }
    
    return result;
}

template <class T, int C, int R>
fsVec<T, C> operator*(fsVec<T, R> v, fsMat<T, C, R> m)
{
    fsVec<T, C> result = {};
    
    for (int c = 0; c < C; ++c) {
        for (int r = 0; r < R; ++r) {
            result.e[c] += v.e[r] * m.v[r * C + c];
        }
    }
    
    return result;
}
