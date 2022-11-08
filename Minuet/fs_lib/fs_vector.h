/*  fs_vector.h - Flyingsand Vectors
 *  v. 0.1
 */

#pragma once
#include <cmath>
#include <type_traits>

#pragma mark - Vector

template <class T, int V>
union fsVec {
    static_assert(V > 1, "fsVec must contain more than 1 element.");
    static_assert(std::is_arithmetic<T>::value, "fsVec value type must be arithmetic.");
    
    T e[V];
};

template <class T>
union fsVec<T, 2> {
    struct { T x, y; };
    struct { T u, v; };
    struct { T w, h; };
    struct { T e[2]; };
};

template <class T>
union fsVec<T, 3> {
    struct { T x, y, z; };
    struct { T u, v, w; };
    struct { T r, g, b; };
    struct { fsVec<T, 2> xy; T pad0_; };
    struct { T pad1_; fsVec<T, 2> yz; };
    struct { fsVec<T, 2> uv; T pad2_; };
    struct { T pad3_; fsVec<T, 2> vw; };
    struct { T e[3]; };
};

template <class T>
union fsVec<T, 4> {
    struct { T x, y, z, w; };
    struct { T r, g, b, a; };
    struct { fsVec<T, 3> xyz; T pad0_; };
    struct { fsVec<T, 2> xy; T pad2_[2]; };
    struct { T pad3_; fsVec<T, 2> yz; T pad4_; };
    struct { T pad5_[2]; fsVec<T, 2> zw; };
    struct { fsVec<T, 3> rgb; T pad1_; };
    struct { T e[4]; };
};

#include <iostream>
template <class T, int V>
std::ostream& operator<< (std::ostream& os, const fsVec<T, V>& rhs)
{
    os << "fsVec<" << V << ">(" << rhs.e[0];
    for (int i = 1; i < V; ++i) {
        os << ", " << rhs.e[i];
    }
    os << ")";
    return os;
}

template <class T, int V, class... Args>
fsVec<T, V> fs_vcreate(Args... args)
{
    const int length = sizeof...(args);
    static_assert(length == V, "Number of arguments in creator function does not match template argument.");
    fsVec<T, V> result = {static_cast<T>(args)...};
    return result;
}

template <class T, int V>
fsVec<T, V> fs_vcreate(T val)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = val;
    }
    
    return result;
}

template <class T, class U>
U fs_vconvert_default_cast(T v)
{
    return (U)v;
}

/*! Converts the vector with V elements of type T to a vector with V elements of type U.
    The default converts using a simple cast, otherwise a function can be specified
    to perform a more customized conversion; e.g. rounding. */
template <class T, int V, class U>
fsVec<U, V> fs_vconvert(fsVec<T, V> v, U(*F)(T) = fs_vconvert_default_cast)
{
    fsVec<U, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = F(v.e[i]);
    }

    return result;
}


#pragma mark - Operators
// ==================================================================================
//                          Operators
// ==================================================================================

template <class T, int V>
fsVec<T, V>& operator+=(fsVec<T, V>& v, const fsVec<T, V> rhs)
{
    v = v + rhs;
    return v;
}

template <class T>
fsVec<T, 2> operator+(fsVec<T, 2> v1, const fsVec<T, 2> v2)
{
    fsVec<T, 2> result = {v1.x + v2.x, v1.y + v2.y};
    return result;
}

template <class T>
fsVec<T, 3> operator+(fsVec<T, 3> v1, const fsVec<T, 3> v2)
{
    fsVec<T, 3> result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return result;
}

template <class T>
fsVec<T, 4> operator+(fsVec<T, 4> v1, const fsVec<T, 4> v2)
{
    fsVec<T, 4> result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
    return result;
}

template <class T, int V>
fsVec<T, V> operator+(fsVec<T, V> v1, const fsVec<T, V> v2)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = v1.e[i] + v2.e[i];
    }
    
    return result;
}

template <class T, int V>
fsVec<T, V>& operator-=(fsVec<T, V>& v, const fsVec<T, V> rhs)
{
    v = v - rhs;
    return v;
}

template <class T>
fsVec<T, 2> operator-(fsVec<T, 2> v1, const fsVec<T, 2> v2)
{
    fsVec<T, 2> result = {v1.x - v2.x, v1.y - v2.y};
    return result;
}

template <class T>
fsVec<T, 3> operator-(fsVec<T, 3> v1, const fsVec<T, 3> v2)
{
    fsVec<T, 3> result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    return result;
}

template <class T>
fsVec<T, 4> operator-(fsVec<T, 4> v1, const fsVec<T, 4> v2)
{
    fsVec<T, 4> result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
    return result;
}

template <class T, int V>
fsVec<T, V> operator-(fsVec<T, V> v1, const fsVec<T, V> v2)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = v1.e[i] - v2.e[i];
    }
    
    return result;
}

template <class T>
fsVec<T, 2> operator*(T scalar, fsVec<T, 2> v)
{
    fsVec<T, 2> result = {v.x * scalar, v.y * scalar};
    return result;
}

template <class T>
fsVec<T, 3> operator*(T scalar, fsVec<T, 3> v)
{
    fsVec<T, 3> result = {v.x * scalar, v.y * scalar, v.z * scalar};
    return result;
}

template <class T>
fsVec<T, 4> operator*(T scalar, fsVec<T, 4> v)
{
    fsVec<T, 4> result = {v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar};
    return result;
}

template <class T, int V>
fsVec<T, V> operator*(T scalar, fsVec<T, V> v)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i]  = scalar * v.e[i];
    }
    
    return result;
}

template <class T, int V>
fsVec<T, V> operator*(fsVec<T, V> v, T scalar)
{
    fsVec<T, V> result = scalar * v;
    return result;
}

template <class T, int V>
fsVec<T, V>& operator*=(fsVec<T, V>& v, T scalar)
{
    v = scalar * v;
    return v;
}

template <class T, int V>
fsVec<T, V> operator/(fsVec<T, V> v, T scalar)
{
    fsVec<T, V> result = v * (T)(1.0/scalar);
    return result;
}

template <class T>
fsVec<T, 2> operator/(T scalar, fsVec<T, 2> v)
{
    fsVec<T, 2> result = {scalar / v.x, scalar / v.y};
    return result;
}

template <class T>
fsVec<T, 3> operator/(T scalar, fsVec<T, 3> v)
{
    fsVec<T, 3> result = {scalar / v.x, scalar / v.y, scalar / v.z};
    return result;
}

template <class T>
fsVec<T, 4> operator/(T scalar, fsVec<T, 4> v)
{
    fsVec<T, 4> result = {scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w};
    return result;
}

template <class T, int V>
fsVec<T, V> operator/(T scalar, fsVec<T, V> v)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = scalar / v.e[i];
    }

    return result;
}

template <class T, int V>
fsVec<T, V>& operator/=(fsVec<T, V>& v, T scalar)
{
    v = v / scalar;
    return v;
}

template <class T>
fsVec<T, 2> operator-(fsVec<T, 2> vec)
{
    fsVec<T, 2> result = {-vec.x, -vec.y};
    return result;
}

template <class T>
fsVec<T, 3> operator-(fsVec<T, 3> vec)
{
    fsVec<T, 3> result = {-vec.x, -vec.y, -vec.z};
    return result;
}

template <class T>
fsVec<T, 4> operator-(fsVec<T, 4> vec)
{
    fsVec<T, 4> result = {-vec.x, -vec.y, -vec.z, -vec.w};
    return result;
}

template <class T, int V>
fsVec<T, V> operator-(fsVec<T, V> vec)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = -vec.e[i];
    }
    return result;
}

template <class T, int V>
bool operator==(fsVec<T, V> v1, fsVec<T, V> v2)
{
    for (int i = 0; i < V; ++i) {
        if (v1.e[i] != v2.e[i]) {
            return false;
        }
    }
    return true;
}

template <class T, int V>
bool operator!=(fsVec<T, V> v1, fsVec<T, V> v2)
{
    return !(v1 == v2);
}


#pragma mark - Functions
// ==================================================================================
//                          Functions
// ==================================================================================

/*! Returns the dot product of the two vectors. */
template <class T>
T fs_vdot(fsVec<T, 2> v1, fsVec<T, 2> v2)
{
    T result = (v1.x * v2.x) + (v1.y * v2.y);
    return result;
}

/*! Returns the dot product of the two vectors. */
template <class T>
T fs_vdot(fsVec<T, 3> v1, fsVec<T, 3> v2)
{
    T result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return result;
}

/*! Returns the dot product of the two vectors. */
template <class T>
T fs_vdot(fsVec<T, 4> v1, fsVec<T, 4> v2)
{
    T result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
    return result;
}

/*! Returns the dot product of the two vectors. */
template <class T, int V>
T fs_vdot(fsVec<T, V> v1, fsVec<T, V> v2)
{
    T result = 0;
    for (int i = 0; i < V; ++i) {
        result += (v1.e[i] * v2.e[i]);
    }
    
    return result;
}

/*! Returns the pseudo-cross product for 2d vectors. */
template <class T>
T fs_vcross(fsVec<T, 2> v1, fsVec<T, 2> v2)
{
    T result = v1.y * v2.x - v1.x * v2.y;
    return result;
}

/*! Returns the cross product of the two vectors. */
template <class T>
fsVec<T, 3> fs_vcross(fsVec<T, 3> v1, fsVec<T, 3> v2)
{
    fsVec<T, 3> result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = -(v1.x * v2.z - v1.z * v2.x);
    result.z = v1.x * v2.y - v1.y * v2.x;
    
    return result;
}

/*! Returns the length squared of the vector. */
template <class T, int V>
T fs_vlength2(fsVec<T, V> v)
{
    T result = fs_vdot(v, v);
    return result;
}

/*! Returns the length of the vector. */
template <class T, int V>
fsr32 fs_vlength(fsVec<T, V> v)
{
    fsr32 result = sqrtf((fsr32)fs_vlength2(v));
    return result;
}

/*! Returns the distance squared between two vectors. */
template <class T, int V>
T fs_vdistance2(fsVec<T, V> v1, fsVec<T, V> v2)
{
    T sum = 0;
    for (int i = 0; i < V; ++i) {
        T diff = (v2.e[i] - v1.e[i]);
        sum += (diff * diff);
    }
    return sum;
}

/*! Returns the distance between two vectors. */
template <class T, int V>
fsr32 fs_vdistance(fsVec<T, V> v1, fsVec<T, V> v2)
{
    fsr32 result = sqrtf(fs_vdistance2(v1, v2));
    return result;
}

/*! Returns the normalized vector. */
template <class T, int V>
fsVec<T, V> fs_vnormalize(fsVec<T, V> v)
{
    static_assert(std::is_floating_point<T>::value, "Vector normalize requires floating point value type.");
    T length = (T)sqrt(fs_vdot(v, v));
    fsVec<T, V> result = v * ((T)1.0 / length);
    return result;
}

/*! Computes the hadamard product of the vector (element-by-element multiplication). */
template <class T>
fsVec<T, 2> fs_vhadamard(fsVec<T, 2> v1, fsVec<T, 2> v2)
{
    fsVec<T, 2> result = {v1.x * v2.x, v1.y * v2.y};
    return result;
}

/*! Computes the hadamard product of the vector (element-by-element multiplication). */
template <class T>
fsVec<T, 3> fs_vhadamard(fsVec<T, 3> v1, fsVec<T, 3> v2)
{
    fsVec<T, 3> result = {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
    return result;
}

/*! Computes the hadamard product of the vector (element-by-element multiplication). */
template <class T>
fsVec<T, 4> fs_vhadamard(fsVec<T, 4> v1, fsVec<T, 4> v2)
{
    fsVec<T, 3> result = {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
    return result;
}

/*! Computes the hadamard product of the vector (element-by-element multiplication). */
template <class T, int V>
fsVec<T, V> fs_vhadamard(fsVec<T, V> v1, fsVec<T, V> v2)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = v1.e[i] * v2.e[i];
    }
    
    return result;
}

/*! Returns a vector with each element clamped between min and max. */
template <class T>
fsVec<T, 2> fs_vclamp(fsVec<T, 2> v, T min, T max)
{
    fsVec<T, 2> result = {fsClamp(v.x, min, max), fsClamp(v.y, min, max)};
    return result;
}

/*! Returns a vector with each element clamped between min and max. */
template <class T>
fsVec<T, 3> fs_vclamp(fsVec<T, 3> v, T min, T max)
{
    fsVec<T, 3> result = {fsClamp(v.x, min, max), fsClamp(v.y, min, max), fsClamp(v.z, min, max)};
    return result;
}

/*! Returns a vector with each element clamped between min and max. */
template <class T>
fsVec<T, 4> fs_vclamp(fsVec<T, 4> v, T min, T max)
{
    fsVec<T, 4> result = {fsClamp(v.x, min, max), fsClamp(v.y, min, max), fsClamp(v.z, min, max), fsClamp(v.w, min, max)};
    return result;
}

/*! Returns a vector with each element clamped between min and max. */
template <class T, int V>
fsVec<T, V> fs_vclamp(fsVec<T, V> v, T min, T max)
{
    fsVec<T, V> result;
    for (int i = 0; i < V; ++i) {
        result.e[i] = (v.e[i] < min ? min :
                       (v.e[i] > max ? max : v.e[i]));
    }
    
    return result;
}

/*! Returns a vector with each element clamped between 0 and 1. */
template <class T, int V>
fsVec<T, V> fs_vclamp01(fsVec<T, V> v)
{
    return fs_vclamp(v, (T)0, (T)1);
}

/*! Returns the linear interpolation between A and B with respect to t. */
template <class T, int V>
fsVec<T, V> fs_vlerp(fsVec<T, V> A, float t, fsVec<T, V> B)
{
    fsAssert(t >= 0.f && t <= 1.f);
    fsVec<T, V> result = (1.f - t)*A + t*B;
    return result;
}

/*! Returns the vector perpendicular to the given vector. */
template <class T>
fsVec<T, 2> fs_vperp(fsVec<T, 2> vec)
{
    fsVec<T, 2> result = { -vec.y, vec.x };
    return result;
}

/*! Returns the reflection of the given vector against the vector normal. */
template <class T>
fsVec<T, 2> fs_vreflect(fsVec<T, 2> vec, fsVec<T, 2> normal) {
    fsVec<T, 2> result = vec - (T)2.0 * normal * fs_vdot(vec, normal);
    return result;
}

/*! Returns the reflection of the given vector against the vector normal. */
template <class T>
fsVec<T, 3> fs_vreflect(fsVec<T, 3> vec, fsVec<T, 3> normal) {
    fsVec<T, 3> result = vec - (T)2.0 * normal * fs_vdot(vec, normal);
    return result;
}

/*! Returns the vector v rotated by the given angle in radians. */
template <class T>
fsVec<T, 2> fs_vrotate(fsVec<T, 2> v, fsr32 radians)
{
    fsVec<T, 2> result;
    
    fsr32 st = sinf(radians);
    fsr32 ct = cosf(radians);
    result.x = v.x * ct - v.y * st;
    result.y = v.x * st + v.y * ct;
    
    return result;
}

template <class T>
fsr32 fs_vangle(fsVec<T, 2> v1, fsVec<T, 2> v2)
{
    // TODO(christian): Can this be optimized?
    fsr32 temp = fs_vdot(v1, v2) / (fs_vlength(v1) * fs_vlength(v2));
    return acosf(temp);
}

/*! Returns the orientation of the three vectors:
    -1 : counter-clockwise
     0 : collinear
    +1 : clockwise */
template <class T>
int fs_vorientation(fsVec<T, 2> v1, fsVec<T, 2> v2, fsVec<T, 2> v3)
{
    // NOTE(christian): Calculated based on the slopes of the two line segments v1->v2, v2->v3.
    T val = ((v2.y - v1.y) * (v3.x - v2.x)) - ((v3.y - v2.y) * (v2.x - v1.x));
    return (val == 0 ? 0 : (val < 0 ? -1 : 1));
}

/*! Returns true if line segments p1->p2, q1->q2 intersect, and sets _ip_ to the point of intersection. */
template <class T, class U>
bool fs_line_segment_intersection(fsVec<T, 2> p1, fsVec<T, 2> p2, fsVec<T, 2> q1, fsVec<T, 2> q2, fsVec<U, 2> *ip = nullptr)
{
    fsVec<T, 2> r = p2 - p1;
    fsVec<T, 2> s = q2 - q1;
    U denom = (U)(r.x * s.y - r.y * s.x);
    
    if (denom == 0) {
        return false; // NOTE(christian): Collinear
    }
    
    fsVec<T, 2> qp = q1 - p1;
    
    U t = (U)(qp.x * s.y - qp.y * s.x) / denom;
    if ((t < 0) || (t > 1)) {
        return false;
    }
    
    U u = (U)(qp.x * r.y - qp.y * r.x) / denom;
    if ((u < 0) || (u > 1)) {
        return false;
    }
    
    if (ip) {
#if FS_COMPILER_CLANG
        *ip = fs_vconvert<T, 2, U>(p1) + (t * fs_vconvert<T, 2, U>(r));
#elif FS_COMPILER_MSVC
        U x = p1.x + (t * r.x);
        U y = p1.y + (t * r.y);
        ip->x = x;
        ip->y = y;
#endif
    }
    
    return true;
}

/*! Returns true if the lines defined by points p1-p2 and q1-q1 intersect, and sets _ip_ to the point of intersection.
    This function treats the two lines  s extending off to infinity in both directions. To test for line _segment_ intersection,
    use fs_line_segment_intersection. */
template <class T, class U>
bool fs_line_intersection(fsVec<T, 2> p1, fsVec<T, 2> p2, fsVec<T, 2> q1, fsVec<T, 2> q2, fsVec<U, 2> *ip = nullptr)
{
    fsVec<T, 2> r = p2 - p1;
    fsVec<T, 2> s = q2 - q1;
    U denom = (U)(r.x * s.y - r.y * s.x);
    
    if (denom == 0) {
        return false; // NOTE(christian): Collinear
    }
    
    fsVec<T, 2> qp = q1 - p1;
    
    U t = (U)(qp.x * s.y - qp.y * s.x) / denom;
    
    if (ip) {
#if FS_COMPILER_CLANG
        *ip = fs_vconvert<T, 2, U>(p1) + (t * fs_vconvert<T, 2, U>(r));
#elif FS_COMPILER_MSVC
        U x = p1.x + (t * r.x);
        U y = p1.y + (t * r.y);
        ip->x = x;
        ip->y = y;
#endif
    }
    
    return true;
}
