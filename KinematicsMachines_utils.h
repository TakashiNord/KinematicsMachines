//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>


//
/* Author: RAE, 11 dec 2002 */

/*=================================================================================*/
/* DESCRIPTION
###############################################################################
#
# DESCRIPTION
#
#   Procs used to detect equality between scalars of real data type.
#
#  global mom_system_tolerance
#  EQ_is_equal(s, t)  (abs(s-t) <= mom_system_tolerance) Return true if scalars are equal
#  EQ_is_ge(s, t)     (s > t - mom_system_tolerance)     Return true if s is greater than
#                                            or equal to t
#  EQ_is_gt(s, t)     (s > t + mom_system_tolerance)     Return true if s is greater than t
#  EQ_is_le(s, t)     (s < t + mom_system_tolerance)     Return true if s is less than or
#                                            equal to t
#  EQ_is_lt(s, t)     (s < t - mom_system_tolerance)     Return true if s is less than t
#  EQ_is_zero(s)      (abs(s) < mom_system_tolerance)    Return true if scalar is zero
################################################################################
*/
/***********************************************************************/
int  EQ_is_equal (double s, double t);
int  EQ_is_ge (double s, double t);
int  EQ_is_gt (double s, double t);
int  EQ_is_le (double s, double t);
int  EQ_is_lt (double s, double t);
int  EQ_is_zero (double s);

//=============================================================
double ARCTAN1 (double y, double x );

//#=============================================================
double ARCTAN2 (double y, double x );

//#=============================================================
double CheckConst ( double angle, double constvar  );

//#=============================================================
double Check360 ( double angle );


//#=============================================================
double CheckLimit ( double angle, double kin_axis_min_limit, double kin_axis_max_limit );


//#=============================================================
int ROTATE_VECTOR (int plane, double angle, double v1[3], double *v[3] );



int read_word(char *s,char w,char *sw);


/***********************************************************************/
/*
Methods:
0 - ABC->ijk
..- ijk->ABC
*/
/*
Machine:
0 - // Variaxis 630-5x ��� A(����),C(����) + DMU-125PA ��� A(������),C(����)
1 - // DMU-50eV ��� B(����),C(����)
2 - // DMU-125P � DMU-200P ��� B(������),C(����)
3 - // ��� B,C (������) - V-STAR (+POGO), ��-5�� B(=A) , NC-1325IP , DMU-35M
4 - // ��� A,B (������) - FSQV80KR , MA655c5 ,��14V7
5 - // RFP-6K ��� A(������),C(������)
6 - // ���-630-800 ��� B(����)
7 - // MAG3ex ��� A(������),C(������)
8 - // Ipac ��� A(���������),E(������ ����������)
*/
int ABCijk(int Machine,int Methods, double ang[12], double ijk[3]);


/***********************************************************************/
/*
Methods:
0 - ABC->ijk
1 - ijk->ABC
*/
// �������: ����������� ���������� -> (Theta,Phi)<->(i,j,k)
int ABCijk_sphera(int Methods, double ang[12], double ijk[3]);

/***********************************************************************/
/*
Methods:
0 - ABC->ijk
1 - ijk->ABC
Machine:
  n - ��� �����
 -1 - �������������� �����������
*/
/*
 ������� - ������ CLSF (i,j,k) - � ������������ ���� A,B,C (������� A1,B1,C1 - A2,B2,C2)
*/
/*===================================================================*/
long int  clsf_ijkABC(int Machine,int Methods,char *namefile,char *namefilelog);

/***********************************************************************/
/*
Methods:
0 - ABC->ijk
1 - ijk->ABC
Machine:
  n - ��� �����
 -1 - �������������� �����������
*/
/*
 ������� - ������ �� (���� A,B,C) - � ������������ i,j,k
*/
/*===================================================================*/
long int  up_ABCijk(int Machine,int Methods,char *namefile,char *namefilelog);



/*===================================================================*/

