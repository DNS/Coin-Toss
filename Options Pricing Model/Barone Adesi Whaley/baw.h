
#pragma once

typedef enum _BAW_OPTION_TYPE {
	BAW_PUT,
	BAW_CALL
} BAW_OPTION_TYPE;

double baw_call (double S, double X, double r, double q, double t, double v);
double baw_put (double S, double X, double r, double q, double t, double v);

double baw3_call (double S, double X, double r, double q, double t, double v);
double baw3_put (double S, double X, double r, double q, double t, double v);

double baw_iv_bisection (double S, double X, double r, double q, double t, double V, BAW_OPTION_TYPE opt_type);

double baw_delta_C (double S, double X, double r, double q, double t, double v);
double baw_delta_P (double S, double X, double r, double q, double t, double v);
double baw_gamma (double S, double X, double r, double q, double t, double v);
double baw_kappa (double S, double X, double r, double q, double t, double v);
double baw_theta_C (double S, double X, double r, double q, double t, double v);
double baw_theta_P (double S, double X, double r, double q, double t, double v);
double baw_rho_C (double S, double X, double r, double q, double t, double v);
double baw_rho_P (double S, double X, double r, double q, double t, double v);

