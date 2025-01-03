
/*
    File Name   : bsm.c
    Description : Black-Scholes-Merton model
    Author      : Daniel Sirait <dsirait@outlook.com>
    Copyright   : Copyright (c) 2014 Daniel Sirait
    License     : Proprietary
    Disclaimer  : I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
                  ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS PROGRAM,
                  WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "bsm.h"


/* Error Function - Abramowitz and Stegun approximations (maximum error: 1.5*10e-7) */
/* BUG: erf() function name conflict with erf() math.h */
double erf_as (double x) {
	double a1, a2, a3, a4, a5, p;
	double t, erf_x;
	int sign;

	// constants
	a1 = 0.254829592;
	a2 = -0.284496736;
	a3 = 1.421413741;
	a4 = -1.453152027;
	a5 = 1.061405429;
	p  = 0.3275911;

	// Save the sign of x
	sign = 1;
	if (x < 0) sign = -1;
	x = fabs(x);

	// Abramowitz and Stegun	 7.1.26
	t = 1.0/(1.0 + p*x);
	erf_x = 1.0 - (a1*t + a2*pow(t,2) + a3*pow(t,3) + a4*pow(t,4) + a5*pow(t,5)) * exp(-pow(x,2));

	return sign * erf_x;
}

/* Normal Distribution - Probability Distribution Fuction */
double N_pdf (double x) {
	return exp(-(pow(x,2))/2) / (sqrt(2*M_PI));
}

/* Normal Distribution - Cumulative Distribution Fuction */
double N_cdf (double x) {
	return 0.5 * (1.0+erf_as(x/sqrt(2.0)));
}

/* Stock, Strike, risk, time, volatility */
BSM bsm (double S, double K, double r, double t, double v) {
	BSM val;
	
	val.d1 = 1/(v*sqrt(t)) * (log(S/K)+(r+pow(v,2.0)/2.0)*t);
	val.d2 = 1/(v*sqrt(t)) * (log(S/K)+(r-pow(v,2.0)/2.0)*t);
	//d2 = d1-v*sqrt(t);
	val.call = N_cdf(val.d1)*S - N_cdf(val.d2)*K*exp(-r*t);
	val.put = N_cdf(-val.d2)*K*exp(-r*t) - N_cdf(-val.d1)*S;

	val.delta_call = N_cdf(val.d1);
	val.delta_put = -N_cdf(-val.d1);
	val.gamma = N_pdf(val.d1) / (S*v*sqrt(t));
	val.kappa = S*N_pdf(val.d1)*sqrt(t);
	val.theta_call = -(S*N_pdf(val.d1)*v)/(2*sqrt(t)) - r*K*exp(-r*t)*N_cdf(val.d2);
	val.theta_put = -(S*N_pdf(val.d1)*v)/(2*sqrt(t)) + r*K*exp(-r*t)*N_cdf(-val.d2);
	val.rho_call = K*t*exp(-r*t)*N_cdf(val.d2);
	val.rho_put = -K*t*exp(-r*t)*N_cdf(-val.d2);

	return val;
}

/* Stock, Strike, risk, time, call, put */
double bsm_iv (double S, double K, double r, double t, double C, double P) {
	BSM val;
	long max_iteration = 1000;
	double accuracy = 1E-12;
	double v = 0.0;
	double v_high = 1.0;
	double v_low = 0.0;
	int i;
	
	// use Binomial Search algorithm
	for (i=0; i<max_iteration; i++) {
		v = (v_high + v_low) * 0.5;
		val = bsm(S, K, r, t, v);

		if (fabs(val.call-C) <= accuracy) break;

		if ((val.call-C) < 0.0) {
			v_low = v;
		} else {
			v_high = v;
		}
	}

	return v;
}


/*
	ln x	  -> log(x)
	log x	 -> log10(x)
	e^x	-> exp(x)

*/




