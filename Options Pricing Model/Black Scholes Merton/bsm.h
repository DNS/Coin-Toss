
#pragma once

typedef enum _BSM_OPTION_TYPE {
	BSM_PUT,
	BSM_CALL
} BSM_OPTION_TYPE;

typedef struct _BSM_OPTION {
	double S;
	double X;
	double r;
	double t;
	double v;			/* implied volatility */
	double q;			/* dividend yield */

	double C;
	double P;
	double delta_C;
	double delta_P;
	double gamma;
	double kappa;		/* kappa / vega */
	double theta_C;
	double theta_P;
	double rho_C;
	double rho_P;
} BSM_OPTION;

double erf_as (double);
double N_pdf (double);
double N_cdf (double);

BSM_OPTION bsm (double S, double X, double r, double q, double t, double v);

double bsm_iv_bisection (double S, double X, double r, double q, double t, double V, BSM_OPTION_TYPE opt_type);
double bsm_iv_newtonraphson (double S, double X, double r, double q, double t, double V, BSM_OPTION_TYPE opt_type);

double bsm_call (double S, double X, double r, double q, double t, double v);
double bsm_put (double S, double X, double r, double q, double t, double v);

double bsm_delta_C (double S, double X, double r, double q, double t, double v);
double bsm_delta_P (double S, double X, double r, double q, double t, double v);
double bsm_gamma (double S, double X, double r, double q, double t, double v);
double bsm_kappa (double S, double X, double r, double q, double t, double v);
double bsm_theta_C (double S, double X, double r, double q, double t, double v);
double bsm_theta_P (double S, double X, double r, double q, double t, double v);
double bsm_rho_C (double S, double X, double r, double q, double t, double v);
double bsm_rho_P (double S, double X, double r, double q, double t, double v);


