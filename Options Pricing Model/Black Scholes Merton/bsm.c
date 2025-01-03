
/*
	File Name	: bsm.c
	Description : Black-Scholes-Merton model
	Author		: Daniel Sirait <dsirait@outlook.com>
	Copyright	: Copyright (c) 2014 Daniel Sirait
	License		: Proprietary
	Disclaimer	: I CAN UNDER NO CIRCUMSTANCES BE HELD RESPONSIBLE FOR
				 ANY CONSEQUENCES OF YOUR USE/MISUSE OF THIS PROGRAM,
				 WHATEVER THAT MAY BE (GET BUSTED, WORLD WAR, ETC..).
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <errno.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "bsm.h"


/* Error Function - Abramowitz and Stegun approximations (maximum error: 1.5*10e-7) */
/* BUG: erf() function name conflict with erf() math.h */
double erf_as (double x) {
	double a1, a2, a3, a4, a5, p;
	double t, val;

	// constants
	a1 = 0.254829592;
	a2 = -0.284496736;
	a3 = 1.421413741;
	a4 = -1.453152027;
	a5 = 1.061405429;
	p  = 0.3275911;
	
	// Abramowitz and Stegun	 7.1.26
	t = 1.0 / (1.0 + p*fabs(x));
	val = 1.0 - (a1*t + a2*pow(t,2) + a3*pow(t,3) + a4*pow(t,4) + a5*pow(t,5)) * exp(-pow(fabs(x),2));

	// Save the sign of x
	if (x < 0) return -val;
	else return val;
}

/* Normal Distribution - Probability Distribution Fuction */
double N_pdf (double x) {
	return exp(-pow(x,2)/2) / sqrt(2*M_PI);
}

/* Normal Distribution - Cumulative Distribution Fuction */
double N_cdf (double x) {
	return 0.5 * (1 + erf_as(x/sqrt(2)));
}

double bsm_call (double S, double X, double r, double q, double t, double v) {
	double d1, d2;
	d1 = 1/(v*sqrt(t)) * (log(S/X)+(r-q+pow(v,2)/2)*t);
	//d2 = 1/(v*sqrt(t)) * (log(S/X)+(r-q-pow(v,2)/2)*t);
	d2 = d1 - v*sqrt(t);
	return S * N_cdf(d1) * exp(-q*t) - X * N_cdf(d2) * exp(-r*t);
}

double bsm_put (double S, double X, double r, double q, double t, double v) {
	double d1, d2;
	d1 = 1/(v*sqrt(t)) * (log(S/X)+(r-q+pow(v,2)/2)*t);
	//d2 = 1/(v*sqrt(t)) * (log(S/X)+(r-q-pow(v,2)/2)*t);
	d2 = d1 - v*sqrt(t);
	return X * N_cdf(-d2) * exp(-r*t) - S * N_cdf(-d1) * exp(-q*t);
}

double bsm_delta_C (double S, double X, double r, double q, double t, double v) {
	double d1;
	d1 = (log(S/X)+(r-q+pow(v,2)/2)*t) / (v*sqrt(t));
	return N_cdf(d1) * exp(-q*t);
}

double bsm_delta_P (double S, double X, double r, double q, double t, double v) {
	double d1;
	d1 = (log(S/X)+(r-q+pow(v,2)/2)*t) / (v*sqrt(t));
	return -N_cdf(-d1) * exp(-q*t);
}

double bsm_gamma (double S, double X, double r, double q, double t, double v) {
	double d1;
	d1 = (log(S/X)+(r-q+pow(v,2)/2)*t) / (v*sqrt(t));
	return N_pdf(d1) / (S*v*sqrt(t)) * exp(-q*t);
}

double bsm_kappa (double S, double X, double r, double q, double t, double v) {
	double d1;
	d1 = (log(S/X)+(r-q+pow(v,2)/2)*t) / (v*sqrt(t));
	return S * N_pdf(d1) * sqrt(t) * exp(-q*t);
}

double bsm_theta_C (double S, double X, double r, double q, double t, double v) {
	double d1, d2;
	d1 = (log(S/X)+(r-q+pow(v,2)/2)*t) / (v*sqrt(t));
	//d2 = (log(S/X)+(r-q-pow(v,2)/2)*t) / (v*sqrt(t));
	d2 = d1 - v*sqrt(t);
	return -(S*N_pdf(d1)*v)/(2*sqrt(t)) * exp(-q*t) - r*X*exp(-r*t)*N_cdf(d2) + q*S*N_pdf(d1)*exp(-q*t);
}


double bsm_theta_P (double S, double X, double r, double q, double t, double v) {
	double d1, d2;
	d1 = (log(S/X)+(r-q+pow(v,2)/2)*t) / (v*sqrt(t));
	//d2 = (log(S/X)+(r-q-pow(v,2)/2)*t) / (v*sqrt(t));
	d2 = d1 - v*sqrt(t);
	return -(S*N_pdf(d1)*v)/(2*sqrt(t)) * exp(-q*t) + r*X*exp(-r*t)*N_cdf(-d2) + q*S*N_pdf(-d1)*exp(-q*t);
}


double bsm_rho_C (double S, double X, double r, double q, double t, double v) {
	double d2;
	d2 = (log(S/X)+(r-q-pow(v,2)/2)*t) / (v*sqrt(t));
	return X*t*exp(-r*t)*N_cdf(d2);
}

double bsm_rho_P (double S, double X, double r, double q, double t, double v) {
	double d2;
	d2 = (log(S/X)+(r-q-pow(v,2)/2)*t) / (v*sqrt(t));
	return -X*t*exp(-r*t)*N_cdf(-d2);
}

/* Stock, Strike, risk, time, implied volatility, dividend yield */
BSM_OPTION bsm (double S, double X, double r, double q, double t, double v) {
	BSM_OPTION opt;
	double d1, d2;

	opt.S = S;
	opt.X = X;
	opt.r = r;
	opt.t = t;
	opt.v = v;
	opt.q = q;

	d1 = (log(S/X)+(r-q+pow(v,2)/2)*t) / (v*sqrt(t));
	//d2 = (log(S/X)+(r-q-pow(v,2)/2)*t) / (v*sqrt(t));
	d2 = d1 - v*sqrt(t);

	opt.C = S * N_cdf(d1) * exp(-q*t) - X * N_cdf(d2) * exp(-r*t);
	opt.P = X * N_cdf(-d2) * exp(-r*t) - S * N_cdf(-d1) * exp(-q*t);

	opt.delta_C = N_cdf(d1) * exp(-q*t);
	opt.delta_P = -N_cdf(-d1) * exp(-q*t);
	opt.gamma = N_pdf(d1) / (S*v*sqrt(t)) * exp(-q*t);
	opt.kappa = S * N_pdf(d1) * sqrt(t) * exp(-q*t);
	opt.theta_C = -(S*N_pdf(d1)*v)/(2*sqrt(t)) * exp(-q*t) - r*X*exp(-r*t)*N_cdf(d2) + q*S*N_pdf(d1)*exp(-q*t);
	opt.theta_P = -(S*N_pdf(d1)*v)/(2*sqrt(t)) * exp(-q*t) + r*X*exp(-r*t)*N_cdf(-d2) + q*S*N_pdf(-d1)*exp(-q*t);
	opt.rho_C = X*t*exp(-r*t)*N_cdf(d2);
	opt.rho_P = -X*t*exp(-r*t)*N_cdf(-d2);

	return opt;
}

/* find implied volatility using bisection */
/* V: option price (call/put) */
double bsm_iv_bisection (double S, double X, double r, double q, double t, double V, BSM_OPTION_TYPE opt_type) {
	BSM_OPTION opt;
	long max_iteration = 200;
	double accuracy = 1e-15;
	double v = 0.0;
	double v_high = 100.0;
	double v_low = 0.0;
	double V_test;
	int i;

	// Check for arbitrage violations. Option price is too low if this happens
	if (V < (S*exp(-q*t) - X*exp(-r*t)) && opt_type == BSM_CALL) {
		fputs("bsm_iv_bisection(): arbitrage violations", stderr);
	} else if (V < (X*exp(-r*t) - S*exp(-q*t)) && opt_type == BSM_PUT) {
		fputs("bsm_iv_bisection(): arbitrage violations", stderr);
	}
	
	// use Binomial Search algorithm
	for (i=0; i<max_iteration; i++) {
		v = (v_high + v_low) * 0.5;
		opt = bsm(S, X, r, t, v, q);

		if (opt_type == BSM_PUT) {
			V_test = opt.P;
		} else if (opt_type == BSM_CALL) {
			V_test = opt.C;
		} else {
			errno = EINVAL;
			perror("bsm_iv_bisection() error on passing argument 6");
			exit(EXIT_FAILURE);
		}

		if (fabs(V_test-V) <= accuracy) break;

		if (V_test-V < 0.0) {
			v_low = v;
		} else {
			v_high = v;
		}
	}

	//printf("bisection iteration break at: %d\n", i);
	return v;
}



double bsm_iv_newtonraphson (double S, double X, double r, double q, double t, double V, BSM_OPTION_TYPE opt_type) {
	BSM_OPTION opt;
	double vn, V_test;
	long max_iteration = 20;
	double accuracy = 1e-15;
	int i;

	// Check for arbitrage violations. Option price is too low if this happens
	if (V < (S - X*exp(-t*r))) {
		fputs("bsm_iv_newtonraphson() arbitrage violations", stderr);
	}

	vn = 1.0;

	for (i=0; i<max_iteration; i++) {
		opt = bsm(S, X, r, t, vn, q);

		if (opt_type == BSM_PUT)
			V_test = opt.P;
		else if (opt_type == BSM_CALL)
			V_test = opt.C;
		else {
			errno = EINVAL;
			perror("bsm_iv_newtonraphson() error on passing argument 7");
			exit(EXIT_FAILURE);
		}

		if (fabs(V_test-V) <= accuracy) break;

		vn = vn - (V_test - V)/opt.kappa;
	};

	printf("bsm_iv_newtonraphson() iteration break at: %d\n", i);
	return vn;
}


double akar (double y) {
	double xn;

	xn = 1.0;

	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);
	xn = xn - (xn*xn - y) / (2*xn);

	return xn;
}

void main () {
	double S, X, r, t, v, q;
	double stock_portfolio, commission;
	double iv;
	BSM_OPTION opt;
	
	S = 100.0;
	X = 100.0;
	t = 1.0;
	r = 0.05;
	v = 0.25;
	q = 0.02;		// dividend yield

	//commission = 10;
	stock_portfolio = 10000.0;
	
	//Call       : 11.117909263637621
	//Put        : 8.230786369766548
	//iv = bsm_iv_bisection(S, X, r, q, t, 8.226833068546192, BSM_PUT);
	//printf("bisection     : %.15f\n", iv);

	//iv = bsm_iv_newtonraphson(S, X, r, q, t, 8.226833068546192, BSM_PUT);
	//printf("newtonraphson : %.15f\n\n", iv);


	opt = bsm(S, X, r, q, t, v);

	printf("Call       : %.15f\n", opt.C);
	printf("Put        : %.15f\n", opt.P);

	printf("Delta Call : %.15f\n", opt.delta_C);
	printf("Delta Put  : %.15f\n", opt.delta_P);
	printf("Gamma      : %.15f\n", opt.gamma);
	printf("Kappa      : %.15f\n", opt.kappa);
	printf("Theta Call : %.15f\n", opt.theta_C);
	printf("Theta Put  : %.15f\n", opt.theta_P);
	printf("Rho Call   : %.15f\n", opt.rho_C);
	printf("Rho Put    : %.15f\n", opt.rho_P);

	


	printf("\nPress any key to exit . . .");
	getch();
}

/*
	ln x	  -> log(x)
	log x	 -> log10(x)
	e^x	-> exp(x)

*/




