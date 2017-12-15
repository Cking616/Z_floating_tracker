/*
 * =====================================================================================
 *
 *       Filename:  filters.c
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

#include "filters.h"

/*
 * =====================================================================================
 *
 *                         Complementary filters
 *
 * =====================================================================================
 */
#ifdef COMPLEMENTARY_FILTER
void complementary1_filter_initialize(complementary1_state *state, float a, float y0)
{
    state->a = a;
    state->y = y0;
}

float complementary1_filter_process(complementary1_state *state, float x)
{
    float yn = x * state->a + (1 - state->a) * state->y;
    state->y = yn;
    return yn;
}

void  complementary2_filter_initialize(complementary2_state *state,
					    float b0, float b1, float b2)
{
    state->b0 = b0;
    state->b1 = b1;
    state->b2 = b2;
    state->xl = 0;
    state->xp = 0;
}

float complementary2_filter_process(complementary2_state *state, float x)
{
    float yn = x * state->b0 + state->xl * state->b1 + state->xp * state->b2;
    state->xp = state->xl;
    state->xl = yn;
    return yn;
}
#endif
/*
 * =====================================================================================
 *
 *                            kalman_filter
 *
 * =====================================================================================
 */

/*
 * @brief   
 *   Init fields of structure @kalman1_state.
 *   I make some defaults in this init function:
 *     A = 1;
 *     H = 1; 
 *   and @q,@r are valued after prior tests.
 *
 *   NOTES: Please change A,H,q,r according to your application.
 *
 * @inputs  
 *   state - Klaman filter structure
 *   init_x - initial x state value   
 *   init_p - initial estimated error convariance
 * @outputs 
 * @retval  
 */
void kalman1_filter_initialize(kalman1_state *state, float init_x, float init_p)
{
    state->x = init_x;
    state->p = init_p;
    state->A = 1;
    state->H = 1;
    state->q = 2e2;//10e-6;  /* predict noise convariance */
    state->r = 5e2;//10e-5;  /* measure error convariance */
}

/*
 * @brief   
 *   1 Dimension Kalman filter
 * @inputs  
 *   state - Klaman filter structure
 *   z_measure - Measure value
 * @outputs 
 * @retval  
 *   Estimated result
 */
float kalman1_filter_process(kalman1_state *state, float z_measure)
{
	if(z_measure < 0.01f && z_measure > -0.01f)
	{
		state->x = 0;
		state->p = 1e3;
	}
	else
	{
		/* Predict */
		state->x = state->A * state->x;
		state->p = state->A * state->A * state->p + state->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

		/* Measurement */
		state->gain = state->p * state->H / (state->p * state->H * state->H + state->r);
		state->x = state->x + state->gain * (z_measure - state->H * state->x);
		state->p = (1 - state->gain * state->H) * state->p;
	}
	return state->x;
}

#ifdef KALMAN2_FILTER
/*
 * @brief
 *   Init fields of structure @kalman1_state.
 *   I make some defaults in this init function:
 *     A = {{1, 0.1}, {0, 1}};
 *     H = {1,0};
 *   and @q,@r are valued after prior tests.
 *
 *   NOTES: Please change A,H,q,r according to your application.
 *
 * @inputs
 * @outputs
 * @retval
 */
void kalman2_filter_initialize(kalman2_state *state, float *init_x, float (*init_p)[2])
{
    state->x[0]    = init_x[0];
    state->x[1]    = init_x[1];
    state->p[0][0] = init_p[0][0];
    state->p[0][1] = init_p[0][1];
    state->p[1][0] = init_p[1][0];
    state->p[1][1] = init_p[1][1];
    //state->A       = {{1, 0.1}, {0, 1}};
    state->A[0][0] = 1;
    state->A[0][1] = 0.1f;
    state->A[1][0] = 0;
    state->A[1][1] = 1;
    //state->H       = {1,0};
    state->H[0]    = 1;
    state->H[1]    = 0;
    //state->q       = {{10e-6,0}, {0,10e-6}};  /* measure noise convariance */
    state->q[0]    = 10e-7f;
    state->q[1]    = 10e-7f;
    state->r       = 10e-7f;  /* estimated error convariance */
}

/*
 * @brief
 *   2 Dimension kalman filter
 * @inputs
 *   state - Klaman filter structure
 *   z_measure - Measure value
 * @outputs
 *   state->x[0] - Updated state value, Such as angle,velocity
 *   state->x[1] - Updated state value, Such as diffrence angle, acceleration
 *   state->p    - Updated estimated error convatiance matrix
 * @retval
 *   Return value is equals to state->x[0], so maybe angle or velocity.
 */
float kalman2_filter_process(kalman2_state *state, float z_measure)
{
    float temp0 = 0.0f;
    float temp1 = 0.0f;
    float temp = 0.0f;

    /* Step1: Predict */
    state->x[0] = state->A[0][0] * state->x[0] + state->A[0][1] * state->x[1];
    state->x[1] = state->A[1][0] * state->x[0] + state->A[1][1] * state->x[1];
    /* p(n|n-1)=A^2*p(n-1|n-1)+q */
    state->p[0][0] = state->A[0][0] * state->p[0][0] + state->A[0][1] * state->p[1][0] + state->q[0];
    state->p[0][1] = state->A[0][0] * state->p[0][1] + state->A[1][1] * state->p[1][1];
    state->p[1][0] = state->A[1][0] * state->p[0][0] + state->A[0][1] * state->p[1][0];
    state->p[1][1] = state->A[1][0] * state->p[0][1] + state->A[1][1] * state->p[1][1] + state->q[1];

    /* Step2: Measurement */
    /* gain = p * H^T * [r + H * p * H^T]^(-1), H^T means transpose. */
    temp0 = state->p[0][0] * state->H[0] + state->p[0][1] * state->H[1];
    temp1 = state->p[1][0] * state->H[0] + state->p[1][1] * state->H[1];
    temp  = state->r + state->H[0] * temp0 + state->H[1] * temp1;
    state->gain[0] = temp0 / temp;
    state->gain[1] = temp1 / temp;
    /* x(n|n) = x(n|n-1) + gain(n) * [z_measure - H(n)*x(n|n-1)]*/
    temp = state->H[0] * state->x[0] + state->H[1] * state->x[1];
    state->x[0] = state->x[0] + state->gain[0] * (z_measure - temp);
    state->x[1] = state->x[1] + state->gain[1] * (z_measure - temp);

    /* Update @p: p(n|n) = [I - gain * H] * p(n|n-1) */
    state->p[0][0] = (1 - state->gain[0] * state->H[0]) * state->p[0][0];
    state->p[0][1] = (1 - state->gain[0] * state->H[1]) * state->p[0][1];
    state->p[1][0] = (1 - state->gain[1] * state->H[0]) * state->p[1][0];
    state->p[1][1] = (1 - state->gain[1] * state->H[1]) * state->p[1][1];

    return state->x[0];
}
#endif
