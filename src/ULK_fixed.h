/*

██    ██ ██      ██   ██         ███████ ██ ██   ██ ███████ ██████  
██    ██ ██      ██  ██          ██      ██  ██ ██  ██      ██   ██ 
██    ██ ██      █████           █████   ██   ███   █████   ██   ██ 
██    ██ ██      ██  ██          ██      ██  ██ ██  ██      ██   ██ 
 ██████  ███████ ██   ██ ███████ ██      ██ ██   ██ ███████ ██████  
                                                                    
*/

/* 
LICENSE OPTION A: 3-clause BSD

   Copyright (C) 2020-2021 Captain4LK (Lukas Holzbeierlein)

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

LICENSE OPTION B: Public Domain - Unlicense

   This is free and unencumbered software released into the public domain.

   Anyone is free to copy, modify, publish, use, compile, sell, or
   distribute this software, either in source code form or as a compiled
   binary, for any purpose, commercial or non-commercial, and by any
   means.

   In jurisdictions that recognize copyright laws, the author or authors
   of this software dedicate any and all copyright interest in the
   software to the public domain. We make this dedication for the benefit
   of the public at large and to the detriment of our heirs and
   successors. We intend this dedication to be an overt act of
   relinquishment in perpetuity of all present and future rights to this
   software under copyright law.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.

   For more information, please refer to <http://unlicense.org/>
*/

/*

██    ██ ███████  █████   ██████  ███████ 
██    ██ ██      ██   ██ ██       ██      
██    ██ ███████ ███████ ██   ███ █████   
██    ██      ██ ██   ██ ██    ██ ██      
 ██████  ███████ ██   ██  ██████  ███████ 
                                          
-----------------------------------------

1. Add ULK_fixed.c to your source files.
2. Add ULK_fixed.h to your include files.
3. Edit ULK_fixed.c, l.72 to match the path to ULK_fixed.h

Linker options: None
*/

#ifndef _ULK_FIXED_H_

#define _ULK_FIXED_H_

#define ULK_FIXED_ENABLE_16BIT 0
#define ULK_FIXED_ENABLE_32BIT 1
#define ULK_FIXED_ENABLE_64BIT 0 //Needs __128_t

#include <stdint.h>

#if ULK_FIXED_ENABLE_16BIT

#define ULK_FIXED16_PI 804
#define ULK_FIXED16_E 696

typedef int16_t ULK_fixed_16;
typedef ULK_fixed_16 ULK_fixed_16_v2d[2];
typedef ULK_fixed_16 ULK_fixed_16_v3d[3];
typedef ULK_fixed_16 ULK_fixed_16_v4d[4];
typedef ULK_fixed_16  ULK_fixed_16_m22[4];
typedef ULK_fixed_16  ULK_fixed_16_m33[9];
typedef ULK_fixed_16  ULK_fixed_16_m44[15];

//Normal mathematics
ULK_fixed_16 ULK_fixed_16_from_int(int in);
int          ULK_fixed_16_to_int(ULK_fixed_16 in);
ULK_fixed_16 ULK_fixed_16_div(ULK_fixed_16 a, ULK_fixed_16 b);
ULK_fixed_16 ULK_fixed_16_mul(ULK_fixed_16 a, ULK_fixed_16 b);
ULK_fixed_16 ULK_fixed_16_round(ULK_fixed_16 in);
ULK_fixed_16 ULK_fixed_16_floor(ULK_fixed_16 in);
ULK_fixed_16 ULK_fixed_16_ceil(ULK_fixed_16 in);
ULK_fixed_16 ULK_fixed_16_sin(ULK_fixed_16 in); //Angle between 0 and 8
ULK_fixed_16 ULK_fixed_16_cos(ULK_fixed_16 in); //Angle between 0 and 8
ULK_fixed_16 ULK_fixed_16_sqrt(ULK_fixed_16 in);
//No conversion from and to degree, since the values are too big for 16bit fixed point
ULK_fixed_16 ULK_fixed_16_rad_to_angle(ULK_fixed_16 in);
ULK_fixed_16 ULK_fixed_16_angle_to_rad(ULK_fixed_16 in);

//Vector mathematics
void         ULK_fixed_16_v2d_add(ULK_fixed_16_v2d out, const ULK_fixed_16_v2d a, const ULK_fixed_16_v2d b);
void         ULK_fixed_16_v3d_add(ULK_fixed_16_v3d out, const ULK_fixed_16_v3d a, const ULK_fixed_16_v3d b);
void         ULK_fixed_16_v4d_add(ULK_fixed_16_v4d out, const ULK_fixed_16_v4d a, const ULK_fixed_16_v4d b);
void         ULK_fixed_16_v3d_cross(ULK_fixed_16_v3d out, const ULK_fixed_16_v3d a, const ULK_fixed_16_v3d b);
ULK_fixed_16 ULK_fixed_16_v2d_dist(const ULK_fixed_16_v2d a, const ULK_fixed_16_v2d b);
ULK_fixed_16 ULK_fixed_16_v3d_dist(const ULK_fixed_16_v3d a, const ULK_fixed_16_v3d b);
ULK_fixed_16 ULK_fixed_16_v4d_dist(const ULK_fixed_16_v4d a, const ULK_fixed_16_v4d b);
ULK_fixed_16 ULK_fixed_16_v2d_dist2(const ULK_fixed_16_v2d a, const ULK_fixed_16_v2d b);
ULK_fixed_16 ULK_fixed_16_v3d_dist2(const ULK_fixed_16_v3d a, const ULK_fixed_16_v3d b);
ULK_fixed_16 ULK_fixed_16_v4d_dist2(const ULK_fixed_16_v4d a, const ULK_fixed_16_v4d b);
void         ULK_fixed_16_v2d_div(ULK_fixed_16_v2d out, const ULK_fixed_16_v2d in, ULK_fixed_16 s);
void         ULK_fixed_16_v3d_div(ULK_fixed_16_v3d out, const ULK_fixed_16_v3d in, ULK_fixed_16 s);
void         ULK_fixed_16_v4d_div(ULK_fixed_16_v4d out, const ULK_fixed_16_v4d in, ULK_fixed_16 s);
ULK_fixed_16 ULK_fixed_16_v2d_dot(const ULK_fixed_16_v2d a, const ULK_fixed_16_v2d b);
ULK_fixed_16 ULK_fixed_16_v3d_dot(const ULK_fixed_16_v3d a, const ULK_fixed_16_v3d b);
ULK_fixed_16 ULK_fixed_16_v4d_dot(const ULK_fixed_16_v4d a, const ULK_fixed_16_v4d b);
void         ULK_fixed_16_v2d_lerp(ULK_fixed_16_v2d out, const ULK_fixed_16_v2d a, const ULK_fixed_16_v2d b, ULK_fixed_16 t);
void         ULK_fixed_16_v3d_lerp(ULK_fixed_16_v3d out, const ULK_fixed_16_v3d a, const ULK_fixed_16_v3d b, ULK_fixed_16 t);
void         ULK_fixed_16_v4d_lerp(ULK_fixed_16_v4d out, const ULK_fixed_16_v4d a, const ULK_fixed_16_v4d b, ULK_fixed_16 t);
ULK_fixed_16 ULK_fixed_16_v2d_mag(const ULK_fixed_16_v2d in);
ULK_fixed_16 ULK_fixed_16_v3d_mag(const ULK_fixed_16_v3d in);
ULK_fixed_16 ULK_fixed_16_v4d_mag(const ULK_fixed_16_v4d in);
ULK_fixed_16 ULK_fixed_16_v2d_mag2(const ULK_fixed_16_v2d in);
ULK_fixed_16 ULK_fixed_16_v3d_mag2(const ULK_fixed_16_v3d in);
ULK_fixed_16 ULK_fixed_16_v4d_mag2(const ULK_fixed_16_v4d in);
void         ULK_fixed_16_v2d_mul(ULK_fixed_16_v2d out, const ULK_fixed_16_v2d in, ULK_fixed_16 s);
void         ULK_fixed_16_v3d_mul(ULK_fixed_16_v3d out, const ULK_fixed_16_v3d in, ULK_fixed_16 s);
void         ULK_fixed_16_v4d_mul(ULK_fixed_16_v4d out, const ULK_fixed_16_v4d in, ULK_fixed_16 s);
void         ULK_fixed_16_v2d_norm(ULK_fixed_16_v2d out, const ULK_fixed_16_v2d in);
void         ULK_fixed_16_v3d_norm(ULK_fixed_16_v3d out, const ULK_fixed_16_v3d in);
void         ULK_fixed_16_v4d_norm(ULK_fixed_16_v4d out, const ULK_fixed_16_v4d in);
void         ULK_fixed_16_v2d_sub(ULK_fixed_16_v2d out, const ULK_fixed_16_v2d a, const ULK_fixed_16_v2d b); 
void         ULK_fixed_16_v3d_sub(ULK_fixed_16_v3d out, const ULK_fixed_16_v3d a, const ULK_fixed_16_v3d b); 
void         ULK_fixed_16_v4d_sub(ULK_fixed_16_v4d out, const ULK_fixed_16_v4d a, const ULK_fixed_16_v4d b); 

//Matrix mathematics
void         ULK_fixed_16_m22_add(ULK_fixed_16_m22 out, const ULK_fixed_16_m22 a, const ULK_fixed_16_m22 b);
void         ULK_fixed_16_m33_add(ULK_fixed_16_m33 out, const ULK_fixed_16_m33 a, const ULK_fixed_16_m33 b);
void         ULK_fixed_16_m44_add(ULK_fixed_16_m44 out, const ULK_fixed_16_m44 a, const ULK_fixed_16_m44 b);
void         ULK_fixed_16_m22_copy(ULK_fixed_16_m22 dst, const ULK_fixed_16_m22 src);
void         ULK_fixed_16_m33_copy(ULK_fixed_16_m33 dst, const ULK_fixed_16_m33 src);
void         ULK_fixed_16_m44_copy(ULK_fixed_16_m44 dst, const ULK_fixed_16_m44 src);
ULK_fixed_16 ULK_fixed_16_m22_determinant(const ULK_fixed_16_m22 in);
ULK_fixed_16 ULK_fixed_16_m33_determinant(const ULK_fixed_16_m33 in);
void         ULK_fixed_16_m22_identity(ULK_fixed_16_m22 out);
void         ULK_fixed_16_m33_identity(ULK_fixed_16_m33 out);
void         ULK_fixed_16_m44_identity(ULK_fixed_16_m44 out);
void         ULK_fixed_16_m22_sub(ULK_fixed_16_m22 out, const ULK_fixed_16_m22 a, const ULK_fixed_16_m22 b);
void         ULK_fixed_16_m33_sub(ULK_fixed_16_m33 out, const ULK_fixed_16_m33 a, const ULK_fixed_16_m33 b);
void         ULK_fixed_16_m44_sub(ULK_fixed_16_m44 out, const ULK_fixed_16_m44 a, const ULK_fixed_16_m44 b);

#endif //ULK_FIXED_ENABLE_16BIT

#if ULK_FIXED_ENABLE_32BIT

#define ULK_FIXED32_PI 205887
#define ULK_FIXED32_E 178145

typedef int32_t ULK_fixed_32;
typedef ULK_fixed_32 ULK_fixed_32_v2d[2];
typedef ULK_fixed_32 ULK_fixed_32_v3d[3];
typedef ULK_fixed_32 ULK_fixed_32_v4d[4];
typedef ULK_fixed_32  ULK_fixed_32_m22[4];
typedef ULK_fixed_32  ULK_fixed_32_m33[9];
typedef ULK_fixed_32  ULK_fixed_32_m44[15];

//Normal mathematics
ULK_fixed_32 ULK_fixed_32_from_int(int in);
int          ULK_fixed_32_to_int(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_div(ULK_fixed_32 a, ULK_fixed_32 b);
ULK_fixed_32 ULK_fixed_32_mul(ULK_fixed_32 a, ULK_fixed_32 b);
ULK_fixed_32 ULK_fixed_32_round(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_floor(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_ceil(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_sin(ULK_fixed_32 in); //Angle between 0 and 8
ULK_fixed_32 ULK_fixed_32_cos(ULK_fixed_32 in); //Angle between 0 and 8
ULK_fixed_32 ULK_fixed_32_sqrt(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_deg_to_angle(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_rad_to_angle(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_angle_to_deg(ULK_fixed_32 in);
ULK_fixed_32 ULK_fixed_32_angle_to_rad(ULK_fixed_32 in);

typedef int32_t ULK_fixed;

//Normal mathematics
ULK_fixed ULK_fixed_from_int(int in);
int          ULK_fixed_to_int(ULK_fixed in);
ULK_fixed ULK_fixed_div(ULK_fixed a, ULK_fixed b);
ULK_fixed ULK_fixed_mul(ULK_fixed a, ULK_fixed b);
ULK_fixed ULK_fixed_round(ULK_fixed in);
ULK_fixed ULK_fixed_floor(ULK_fixed in);
ULK_fixed ULK_fixed_ceil(ULK_fixed in);
ULK_fixed ULK_fixed_sin(ULK_fixed in); //Angle between 0 and 8
ULK_fixed ULK_fixed_cos(ULK_fixed in); //Angle between 0 and 8
ULK_fixed ULK_fixed_sqrt(ULK_fixed in);
ULK_fixed ULK_fixed_deg_to_angle(ULK_fixed in);
ULK_fixed ULK_fixed_rad_to_angle(ULK_fixed in);
ULK_fixed ULK_fixed_angle_to_deg(ULK_fixed in);
ULK_fixed ULK_fixed_angle_to_rad(ULK_fixed in);

//Vector mathematics
void         ULK_fixed_32_v2d_add(ULK_fixed_32_v2d out, const ULK_fixed_32_v2d a, const ULK_fixed_32_v2d b);
void         ULK_fixed_32_v3d_add(ULK_fixed_32_v3d out, const ULK_fixed_32_v3d a, const ULK_fixed_32_v3d b);
void         ULK_fixed_32_v4d_add(ULK_fixed_32_v4d out, const ULK_fixed_32_v4d a, const ULK_fixed_32_v4d b);
void         ULK_fixed_32_v3d_cross(ULK_fixed_32_v3d out, const ULK_fixed_32_v3d a, const ULK_fixed_32_v3d b);
ULK_fixed_32 ULK_fixed_32_v2d_dist(const ULK_fixed_32_v2d a, const ULK_fixed_32_v2d b);
ULK_fixed_32 ULK_fixed_32_v3d_dist(const ULK_fixed_32_v3d a, const ULK_fixed_32_v3d b);
ULK_fixed_32 ULK_fixed_32_v4d_dist(const ULK_fixed_32_v4d a, const ULK_fixed_32_v4d b);
ULK_fixed_32 ULK_fixed_32_v2d_dist2(const ULK_fixed_32_v2d a, const ULK_fixed_32_v2d b);
ULK_fixed_32 ULK_fixed_32_v3d_dist2(const ULK_fixed_32_v3d a, const ULK_fixed_32_v3d b);
ULK_fixed_32 ULK_fixed_32_v4d_dist2(const ULK_fixed_32_v4d a, const ULK_fixed_32_v4d b);
void         ULK_fixed_32_v2d_div(ULK_fixed_32_v2d out, const ULK_fixed_32_v2d in, ULK_fixed_32 s);
void         ULK_fixed_32_v3d_div(ULK_fixed_32_v3d out, const ULK_fixed_32_v3d in, ULK_fixed_32 s);
void         ULK_fixed_32_v4d_div(ULK_fixed_32_v4d out, const ULK_fixed_32_v4d in, ULK_fixed_32 s);
void         ULK_fixed_32_v2d_lerp(ULK_fixed_32_v2d out, const ULK_fixed_32_v2d a, const ULK_fixed_32_v2d b, ULK_fixed_32 t);
void         ULK_fixed_32_v3d_lerp(ULK_fixed_32_v3d out, const ULK_fixed_32_v3d a, const ULK_fixed_32_v3d b, ULK_fixed_32 t);
void         ULK_fixed_32_v4d_lerp(ULK_fixed_32_v4d out, const ULK_fixed_32_v4d a, const ULK_fixed_32_v4d b, ULK_fixed_32 t);
ULK_fixed_32 ULK_fixed_32_v2d_dot(const ULK_fixed_32_v2d a, const ULK_fixed_32_v2d b);
ULK_fixed_32 ULK_fixed_32_v3d_dot(const ULK_fixed_32_v3d a, const ULK_fixed_32_v3d b);
ULK_fixed_32 ULK_fixed_32_v4d_dot(const ULK_fixed_32_v4d a, const ULK_fixed_32_v4d b);
ULK_fixed_32 ULK_fixed_32_v2d_mag(const ULK_fixed_32_v2d in);
ULK_fixed_32 ULK_fixed_32_v3d_mag(const ULK_fixed_32_v3d in);
ULK_fixed_32 ULK_fixed_32_v4d_mag(const ULK_fixed_32_v4d in);
ULK_fixed_32 ULK_fixed_32_v2d_mag2(const ULK_fixed_32_v2d in);
ULK_fixed_32 ULK_fixed_32_v3d_mag2(const ULK_fixed_32_v3d in);
ULK_fixed_32 ULK_fixed_32_v4d_mag2(const ULK_fixed_32_v4d in);
void         ULK_fixed_32_v2d_mul(ULK_fixed_32_v2d out, const ULK_fixed_32_v2d in, ULK_fixed_32 s);
void         ULK_fixed_32_v3d_mul(ULK_fixed_32_v3d out, const ULK_fixed_32_v3d in, ULK_fixed_32 s);
void         ULK_fixed_32_v4d_mul(ULK_fixed_32_v4d out, const ULK_fixed_32_v4d in, ULK_fixed_32 s);
void         ULK_fixed_32_v2d_norm(ULK_fixed_32_v2d out, const ULK_fixed_32_v2d in);
void         ULK_fixed_32_v3d_norm(ULK_fixed_32_v3d out, const ULK_fixed_32_v3d in);
void         ULK_fixed_32_v4d_norm(ULK_fixed_32_v4d out, const ULK_fixed_32_v4d in);
void         ULK_fixed_32_v2d_sub(ULK_fixed_32_v2d out, const ULK_fixed_32_v2d a, const ULK_fixed_32_v2d b); 
void         ULK_fixed_32_v3d_sub(ULK_fixed_32_v3d out, const ULK_fixed_32_v3d a, const ULK_fixed_32_v3d b); 
void         ULK_fixed_32_v4d_sub(ULK_fixed_32_v4d out, const ULK_fixed_32_v4d a, const ULK_fixed_32_v4d b); 

//Matrix mathematics
void         ULK_fixed_32_m22_add(ULK_fixed_32_m22 out, const ULK_fixed_32_m22 a, const ULK_fixed_32_m22 b);
void         ULK_fixed_32_m33_add(ULK_fixed_32_m33 out, const ULK_fixed_32_m33 a, const ULK_fixed_32_m33 b);
void         ULK_fixed_32_m44_add(ULK_fixed_32_m44 out, const ULK_fixed_32_m44 a, const ULK_fixed_32_m44 b);
void         ULK_fixed_32_m22_copy(ULK_fixed_32_m22 dst, const ULK_fixed_32_m22 src);
void         ULK_fixed_32_m33_copy(ULK_fixed_32_m33 dst, const ULK_fixed_32_m33 src);
void         ULK_fixed_32_m44_copy(ULK_fixed_32_m44 dst, const ULK_fixed_32_m44 src);
ULK_fixed_32 ULK_fixed_32_m22_determinant(const ULK_fixed_32_m22 in);
ULK_fixed_32 ULK_fixed_32_m33_determinant(const ULK_fixed_32_m33 in);
void         ULK_fixed_32_m22_identity(ULK_fixed_32_m22 out);
void         ULK_fixed_32_m33_identity(ULK_fixed_32_m33 out);
void         ULK_fixed_32_m44_identity(ULK_fixed_32_m44 out);
void         ULK_fixed_32_m22_sub(ULK_fixed_32_m22 out, const ULK_fixed_32_m22 a, const ULK_fixed_32_m22 b);
void         ULK_fixed_32_m33_sub(ULK_fixed_32_m33 out, const ULK_fixed_32_m33 a, const ULK_fixed_32_m33 b);
void         ULK_fixed_32_m44_sub(ULK_fixed_32_m44 out, const ULK_fixed_32_m44 a, const ULK_fixed_32_m44 b);

#endif //ULK_FIXED_ENABLE_32BIT

#if ULK_FIXED_ENABLE_64BIT

#define ULK_FIXED64_PI 13493038080L
#define ULK_FIXED64_E 11674931555L

typedef int64_t ULK_fixed_64;
typedef ULK_fixed_64 ULK_fixed_64_v2d[2];
typedef ULK_fixed_64 ULK_fixed_64_v3d[3];
typedef ULK_fixed_64 ULK_fixed_64_v4d[4];
typedef ULK_fixed_64  ULK_fixed_64_m22[4];
typedef ULK_fixed_64  ULK_fixed_64_m33[9];
typedef ULK_fixed_64  ULK_fixed_64_m44[15];

//Normal mathematics
ULK_fixed_64 ULK_fixed_64_from_int(int in);
int          ULK_fixed_64_to_int(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_div(ULK_fixed_64 a, ULK_fixed_64 b);
ULK_fixed_64 ULK_fixed_64_mul(ULK_fixed_64 a, ULK_fixed_64 b);
ULK_fixed_64 ULK_fixed_64_round(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_floor(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_ceil(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_sin(ULK_fixed_64 in); //Angle between 0 and 8
ULK_fixed_64 ULK_fixed_64_cos(ULK_fixed_64 in); //Angle between 0 and 8
ULK_fixed_64 ULK_fixed_64_sqrt(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_deg_to_angle(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_rad_to_angle(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_angle_to_deg(ULK_fixed_64 in);
ULK_fixed_64 ULK_fixed_64_angle_to_rad(ULK_fixed_64 in);

//Vector mathematics
void         ULK_fixed_64_v2d_add(ULK_fixed_64_v2d out, const ULK_fixed_64_v2d a, const ULK_fixed_64_v2d b);
void         ULK_fixed_64_v3d_add(ULK_fixed_64_v3d out, const ULK_fixed_64_v3d a, const ULK_fixed_64_v3d b);
void         ULK_fixed_64_v4d_add(ULK_fixed_64_v4d out, const ULK_fixed_64_v4d a, const ULK_fixed_64_v4d b);
void         ULK_fixed_64_v3d_cross(ULK_fixed_64_v3d out, const ULK_fixed_64_v3d a, const ULK_fixed_64_v3d b);
ULK_fixed_64 ULK_fixed_64_v2d_dist(const ULK_fixed_64_v2d a, const ULK_fixed_64_v2d b);
ULK_fixed_64 ULK_fixed_64_v3d_dist(const ULK_fixed_64_v3d a, const ULK_fixed_64_v3d b);
ULK_fixed_64 ULK_fixed_64_v4d_dist(const ULK_fixed_64_v4d a, const ULK_fixed_64_v4d b);
ULK_fixed_64 ULK_fixed_64_v2d_dist2(const ULK_fixed_64_v2d a, const ULK_fixed_64_v2d b);
ULK_fixed_64 ULK_fixed_64_v3d_dist2(const ULK_fixed_64_v3d a, const ULK_fixed_64_v3d b);
ULK_fixed_64 ULK_fixed_64_v4d_dist2(const ULK_fixed_64_v4d a, const ULK_fixed_64_v4d b);
void         ULK_fixed_64_v2d_div(ULK_fixed_64_v2d out, const ULK_fixed_64_v2d in, ULK_fixed_64 s);
void         ULK_fixed_64_v3d_div(ULK_fixed_64_v3d out, const ULK_fixed_64_v3d in, ULK_fixed_64 s);
void         ULK_fixed_64_v4d_div(ULK_fixed_64_v4d out, const ULK_fixed_64_v4d in, ULK_fixed_64 s);
ULK_fixed_64 ULK_fixed_64_v2d_dot(const ULK_fixed_64_v2d a, const ULK_fixed_64_v2d b);
ULK_fixed_64 ULK_fixed_64_v3d_dot(const ULK_fixed_64_v3d a, const ULK_fixed_64_v3d b);
ULK_fixed_64 ULK_fixed_64_v4d_dot(const ULK_fixed_64_v4d a, const ULK_fixed_64_v4d b);
void         ULK_fixed_64_v2d_lerp(ULK_fixed_64_v2d out, const ULK_fixed_64_v2d a, const ULK_fixed_64_v2d b, ULK_fixed_64 t);
void         ULK_fixed_64_v3d_lerp(ULK_fixed_64_v3d out, const ULK_fixed_64_v3d a, const ULK_fixed_64_v3d b, ULK_fixed_64 t);
void         ULK_fixed_64_v4d_lerp(ULK_fixed_64_v4d out, const ULK_fixed_64_v4d a, const ULK_fixed_64_v4d b, ULK_fixed_64 t);
ULK_fixed_64 ULK_fixed_64_v2d_mag(const ULK_fixed_64_v2d in);
ULK_fixed_64 ULK_fixed_64_v3d_mag(const ULK_fixed_64_v3d in);
ULK_fixed_64 ULK_fixed_64_v4d_mag(const ULK_fixed_64_v4d in);
ULK_fixed_64 ULK_fixed_64_v2d_mag2(const ULK_fixed_64_v2d in);
ULK_fixed_64 ULK_fixed_64_v3d_mag2(const ULK_fixed_64_v3d in);
ULK_fixed_64 ULK_fixed_64_v4d_mag2(const ULK_fixed_64_v4d in);
void         ULK_fixed_64_v2d_mul(ULK_fixed_64_v2d out, const ULK_fixed_64_v2d in, ULK_fixed_64 s);
void         ULK_fixed_64_v2d_mul(ULK_fixed_64_v3d out, const ULK_fixed_64_v3d in, ULK_fixed_64 s);
void         ULK_fixed_64_v2d_mul(ULK_fixed_64_v4d out, const ULK_fixed_64_v4d in, ULK_fixed_64 s);
void         ULK_fixed_64_v2d_norm(ULK_fixed_64_v2d out, const ULK_fixed_64_v2d in);
void         ULK_fixed_64_v3d_norm(ULK_fixed_64_v3d out, const ULK_fixed_64_v3d in);
void         ULK_fixed_64_v4d_norm(ULK_fixed_64_v4d out, const ULK_fixed_64_v4d in);
void         ULK_fixed_64_v2d_sub(ULK_fixed_64_v2d out, const ULK_fixed_64_v2d a, const ULK_fixed_64_v2d b); 
void         ULK_fixed_64_v3d_sub(ULK_fixed_64_v3d out, const ULK_fixed_64_v3d a, const ULK_fixed_64_v3d b); 
void         ULK_fixed_64_v4d_sub(ULK_fixed_64_v4d out, const ULK_fixed_64_v4d a, const ULK_fixed_64_v4d b); 

//Matrix mathematics
void         ULK_fixed_64_m22_add(ULK_fixed_64_m22 out, const ULK_fixed_64_m22 a, const ULK_fixed_64_m22 b);
void         ULK_fixed_64_m33_add(ULK_fixed_64_m33 out, const ULK_fixed_64_m33 a, const ULK_fixed_64_m33 b);
void         ULK_fixed_64_m44_add(ULK_fixed_64_m44 out, const ULK_fixed_64_m44 a, const ULK_fixed_64_m44 b);
void         ULK_fixed_64_m22_copy(ULK_fixed_64_m22 dst, const ULK_fixed_64_m22 src);
void         ULK_fixed_64_m33_copy(ULK_fixed_64_m33 dst, const ULK_fixed_64_m33 src);
void         ULK_fixed_64_m44_copy(ULK_fixed_64_m44 dst, const ULK_fixed_64_m44 src);
ULK_fixed_64 ULK_fixed_64_m22_determinant(const ULK_fixed_64_m22 in);
ULK_fixed_64 ULK_fixed_64_m33_determinant(const ULK_fixed_64_m33 in);
void         ULK_fixed_64_m22_identity(ULK_fixed_64_m22 out);
void         ULK_fixed_64_m33_identity(ULK_fixed_64_m33 out);
void         ULK_fixed_64_m44_identity(ULK_fixed_64_m44 out);
void         ULK_fixed_64_m22_sub(ULK_fixed_64_m22 out, const ULK_fixed_64_m22 a, const ULK_fixed_64_m22 b);
void         ULK_fixed_64_m33_sub(ULK_fixed_64_m33 out, const ULK_fixed_64_m33 a, const ULK_fixed_64_m33 b);
void         ULK_fixed_64_m44_sub(ULK_fixed_64_m44 out, const ULK_fixed_64_m44 a, const ULK_fixed_64_m44 b);

#endif //ULK_FIXED_ENABLE_64BIT

#endif
