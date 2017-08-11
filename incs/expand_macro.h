/*
** Thanks to https://github.com/irondoge
*/

#pragma once
#ifndef EXPAND_MACRO_H_
# define EXPAND_MACRO_H_

# define __SELECT( \
    _0, _1, _2, _3, _4, _5, _6, _7, \
    _8, _9, _10, _11, _12, _13, _14, _15, \
    _16, _17, _18, _19, _20, _21, _22, _23, \
    _24, _25, _26, _27, _28, _29, _30, _31, \
    N, ...) N
  
  # define __CMEX1(MACRO, ARG) MACRO(ARG)
  # define __CMEX2(MACRO, ARG, ...) MACRO(ARG), __CMEX1(MACRO, __VA_ARGS__)
  # define __CMEX3(MACRO, ARG, ...) MACRO(ARG), __CMEX2(MACRO, __VA_ARGS__)
  # define __CMEX4(MACRO, ARG, ...) MACRO(ARG), __CMEX3(MACRO, __VA_ARGS__)
  # define __CMEX5(MACRO, ARG, ...) MACRO(ARG), __CMEX4(MACRO, __VA_ARGS__)
  # define __CMEX6(MACRO, ARG, ...) MACRO(ARG), __CMEX5(MACRO, __VA_ARGS__)
  # define __CMEX7(MACRO, ARG, ...) MACRO(ARG), __CMEX6(MACRO, __VA_ARGS__)
  # define __CMEX8(MACRO, ARG, ...) MACRO(ARG), __CMEX7(MACRO, __VA_ARGS__)
  # define __CMEX9(MACRO, ARG, ...) MACRO(ARG), __CMEX8(MACRO, __VA_ARGS__)
  # define __CMEX10(MACRO, ARG, ...) MACRO(ARG), __CMEX9(MACRO, __VA_ARGS__)
  # define __CMEX11(MACRO, ARG, ...) MACRO(ARG), __CMEX10(MACRO, __VA_ARGS__)
  # define __CMEX12(MACRO, ARG, ...) MACRO(ARG), __CMEX11(MACRO, __VA_ARGS__)
  # define __CMEX13(MACRO, ARG, ...) MACRO(ARG), __CMEX12(MACRO, __VA_ARGS__)
  # define __CMEX14(MACRO, ARG, ...) MACRO(ARG), __CMEX13(MACRO, __VA_ARGS__)
  # define __CMEX15(MACRO, ARG, ...) MACRO(ARG), __CMEX14(MACRO, __VA_ARGS__)
  # define __CMEX16(MACRO, ARG, ...) MACRO(ARG), __CMEX15(MACRO, __VA_ARGS__)
  # define __CMEX17(MACRO, ARG, ...) MACRO(ARG), __CMEX16(MACRO, __VA_ARGS__)
  # define __CMEX18(MACRO, ARG, ...) MACRO(ARG), __CMEX17(MACRO, __VA_ARGS__)
  # define __CMEX19(MACRO, ARG, ...) MACRO(ARG), __CMEX18(MACRO, __VA_ARGS__)
  # define __CMEX20(MACRO, ARG, ...) MACRO(ARG), __CMEX19(MACRO, __VA_ARGS__)
  # define __CMEX21(MACRO, ARG, ...) MACRO(ARG), __CMEX20(MACRO, __VA_ARGS__)
  # define __CMEX22(MACRO, ARG, ...) MACRO(ARG), __CMEX21(MACRO, __VA_ARGS__)
  # define __CMEX23(MACRO, ARG, ...) MACRO(ARG), __CMEX22(MACRO, __VA_ARGS__)
  # define __CMEX24(MACRO, ARG, ...) MACRO(ARG), __CMEX23(MACRO, __VA_ARGS__)
  # define __CMEX25(MACRO, ARG, ...) MACRO(ARG), __CMEX24(MACRO, __VA_ARGS__)
  # define __CMEX26(MACRO, ARG, ...) MACRO(ARG), __CMEX25(MACRO, __VA_ARGS__)
  # define __CMEX27(MACRO, ARG, ...) MACRO(ARG), __CMEX26(MACRO, __VA_ARGS__)
  # define __CMEX28(MACRO, ARG, ...) MACRO(ARG), __CMEX27(MACRO, __VA_ARGS__)
  # define __CMEX29(MACRO, ARG, ...) MACRO(ARG), __CMEX28(MACRO, __VA_ARGS__)
  # define __CMEX30(MACRO, ARG, ...) MACRO(ARG), __CMEX29(MACRO, __VA_ARGS__)
  # define __CMEX31(MACRO, ARG, ...) MACRO(ARG), __CMEX30(MACRO, __VA_ARGS__)
  # define __CMEX32(MACRO, ARG, ...) MACRO(ARG), __CMEX31(MACRO, __VA_ARGS__)
  # define COMMA_MACRO_EXPAND(MACRO, ...) __SELECT(__VA_ARGS__, \
    __CMEX32, __CMEX31, __CMEX30, __CMEX29, __CMEX28, __CMEX27, __CMEX26, __CMEX25, \
    __CMEX24, __CMEX23, __CMEX22, __CMEX21, __CMEX20, __CMEX19, __CMEX18, __CMEX17, \
    __CMEX16, __CMEX15, __CMEX14, __CMEX13, __CMEX12, __CMEX11, __CMEX10, __CMEX9, \
    __CMEX8, __CMEX7, __CMEX6, __CMEX5, __CMEX4, __CMEX3, __CMEX2, __CMEX1 \
    )(MACRO, __VA_ARGS__)  

#endif