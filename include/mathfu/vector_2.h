/*
* Copyright 2014 Google Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef MATHFU_VECTOR_2_H_
#define MATHFU_VECTOR_2_H_

#include "mathfu/vector.h"
#include "mathfu/utilities.h"

#include <math.h>

#if !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
#include <vectorial/simd2f.h>
#endif

namespace mathfu {

#if !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
template<>
class Vector<float, 2> {
 public:
  typedef float Scalar;

  inline Vector() {}

  inline Vector(const Vector<float, 2>& v) {
    data_.simd = v.data_.simd;
  }

  inline Vector(const simd2f& v) {
    data_.simd = v;
  }

  explicit inline Vector(const float& s) {
    data_.simd = simd2f_create(s, s);
  }

  inline Vector(const float& s1, const float& s2) {
    data_.simd = simd2f_create(s1, s2);
  }

  explicit inline Vector(const float* v) {
    data_.simd = simd2f_create(v[0], v[1]);
  }

  inline float& operator()(const int i) {
    return data_.float_array[i];
  }

  inline const float& operator()(const int i) const {
    return data_.float_array[i];
  }

  inline float& operator[](const int i) {
    return data_.float_array[i];
  }

  inline const float& operator[](const int i) const {
    return data_.float_array[i];
  }

  inline float& x() { return (*this)[0]; }
  inline float& y() { return (*this)[1]; }

  inline const float& x() const { return (*this)[0]; }
  inline const float& y() const { return (*this)[1]; }

  inline Vector<float, 2> operator-() const {
    return Vector<float, 2>(simd2f_sub(simd2f_zero(), data_.simd));
  }

  inline Vector<float, 2> operator*(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_mul(data_.simd,
                                       v.data_.simd));
  }

  inline Vector<float, 2> operator/(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_div(data_.simd,
                                       v.data_.simd));
  }

  inline Vector<float, 2> operator+(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_add(data_.simd,
                                       v.data_.simd));
  }

  inline Vector<float, 2> operator-(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_sub(data_.simd,
                                       v.data_.simd));
  }

  inline Vector<float, 2> operator*(const float& s) const {
    return Vector<float, 2>(simd2f_mul(data_.simd, simd2f_splat(s)));
  }

  inline Vector<float, 2> operator/(const float& s) const {
    return Vector<float, 2>(simd2f_mul(data_.simd,
                                       simd2f_splat(1 / s)));
  }

  inline Vector<float, 2> operator+(const float& s) const {
    return Vector<float, 2>(simd2f_add(data_.simd, simd2f_splat(s)));
  }

  inline Vector<float, 2> operator-(const float& s) const {
    return Vector<float, 2>(simd2f_sub(data_.simd, simd2f_splat(s)));
  }

  inline Vector<float, 2>& operator*=(const Vector<float, 2>& v) {
    data_.simd = simd2f_mul(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator/=(const Vector<float, 2>& v) {
    data_.simd = simd2f_div(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator+=(const Vector<float, 2>& v) {
    data_.simd = simd2f_add(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator-=(const Vector<float, 2>& v) {
    data_.simd = simd2f_sub(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator*=(const float& s) {
    data_.simd = simd2f_mul(data_.simd, simd2f_splat(s));
    return *this;
  }

  inline Vector<float, 2>& operator/=(const float& s) {
    data_.simd = simd2f_mul(data_.simd, simd2f_splat(1 / s));
    return *this;
  }

  inline Vector<float, 2>& operator+=(const float& s) {
    data_.simd = simd2f_add(data_.simd, simd2f_splat(s));
    return *this;
  }

  inline Vector<float, 2>& operator-=(const float& s) {
    data_.simd = simd2f_sub(data_.simd, simd2f_splat(s));
    return *this;
  }

  inline float LengthSquared() const {
    return simd2f_get_x(simd2f_dot2(data_.simd, data_.simd));
  }

  inline float Length() const {
    return simd2f_get_x(simd2f_length2(data_.simd));
  }

  inline float Normalize() {
    const float length = Length();
    data_.simd = simd2f_mul(data_.simd,
                                    simd2f_splat(1 / length));
    return length;
  }

  inline Vector<float, 2> Normalized() const {
    return simd2f_normalize2(data_.simd);
  }

  static inline float DotProduct(
    const Vector<float, 2>& v1, const Vector<float, 2>& v2) {
    return simd2f_get_x(simd2f_dot2(v1.data_.simd,
                                    v2.data_.simd));
  }

  static inline Vector<float, 2> HadamardProduct(
    const Vector<float, 2>& v1, const Vector<float, 2>& v2) {
    return Vector<float, 2>(simd2f_mul(v1.data_.simd,
                                       v2.data_.simd));
  }

  static inline Vector<float, 2> Lerp(
    const Vector<float, 2>& v1, const Vector<float, 2>& v2, float percent) {
    const Vector<float, 2> percentv(percent);
    const Vector<float, 2> one_minus_percent(
        simd2f_sub(simd2f_splat(1.0f), percentv.data_.simd));
    return Vector<float, 2>(
        simd2f_add(simd2f_mul(one_minus_percent.data_.simd,
                              v1.data_.simd),
                   simd2f_mul(percentv.data_.simd,
                              v2.data_.simd)));
  }

 private:
  union {
    simd2f simd;
    float float_array[2];
  } data_;
};
#endif  // !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT) &&
        // defined(__ARM_NEON__)

}  // namespace mathfu

#endif  // MATHFU_VECTOR_2_H_
