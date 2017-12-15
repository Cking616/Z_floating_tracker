/*
 * =====================================================================================
 *
 *       Filename:  filters.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017/12/11 
 *
 *         Author:  cking ((kanghaitao)), cking616@mail.ustc.edu.cn
 *        License:  Apache Licence 
 *
 * =====================================================================================
 */

#ifndef _FILTERS_H
#define _FILTERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * =====================================================================================
 *
 *                         Complementary filters
 *
 * =====================================================================================
 */

#ifdef COMPLEMENTARY_FILTER
/* first-order filter : y(n) = a*x(n)+(1 - a)*y(n - 1) */
typedef struct {
    float y;
    float a;
} complementary1_state;

void  complementary1_filter_initialize(complementary1_state *state, float a, float y0);
float complementary1_filter_process(complementary1_state *state, float x);
#define complementary1_get_yvalue(state) ((state)->y)

/* second-order filter : y(n) = b0*x(n)+b1*x(n-1)+b2*x(n-2)*/
typedef struct {
    float xl;	/* x(n-1) */
    float xp; /* x(n-2) */
    float b0;
    float b1;
    float b2;
} complementary2_state;

void  complementary2_filter_initialize(complementary2_state *state,
					    float b0, float b1, float b2);
float complementary2_filter_process(complementary2_state *state, float x);
#endif
/*
 * =====================================================================================
 *
 *                            Kalman filters
 *
 * =====================================================================================
 */

/* 
 * NOTES: n Dimension means the state is n dimension, 
 * measurement always 1 dimension 
 */

/* 1 Dimension */
typedef struct {
    float x;  /* state */
    float A;  /* x(n)=A*x(n-1)+u(n),u(n)~N(0,q) */
    float H;  /* z(n)=H*x(n)+w(n),w(n)~N(0,r)   */
    float q;  /* process(predict) noise convariance */
    float r;  /* measure noise convariance */
    float p;  /* estimated error convariance */
    float gain;
} kalman1_state;

void  kalman1_filter_initialize(kalman1_state *state, float init_x, float init_p);
float kalman1_filter_process(kalman1_state *state, float z_measure);
#define kalman1_filter_get_yvalue(state) ((state)->x)

#ifdef KALMAN2_FILTER
/* 2 Dimension */
typedef struct {
    float x[2];     /* state: [0]-angle [1]-diffrence of angle, 2x1 */
    float A[2][2];  /* X(n)=A*X(n-1)+U(n),U(n)~N(0,q), 2x2 */
    float H[2];     /* Z(n)=H*X(n)+W(n),W(n)~N(0,r), 1x2   */
    float q[2];     /* process(predict) noise convariance,2x1 [q0,0; 0,q1] */
    float r;        /* measure noise convariance */
    float p[2][2];  /* estimated error convariance,2x2 [p0 p1; p2 p3] */
    float gain[2];  /* 2x1 */
} kalman2_state;

void  kalman2_filter_initialize(kalman2_state *state, float *init_x, float (*init_p)[2]);
float kalman2_filter_process(kalman2_state *state, float z_measure);

#define kalman2_filter_get_yvalue(state) ((state)->x[0])
#endif

#ifdef __cplusplus
}

#endif

#endif
