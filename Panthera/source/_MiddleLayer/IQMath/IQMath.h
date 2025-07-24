/*
 * IQMath.h
 *
 *  Created on: Jul 24, 2025
 *      Author: ADMIN
 */

#ifndef MIDDLELAYER_IQMATH_IQMATH_H_
#define MIDDLELAYER_IQMATH_IQMATH_H_
#include "math.h"
#include "stdint.h"
#include "stddef.h"

#define _q15(a) 			    (q31_t)(a * 32768.0f - 0.5f)

#define _q15_sub(a, b)		    (q31_t)((a)	- (b))
#define _q15_add(a, b)		    (q31_t)((a)	+ (b))
#define _q15_mul(a, b) 		    (q31_t)(((q31_t)(a) * (b)) >> 15)

#define _q15_i2f(a)			    ((float)(0.000030517578125f*(a)))

typedef uint16_t		        uq15_t;
typedef uint32_t		        uq31_t;
typedef int16_t		            q15_t;
typedef int32_t		            q31_t;
typedef int64_t                 q63_t;

#endif /* MIDDLELAYER_IQMATH_IQMATH_H_ */
