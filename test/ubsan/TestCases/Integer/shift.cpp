// RUN: %clangxx -DLSH_OVERFLOW -DOP='<<' -fsanitize=shift-base -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-LSH_OVERFLOW
// RUN: %clangxx -DLSH_OVERFLOW -DOP='<<=' -fsanitize=shift -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-LSH_OVERFLOW
// RUN: %clangxx -DTOO_LOW -DOP='<<' -fsanitize=shift-exponent -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_LOW
// RUN: %clangxx -DTOO_LOW -DOP='>>' -fsanitize=shift -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_LOW
// RUN: %clangxx -DTOO_LOW -DOP='<<=' -fsanitize=shift -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_LOW
// RUN: %clangxx -DTOO_LOW -DOP='>>=' -fsanitize=shift -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_LOW
// RUN: %clangxx -DTOO_HIGH -DOP='<<' -fsanitize=shift-exponent -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_HIGH
// RUN: %clangxx -DTOO_HIGH -DOP='>>' -fsanitize=shift -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_HIGH
// RUN: %clangxx -DTOO_HIGH -DOP='<<=' -fsanitize=shift -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_HIGH
// RUN: %clangxx -DTOO_HIGH -DOP='>>=' -fsanitize=shift -fno-sanitize-recover=shift %s -o %t && not %run %t 2>&1 | FileCheck %s --check-prefix=CHECK-TOO_HIGH

// RUN: %clangxx -DLSH_OVERFLOW -DOP='<<' -fsanitize=shift-exponent -fno-sanitize-recover=shift %s -o %t && %run %t
// RUN: %clangxx -DLSH_OVERFLOW -DOP='>>' -fsanitize=shift-exponent -fno-sanitize-recover=shift %s -o %t && %run %t
// RUN: %clangxx -DTOO_LOW -DOP='<<' -fsanitize=shift-base -fno-sanitize-recover=shift %s -o %t && %run %t
// RUN: %clangxx -DTOO_LOW -DOP='>>' -fsanitize=shift-base -fno-sanitize-recover=shift %s -o %t && %run %t
// RUN: %clangxx -DTOO_HIGH -DOP='<<' -fsanitize=shift-base -fno-sanitize-recover=shift %s -o %t && %run %t
// RUN: %clangxx -DTOO_HIGH -DOP='>>' -fsanitize=shift-base -fno-sanitize-recover=shift %s -o %t && %run %t

#include <stdint.h>

int main() {
  int a = 1;
  unsigned b = 1;

  a <<= 31; // ok in C++11, not ok in C99/C11
  b <<= 31; // ok
  b <<= 1; // still ok, unsigned

#ifdef LSH_OVERFLOW
  // CHECK-LSH_OVERFLOW: shift.cpp:[[@LINE+1]]:5: runtime error: left shift of negative value -2147483648
  a OP 1;
#endif

#ifdef TOO_LOW
  a = 0;
  // CHECK-TOO_LOW: shift.cpp:[[@LINE+1]]:5: runtime error: shift exponent -3 is negative
  a OP (-3);
#endif

#ifdef TOO_HIGH
  a = 0;
  // CHECK-TOO_HIGH: shift.cpp:[[@LINE+1]]:5: runtime error: shift exponent 32 is too large for 32-bit type 'int'
  a OP 32;
#endif
}
