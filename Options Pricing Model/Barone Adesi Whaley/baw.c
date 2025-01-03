
/*
	File Name	: baw.c
	Description : Barone-Adesi-Whaley approximation model for american option
	Author		: Daniel Sirait <dsirait@outlook.com>
	Copyright	: Copyright (c) 2015 Daniel Sirait
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

#include "baw.h"

static double h = 1e-4;

/* Normal Distribution - Probability Distribution Fuction */
double N_pdf (double x) {
	return exp(-pow(x,2)/2) / sqrt(2*M_PI);
}

/* Normal Distribution - Cumulative Distribution Fuction */
double N_cdf (double x) {
	return 0.5 * (1 + erf(x/sqrt(2)));
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


double baw_call (double S, double X, double r, double q, double t, double v) {
	double C, b, A, h, L1, L2;
	double Sc, d1, d2, A2, Su, Sl, IterationCountE;
	double c1, c2, Suu, E_st;

	if (q == 0) return bsm_call(S, X, r, q, t, v);
	
	b = 2*(r-q) / pow(v,2) - 1;
	A = 2*r / pow(v,2);
	h = 1 - exp(-r*t);
	//L1 = (-b - sqrt(pow(b,2) + 4*A/h)) / 2;
	L2 = (-b + sqrt(pow(b,2) + 4*A/h)) / 2;


	IterationCountE = 1e-9;
	d1 = (log(S/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
	d2 = d1 - v*sqrt(t);

	E_st = S * exp((r-q)*t) * N_cdf(d1) / N_cdf(d2);
	Su = E_st;
	Sl = S;
	Suu = Su;
	Sc = (Su + Sl) / 2;

Start_Iteration:
		
	while ((Su - Sl) > IterationCountE) {
		Sc = (Su + Sl) / 2;
		d1 = (log(Sc/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
		c1 = Sc - X;
		c2 = bsm_call(Sc, X, r, q, t, v) + (1 - exp(-q*t) * N_cdf(d1)) * Sc/L2;
		if ((c2-c1) > 0)
			Sl = Sc;
		else
			Su = Sc;
	}

	if ((round(Sc*1e+4)/1e+4) == (round(Suu*1e+4)/1e+4) ) {
		Su = 2 * Suu;
		Suu = Su;
		goto Start_Iteration;
	}
    
	d1 = (log(Sc/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
	A2 = (1 - exp(-q*t) * N_cdf(d1)) * (Sc/L2);
	if (S < Sc) {
		C = bsm_call(S, X, r, q, t, v) + A2 * pow(S/Sc, L2);
	} else {
		C = S - X;
	}
	

	return C;
}

double baw_put (double S, double X, double r, double q, double t, double v) {
	double P, b, A, h, L1;
	double Sp, d1, d2, A1, Su, Sl, IterationCountE;
	double p1, p2, Sll, E_st;

	if (r == 0) return bsm_put(S, X, r, q, t, v);
	
	b = 2*(r-q) / pow(v,2) - 1;
	//bB = 2*(r-q) / pow(v,2);
	A = 2*r / pow(v,2);
	h = 1 - exp(-r*t);
	L1 = (-b - sqrt(pow(b,2) + 4*A/h)) / 2;
	//L1B = (-(bB-1) - sqrt(pow(bB-1,2) + 4*A/h)) / 2;


	IterationCountE = 1e-9;
	d1 = (log(S/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
	d2 = d1 - v*sqrt(t);

	E_st = S * exp((r-q)*t) * N_cdf(-d1) / N_cdf(-d2);
	Sl = 0;
	Su = S;
	Sll = Sl;
	Sp = (Su + Sl) / 2;

Start_Iteration:
		
	while ((Su - Sl) > IterationCountE) {
		Sp = (Su + Sl) / 2;
		d1 = (log(Sp/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
		p1 = X - Sp;
		p2 = bsm_put(Sp, X, r, q, t, v) - (1 - exp(-q*t) * N_cdf(-d1)) * Sp/L1;
		if ((p2-p1) > 0)
			Su = Sp;
		else
			Sl = Sp;
	}

	/*if ((round(Sp*1e+4)/1e+4) == (round(Sll*1e+4)/1e+4) ) {
		Sl = 2 * Sll;
		Sll = Sl;
		goto Start_Iteration;
	}*/
		
	d1 = (log(Sp/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
	A1 = -(1 - exp(-q*t) * N_cdf(-d1)) * (Sp/L1);
	if (S > Sp) {
		P = bsm_put(S, X, r, q, t, v) + A1 * pow(S/Sp, L1);
	} else {
		P = X - S;
	}
	

	return P;
}


double baw3_call (double S, double X, double r, double q, double t, double v) {
	double C, M, N, K, q2, A2, Sx, d1;
	double c1, c2;

	if (q == 0) return bsm_call(S, X, r, q, t, v);
	
	M = 2*r / pow(v,2);
	N = 2*q / pow(v,2);
	K = 1 - exp(-r*t);
	q2 = 0.5 * (-(N-1) + sqrt(pow(N-1, 2) + 4*M/K));

	Sx = S * exp(-(r-q)*t);
	while (1) {
		d1 = (log(Sx/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
		A2 = (1-exp((q-r)*t)*N_cdf(d1)) * Sx/q2;
		c1 = Sx - X;
		c2 = bsm_call(Sx, X, r, q, t, v) + A2;
		if (fabs(c1-c2) < 1e-1) break;
		Sx += 1e-2;
		//printf("%f\n", Sx);
	}

	/*printf("%f\n", fabs(c1-c2));*/
	//printf("%f\n", Sx);

	if (S < Sx) {
		d1 = (log(Sx/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
		A2 = (1-exp((q-r)*t)*N_cdf(d1)) * Sx/q2;
		C = bsm_call(S, X, r, q, t, v) + A2 * pow(S/Sx, q2);
	} else {
		C = S - X;
	}

	return C;
}



double baw3_put (double S, double X, double r, double q, double t, double v) {
	double P, M, N, K, q1, A1, Sx, d1;
	double c1, c2;

	if (r == 0) return bsm_put(S, X, r, q, t, v);
	
	M = 2*r / pow(v,2);
	N = 2*q / pow(v,2);
	K = 1 - exp(-r*t);
	q1 = 0.5 * (-(N-1) - sqrt(pow(N-1, 2) + 4*M/K));

	Sx = S * exp((r-q)*t);
	while (1) {
		d1 = (log(Sx/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
		A1 = (1-exp((q-r)*t)*N_cdf(-d1)) * -Sx/q1;
		c1 = X - Sx;
		c2 = bsm_put(Sx, X, r, q, t, v) + A1;
		if (fabs(c1-c2) < 1e-3) break;
		Sx -= 1e-3;
		//printf("%f\n", Sx);
	}

	/*printf("%f\n", fabs(c1-c2));*/
	//printf("%f\n", Sx);

	if (S > Sx) {
		d1 = (log(Sx/X) + (r-q + 0.5*pow(v,2)*t)) / (v*sqrt(t));
		A1 = (1-exp((q-r)*t)*N_cdf(-d1)) * -Sx/q1;
		P = bsm_put(S, X, r, q, t, v) + A1 * pow(S/Sx, q1);
	} else {
		P = X - S;
	}

	return P;
}


double baw_delta_C (double S, double X, double r, double q, double t, double v) {
	return (baw_call(S+h, X, r, q, t, v) - baw_call(S, X, r, q, t, v) ) / h;
}

double baw_delta_P (double S, double X, double r, double q, double t, double v) {
	return (baw_put(S+h, X, r, q, t, v) - baw_put(S, X, r, q, t, v) ) / h;
}

double baw_gamma (double S, double X, double r, double q, double t, double v) {
	return (baw_call(S+h, X, r, q, t, v) - (2*baw_call(S, X, r, q, t, v)) + baw_call(S-h, X, r, q, t, v)) / pow(h, 2);
}

double baw_kappa (double S, double X, double r, double q, double t, double v) {
	return (baw_call(S, X, r, q, t, v+h) - baw_call(S, X, r, q, t, v) ) / h;
	//return (baw_put(S, X, r, q, t, v+h) - baw_put(S, X, r, q, t, v) ) / h;
}

double baw_theta_C (double S, double X, double r, double q, double t, double v) {
	return -(baw_call(S, X, r, q, t+h, v) - baw_call(S, X, r, q, t, v) ) / h;
}

double baw_theta_P (double S, double X, double r, double q, double t, double v) {
	return -(baw_put(S, X, r, q, t+h, v) - baw_put(S, X, r, q, t, v) ) / h;
}

double baw_rho_C (double S, double X, double r, double q, double t, double v) {
	return (baw_call(S, X, r+h, q, t, v) - baw_call(S, X, r, q, t, v) ) / h;
}

double baw_rho_P (double S, double X, double r, double q, double t, double v) {
	return (baw_put(S, X, r+h, q, t, v) - baw_put(S, X, r, q, t, v) ) / h;
}


double baw_iv_bisection (double S, double X, double r, double q, double t, double V, BAW_OPTION_TYPE opt_type) {
	long max_iteration = 200;
	double accuracy = 1e-15;
	double v = 0.0;
	double v_high = 10.0;
	double v_low = 0.0;
	double V_test;
	int i;

	// Check for arbitrage violations. Option price is too low if this happens
	if (V < (S*exp(-q*t) - X*exp(-r*t)) && opt_type == BAW_CALL) {
		fputs("baw_iv_bisection(): arbitrage violations", stderr);
	} else if (V < (X*exp(-r*t) - S*exp(-q*t)) && opt_type == BAW_PUT) {
		fputs("baw_iv_bisection(): arbitrage violations", stderr);
	}
	
	// use Binomial Search algorithm
	for (i=0; i<max_iteration; i++) {
		v = (v_high + v_low) * 0.5;

		if (opt_type == BAW_PUT) {
			V_test = baw_put(S, X, r, q, t, v);
		} else if (opt_type == BAW_CALL) {
			V_test = baw_call(S, X, r, q, t, v);
		} else {
			errno = EINVAL;
			perror("baw_iv_bisection() error on passing argument 6");
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

void main () {
	double V, iv;
	double S, X, r, q, t, v;
	int i;

	S = 100.0;
	X = 100.0;
	r = .05;
	q = .02;
	t = 1.0;
	v = .25;

	/*for (i=0; i<10; i++) {

	V = baw_call(S, X, r, q, t, v);
	printf("%.15f\n", V);
	V = baw3_call(S, X, r, q, t, v);
	printf("%.15f\n", V);

	V = baw_put(S, X, r, q, t, v);
	printf("%.15f\n", V);
	V = baw3_put(S, X, r, q, t, v);
	printf("%.15f\n", V);

	printf("%f\n", r);
	puts("-------------");
	
	r += .01;

	}*/
	V = baw_call(S, X, r, q, t, v);

	printf("%.15f\n", V);


	iv = baw_iv_bisection(S, X, r, q, t, 11.125687251819270, BAW_CALL);
	printf("iv: %.15f\n", iv);


	_getch();
}

