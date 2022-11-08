/* fs_quaternion.h - Flyingsand Quaternions
 * v. 0.1
 */

#pragma once
#include <cmath>
#include <type_traits>
#include "fs_vector.h"


template <class T>
struct fsQuat {
    T s;
    fsVec<T, 3> v;
};

#include <iostream>
template <class T>
std::ostream& operator<< (std::ostream& os, const fsQuat<T>& rhs)
{
    os << "fsQuat[" << rhs.s << ", " << rhs.v.x << "i, " << rhs.v.y << "j, " << rhs.v.z << "k]";
    return os;
}

template <class T>
fsQuat<T> fs_quat_create(const T scalar, const fsVec<T, 3> vector)
{
    fsQuat<T> result = {scalar, vector};
    return result;
}

template <class T>
fsQuat<T> fs_quat_create(const T scalar, const T x, const T y, const T z)
{
    return fs_quat_create(scalar, {x, y, z});
}


#pragma mark - Operators
// ==================================================================================
//                          Operators
// ==================================================================================

template <class T>
fsQuat<T>& operator+=(fsQuat<T>& q, const fsQuat<T> rhs)
{
    q = q + rhs;
    return q;
}

template <class T>
fsQuat<T> operator+(fsQuat<T> q1, const fsQuat<T> q2)
{
    fsQuat<T> result = {q1.s + q2.s, q1.v + q2.v};
    return result;
}

template <class T>
fsQuat<T>& operator-=(fsQuat<T>& q, const fsQuat<T> rhs)
{
    q = q - rhs;
    return q;
}

template <class T>
fsQuat<T> operator-(fsQuat<T> q1, const fsQuat<T> q2)
{
    fsQuat<T> result = {q1.s - q2.s, q1.v - q2.v};
    return result;
}

template <class T>
fsQuat<T> operator*(const fsQuat<T> q1, const fsQuat<T> q2)
{
    T s = (q1.s * q2.s) - fs_vdot(q1.v, q2.v);
    fsVec<T, 3> v = (q1.s * q2.v) + (q2.s * q1.v) + fs_vcross(q1.v, q2.v);
    fsQuat<T> result = {s, v};
    return result;
}

template <class T>
fsQuat<T>& operator*=(fsQuat<T>& q, const fsQuat<T> rhs)
{
    q = q * rhs;
    return q;
}

template <class T>
fsQuat<T> operator*(const fsQuat<T> q, const T scalar)
{
    fsQuat<T> result = {q.s * scalar, q.v * scalar};
    return result;
}

template <class T>
fsQuat<T> operator*(const T scalar, const fsQuat<T> q)
{
    fsQuat<T> result = q * scalar;
    return result;
}

template <class T>
fsQuat<T>& operator*=(fsQuat<T>& q, const T scalar)
{
    q = q * scalar;
    return q;
}


#pragma mark - Matrix/Vector Operators
// ==================================================================================
//                          Matrix/Vector Operators
// ==================================================================================

template <class T>
T fs_quat_magnitude(const fsQuat<T> q)
{
    T s2 = q.s * q.s;
    T v2 = fs_vdot(q.v, q.v);
    return (T)sqrt(s2 + v2);
}

template <class T>
fsQuat<T> fs_quat_normalize(const fsQuat<T> q)
{
    T mag = fs_quat_magnitude(q);
    if (mag != 0) {
        fsQuat<T> result = (T)(1/mag) * q;
        return result;
    } else {
        return q;
    }
}

template <class T>
fsQuat<T> fs_quat_unit_normal(const fsQuat<T> q)
{
    T rad = q.s;
    fsQuat<T> result = {(T)cos(rad * 0.5), (T)sin(rad * 0.5) * fs_vnormalize(q.v)};
    return result;
}

template <class T>
fsQuat<T> fs_quat_unit_normal(const T angleDeg, const fsVec<T, 3> axis)
{
    fsQuat<T> q = {angleDeg, axis};
    fsQuat<T> result = fs_quat_unit_normal(q);
    return result;
}

template <class T>
fsQuat<T> fs_quat_conjugate(const fsQuat<T> q)
{
    fsQuat<T> result = {q.s, -q.v};
    return result;
}

template <class T>
fsQuat<T> fs_quat_inverse(const fsQuat<T> q)
{
    fsQuat<T> conjugate = fs_quat_conjugate(q);
    T mag = fs_quat_magnitude(q);
    fsQuat<T> result = ((T)1/(mag * mag)) * conjugate;
    return result;
}

template <class T>
fsVec<T, 3> fs_quat_rotate(const fsVec<T, 3> v, const fsQuat<T> q)
{
    fsQuat<T> p = {0, v};
    fsQuat<T> result = q * p * fs_quat_inverse(q);
    return result.v;
}
