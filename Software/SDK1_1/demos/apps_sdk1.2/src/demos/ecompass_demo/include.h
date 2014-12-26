// -----------------------------------------------------------------------
// Copyright (c) 2011, 2012, 2013 Freescale Semiconductor, Inc.
// ----------------------------------------------------------------------------
// FILENAME:     include.h
// CONTENTS:     COMPILE TIME CONSTANTS AND STRUCTURES
// DEPARTMENT:   ANALOG AND SENSORS DIVISION
// AUTHOR:       MARK PEDLEY
// ----------------------------------------------------------------------------
// VERSION             DATE           AUTHOR
// V1.0                20 FEB 2012    MARK PEDLEY
// V1.1                1 JUN 2012     MARK PEDLEY
// V1.2                15 JUN 2012    MARK PEDLEY
// V2.0                3 JUL 2012     MARK PEDLEY
// V3.0 (this version) 18 MAR 2013     MARK PEDLEY
// ----------------------------------------------------------------------------
// PURPOSE:
// Tilt-compensated e-compass with magnetic calibration with support
// for Aerospace (NED), Android and Windows 8 coordinate systems.
// Sensor simulation is used to verify code on target platform
// before interfacing to real sensors.
// This code does not include low level drivers for the accelerometer
// and magnetometer sensors since these are straightforward to write
// and specific to the processor and sensors being used.
//
// Most errors when moving to a real platform result from not aligning
// the individual sensor axes to the product axes. If you see roll, pitch
// or compass heading angle errors of -90, 90 or 180 degrees then
// re-check your sensor alignment.
// ----------------------------------------------------------------------------
// USER-CONFIGURABLE VALUES:
// FCOUNTSPERUT, FCOUNTSPERG, FUTPERCOUNT: Set to values for sensors selected
// ANGLE_LPF_FPU: Set to degree of low pass filtering required.
// DELTAT: Set to sampling interval (s) between runs of eCOMPASS
// CALINTERVAL: Calibration need only run intermittently
// ----------------------------------------------------------------------------
// V3.0 ENHANCEMENTS:
// - Numerous minor enhancements to improve accuracy
// - Significant reduction in RAM and CPU in the magnetic calibration solvers
// - Users of V2 code need not upgrade to V3 unless RAM and CPU are an issue.
// ----------------------------------------------------------------------------

#include "stdio.h"
//#include "conio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
//#include "time.h"

// typedefs for Microsoft Visual Studio
//typedef _int16 int16;
//typedef unsigned _int16 uint16;
//typedef _int32 int32;
//typedef unsigned _int32 uint32;

// typedefs for Freescale Codewarrior
typedef char int8;
typedef unsigned char uint8;
typedef short int int16;
typedef unsigned short int uint16;
typedef long int int32;
typedef unsigned long int uint32;

// sensor scaling constants
#define FCOUNTSPERUT 10.0F              // sensor specific: MAG3110 and FXOS8700 provide 10 counts / uT
#define FCOUNTSPERG 2048.0F             // sensor specific: MMA8451 and FXOS8700 provide 2048 counts / g in 4g mode
#define FUTPERCOUNT 0.1F                // MAG3110 and FXOS8700 provide 0.1uT per count resolution

// orientation LPF cutoff = Nyquist / ANGLE_LPF_FPU
// so about ANGLE_LPF_FPU samples response
// ANGLE_LPF_FPU equal to 1.0F is valid and corresponds to all pass.
#define ANGLE_LPF_FPU 1.0F
// sampling interval (secs)
#define DELTAT 0.02F

// coordinate system and eCompass option selected
#define NED 0                       // identifier for NED angle output
#define ANDROID 1                   // identifier for Android angle output
#define WIN8 2                      // identifier for Windows 8 angle output

// multiplicative conversion constants
#define PI 3.141592654F             // Pi
#define FDEGTORAD 0.01745329251994F // degrees to radians conversion = pi / 180
#define FRADTODEG 57.2957795130823F // radians to degrees conversion = 180 / pi
#define FRECIP180 0.0055555555555F  // multiplicative factor 1/180
#define FMATRIXSCALING 0.02F        // approx normalizes geomagnetic field 50uT
#define FINVMATRIXSCALING 50.0F     // inverse of FMATRIXSCALING

// magnetic calibration constants
#define MAGBUFFSIZE 6           // magnetic buffer size: 6 implies 6^3 = 216 entries
#define INITIALCALINTERVAL 50   // 1s at 50Hz: initial (limited measurements) calibration interval
#define FINALCALINTERVAL 1500   // 30s at 50Hz: normal (sufficient measurements) calibration interval
#define MINEQUATIONS 24         // minimum number of measurements used for calibration
#define MEDEQUATIONS 80         // new solution always accepted up to this point
#define MAXEQUATIONS 120        // maximum number of measurements used for calibration
#define MINBFIT 5.0F            // minimum acceptable geomagnetic field B (uT) for valid calibration
#define MAXBFIT 100.0F          // maximum acceptable geomagnetic field B (uT) for valid calibration
#define FITERRORAGING 0.0033333F// reciprocal of time (s) for fit error to increase by e=2.718 (so here 300s=5 mins)

// matrix inverse integer arrays
#define MAXMATINV 6             // maximum size supported in matrix inverse function

// file access buffer size
#define BUFFSIZE 2048

// quaternion structure definition
struct fquaternion
{
    float q0;   // scalar component
    float q1;   // x vector component
    float q2;   // y vector component
    float q3;   // z vector component
};

// simulation model structure definition
struct SimulationModel
{
    // hard iron offset
    float fVx;                      // simulated x component of hard iron
    float fVy;                      // simulated y component of hard iron
    float fVz;                      // simulated z component of hard iron
    // soft iron matrix
    float xinvW[3][3];              // simulated inverse soft iron matrix
    float *finvW[3];
    float xW[3][3];                 // simulated forward soft iron matrix
    float *fW[3];
    // geomagnetic field
    float fB;                       // simulated geomagnetic field strength (uT)
    float fDeltaDeg;                // simulated geomagnetic inclination angle (deg)
    // orientation Euler angles
    float fPhiDeg;                  // simulated roll angle (deg)
    float fTheDeg;                  // simulated pitch angle (deg)
    float fPsiDeg;                  // simulated yaw angle (deg)
    float fRhoDeg;                  // simulated compass angle (deg)
};

// accelerometer sensor structure definition
struct AccelSensor
{
    int16 iGpx;                        // accelerometer sensor output x (counts)
    int16 iGpy;                        // accelerometer sensor output y (counts)
    int16 iGpz;                        // accelerometer sensor output z (counts)
    float fGpx;                        // accelerometer sensor output x (g)
    float fGpy;                        // accelerometer sensor output y (g)
    float fGpz;                        // accelerometer sensor output z (g)
};

// magnetometer sensor structure definition
struct MagSensor
{
    int16 iBpx;                       // magnetometer sensor output x (counts)
    int16 iBpy;                       // magnetometer sensor output y (counts)
    int16 iBpz;                       // magnetometer sensor output z (counts)
    int16 iBcx;                       // calibrated magnetometer sensor output x (counts)
    int16 iBcy;                       // calibrated magnetometer sensor output y (counts)
    int16 iBcz;                       // calibrated magnetometer sensor output z (counts)
    int16 iBfx;                       // de-rotated magnetometer sensor output x (counts)
    int16 iBfy;                       // de-rotated magnetometer sensor output y (counts)
    int16 iBfz;                       // de-rotated magnetometer sensor output z (counts)
    float fBpx;                       // magnetometer sensor output x (uT)
    float fBpy;                       // magnetometer sensor output y (uT)
    float fBpz;                       // magnetometer sensor output z (uT)
    float fBcx;                       // calibrated magnetometer sensor output x (uT)
    float fBcy;                       // calibrated magnetometer sensor output y (uT)
    float fBcz;                       // calibrated magnetometer sensor output z (uT)
    float fBfx;                       // de-rotated magnetometer sensor output x (counts)
    float fBfy;                       // de-rotated magnetometer sensor output y (counts)
    float fBfz;                       // de-rotated magnetometer sensor output z (counts)
};

// magnetic calibration structure
struct MagCalibration
{
    float fVx;                        // x component of computed hard iron offset
    float fVy;                        // y component of computed hard iron offset
    float fVz;                        // z component of computed hard iron offset
    float fB;                         // computed geomagnetic field magnitude in uT
    float fFitErrorpc;                // computed fit error %
    float ftrVx;                      // trial value of x component of hard iron offset
    float ftrVy;                      // trial value of y component of hard iron offset
    float ftrVz;                      // trial value of z component of hard iron offset
    float ftrB;                       // trial value of geomagnetic field magnitude in uT
    float ftrFitErrorpc;              // trial value of fit error %
    int32 iValidMagCal;               // valid magnetic calibration flag
    float xfinvW[3][3];               // estimated inverse soft iron matrix size
    float *finvW[3];
    float xfA[3][3];                  // estimated ellipsoid matrix A
    float *fA[3];
    float xinvA[3][3];                // inverse of ellipsoid matrix A
    float *finvA[3];
    float xftrinvW[3][3]              ;// trial computed inverse soft iron matrix size
    float *ftrinvW[3];
};

// 6DOF orientation structure definition
struct SV6DOF
{
    // Euler angles
    float fPhi6DOF;                     // 6DOF roll (deg)
    float fThe6DOF;                     // 6DOF pitch (deg)
    float fPsi6DOF;                     // 6DOF yaw (deg)
    float fRho6DOF;                     // 6DOF compass (deg)
    float fLPPhi6DOF;                   // 6DOF low pass roll (deg)
    float fLPThe6DOF;                   // 6DOF low pass pitch (deg)
    float fLPPsi6DOF;                   // 6DOF low pass yaw (deg)
    float fLPRho6DOF;                   // 6DOF low pass compass (deg)
    float fDelta6DOFn;                  // 6DOF inclination angle (deg) sample n
    float fDelta6DOFnm1;                // 6DOF inclination angle (deg) sample n-1
    float fDelta6DOFnm2;                // 6DOF inclination angle (deg) sample n-2
    float fLPDelta6DOFn;                // 6DOF low pass inclination angle (deg) sample n
    float fLPDelta6DOFnm1;              // 6DOF low pass inclination angle (deg) sample n-1
    float fLPDelta6DOFnm2;              // 6DOF low pass inclination angle (deg) sample n-2
    // orientation matrices and quaternions
    float xfR6DOFn[3][3];               // 6DOF rotation matrix for current sample n
    float *fR6DOFn[3];
    float xfR6DOFnm1[3][3];// 6DOF rotation matrix for sample n-1
    float *fR6DOFnm1[3];
    float xfR6DOFnm2[3][3];             // 6DOF rotation matrix for sample n-2
    float *fR6DOFnm2[3];
    float xfLPR6DOFn[3][3];             // low pass 6DOF rotation matrix for sample n
    float *fLPR6DOFn[3];
    float xfLPR6DOFnm1[3][3];           // low pass 6DOF rotation matrix for sample n-1
    float *fLPR6DOFnm1[3];
    float xfLPR6DOFnm2[3][3];           // low pass 6DOF rotation matrix for sample n-2
    float *fLPR6DOFnm2[3];
    struct fquaternion fq6DOFn;         // current 6DOF orientation quaternion
    struct fquaternion fLPq6DOFn;       // low pass 6DOF orientation quaternion
};

// magnetometer measurement buffer
struct MagneticBuffer
{
    int16 iBx[MAGBUFFSIZE][MAGBUFFSIZE][MAGBUFFSIZE];   // array of x magnetic fields
    int16 iBy[MAGBUFFSIZE][MAGBUFFSIZE][MAGBUFFSIZE];   // array of y magnetic fields
    int16 iBz[MAGBUFFSIZE][MAGBUFFSIZE][MAGBUFFSIZE];   // array of z magnetic fields
    int32 index[MAGBUFFSIZE][MAGBUFFSIZE][MAGBUFFSIZE]; // array of time indices
    int32 iMagBufferCount;                              // number of magnetometer readings
};

// sensor driver function prototypes
uint8 fSixDOFSensorDrivers(struct AccelSensor *pthisAccel, struct MagSensor *pthisMag);
// sensor driver init function prototype
uint8 SensorDrivers_Init(void);
// floating point xtrinsic eCompass function prototypes
void feCompassDirectNED(struct SV6DOF *pthisSV6DOF, struct MagSensor *pthisMag,
    struct AccelSensor *pthisAccel);
void feCompassDirectAndroid(struct SV6DOF *pthisSV6DOF, struct MagSensor *pthisMag,
    struct AccelSensor *pthisAccel);
void feCompassDirectWin8(struct SV6DOF *pthisSV6DOF, struct MagSensor *pthisMag,
    struct AccelSensor *pthisAccel);
void fInitLPFOrientationMatrix(float *pfb0, float *pfa1, float *pfa2);
void fLPFOrientationMatrix(struct SV6DOF *pthisSV6DOF, int32 iCoordSystem, int32 loopcounter,
    float fb0, float fa1, float fa2);
// magnetometer buffer function prototypes
void fUpdateMagnetometerBuffer(struct MagneticBuffer *pthisMagneticBuffer,
    struct MagSensor *pthisMag, struct AccelSensor *pthisAccel, int32 loopcounter);
// magnetic calibration function prototypes
void fUpdateCalibration10EIG(struct MagCalibration *pthisMagCal, struct MagneticBuffer *pthisMagneticBuffer,
    float **ftmpA10x10, float **ftmpB10x10, float **ftmpA10x1, float **ftmpA3x3, float **ftmpA3x1);
void fUpdateCalibration7EIG(struct MagCalibration *pthisMagCal, struct MagneticBuffer *pthisMagneticBuffer,
    float **ftmpA7x7, float **ftmpB7x7, float **ftmpA7x1);
void fUpdateCalibration4INV(struct MagCalibration *pthisMagCal, struct MagneticBuffer *pthisMagneticBuffer,
    float **ftmpA4x4, float **ftmpB4x4, float **ftmpA4x1, float **ftmpB4x1,
    int32 **icolind, int32 **irowind, int32 **ipivot);
void ResetMagCalibration(struct MagCalibration *pthisMagCal, struct MagneticBuffer *pthisMagneticBuffer);
void fInvertMagCal(struct MagSensor *pthisMag, struct MagCalibration *pthisMagCal);
// matrix algebra function prototypes
void fmatrixAeqBxC(float **A, float **B, float **C, int32 rB, int32 cBrC, int32 cC);
void fmatrixAeqTrBxC(float **A, float **B, float **C, int32 rBrC, int32 cB, int32 cC);
void fmatrixAeqBxTrB(float **A, float **B, int32 rB, int32 cB);
void fmatrixAeqI(float **A, int32 rc);
void fmatrixPrintA(float **A, int32 r1, int32 r2, int32 c1, int32 c2);
void f3x3matrixAeqInvSymB(float **A, float **B);
void fmatrixAeqRootSymB(float **A, float **B, float **ftmpmat, float **ftmpvec, int32 rc);
void fmatrixAeqB(float **A, float **B, int32 r, int32 c);
void fmatrixAeqAxScalar(float **A, float Scalar, int32 r, int32 c);
void fmatrixAeqMinusA(float **A, int32 r, int32 c);
void fmatrixAeqInvA(float **A, int32 isize, int32 **icolind, int32 **irowind, int32 **ipivot);
float f3x3matrixDetA(float **inp);
void eigencompute(float **mat, int32 n, float **eigval, float **eigvec);
void fmatrixAeqRenormRotA(float **A);
// conversion functions
void fQuaternionFromRotationMatrix(float **R, struct fquaternion *pq);
void fNEDAnglesDegFromRotationMatrix(float **R, float *pfPhiDeg, float *pfTheDeg, float *pfPsiDeg, float *pfRhoDeg);
void fAndroidAnglesDegFromRotationMatrix(float **R, float *pfPhiDeg, float *pfTheDeg, float *pfPsiDeg, float *pfRhoDeg);
void fWin8AnglesDegFromRotationMatrix(float **R, float *pfPhiDeg, float *pfTheDeg, float *pfPsiDeg, float *pfRhoDeg);
