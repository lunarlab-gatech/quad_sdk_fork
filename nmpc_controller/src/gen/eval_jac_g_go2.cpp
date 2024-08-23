/* This file was automatically generated by CasADi 3.6.6.
 *  It consists of: 
 *   1) content generated by CasADi runtime: not copyrighted
 *   2) template code copied from CasADi source: permissively licensed (MIT-0)
 *   3) user code: owned by the user
 *
 */
/* How to prefix internal symbols */
#ifdef CASADI_CODEGEN_PREFIX
  #define CASADI_NAMESPACE_CONCAT(NS, ID) _CASADI_NAMESPACE_CONCAT(NS, ID)
  #define _CASADI_NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) CASADI_NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else
  #define CASADI_PREFIX(ID) eval_jac_g_go2_ ## ID
#endif

#include <math.h>

#ifndef casadi_real
#define casadi_real double
#endif

#ifndef casadi_int
#define casadi_int long long int
#endif

/* Add prefix to internal symbols */
#define casadi_f0 CASADI_PREFIX(f0)
#define casadi_s0 CASADI_PREFIX(s0)
#define casadi_s1 CASADI_PREFIX(s1)
#define casadi_s2 CASADI_PREFIX(s2)

/* Symbol visibility in DLLs */
#ifndef CASADI_SYMBOL_EXPORT
  #if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #if defined(STATIC_LINKED)
      #define CASADI_SYMBOL_EXPORT
    #else
      #define CASADI_SYMBOL_EXPORT __declspec(dllexport)
    #endif
  #elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
    #define CASADI_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define CASADI_SYMBOL_EXPORT
  #endif
#endif

static const casadi_int casadi_s0[40] = {36, 1, 0, 36, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
static const casadi_int casadi_s1[18] = {14, 1, 0, 14, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
static const casadi_int casadi_s2[168] = {28, 36, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 15, 21, 27, 34, 40, 46, 53, 59, 65, 72, 78, 84, 91, 92, 93, 94, 99, 104, 107, 109, 111, 113, 117, 123, 129, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 10, 11, 10, 11, 6, 9, 10, 11, 12, 13, 7, 9, 10, 11, 14, 15, 8, 9, 10, 12, 13, 14, 15, 6, 9, 10, 11, 16, 17, 7, 9, 10, 11, 18, 19, 8, 9, 10, 16, 17, 18, 19, 6, 9, 10, 11, 20, 21, 7, 9, 10, 11, 22, 23, 8, 9, 10, 20, 21, 22, 23, 6, 9, 10, 11, 24, 25, 7, 9, 10, 11, 26, 27, 8, 9, 10, 24, 25, 26, 27, 0, 1, 2, 3, 4, 5, 10, 11, 3, 4, 5, 9, 11, 5, 9, 10, 0, 6, 1, 7, 2, 8, 3, 9, 10, 11, 3, 4, 5, 9, 10, 11, 3, 4, 5, 9, 10, 11};

/* eval_jac_g_go2:(w[36],p[14])->(jac_g[28x36,129nz]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem) {
  casadi_real a0, a1, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a2, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a3, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a4, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a5, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a6, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a7, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a8, a80, a81, a9;
  a0=-1.;
  if (res[0]!=0) res[0][0]=a0;
  if (res[0]!=0) res[0][1]=a0;
  if (res[0]!=0) res[0][2]=a0;
  if (res[0]!=0) res[0][3]=a0;
  if (res[0]!=0) res[0][4]=a0;
  if (res[0]!=0) res[0][5]=a0;
  a1=-1.3300000000000001e+01;
  if (res[0]!=0) res[0][6]=a1;
  if (res[0]!=0) res[0][7]=a1;
  if (res[0]!=0) res[0][8]=a1;
  a1=-1.2102232626086960e-01;
  if (res[0]!=0) res[0][9]=a1;
  a2=arg[0]? arg[0][28] : 0;
  a3=sin(a2);
  a4=(a1*a3);
  a5=(-a4);
  if (res[0]!=0) res[0][10]=a5;
  a5=4.3125046780173909e-01;
  a6=arg[0]? arg[0][27] : 0;
  a7=cos(a6);
  a8=(a5*a7);
  a9=(-a8);
  if (res[0]!=0) res[0][11]=a9;
  a9=cos(a2);
  a10=sin(a6);
  a11=(a9*a10);
  a11=(a5*a11);
  a12=(-a11);
  if (res[0]!=0) res[0][12]=a12;
  a12=-4.8802931580173908e-01;
  a13=(a12*a10);
  a14=(-a13);
  if (res[0]!=0) res[0][13]=a14;
  a14=4.8802931580173908e-01;
  a15=(a7*a9);
  a15=(a14*a15);
  a16=(-a15);
  if (res[0]!=0) res[0][14]=a16;
  a16=arg[1]? arg[1][0] : 0;
  a17=(-a16);
  if (res[0]!=0) res[0][15]=a17;
  a18=arg[1]? arg[1][3] : 0;
  a19=(a18*a3);
  a20=arg[1]? arg[1][4] : 0;
  a21=(a20*a9);
  a22=arg[0]? arg[0][29] : 0;
  a23=sin(a22);
  a24=(a21*a23);
  a19=(a19+a24);
  a19=(a19*a16);
  if (res[0]!=0) res[0][16]=a19;
  a19=cos(a22);
  a24=(a20*a19);
  a24=(a24*a16);
  if (res[0]!=0) res[0][17]=a24;
  a24=(a18*a16);
  a24=(-a24);
  if (res[0]!=0) res[0][18]=a24;
  a24=1.;
  if (res[0]!=0) res[0][19]=a24;
  if (res[0]!=0) res[0][20]=a0;
  a25=(-a16);
  if (res[0]!=0) res[0][21]=a25;
  a26=arg[1]? arg[1][2] : 0;
  a27=(a26*a3);
  a28=(a20*a9);
  a29=(a28*a19);
  a27=(a27+a29);
  a27=(a27*a16);
  a27=(-a27);
  if (res[0]!=0) res[0][22]=a27;
  a27=(a20*a23);
  a27=(a27*a16);
  if (res[0]!=0) res[0][23]=a27;
  a27=(a26*a16);
  if (res[0]!=0) res[0][24]=a27;
  if (res[0]!=0) res[0][25]=a24;
  if (res[0]!=0) res[0][26]=a0;
  a27=(-a16);
  if (res[0]!=0) res[0][27]=a27;
  a29=(a18*a19);
  a30=(a26*a23);
  a29=(a29-a30);
  a30=(a9*a29);
  a30=(a30*a16);
  if (res[0]!=0) res[0][28]=a30;
  a30=(a26*a19);
  a31=(a18*a23);
  a30=(a30+a31);
  a30=(a30*a16);
  a30=(-a30);
  if (res[0]!=0) res[0][29]=a30;
  a30=arg[1]? arg[1][1] : 0;
  a31=(-a30);
  if (res[0]!=0) res[0][30]=a31;
  a31=(-a30);
  if (res[0]!=0) res[0][31]=a31;
  a31=(-a30);
  if (res[0]!=0) res[0][32]=a31;
  a31=(-a30);
  if (res[0]!=0) res[0][33]=a31;
  if (res[0]!=0) res[0][34]=a17;
  a31=arg[1]? arg[1][6] : 0;
  a32=(a31*a3);
  a33=arg[1]? arg[1][7] : 0;
  a34=(a33*a9);
  a35=(a34*a23);
  a32=(a32+a35);
  a32=(a32*a16);
  if (res[0]!=0) res[0][35]=a32;
  a32=(a33*a19);
  a32=(a32*a16);
  if (res[0]!=0) res[0][36]=a32;
  a32=(a31*a16);
  a32=(-a32);
  if (res[0]!=0) res[0][37]=a32;
  if (res[0]!=0) res[0][38]=a24;
  if (res[0]!=0) res[0][39]=a0;
  if (res[0]!=0) res[0][40]=a25;
  a32=arg[1]? arg[1][5] : 0;
  a35=(a32*a3);
  a36=(a33*a9);
  a37=(a36*a19);
  a35=(a35+a37);
  a35=(a35*a16);
  a35=(-a35);
  if (res[0]!=0) res[0][41]=a35;
  a35=(a33*a23);
  a35=(a35*a16);
  if (res[0]!=0) res[0][42]=a35;
  a35=(a32*a16);
  if (res[0]!=0) res[0][43]=a35;
  if (res[0]!=0) res[0][44]=a24;
  if (res[0]!=0) res[0][45]=a0;
  if (res[0]!=0) res[0][46]=a27;
  a35=(a31*a19);
  a37=(a32*a23);
  a35=(a35-a37);
  a37=(a9*a35);
  a37=(a37*a16);
  if (res[0]!=0) res[0][47]=a37;
  a37=(a32*a19);
  a38=(a31*a23);
  a37=(a37+a38);
  a37=(a37*a16);
  a37=(-a37);
  if (res[0]!=0) res[0][48]=a37;
  a37=(-a30);
  if (res[0]!=0) res[0][49]=a37;
  a37=(-a30);
  if (res[0]!=0) res[0][50]=a37;
  a37=(-a30);
  if (res[0]!=0) res[0][51]=a37;
  a37=(-a30);
  if (res[0]!=0) res[0][52]=a37;
  if (res[0]!=0) res[0][53]=a17;
  a37=arg[1]? arg[1][9] : 0;
  a38=(a37*a3);
  a39=arg[1]? arg[1][10] : 0;
  a40=(a39*a9);
  a41=(a40*a23);
  a38=(a38+a41);
  a38=(a38*a16);
  if (res[0]!=0) res[0][54]=a38;
  a38=(a39*a19);
  a38=(a38*a16);
  if (res[0]!=0) res[0][55]=a38;
  a38=(a37*a16);
  a38=(-a38);
  if (res[0]!=0) res[0][56]=a38;
  if (res[0]!=0) res[0][57]=a24;
  if (res[0]!=0) res[0][58]=a0;
  if (res[0]!=0) res[0][59]=a25;
  a38=arg[1]? arg[1][8] : 0;
  a41=(a38*a3);
  a42=(a39*a9);
  a43=(a42*a19);
  a41=(a41+a43);
  a41=(a41*a16);
  a41=(-a41);
  if (res[0]!=0) res[0][60]=a41;
  a41=(a39*a23);
  a41=(a41*a16);
  if (res[0]!=0) res[0][61]=a41;
  a41=(a38*a16);
  if (res[0]!=0) res[0][62]=a41;
  if (res[0]!=0) res[0][63]=a24;
  if (res[0]!=0) res[0][64]=a0;
  if (res[0]!=0) res[0][65]=a27;
  a41=(a37*a19);
  a43=(a38*a23);
  a41=(a41-a43);
  a43=(a9*a41);
  a43=(a43*a16);
  if (res[0]!=0) res[0][66]=a43;
  a43=(a38*a19);
  a44=(a37*a23);
  a43=(a43+a44);
  a43=(a43*a16);
  a43=(-a43);
  if (res[0]!=0) res[0][67]=a43;
  a43=(-a30);
  if (res[0]!=0) res[0][68]=a43;
  a43=(-a30);
  if (res[0]!=0) res[0][69]=a43;
  a43=(-a30);
  if (res[0]!=0) res[0][70]=a43;
  a43=(-a30);
  if (res[0]!=0) res[0][71]=a43;
  if (res[0]!=0) res[0][72]=a17;
  a17=arg[1]? arg[1][12] : 0;
  a43=(a17*a3);
  a44=arg[1]? arg[1][13] : 0;
  a45=(a44*a9);
  a46=(a45*a23);
  a43=(a43+a46);
  a43=(a43*a16);
  if (res[0]!=0) res[0][73]=a43;
  a43=(a44*a19);
  a43=(a43*a16);
  if (res[0]!=0) res[0][74]=a43;
  a43=(a17*a16);
  a43=(-a43);
  if (res[0]!=0) res[0][75]=a43;
  if (res[0]!=0) res[0][76]=a24;
  if (res[0]!=0) res[0][77]=a0;
  if (res[0]!=0) res[0][78]=a25;
  a25=arg[1]? arg[1][11] : 0;
  a43=(a25*a3);
  a46=(a44*a9);
  a47=(a46*a19);
  a43=(a43+a47);
  a43=(a43*a16);
  a43=(-a43);
  if (res[0]!=0) res[0][79]=a43;
  a43=(a44*a23);
  a43=(a43*a16);
  if (res[0]!=0) res[0][80]=a43;
  a43=(a25*a16);
  if (res[0]!=0) res[0][81]=a43;
  if (res[0]!=0) res[0][82]=a24;
  if (res[0]!=0) res[0][83]=a0;
  if (res[0]!=0) res[0][84]=a27;
  a27=(a17*a19);
  a0=(a25*a23);
  a27=(a27-a0);
  a0=(a9*a27);
  a0=(a0*a16);
  if (res[0]!=0) res[0][85]=a0;
  a0=(a25*a19);
  a43=(a17*a23);
  a0=(a0+a43);
  a0=(a0*a16);
  a0=(-a0);
  if (res[0]!=0) res[0][86]=a0;
  a0=(-a30);
  if (res[0]!=0) res[0][87]=a0;
  a0=(-a30);
  if (res[0]!=0) res[0][88]=a0;
  a0=(-a30);
  if (res[0]!=0) res[0][89]=a0;
  a30=(-a30);
  if (res[0]!=0) res[0][90]=a30;
  if (res[0]!=0) res[0][91]=a24;
  if (res[0]!=0) res[0][92]=a24;
  if (res[0]!=0) res[0][93]=a24;
  a30=sin(a6);
  a0=arg[0]? arg[0][35] : 0;
  a43=(a16/a9);
  a47=(a3*a43);
  a48=(a0*a47);
  a48=(a30*a48);
  a6=cos(a6);
  a49=arg[0]? arg[0][34] : 0;
  a50=(a3*a43);
  a51=(a49*a50);
  a51=(a6*a51);
  a48=(a48-a51);
  a48=(a48+a24);
  if (res[0]!=0) res[0][94]=a48;
  a48=(a0*a16);
  a48=(a6*a48);
  a51=(a49*a16);
  a51=(a30*a51);
  a48=(a48+a51);
  if (res[0]!=0) res[0][95]=a48;
  a48=(a16/a9);
  a51=(a0*a48);
  a51=(a30*a51);
  a52=(a49*a48);
  a52=(a6*a52);
  a51=(a51-a52);
  if (res[0]!=0) res[0][96]=a51;
  a51=22354293482292820.;
  a52=arg[0]? arg[0][33] : 0;
  a53=-1.3877787807814460e-17;
  a53=(a53*a16);
  a54=(a52*a53);
  a55=(a51*a54);
  a56=(a49*a55);
  a57=arg[0]? arg[0][11] : 0;
  a57=(a0-a57);
  a12=(a12*a57);
  a56=(a56+a12);
  a56=(a6*a56);
  a12=26445640661418040.;
  a54=(a12*a54);
  a58=(a0*a54);
  a59=arg[0]? arg[0][10] : 0;
  a59=(a49-a59);
  a60=(a5*a59);
  a58=(a58+a60);
  a58=(a30*a58);
  a56=(a56-a58);
  if (res[0]!=0) res[0][97]=a56;
  a5=(a5*a59);
  a59=(a9*a5);
  a56=(a52*a0);
  a58=(a56*a9);
  a60=3.6700698954086958e-01;
  a60=(a60*a16);
  a58=(a58*a60);
  a59=(a59-a58);
  a6=(a6*a59);
  a59=(a52*a49);
  a58=3.1022814154086947e-01;
  a58=(a58*a16);
  a61=(a9*a58);
  a62=(a59*a61);
  a14=(a14*a57);
  a57=(a9*a14);
  a62=(a62+a57);
  a30=(a30*a62);
  a6=(a6-a30);
  if (res[0]!=0) res[0][98]=a6;
  a6=cos(a2);
  a30=(a49*a10);
  a62=(a30*a43);
  a57=(a0*a7);
  a63=(a57*a43);
  a62=(a62+a63);
  a62=(a6*a62);
  a2=sin(a2);
  a63=(a52*a9);
  a57=(a57*a3);
  a63=(a63+a57);
  a30=(a30*a3);
  a63=(a63+a30);
  a63=(a63/a9);
  a63=(a63/a9);
  a63=(a63*a16);
  a43=(a52*a43);
  a63=(a63-a43);
  a63=(a2*a63);
  a62=(a62+a63);
  a62=(-a62);
  if (res[0]!=0) res[0][99]=a62;
  if (res[0]!=0) res[0][100]=a24;
  a62=(a0*a7);
  a63=(a49*a10);
  a62=(a62+a63);
  a62=(a62/a9);
  a62=(a62/a9);
  a62=(a62*a16);
  a62=(a2*a62);
  a62=(-a62);
  if (res[0]!=0) res[0][101]=a62;
  a62=arg[0]? arg[0][21] : 0;
  a63=(a62*a16);
  a43=(a17*a63);
  a30=arg[0]? arg[0][22] : 0;
  a57=(a30*a16);
  a64=(a25*a57);
  a43=(a43-a64);
  a64=arg[0]? arg[0][19] : 0;
  a65=(a64*a16);
  a66=(a38*a65);
  a43=(a43-a66);
  a66=arg[0]? arg[0][18] : 0;
  a67=(a66*a16);
  a68=(a37*a67);
  a43=(a43+a68);
  a68=arg[0]? arg[0][16] : 0;
  a69=(a68*a16);
  a70=(a32*a69);
  a43=(a43-a70);
  a70=arg[0]? arg[0][15] : 0;
  a71=(a70*a16);
  a72=(a31*a71);
  a43=(a43+a72);
  a72=arg[0]? arg[0][12] : 0;
  a73=(a72*a16);
  a74=(a18*a73);
  a43=(a43+a74);
  a74=arg[0]? arg[0][13] : 0;
  a75=(a74*a16);
  a76=(a26*a75);
  a43=(a43-a76);
  a43=(a6*a43);
  a76=arg[0]? arg[0][23] : 0;
  a77=(a76*a16);
  a27=(a27*a77);
  a78=(a19*a57);
  a78=(a44*a78);
  a27=(a27-a78);
  a78=(a23*a63);
  a78=(a44*a78);
  a27=(a27+a78);
  a78=arg[0]? arg[0][20] : 0;
  a79=(a78*a16);
  a41=(a41*a79);
  a27=(a27+a41);
  a41=(a19*a65);
  a41=(a39*a41);
  a27=(a27-a41);
  a41=(a23*a67);
  a41=(a39*a41);
  a27=(a27+a41);
  a41=arg[0]? arg[0][17] : 0;
  a80=(a41*a16);
  a35=(a35*a80);
  a27=(a27+a35);
  a35=(a19*a69);
  a35=(a33*a35);
  a27=(a27-a35);
  a35=(a23*a71);
  a35=(a33*a35);
  a27=(a27+a35);
  a35=arg[0]? arg[0][14] : 0;
  a81=(a35*a16);
  a29=(a29*a81);
  a27=(a27+a29);
  a23=(a23*a73);
  a23=(a20*a23);
  a27=(a27+a23);
  a19=(a19*a75);
  a19=(a20*a19);
  a27=(a27-a19);
  a27=(a2*a27);
  a43=(a43-a27);
  if (res[0]!=0) res[0][102]=a43;
  a43=(a49*a0);
  a27=-5.6778848000000048e-02;
  a27=(a27*a16);
  a43=(a43*a27);
  a19=arg[0]? arg[0][9] : 0;
  a19=(a52-a19);
  a1=(a1*a19);
  a43=(a43+a1);
  a6=(a6*a43);
  a59=(a59*a7);
  a59=(a59*a58);
  a60=(a10*a60);
  a56=(a56*a60);
  a59=(a59-a56);
  a14=(a7*a14);
  a59=(a59+a14);
  a5=(a10*a5);
  a59=(a59+a5);
  a2=(a2*a59);
  a6=(a6-a2);
  if (res[0]!=0) res[0][103]=a6;
  if (res[0]!=0) res[0][104]=a24;
  a24=cos(a22);
  a45=(a45*a63);
  a77=(a9*a77);
  a63=(a25*a77);
  a45=(a45-a63);
  a79=(a9*a79);
  a63=(a38*a79);
  a45=(a45-a63);
  a40=(a40*a67);
  a45=(a45+a40);
  a80=(a9*a80);
  a40=(a32*a80);
  a45=(a45-a40);
  a34=(a34*a71);
  a45=(a45+a34);
  a81=(a9*a81);
  a34=(a26*a81);
  a45=(a45-a34);
  a21=(a21*a73);
  a45=(a45+a21);
  a45=(a24*a45);
  a22=sin(a22);
  a77=(a17*a77);
  a46=(a46*a57);
  a77=(a77-a46);
  a79=(a37*a79);
  a77=(a77+a79);
  a42=(a42*a65);
  a77=(a77-a42);
  a80=(a31*a80);
  a77=(a77+a80);
  a36=(a36*a69);
  a77=(a77-a36);
  a81=(a18*a81);
  a77=(a77+a81);
  a28=(a28*a75);
  a77=(a77-a28);
  a77=(a22*a77);
  a45=(a45-a77);
  if (res[0]!=0) res[0][105]=a45;
  a30=(a30*a16);
  a30=(a44*a30);
  a76=(a76*a16);
  a17=(a17*a76);
  a30=(a30-a17);
  a78=(a78*a16);
  a37=(a37*a78);
  a30=(a30-a37);
  a64=(a64*a16);
  a64=(a39*a64);
  a30=(a30+a64);
  a41=(a41*a16);
  a31=(a31*a41);
  a30=(a30-a31);
  a68=(a68*a16);
  a68=(a33*a68);
  a30=(a30+a68);
  a74=(a74*a16);
  a74=(a20*a74);
  a30=(a30+a74);
  a35=(a35*a16);
  a18=(a18*a35);
  a30=(a30-a18);
  a24=(a24*a30);
  a62=(a62*a16);
  a44=(a44*a62);
  a25=(a25*a76);
  a44=(a44-a25);
  a38=(a38*a78);
  a44=(a44-a38);
  a66=(a66*a16);
  a39=(a39*a66);
  a44=(a44+a39);
  a32=(a32*a41);
  a44=(a44-a32);
  a70=(a70*a16);
  a33=(a33*a70);
  a44=(a44+a33);
  a72=(a72*a16);
  a20=(a20*a72);
  a44=(a44+a20);
  a26=(a26*a35);
  a44=(a44-a26);
  a22=(a22*a44);
  a24=(a24-a22);
  if (res[0]!=0) res[0][106]=a24;
  a24=(-a16);
  if (res[0]!=0) res[0][107]=a24;
  a24=1.3300000000000001e+01;
  if (res[0]!=0) res[0][108]=a24;
  a22=(-a16);
  if (res[0]!=0) res[0][109]=a22;
  if (res[0]!=0) res[0][110]=a24;
  a22=(-a16);
  if (res[0]!=0) res[0][111]=a22;
  if (res[0]!=0) res[0][112]=a24;
  a24=(-a16);
  if (res[0]!=0) res[0][113]=a24;
  a24=1.2102232626086960e-01;
  if (res[0]!=0) res[0][114]=a24;
  a24=(a0*a7);
  a12=(a12*a24);
  a24=(a49*a10);
  a51=(a51*a24);
  a12=(a12+a51);
  a12=(a12*a53);
  if (res[0]!=0) res[0][115]=a12;
  a61=(a7*a61);
  a12=(a49*a61);
  a9=(a9*a60);
  a60=(a0*a9);
  a12=(a12-a60);
  a12=(a12+a4);
  if (res[0]!=0) res[0][116]=a12;
  a50=(a10*a50);
  a50=(-a50);
  if (res[0]!=0) res[0][117]=a50;
  a50=(a7*a16);
  a50=(-a50);
  if (res[0]!=0) res[0][118]=a50;
  a50=(a10*a48);
  a50=(-a50);
  if (res[0]!=0) res[0][119]=a50;
  a50=5.6778848000000048e-02;
  a50=(a50*a16);
  a12=(a0*a50);
  if (res[0]!=0) res[0][120]=a12;
  a55=(a10*a55);
  a55=(a55+a8);
  if (res[0]!=0) res[0][121]=a55;
  a61=(a52*a61);
  a3=(a3*a27);
  a0=(a0*a3);
  a61=(a61+a0);
  a61=(a61+a11);
  if (res[0]!=0) res[0][122]=a61;
  a47=(a7*a47);
  a47=(-a47);
  if (res[0]!=0) res[0][123]=a47;
  a10=(a10*a16);
  if (res[0]!=0) res[0][124]=a10;
  a48=(a7*a48);
  a48=(-a48);
  if (res[0]!=0) res[0][125]=a48;
  a50=(a49*a50);
  if (res[0]!=0) res[0][126]=a50;
  a7=(a7*a54);
  a7=(a7+a13);
  if (res[0]!=0) res[0][127]=a7;
  a49=(a49*a3);
  a52=(a52*a9);
  a49=(a49-a52);
  a49=(a49+a15);
  if (res[0]!=0) res[0][128]=a49;
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_go2(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem){
  return casadi_f0(arg, res, iw, w, mem);
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_go2_alloc_mem(void) {
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_go2_init_mem(int mem) {
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_go2_free_mem(int mem) {
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_go2_checkout(void) {
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_go2_release(int mem) {
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_go2_incref(void) {
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_go2_decref(void) {
}

extern "C" CASADI_SYMBOL_EXPORT casadi_int eval_jac_g_go2_n_in(void) { return 2;}

extern "C" CASADI_SYMBOL_EXPORT casadi_int eval_jac_g_go2_n_out(void) { return 1;}

extern "C" CASADI_SYMBOL_EXPORT casadi_real eval_jac_g_go2_default_in(casadi_int i) {
  switch (i) {
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const char* eval_jac_g_go2_name_in(casadi_int i) {
  switch (i) {
    case 0: return "w";
    case 1: return "p";
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const char* eval_jac_g_go2_name_out(casadi_int i) {
  switch (i) {
    case 0: return "jac_g";
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const casadi_int* eval_jac_g_go2_sparsity_in(casadi_int i) {
  switch (i) {
    case 0: return casadi_s0;
    case 1: return casadi_s1;
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const casadi_int* eval_jac_g_go2_sparsity_out(casadi_int i) {
  switch (i) {
    case 0: return casadi_s2;
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_go2_work(casadi_int *sz_arg, casadi_int* sz_res, casadi_int *sz_iw, casadi_int *sz_w) {
  if (sz_arg) *sz_arg = 2;
  if (sz_res) *sz_res = 1;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 0;
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_go2_work_bytes(casadi_int *sz_arg, casadi_int* sz_res, casadi_int *sz_iw, casadi_int *sz_w) {
  if (sz_arg) *sz_arg = 2*sizeof(const casadi_real*);
  if (sz_res) *sz_res = 1*sizeof(casadi_real*);
  if (sz_iw) *sz_iw = 0*sizeof(casadi_int);
  if (sz_w) *sz_w = 0*sizeof(casadi_real);
  return 0;
}


