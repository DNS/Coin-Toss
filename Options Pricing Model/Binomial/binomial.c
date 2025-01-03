/*
	Binomial Options Pricing Model - Cox, Ross, & Rubinstein (CRR)
	Implementation by: Daniel Sirait <dsirait.outlook.com>

	Equal probabilities tree has the advantage over the CRR model 
	of working correctly when the volatility is very low and the 
	interest rate very high.
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <errno.h>
#include <process.h>

#define _USE_MATH_DEFINES
#include <math.h>

typedef enum _BINOMIAL_OPTION_TYPE {
	BINOMIAL_PUT,
	BINOMIAL_CALL
} BINOMIAL_OPTION_TYPE;

typedef enum _BINOMIAL_OPTION_STYLE {
	BINOMIAL_EUROPEAN,
	BINOMIAL_AMERICAN
} BINOMIAL_OPTION_STYLE;

typedef struct _BINOMIAL_OPTION {
	double S;
	double X;
	double r;
	double t;
	double v;			/* implied volatility */
	double q;			/* dividend yield */

	unsigned int n;		/* steps */
	BINOMIAL_OPTION_STYLE style;

	double C;
	double P;
	double delta_C;
	double delta_P;
	double gamma;
	double kappa;		/* vega */
	double theta_C;
	double theta_P;
	double rho_C;
	double rho_P;
} BINOMIAL_OPTION;


typedef struct _BTREE {
	double S;
	double C;
	double P;
} BTREE;

BINOMIAL_OPTION binomial_V (double, double, double, double, double, double, unsigned long, BINOMIAL_OPTION_STYLE);
BINOMIAL_OPTION binomial (double, double, double, double, double, double, unsigned long, BINOMIAL_OPTION_STYLE);
double binomial_iv_bisection (double, double, double, double, double, unsigned long, double, BINOMIAL_OPTION_STYLE, BINOMIAL_OPTION_TYPE);
void binomial_greeks (BINOMIAL_OPTION *);

int test () {
	int x;
	void *p = malloc(sizeof(int));

	*(int*) p = 777;
	x = *(int*) p;
	free(p);

	printf("%d\n", x);
	return x;
}

double test2 () {
	return exp(-1.234*9.56/5.4);
}

typedef struct _MYDATA {
	double r; double t; unsigned long n; double p; double C;
} MYDATA;

DWORD WINAPI callbackFunc (MYDATA *p) {
	//p->C = exp(-p->r*p->t/p->n) * (p->p * p->C + (1.0-p->p) * p->C);
	//printf("callbackFunc()\n");
	//Sleep(2000);
	//ExitThread(0);
	return 0;
}

// find V (Call & Put) value
BINOMIAL_OPTION binomial_V (double S, double X, double r, double t, double v, double q, unsigned long n, BINOMIAL_OPTION_STYLE style) {
	double eC, eP;			// exercise value
	double u, d, p;			// (u, d, p -> up, down, probability)
	BTREE *tree;
	int i, j, k;
	BINOMIAL_OPTION opt;
	int ux, dx;

	if (n <= 0) {
		errno = EINVAL;
		fputs("error: binomial() argument 7 minimum value is: 1", stderr);
		exit(EXIT_FAILURE);
	}

	// STEP 1

	if (r<=0.1 && v >= 0.1) {
		// CRR (Cox, Ross, and Rubinstein)
		u = exp(v * sqrt(t/n));
		//d = exp(-v * sqrt(t/n));
		d = 1/u;
		p = (exp((r-q)*t/n) - d) / (u-d);
	} else {
		// EQUAL PROBABILITY (Jarrow-Rudd)
		if (n <= 1) {
			fputs("error: binomial() argument 7 minimum value is: 2", stderr);
			exit(EXIT_FAILURE);
		}
		
		u = exp( ((r-q) - pow(v,2)/2) * (t/n) + v * sqrt(t/n) );
		d = exp( ((r-q) - pow(v,2)/2) * (t/n) - v * sqrt(t/n) );
		//p = (exp(r*t/n)-d)/(u-d);		// Jarrow-Rudd Risk Neutral
		p = 0.5;						// Jarrow-Rudd Equal-Probability model
	}

	tree = malloc(sizeof(BTREE) * (n + 1));


	// STEP 2 & 3 (binomial value, exercise value, & iteration)
	if (style == BINOMIAL_EUROPEAN) {
		ux=n;
		dx=0;
		for (i=0; i<=n; i++) {
			tree[i].S = S * pow(u,ux) * pow(d,dx);
			ux--;
			dx++;
			tree[i].C = max(0, tree[i].S - X);
			tree[i].P = max(0, X - tree[i].S);
		}

		for (i=n; i>0; i--) {
			for (j=0; j<i; j++) {
				tree[j].C = exp(-r*t/n) * (p * tree[j].C + (1.0-p) * tree[j+1].C);
				tree[j].P = exp(-r*t/n) * (p * tree[j].P + (1.0-p) * tree[j+1].P);
			}
		}

	} else if (style == BINOMIAL_AMERICAN) {
		for (i=n; i>0; i--) {
			ux=i-1;
			dx=0;
			for (j=0; j<i; j++) {
				tree[j].S = S * pow(u,ux) * pow(d,dx);
				ux--;
				dx++;

				eC = max(0, tree[j].S - X);
				eP = max(0, X - tree[j].S);

				tree[j].C = exp(-r*t/n) * (p * tree[j].C + (1.0-p) * tree[j+1].C);
				tree[j].P = exp(-r*t/n) * (p * tree[j].P + (1.0-p) * tree[j+1].P);

				tree[j].C = max(eC, tree[j].C);
				tree[j].P = max(eP, tree[j].P);

				
			}
		}
	} else {
		errno = EINVAL;
		fputs("error: binomial() argument 8", stderr);
		exit(EXIT_FAILURE);
	}
	
	opt.C = tree[0].C;
	opt.P = tree[0].P;
	free(tree);

	return opt;
}

// find greeks using finite difference
void binomial_greeks (BINOMIAL_OPTION *opt) {
	BINOMIAL_OPTION opt_plus, opt_minus;
	opt_plus = binomial_V(opt->S + 1e-7, opt->X, opt->r, opt->t, opt->v, opt->q, opt->n, opt->style);
	opt_minus = binomial_V(opt->S - 1e-7, opt->X, opt->r, opt->t, opt->v, opt->q, opt->n, opt->style);
	opt->delta_C = (opt_plus.C - opt->C) / 1e-7;
	opt->delta_P = (opt_plus.P - opt->P) / 1e-7;
	//opt->gamma = (opt_plus.C - (2* opt->C) + opt_minus.C) / (1e-2 * 1e-2);		// ERROR, NOT ACCURATE
	//opt->kappa = V(S, X, r, t, v + 1e-7, q) - V(S, X, r, t, v, q) / 1e-7;
	//opt->theta_C = 
	//opt->theta_P = 
	//opt->rho_C = 
	//opt->rho_P = 
}

BINOMIAL_OPTION binomial (double S, double X, double r, double t, double v, double q, unsigned long n, BINOMIAL_OPTION_STYLE style) {
	BINOMIAL_OPTION opt;
	
	opt = binomial_V(S, X, r, t, v, q, n, style);		// V: Call & Put

	opt.S = S;
	opt.X = X;
	opt.r = r;
	opt.t = t;
	opt.v = v;
	opt.q = q;
	opt.n = n;
	opt.style = style;
	
	// TODO: calc greeks using finite diff
	//binomial_greeks(&opt);

	return opt;
}

/* find implied volatility using bisection */
double binomial_iv_bisection (double S, double X, double r, double t, double q, unsigned long n, double V, BINOMIAL_OPTION_STYLE opt_style, BINOMIAL_OPTION_TYPE opt_type) {
	BINOMIAL_OPTION opt;
	long max_iteration = 200;
	double accuracy = 1e-15;
	double v = 0.0;
	double v_high = 1.0;
	double v_low = 0.0;
	double V_test;
	int i;

	// Check for arbitrage violations. Option price is too low if this happens
	if (V < (S*exp(-q*t) - X*exp(-t*r)) && opt_type == BINOMIAL_CALL) {
		fputs("binomial_iv_bisection() arbitrage violations", stderr);
	} else if (V < (X*exp(-t*r) - S*exp(-q*t)) && opt_type == BINOMIAL_PUT) {
		fputs("binomial_iv_bisection() arbitrage violations", stderr);
	}
	
	// use Binomial Search algorithm
	for (i=0; i<max_iteration; i++) {
		v = (v_high + v_low) * 0.5;
		opt = binomial_V(S, X, r, t, v, q, n, opt_style);

		if (opt_type == BINOMIAL_PUT) {
			V_test = opt.P;
		} else if (opt_type == BINOMIAL_CALL) {
			V_test = opt.C;
		} else {
			errno = EINVAL;
			perror("binomial_iv_bisection() error on passing argument 8");
			exit(EXIT_FAILURE);
		}

		if (fabs(V_test-V) <= accuracy) break;

		if (V_test-V < 0.0) {
			v_low = v;
		} else {
			v_high = v;
		}
	}

	printf("binomial_iv_bisection() iteration break at: %d\n", i);
	return v;
}





void main () {
	double S, X, r, t, v, q;
	unsigned int n;
	BINOMIAL_OPTION opt;
	double iv;
	DWORD ms1, ms2;

	S = 100.0;
	X = 100.0;
	t = 1.0;
	r = 0.05;
	v = 0.25;
	q = 0.02;		// dividend yield

	n = 1000;

	//Call       : 11.123757949150317
	//Put        : 8.226833068546192
	/*iv = binomial_iv_bisection(S, X, r, t, q, n, 8.226833068546192, BINOMIAL_EUROPEAN, BINOMIAL_PUT);
	printf("binomial_iv_bisection : %.15f\n", iv);*/

	ms1 = GetTickCount();
	// BINOMIAL_EUROPEAN   BINOMIAL_AMERICAN
	opt = binomial(S, X, r, t, v, q, n, BINOMIAL_AMERICAN);

	printf("Call       : %.15f\n", opt.C);
	printf("Put        : %.15f\n", opt.P);

	printf("Delta Call : %.15f\n", opt.delta_C);
	printf("Delta Put  : %.15f\n", opt.delta_P);
	/*printf("Gamma      : %.15f\n", opt.gamma);
	printf("Kappa      : %.15f\n", opt.kappa);
	printf("Theta Call : %.15f\n", opt.theta_C);
	printf("Theta Put  : %.15f\n", opt.theta_P);
	printf("Rho Call   : %.15f\n", opt.rho_C);
	printf("Rho Put    : %.15f\n", opt.rho_P);*/


	ms2 = GetTickCount();
	printf("\nt=%f\n", (ms2-ms1)/1000.0);

	

	_getch();
}



