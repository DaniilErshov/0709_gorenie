#pragma once
#include <iostream>
#include <fstream>
#include "constants.h"
#include "concentration.h"
#include "thermodynamic.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <cmath>
#include "chemkinReader.h"
#include "boost/regex.hpp"
#include "boost/format.hpp"
using namespace std;

#include <ida/ida.h>   
#include <kinsol/kinsol.h>             /* access to KINSOL func., consts. */
#include <nvector/nvector_serial.h>    /* access to serial N_Vector       */
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix       */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver */
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */
#include <sunnonlinsol/sunnonlinsol_newton.h> /* access to Newton SUNNonlinearSolver  */

#define FTOL   RCONST(1.e-15) /* function tolerance */
#define STOL   RCONST(1.e-15) /* step tolerance     */

#define ZERO   RCONST(0.0)
#define PT25   RCONST(0.25)
#define PT5    RCONST(0.5)
#define ONE    RCONST(1.0)
#define ONEPT5 RCONST(1.5)
#define TWO    RCONST(2.0)

#define Ith(v,i)    NV_Ith_S(v,i-1)
#define IJth(A,i,j) SM_ELEMENT_D(A,i-1,j-1)


#define PI     RCONST(3.1415926)
#define E      RCONST(2.7182818)

/* Problem Constants */
extern double k_mol;
extern double Y_N2;
extern double Y_max;
extern double P;
extern double R;
extern double x_center;
extern double koeff_l;
extern double l;
extern long int myiter;
extern long int nniters;
extern double eps_x ;
extern double eps_func;
extern double eps_fr ;
extern double Tstart ;
extern double Tfinish ;
extern const double kB ;
extern const double Angstroem__ ;
extern const double santimetr ;
extern const vector<double> M ;
extern string name_species[9];

typedef struct {
    realtype* x; //cells
    realtype* T; // temperature
    realtype* Yiprev;
    realtype* Yi; // mass fr in one cell
    realtype* Yinext; //grad mol fr
    realtype* Xiprev; // mol fr in one cell
    realtype* Xi;
    realtype* Xinext;
    realtype* gradX;
    realtype* Y_tmp;
    realtype* Y_left_bound;
    realtype* wk_add;
    int Nx;
    int N_m;
    int NEQ;
    int N_centr;
    realtype Tl;
    realtype M;
    realtype T_center;
    realtype* y;  // molar cons
    realtype* ydot; // d(molar cons) / dt
    void* mykmem;
    IO::ChemkinReader* chemkinReader;
} *UserData;

static int num_gas_species = 9;
static int num_react = 22;

static int check_retval(void* retvalvalue, const char* funcname, int opt);

double F_right(IO::ChemkinReader* chemkinReader, double* Yi, double* Yinext,
    double Tprev, double T, double Tnext, double xprev, double x, double xnext, double* Xiprev, double* Xi, double* Xinext, double* gradX, double* Y_tmp,
    double M, double* ydot, double* wk_add);

double F_rightY(IO::ChemkinReader* chemkinReader, double* Yiprev, double* Yi, double* Yinext,
    double Tprev, double T, double Tnext, double xprev, double x, double xnext, double* Xiprev, double* Xi, double* Xinext, double* gradX, double* Y_tmp,
    double M, const int k_spec, double* ydot, double* wk_add);


static int func_Y(N_Vector u, N_Vector f, void* user_data);

int Integrate_Y(IO::ChemkinReader* chemkinReader_temp, int N_x, vector<double>& x_vect,
    vector<double>& T_vect, vector<double>& Y_vect, double& M, int N_center, double* Y_leftb);

int InitialData(int& Nx, vector<double>& x_vect, vector<double>& T_vect, vector<double>& Y_vect, 
    double& M, double Tstart, double Tfinish, double* Ystart, double* Yend);

void Write_to_file2(string str, ofstream& fout, vector<double>& x_vect,
    vector<double>& T_vect, vector<double>& Y_vect, double M, int N_x, int number);

int Find_final_state_IDA(IO::ChemkinReader* chemkinReader_temp, double& Tinitial, double* Y_vect);

static int func_final_state(realtype tres, N_Vector yy, N_Vector yp, N_Vector rr, void* user_data);

int integrate_Y_IDA(IO::ChemkinReader* chemkinReader_temp, int N_x, vector<double>& x_vect,
    vector<double>& T_vect, vector<double>& Y_vect, double& M, int N_center, double* Y_leftb);

static int func_Y_IDA(realtype tres, N_Vector yy, N_Vector yp, N_Vector rr, void* user_data);

double gauss_func(double A, double mu, double sigma, double x, int k_spec);

void Add_elem(vector<double>& T, vector<double>& Y, vector<double>& x, int& N_x, int& N_center, double b);

int find_M(IO::ChemkinReader* chemkinReader_temp, int N_x, vector<double>& x_vect,
    vector<double>& T_vect, vector<double>& Y_vect, double& M, int N_center, double* Y_leftb);

static int func_M(realtype tres, N_Vector yy, N_Vector yp, N_Vector rr, void* user_data);

double tanh_OH(double A, double mu, double sigma, double x, int k_spec);