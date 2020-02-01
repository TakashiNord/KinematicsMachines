//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
//#include <dir.h>

//
/* Author: RAE, 11 dec 2002 */

#define mStr 128

#define PI (2.0 * asin(1.0))  /*value PI*/
#define RAD2DEG (90.0 / asin(1.0))    /*multiplier to convert radians to degrees*/
#define DEG2RAD (asin(1.0) / 90.0)    /*multiplier to convert degrees to radians*/
#define system_tolerance  0.000001


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
int  EQ_is_equal (double s, double t)
{
  if (fabs(s-t)<= system_tolerance)  { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_ge (double s, double t)
{
  if (s > (t - system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_gt (double s, double t)
{
  if (s > (t + system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_le (double s, double t)
{
  if (s < (t + system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_lt (double s, double t)
{
  if (s < (t - system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_zero (double s)
{
  if (fabs(s)<= system_tolerance)  { return(1); } else { return(0) ; }
}

//=============================================================
double ARCTAN1 (double y, double x )
//#=============================================================
{
   double ang;
   if (EQ_is_zero(y)) { y=0; }
   if (EQ_is_zero(x)) { x=0; }

   if (y == 0 && x == 0) { return(0); }
   ang=atan2(y,x);
   if (ang < 0 ) {
      return(ang + PI*2);
   }
   return(ang);
}

//#=============================================================
double ARCTAN2 (double y, double x )
//#=============================================================
{
   double ang;
   if (EQ_is_zero(y)) {
      if (x < 0.0) { return (PI); }
      return (0.0);
   }
   if (EQ_is_zero(x)) {
      if (y < 0.0) { return(PI*1.5); }
      return(PI*.5);
   }
   ang=atan(y/x);
   if (x > 0.0 && y < 0.0) { return(ang+PI*2.0); }
   if (x < 0.0 && y < 0.0) { return(ang+PI); }
   if (x < 0.0 && y > 0.0) { return(ang+PI); }

   return(ang);
}

//#=============================================================
double CheckConst ( double angle, double constvar  )
//#=============================================================
{
  /*while {$angle < 0.0} {set angle [expr $angle+360.0]}
   while {$angle >= 360.0} {set angle [expr $angle-360.0]}
   */
   while (angle < -constvar) { angle+=constvar ; }
   while (angle >= constvar) { angle-=constvar ; }
   return (angle) ;
}

//#=============================================================
double Check360 ( double angle )
//#=============================================================
{
  /*while {$angle < 0.0} {set angle [expr $angle+360.0]}
   while {$angle >= 360.0} {set angle [expr $angle-360.0]}
   */
   while (angle < -360.) { angle+=360. ; }
   while (angle >= 360.) { angle-=360. ; }
   return (angle) ;
}

//#=============================================================
double CheckLimit ( double angle, double kin_axis_min_limit, double kin_axis_max_limit )
//#=============================================================
{
  /*while {[expr $a4-$mom_kin_4th_axis_min_limit] > 360.0} {set a4 [expr $a4-360.0]}
   */
   while ((angle-kin_axis_min_limit) > 360.) { angle-=360. ; }
   while ((kin_axis_max_limit-angle) <= -360.) { angle+=360. ; }
   return (angle) ;
}


//#=============================================================
int ROTATE_VECTOR (int plane, double angle, double v1[3], double *v[3] ) {
//output_vector=v ; input_vector=v1
//#=============================================================
   if (plane == 0) {
      *v[0]=v1[0];
      *v[1]=v1[1]*cos(angle) - v1[2]*sin(angle);
      *v[2]=v1[2]*cos(angle) + v1[1]*sin(angle);
   } else if (plane == 1) {
      *v[0]=v1[0]*cos(angle) + v1[2]*sin(angle);
      *v[1]=v1[1];
      *v[2]=v1[2]*cos(angle) - v1[0]*sin(angle);
   } else if (plane == 2) {
      *v[0]=v1[0]*cos(angle) - v1[1]*sin(angle);
      *v[1]=v1[1]*cos(angle) + v1[0]*sin(angle);
      *v[2]=v1[2];
   }
   return(0);
}

/*################################################################################*/
/**/
//---------------------------------------------------------------------------
/* This function read the numeric string after key letter
  in success it return 1, otherwice 0
*/
int read_word(char *s,char w,char *sw)
{
         char *word,sym;
   int  n;
   if(s==NULL) return(0);
   if ((word=strchr(s,w))==NULL) return(0);
   n=0;
   do
   {  sym=*(word+(n+1));
     if (isdigit(sym)||sym==' '||sym=='+'||sym=='-'||sym=='.') n++;
     else break;
   } while (1);
   if (n>0){ strncpy(sw,word+1,n); sw[n]='\0'; return(n); }
   else                                        return(0);
}


/***********************************************************************/
/*
Methods:
0 - ABC->ijk
..- ijk->ABC
*/
/*
Machine:
0 - // Variaxis 630-5x оси A(стол),C(стол) + DMU-125PA оси A(голова),C(стол)
1 - // DMU-50eV оси B(стол),C(стол)
2 - // DMU-125P и DMU-200P оси B(голова),C(стол)
3 - // оси B,C (голова) - V-STAR (+POGO), ВФ-5ВС B(=A) , NC-1325IP , DMU-35M
4 - // оси A,B (голова) - FSQV80KR , MA655c5 ,ФП14V7
5 - // RFP-6K оси A(голова),C(голова)
6 - // АГП-630-800 оси B(стол)
7 - // MAG3ex оси A(голова),C(голова)
8 - // Ipac оси A(платформа),E(нижний инструмент)
*/
/*
norm:
1 - нормировать вектора
0 - не нормировать вектора
*/
int ABCijk(int Machine,int Methods, double ang[12], double ijk[3], int norm)
{
  double ang_rad[12];
  double i,j,k ;
  double phi , theta , r ;
  double p1 , p2 , p0 ;
  double rad ;
  double A0,A1,A2,B0,B1,B2,C0,C1,C2 ;
  int ii;

  rad=3.1415926/180.;
  i=0.; j=0. ; k=1.;

 // проверка и установка значений.
  switch (Methods)
  {
   case 0 :  // A,B,C->i,j,k
       ang_rad[0]=ang[0]*rad; //A
       ang_rad[1]=ang[1]*rad; //B
       ang_rad[2]=ang[2]*rad; //C
       ang_rad[3]=0.*rad; //A2
       ang_rad[4]=0.*rad; //B2
       ang_rad[5]=0.*rad; //C2
       ang_rad[6]=0.*rad; //A3
       ang_rad[7]=0.*rad; //B3
       ang_rad[8]=0.*rad; //C3
       ang_rad[9]=0.*rad; //A4
       ang_rad[10]=0.*rad; //B4
       ang_rad[11]=0.*rad; //C4
   break ;
   default :
       i=ijk[0]; j=ijk[1]; k=ijk[2];
       r=sqrt(i*i+j*j+k*k);
       if (EQ_is_zero(r)) { i=0.; j=0.; k=1.; r=1.; }
       if (norm) if (!EQ_is_equal(r,1.000)) { i/= r; j/= r; k/= r; }
  }
  
  if (i>1.) i=1.; if (i<-1.) i=-1.;
  if (j>1.) j=1.; if (j<-1.) j=-1.;
  if (k>1.) k=1.; if (k<-1.) k=-1.;

  // расчет
  switch (Machine)
  {

   //Variaxis 630-5x оси A(стол),C(стол) + DMU-125PA оси A(голова),C(стол)
   case 0 :
       switch (Methods)
       {
        case 0 :
        i=sin(ang_rad[0])*sin(ang_rad[2]);
        j=-sin(ang_rad[0])*cos(ang_rad[2]);
        k=cos(ang_rad[0]);
        break ;
        default :
          C1=0.;
          A1=acos(k);
          if (EQ_is_ge(i,0.) && EQ_is_le(j,0.)) {
            if (EQ_is_zero(j)) C0=PI/2 ; else C0=-atan(i/j);
            C1=C0;
          }
          if (EQ_is_ge(i,0.) && EQ_is_gt(j,0.)) {
            C0=-atan(i/j);
            C1=PI+C0;
          }
          if (EQ_is_lt(i,0.) && EQ_is_gt(j,0.)) {
            C0=-atan(i/j);
            C1=PI+C0;
          }
          if (EQ_is_lt(i,0.) && EQ_is_le(j,0.)) {
            if (EQ_is_zero(j)) C0=-PI/2. ; else C0=-atan(i/j);
            C1=2.*PI+C0;
          }

          A2=-acos(k);
          C2=C1+PI;
          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }

          ang_rad[0]=A1; //A1
          ang_rad[1]=0.0; //B1
          ang_rad[2]=C1; //C1

          ang_rad[3]=A2; //A2
          ang_rad[4]=0.; //B2
          ang_rad[5]=C2; //C2

        }
   break;
   
   // DMU-50eV оси B(стол),C(стол)
   case 1 :
       switch (Methods)
       {
        case 0 :
          k=(1+cos(ang_rad[1]))/2.0;
          j=cos(ang_rad[1])*cos(ang_rad[2])-0.5*(1+cos(ang_rad[1]))*cos(ang_rad[2])+cos(45.*rad)*sin(ang_rad[1])*sin(ang_rad[2]);
          i=cos(45.*rad)*sin(ang_rad[1])*cos(ang_rad[2])-cos(ang_rad[1])*sin(ang_rad[2])+0.5*(1+cos(ang_rad[1]))*sin(ang_rad[2]);
        break ;
        default :

          if (k<0.) { k=fabs(k); }

          B1=0.; C1=0.; B2=0.; C2=0.;

          B1=acos(2.*k-1.0);

          p0=sin(B1)*cos(45.*rad);
          p1=(-i)*(k-1.)+j*p0;
          p2=(-j)*(k-1.)-i*p0;

          if (EQ_is_ge(p1,0.) && EQ_is_le(p2,0.)) {
            if (EQ_is_zero(p1)) C0=0. ; else C0=PI/2.+atan(p2/p1);
            C1=C0;
          }
          if (EQ_is_gt(p1,0.) && EQ_is_gt(p2,0.)) {
                C0=atan(p2/p1);
            C1=PI/2.+C0;
          }
          if (EQ_is_le(p1,0.) && EQ_is_ge(p2,0.)) {
            if (EQ_is_zero(p1)) C0=-PI/2. ; else C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }
          if (EQ_is_lt(p1,0.) && EQ_is_lt(p2,0.)) {
            C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }

          if (EQ_is_ge(C1,(2.*PI))) { C1-=(2.*PI) ; }
          if (EQ_is_le(C1,(-2.*PI))) { C1+=(2.*PI) ; }

          ang_rad[0]=0.0; //A1
          ang_rad[1]=B1; //B1
          ang_rad[2]=C1; //C1

          B2=B1;
          C2=C1-(2.*PI);

          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }
          if (EQ_is_le(C2,(-2.*PI))) { C2+=(2.*PI) ; }

          ang_rad[3]=0.; //A2
          ang_rad[4]=B2; //B2
          ang_rad[5]=C2; //C2

          // дополнительное решение
          B1=-acos(2.*k-1.0);
          B2=2.*PI+B1;

          p0=sin(B2)*cos(45.*rad);
          p1=(-i)*(k-1.)+j*p0;
          p2=(-j)*(k-1.)-i*p0;

          if (EQ_is_ge(p1,0.) && EQ_is_le(p2,0.)) {
            if (EQ_is_zero(p1)) C0=0. ; else C0=PI/2.+atan(p2/p1);
            C1=C0;
          }
          if (EQ_is_gt(p1,0.) && EQ_is_gt(p2,0.)) {
                C0=atan(p2/p1);
            C1=PI/2.+C0;
          }
          if (EQ_is_le(p1,0.) && EQ_is_ge(p2,0.)) {
            if (EQ_is_zero(p1)) C0=-PI/2. ; else C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }
          if (EQ_is_lt(p1,0.) && EQ_is_lt(p2,0.)) {
            C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }

          if (EQ_is_ge(C1,(2.*PI))) { C1-=(2.*PI) ; }
          if (EQ_is_le(C1,(-2.*PI))) { C1+=(2.*PI) ; }
          //mes[0]='\0';sprintf(mes,"Доп. решение 1: B3=%.4lf  С3=%.4lf ",B1/rad,C1/rad);
          ang_rad[6]=0.; //A3
          ang_rad[7]=B1; //B3
          ang_rad[8]=C1; //C3

          C2=C1-(2.*PI);
          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }
          if (EQ_is_le(C2,(-2.*PI))) { C2+=(2.*PI) ; }
          //mes[0]='\0';sprintf(mes,"Доп. решение 2: B4=%.4lf  С4=%.4lf ",B1/rad,C2/rad);
          ang_rad[9]=0.; //A4
          ang_rad[10]=B1; //B4
          ang_rad[11]=C2; //C4

        }
   break;

   // DMU-125P и DMU-200P оси B(голова),C(стол)
   case 2 :
       switch (Methods)
       {
        case 0 :
        k=(1+cos(ang_rad[1]))/2.0;
        j=-cos(ang_rad[1])*cos(ang_rad[2])+0.5*(1+cos(ang_rad[1]))*cos(ang_rad[2])+cos(45.*rad)*sin(ang_rad[1])*sin(ang_rad[2]);
        i=cos(45.*rad)*sin(ang_rad[1])*cos(ang_rad[2])+cos(ang_rad[1])*sin(ang_rad[2])-0.5*(1+cos(ang_rad[1]))*sin(ang_rad[2]);
        break ;
        default :
          B1=0.; C1=0.; B2=0.; C2=0.;

          B1=acos(2.*k-1.0);

          p0=sin(B1)*cos(45.*rad);
          p1=i*(k-1.)+j*p0;
          p2=j*(k-1.)-i*p0;

          if (EQ_is_ge(p1,0.) && EQ_is_le(p2,0.)) {
            if (EQ_is_zero(p1)) C0=0. ; else C0=PI/2.+atan(p2/p1);
            C1=C0;
          }
          if (EQ_is_gt(p1,0.) && EQ_is_gt(p2,0.)) {
                C0=atan(p2/p1);
            C1=PI/2.+C0;
          }
          if (EQ_is_le(p1,0.) && EQ_is_ge(p2,0.)) {
            if (EQ_is_zero(p1)) C0=-PI/2. ; else C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }
          if (EQ_is_lt(p1,0.) && EQ_is_lt(p2,0.)) {
            C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }

          if (EQ_is_ge(C1,(2.*PI))) { C1-=(2.*PI) ; }
          if (EQ_is_le(C1,(-2.*PI))) { C1+=(2.*PI) ; }

          ang_rad[0]=0.;
          ang_rad[1]=B1;
          ang_rad[2]=C1;

          B2=B1;
          C2=C1-(2.*PI);

          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }
          if (EQ_is_le(C2,(-2.*PI))) { C2+=(2.*PI) ; }

          ang_rad[3]=0.;
          ang_rad[4]=B2;
          ang_rad[5]=C2;

          // дополнительное решение

          B1=-acos(2.*k-1.0);
          B2=2.*PI+B1;

          p0=sin(B2)*cos(45.*rad);
          p1=(-i)*(k-1.)+j*p0;
          p2=(-j)*(k-1.)-i*p0;

          if (EQ_is_ge(p1,0.) && EQ_is_le(p2,0.)) {
            if (EQ_is_zero(p1)) C0=0. ; else C0=PI/2.+atan(p2/p1);
            C1=C0;
          }
          if (EQ_is_gt(p1,0.) && EQ_is_gt(p2,0.)) {
                C0=atan(p2/p1);
            C1=PI/2.+C0;
          }
          if (EQ_is_le(p1,0.) && EQ_is_ge(p2,0.)) {
            if (EQ_is_zero(p1)) C0=-PI/2. ; else C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }
          if (EQ_is_lt(p1,0.) && EQ_is_lt(p2,0.)) {
            C0=atan(p2/p1);
            C1=3.*PI/2.+C0;
          }

          if (EQ_is_ge(C1,(2.*PI))) { C1-=(2.*PI) ; }
          if (EQ_is_le(C1,(-2.*PI))) { C1+=(2.*PI) ; }

          //mes[0]='\0';sprintf(mes,"Доп. решение 1: B3=%.4lf  С3=%.4lf ",B1/rad,C1/rad);
          ang_rad[6]=0.; //A3
          ang_rad[7]=B1; //B3
          ang_rad[8]=C1; //C3

          C2=C1-(2.*PI);
          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }
          if (EQ_is_le(C2,(-2.*PI))) { C2+=(2.*PI) ; }
          //mes[0]='\0';sprintf(mes,"Доп. решение 2: B4=%.4lf  С4=%.4lf ",B1/rad,C2/rad);
          ang_rad[9]=0.; //A4
          ang_rad[10]=B1; //B4
          ang_rad[11]=C2; //C4

        }
   break;

   // V-STAR v3500ORB - оси B,C (голова) + ВФ-5ВС B(=A), C - голова +    NC-1325IP оси B(голова),C(голова) + DMU-35M
   case 3 :
       switch (Methods)
       {
        case 0 :
          i=sin(ang_rad[1])*cos(ang_rad[2]);
          j=sin(ang_rad[1])*sin(ang_rad[2]);
          k=cos(ang_rad[1]);
        break ;
        default :
          C1=0.;
          B1=acos(k);
          if (EQ_is_gt(i,0.) && EQ_is_gt(j,0.)) {
            C0=atan(j/i);
            C1=C0;
          }
          if (EQ_is_le(i,0.) && EQ_is_ge(j,0.)) {
            if (EQ_is_zero(i)) C0=-PI/2 ; else C0=atan(j/i);
            C1=PI+C0;
          }
          if (EQ_is_lt(i,0.) && EQ_is_lt(j,0.)) {
            C0=atan(j/i);
            C1=PI+C0;
          }
          if (EQ_is_ge(i,0.) && EQ_is_le(j,0.)) {
            if (EQ_is_zero(i)) C0=-PI/2. ; else C0=atan(j/i);
            C1=2.*PI+C0;
          }

          B2=-acos(k);
          C2=C1+PI;
          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }

          ang_rad[0]=0.;
          ang_rad[1]=B1;
          ang_rad[2]=C1;

          ang_rad[3]=0.;
          ang_rad[4]=B2;
          ang_rad[5]=C2;

        }
   break;

   // оси A,B (голова) - FSQV80KR , MA655c5 ,ФП14V7
   case 4 :
       switch (Methods)
       {
        case 0 :
          i=cos(ang_rad[0])*sin(ang_rad[1]);
          j=-sin(ang_rad[0]);
          k=cos(ang_rad[0])*cos(ang_rad[1]);
        break ;
        default :
          ang_rad[0]=-asin(j);
          if (EQ_is_zero(k)) ang_rad[1]=0; else ang_rad[1] = atan(i/k) ;
          ang_rad[2]=0.0;
        }
   break;

   // RFP-6K оси A(голова),C(голова)
   case 5 :
       switch (Methods)
       {
        case 0 :
        i=-sin(ang_rad[0])*sin(ang_rad[2]);
        j=sin(ang_rad[0])*cos(ang_rad[2]);
        k=cos(ang_rad[0]);
        break ;
        default :
          C1=0.;
          A1=acos(k);
          if (EQ_is_le(i,0.) && EQ_is_gt(j,0.)) {
            C0=-atan(i/j);
            C1=C0;
          }
          if (EQ_is_le(i,0.) && EQ_is_le(j,0.)) {
            if (EQ_is_zero(j)) C0=-PI/2 ; else C0=-atan(i/j);
            C1=PI+C0;
          }
          if (EQ_is_gt(i,0.) && EQ_is_lt(j,0.)) {
            C0=-atan(i/j);
            C1=PI+C0;
          }
          if (EQ_is_gt(i,0.) && EQ_is_ge(j,0.)) {
            if (EQ_is_zero(j)) C0=-PI/2. ; else C0=-atan(i/j);
            C1=2.*PI+C0;
          }

          A2=-acos(k);
          C2=C1+PI;
          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }

          ang_rad[0]=A1;
          ang_rad[1]=0.0;
          ang_rad[2]=C1;

          ang_rad[3]=A2;
          ang_rad[4]=0.;
          ang_rad[5]=C2;

        }
   break;

   // АГП-630-800 оси B(стол)
   case 6 :
       switch (Methods)
       {
        case 0 :
         i=sin(ang_rad[1]);
         j=0.0;
         k=cos(ang_rad[1]);
        break ;
        default :
          j=0.;
          B1=0.;
          B0=0.;
          if (EQ_is_ge(i,0.)) {
             if (EQ_is_gt(k,0.)) { B0=acos(k); B1=B0; } else { B0=acos(k); B1=B0; }
          }
          if (EQ_is_lt(i,0.)) {
             if (EQ_is_lt(k,0.)) {
                B0=atan(i/k);
                B1=B0+PI ;
             } else {
              if (EQ_is_zero(k)) { B0=-PI/2. ; } else { B0=atan(i/k); }
              B1=2.*PI+B0 ;
             }
          }

          if (EQ_is_ge(B1,0.)) B0=1.; else B0=-1. ;
          B2=(-1.)*B0*(2*PI-fabs(B1));

          ang_rad[0]=0.;
          ang_rad[1]=B1;
          ang_rad[2]=0.;

          ang_rad[3]=0.;
          ang_rad[4]=B2;
          ang_rad[5]=0.;

        }
   break;

   //MAG3ex оси A(голова),C(голова)
   case 7 :
       switch (Methods)
       {
        case 0 :
        i=sin(ang_rad[0])*sin(ang_rad[2]);
        j=-sin(ang_rad[0])*cos(ang_rad[2]);
        k=cos(ang_rad[0]);
        break ;
        default :
          C1=0.;
          A1=acos(k);
          if (EQ_is_ge(i,0.) && EQ_is_lt(j,0.)) {
            C0=-atan(i/j);
            C1=C0;
          }
          if (EQ_is_ge(i,0.) && EQ_is_ge(j,0.)) {
            if (EQ_is_zero(j)) C0=0 ; else C0=-atan(i/j);
            C1=PI+C0;
          }
          if (EQ_is_lt(i,0.) && EQ_is_gt(j,0.)) {
            C0=-atan(i/j);
            C1=PI+C0;
          }
          if (EQ_is_lt(i,0.) && EQ_is_le(j,0.)) {
            if (EQ_is_zero(j)) C0=-PI/2. ; else C0=-atan(i/j);
            C1=2.*PI+C0;
          }

          A2=-acos(k);
          C2=C1+PI;
          if (EQ_is_ge(C2,(2.*PI))) { C2-=(2.*PI) ; }

          ang_rad[0]=A1; //A1
          ang_rad[1]=0.0; //B1
          ang_rad[2]=C1; //C1

          ang_rad[3]=A2; //A2
          ang_rad[4]=0.; //B2
          ang_rad[5]=C2; //C2

        }
   break;

   // Ipac оси A(платформа)
   case 8 :
       switch (Methods)
       {
        case 0 :
         i=0.0;
         j=sin(ang_rad[0]);
         k=cos(ang_rad[0]);
        break ;
        default :
          i=0.;
          A1=0.;
          A0=0.;
          if (EQ_is_ge(j,0.)) {
             if (EQ_is_gt(k,0.)) { A0=acos(k); A1=A0; } else { A0=acos(k); A1=A0; }
          }
          if (EQ_is_lt(j,0.)) {
             if (EQ_is_lt(k,0.)) {
                A0=atan(j/k);
                A1=A0+PI ;
             } else {
              if (EQ_is_zero(k)) { A0=-PI/2. ; } else { A0=atan(j/k); }
              A1=2.*PI+A0 ;
             }
          }

          if (EQ_is_ge(A1,0.)) A0=1.; else A0=-1. ;
          A2=(-1.)*A0*(2*PI-fabs(A1));

          ang_rad[0]=A1;
          ang_rad[1]=0.;
          ang_rad[2]=0.;

          ang_rad[3]=A2;
          ang_rad[4]=0.;
          ang_rad[5]=0.;

        }
   break;



   default :break;
  }

  switch (Methods)
  {
   case 0 :  // A,B,C->i,j,k
   break ;
   default : // i,j,k->A,B,C
       for(ii=0;ii<12;ii++) ang[ii]=ang_rad[ii]/rad; // i
   break;
  }

 ijk[0]=i; ijk[1]=j; ijk[2]=k;

 return (0);
}


/***********************************************************************/
/*
Methods:
0 - ABC->ijk
..- ijk->ABC
*/
/*
norm:
1 - нормировать вектора
0 - не нормировать вектора
*/
// Перевод: Сферические координаты -> (Theta,Phi)<->(i,j,k)
int ABCijk_sphera(int Methods, double ang[12], double ijk[3], int norm)
{
  double ang_rad[12];
  double i,j,k ;
  double phi , theta , r ;
  double rad ;
  double A1 ;

  rad=3.1415926/180.;
  i=0.; j=0. ; k=1.;

 // проверка и установка значений.
  switch (Methods)
  {
   case 0 :  // A,B,C->i,j,k
       ang_rad[0]=ang[0]*rad; //A
       ang_rad[1]=ang[1]*rad; //B
       ang_rad[2]=ang[2]*rad; //C
       ang_rad[3]=0.*rad; //A2
       ang_rad[4]=0.*rad; //B2
       ang_rad[5]=0.*rad; //C2
       ang_rad[6]=0.*rad; //A3
       ang_rad[7]=0.*rad; //B3
       ang_rad[8]=0.*rad; //C3
       ang_rad[9]=0.*rad; //A4
       ang_rad[10]=0.*rad; //B4
       ang_rad[11]=0.*rad; //C4

       i=sin(ang_rad[0])*cos(ang_rad[1]);
       j=sin(ang_rad[0])*sin(ang_rad[1]);
       k=cos(ang_rad[0]);

   break ;
   default :
       i=ijk[0]; j=ijk[1]; k=ijk[2];
       r=sqrt(i*i+j*j+k*k);
       if (EQ_is_zero(r)) { i=0.; j=0.; k=1.;  }
       r=sqrt(i*i+j*j+k*k);
       if (norm) if (!EQ_is_equal(r,1.000)) { i/= r; j/= r; k/= r; }
  }

  if (i>1.) i=1.; if (i<-1.) i=-1.;
  if (j>1.) j=1.; if (j<-1.) j=-1.;
  if (k>1.) k=1.; if (k<-1.) k=-1.;

  // перерасчет углов - исходя из i,j,k и ограничений
  phi=0.;
  theta=0.;
  r=sqrt(i*i+j*j+k*k);
  A1=sqrt(i*i+j*j);
  if (!EQ_is_zero(A1)) {
    A1=i/A1;
    if (EQ_is_ge(j,0.)) phi=acos(A1); else { phi=2.*PI-acos(A1); }
  }
  theta=acos(k);

  ang[0]=theta/rad; //theta
  ang[1]=phi/rad; //phi
  ang[2]=r; //r

  ang[0]=CheckConst(ang[0],180.);
  ang[1]=CheckConst(ang[1],360.);

  ijk[0]=i; ijk[1]=j; ijk[2]=k;

  return (0);
}


/*
Methods:
0 - ABC->ijk
..- ijk->ABC
*/
/*
norm:
1 - нормировать вектора
0 - не нормировать вектора
*/
/*
 функция - читает CLSF (i,j,k) - и рассчитывает углы A,B,C (выводит A1,B1,C1 - A2,B2,C2)
*/
/*===================================================================*/
long int  clsf_ijkABC(int Machine,int Methods,char *namefile,char *namefilelog, int norm)
{
/*локальные переменные*/
 FILE *rf,*ff; /*Указатель на файл*/
 long int numbers_of_kadrs;/* Число кадров*/
 double min_points[6],max_points[6];
 double p[6];
 double ang[12], ijk[3];
 char info[mStr*3];

//локальные переменные
 char curr[mStr],currtmp[mStr*2],s[mStr],*ptr;
 long int i ;
 time_t t;

 /*Открываем файл*/
 rf=fopen(namefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",namefile );
   fprintf(stderr, info);
   return(-1);
  }

 /*Открываем файл*/
 ff=fopen(namefilelog,"w");
 if (ff==NULL) {
   fclose(rf);
   info[0]='\0';
   sprintf(info,"Невозможно открыть файл:\n %s \n на 'запись' \n",namefilelog );
   fprintf(stderr, info);
   return(-2);
  }


 /*Начальные условия*/
 numbers_of_kadrs=0;
 for(i=0;i<6;i++)
 {
   min_points[i]=999999.999;
   max_points[i]=-999999.999;
 }
 for(i=0;i<12;i++) { ang[i]=0.0; }
 for(i=0;i<6;i++) p[i]=0.;


 t = time(NULL);
 /* параметры вывода */
 fprintf(ff,"$$ Current date : %s",ctime(&t) );
 fprintf(ff,"$$ Machine %d / Methods %d \n" , Machine, Methods );
 fprintf(ff,"$$ =====================================================================\n");
 fprintf(ff,"$$ FORMAT = $$       A1         B1         C1      |       A2         B2         C2 \n");
 fprintf(ff,"$$ =====================================================================\n");

 curr[0]='\0'; fgets(curr,mStr,rf);
 while(!feof(rf))
 {

   currtmp[0]='\0';
   strcpy(currtmp,curr);
   if ((NULL!=strstr(curr, "GOTO/")) || (NULL!=strstr(curr, "FROM/")) || (NULL!=strstr(curr, "GOHOME/")))
   {
   ptr = strtok(curr, "GOTO/");
   if (ptr==NULL) { ptr = strtok(curr, "FROM/"); }
   if (ptr==NULL) { ptr = strtok(curr, "GOHOME/"); }

   if (ptr!=NULL) {
    s[0]='\0'; strcpy(s,ptr); strcat(s,"\0");
    i=0;
    while (s[i]!='\0')
    {
      if (s[i]==',' || s[i]=='/') s[i]=' ';
      i++;
    }
    i=sscanf(s,"%lf %lf %lf %lf %lf %lf",&p[0],&p[1],&p[2],&p[3],&p[4],&p[5]);
    if (6==i) {
      /*=Преобразование=*/
      ijk[0]=p[3]; ijk[1]=p[4]; ijk[2]=p[5];
      if (Machine<0) i=ABCijk_sphera(Methods,ang,ijk,norm); else i=ABCijk(Machine,Methods,ang, ijk, norm);
      /*==============*/
      i=0;
      while (currtmp[i]!='\0')
      {
       if (currtmp[i]=='\n' || currtmp[i]=='\r') { currtmp[i]=' '; } // { currtmp[i]='\0'; break ; }
       i++;
      }
      curr[0]='\0'; sprintf(curr," $$  %.8lf  %.8lf  %.8lf  |  %.8lf  %.8lf  %.8lf\n",ang[0],ang[1],ang[2],ang[3],ang[4],ang[5]);
      strcat(currtmp,curr); strcat(currtmp,"\0");
    }
   }
   }

   /*Анализ на экстремум 1*/
   for(i=0;i<6;i++) {
      if (ang[i]<=min_points[i]) { min_points[i]=ang[i]; }
      if (ang[i]>=max_points[i]) { max_points[i]=ang[i]; }
   }

   /*Запись*/
   fputs(currtmp,ff);

   curr[0]='\0'; fgets(curr,mStr,rf);
   numbers_of_kadrs++;
 } // end while

 fclose(rf);

  /* параметры вывода */
 fprintf(ff,"$$ =====================================================================\n");
 fprintf(ff,"$$ MIN = %.8lf %.8lf %.8lf  | %.8lf %.8lf %.8lf\n",min_points[0],min_points[1],min_points[2],min_points[3],min_points[4],min_points[5]);
 fprintf(ff,"$$ MAX = %.8lf %.8lf %.8lf  | %.8lf %.8lf %.8lf\n",max_points[0],max_points[1],max_points[2],max_points[3],max_points[4],max_points[5]);
 fprintf(ff,"$$ =====================================================================\n");
 fclose(ff);

 return(numbers_of_kadrs);
}


/*
Methods:
0 - ABC->ijk
..- ijk->ABC
*/
/*
norm:
1 - нормировать вектора
0 - не нормировать вектора
*/
/*
 функция - читает УП (углы A,B,C) - и рассчитывает i,j,k
*/
/*===================================================================*/
long int  up_ABCijk(int Machine,int Methods,char *namefile,char *namefilelog, int norm)
{
/*локальные переменные*/
 FILE *rf,*ff; /*Указатель на файл*/
 long int numbers_of_kadrs;/* Число кадров*/
 double min_points[3],max_points[3];
 double ang[12],ijk[3];
 char info[mStr*3];

//локальные переменные
 char curr[mStr],currtmp[mStr*2];
 char word[mStr];
 double curr_points[3],prev_points[3];
 long int i ;
 double Gmode_coord, Gmotion ;
 double gcode;
 char *jpd,*jpdg;

 /*Открываем файл*/
 rf=fopen(namefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",namefile );
   fprintf(stderr, info);
   return(-1);
  }

 /*Открываем файл*/
 ff=fopen(namefilelog,"w");
 if (ff==NULL) {
   fclose(rf);
   info[0]='\0';
   sprintf(info,"Невозможно открыть файл:\n %s \n на 'запись' \n",namefilelog );
   fprintf(stderr, info);
   return(-2);
  }


 /*Начальные условия*/
 numbers_of_kadrs=0;
 for(i=0;i<3;i++)
 {
   min_points[i]=999999.999;
   max_points[i]=-999999.999;
   prev_points[i]=0;
   curr_points[i]=0;
 }
 for(i=0;i<12;i++) { ang[i]=0.0; }
 for(i=0;i<3;i++) { ijk[i]=0.0; }

 Gmode_coord=90;
 Gmotion=0;

 curr[0]='\0'; fgets(curr,mStr,rf);
 while(!feof(rf))
 {

   /*Кадры с G - взято из проекта Symmetry ( блок trans)*/
   if (read_word(curr,'G',word))  {
     currtmp[0]='\0';strcpy(currtmp,curr);
     jpd=strchr(currtmp,'G');
     if(jpd!=NULL) {
       while(strcmp(jpd,""))
       {
         if (read_word(jpd,'G',word))
         { sscanf(word,"%lf",&gcode);
           /*printf(" Gf==%f Gs=%s |" ,gcode,word);*/
           if (gcode==0) {
               Gmotion=gcode;
           }
           if (gcode==1) {
               Gmotion=gcode;
           }
           if ((gcode==2)||(gcode==3)) {
               Gmotion=gcode;
           }
           if ((gcode==90)||(gcode==91)) {
               Gmode_coord=gcode;
           }
         }
         jpdg=strchr(jpd,'G');
         if(jpdg==NULL) strcpy(jpd,"");else strcpy(jpd,jpdg+1);
       }
     }

   }

   if (Gmode_coord==91) {
       for(i=0;i<3;i++) curr_points[i]=0.0;
   }

   if (read_word(curr,'A',word)) { sscanf(word,"%lf",&curr_points[0]); }
   if (read_word(curr,'B',word)) { sscanf(word,"%lf",&curr_points[1]); }
   if (read_word(curr,'C',word)) { sscanf(word,"%lf",&curr_points[2]); }

   if (Gmode_coord==91) {
       for(i=0;i<3;i++) { curr_points[i]+=prev_points[i]; }
   }

   /*Анализ на экстремум 1*/
   for(i=0;i<3;i++) {
      if (curr_points[i]<=min_points[i]) { min_points[i]=curr_points[i]; }
      if (curr_points[i]>=max_points[i]) { max_points[i]=curr_points[i]; }
   }

   for(i=0;i<3;i++) prev_points[i]=curr_points[i];

   /*=Преобразование=*/
   currtmp[0]='\0';  strcpy(currtmp,curr);
   for(i=0;i<3;i++) { ang[i]=curr_points[i]; ijk[i]=0.0; }
   if (Machine<0) i=ABCijk_sphera(Methods,ang,ijk,norm); else i=ABCijk(Machine,Methods,ang, ijk, norm);
   /*==============*/
   i=0;
   while (currtmp[i]!='\0')
   {
   if (currtmp[i]=='\n' || currtmp[i]=='\r') { currtmp[i]=' '; } // { currtmp[i]='\0'; break ; }
     i++;
   }
   curr[0]='\0'; sprintf(curr,"                  $$  %.8lf  %.8lf  %.8lf \n",ijk[0],ijk[1],ijk[2]);
   strcat(currtmp,curr); strcat(currtmp,"\0");

   /*Запись*/
   fputs(currtmp,ff);

   curr[0]='\0'; fgets(curr,mStr,rf);
   numbers_of_kadrs++;
 } // end while

 fclose(rf);
 fclose(ff);

 /* параметры вывода */

 return(numbers_of_kadrs);
}


/*=================================================================================*/
