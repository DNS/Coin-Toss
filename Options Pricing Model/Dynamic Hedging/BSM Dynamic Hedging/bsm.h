
#pragma once


typedef struct _BSM {
	double d1;
	double d2;
	double call;
	double put;
	double delta_call;
	double delta_put;
	double gamma;
	double kappa;		/* vega */
	double theta_call;
	double theta_put;
	double rho_call;
	double rho_put;
} BSM;

double erf_as (double);
double N_pdf (double);
double N_cdf (double);

BSM bsm (double, double, double, double, double);
double bsm_iv (double, double, double, double, double, double);


