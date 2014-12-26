// -----------------------------------------------------------------------
// Copyright (c) 2011, 2012, 2013 Freescale Semiconductor, Inc.
// ----------------------------------------------------------------------------
// FILENAME:     main.c
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
//
// This main() is written as a loop for verification of the algorithm
// on a target processor with real sensor data input.
// ----------------------------------------------------------------------------
// V3.0 ENHANCEMENTS:
// - Numerous minor enhancements to improve accuracy
// - Significant reduction in RAM and CPU in the magnetic calibration solvers
// - Users of V2 code need not upgrade to V3 unless RAM and CPU are an issue.
// ----------------------------------------------------------------------------

#include "include.h"
#include "board.h"
#include "fsl_hwtimer.h"
#include "fsl_os_abstraction.h"


volatile uint16 FSL_Sample_Event_Flag;

hwtimer_t hwtimer;
extern const hwtimer_devif_t kSystickDevif;
extern const hwtimer_devif_t kPitDevif;

#define HWTIMER_LL_DEVIF    kSystickDevif
#define HWTIMER_LL_ID       0
#define HWTIMER_LL_SRCCLK   kCoreClock
#define HWTIMER_PERIOD          100000

static void hwtimer1_callback(void *p)
{
   FSL_Sample_Event_Flag = 1;
}


int main(void)
{
    // *********************************************************************************
    // Local variables
    // *********************************************************************************

    OSA_Init();
    hardware_init();
    dbg_uart_init();

    // sensor fusion state vectors and magnetometer data buffer
    struct AccelSensor thisAccel;               // accelerometer sensor
    struct MagSensor thisMag;                   // magnetometer sensor
    struct SV6DOF thisSV6DOF;                   // 6DOF state vector
    struct MagCalibration thisMagCal;           // hard and soft iron magnetic calibration
    struct MagneticBuffer thisMagneticBuffer;   // magnetometer measurement buffer

    // general purpose arrays
    float xftmpA10x10[10][10], *ftmpA10x10[10];             // scratch 10x10 matrix
    float xftmpB10x10[10][10], *ftmpB10x10[10];             // scratch 10x10 matrix
    float xftmpA10x1[10][1], *ftmpA10x1[10];                // scratch 10x1 matrix
    float xftmpA7x7[7][7], *ftmpA7x7[7];                    // scratch 7x7 matrix
    float xftmpB7x7[7][7], *ftmpB7x7[7];                    // scratch 7x7 matrix
    float xftmpA7x1[7][1], *ftmpA7x1[7];                    // scratch 7x1 matrix
    float xftmpA4x4[4][4], *ftmpA4x4[4];                    // scratch 4x4 matrix
    float xftmpB4x4[4][4], *ftmpB4x4[4];                    // scratch 4x4 matrix
    float xftmpA3x3[3][3], *ftmpA3x3[3];                    // scratch 3x3 matrix
    float xftmpA4x1[4][1], *ftmpA4x1[4];                    // scratch 4x1 matrix
    float xftmpB4x1[4][1], *ftmpB4x1[4];                    // scratch 4x1 matrix
    float xftmpA3x1[3][1], *ftmpA3x1[3];                    // scratch 3x1 vector

    // working index arrays for matrix inverse function
    int32 xicolind[MAXMATINV][1], *icolind[MAXMATINV];
    int32 xirowind[MAXMATINV][1], *irowind[MAXMATINV];
    int32 xipivot[MAXMATINV][1], *ipivot[MAXMATINV];

    // low pass filter parameters
    float fb0, fa1, fa2;

    // other local variables
    int32 i, j;                         // loop counters
    int32 loopcounter;                  // global counter incrementing each iteration of compass

    // variables which are not needed for a given real-time implementation where these options are fixed
    // and the irrelevant code is deleted
    int32 iCoordSystem;                 // angle coordinate system: NED, ANDROID or WIN8
    int32 iSolutionSize;                // calibration model size: 4, 7 or 10 elements

    // initialize the pointers to arrays as workaround to C function limitations with variable size arrays
    // keep in main since C functions require compile-time knowledge of the number of matrix rows
    // 3 row arrays
    for (i = 0; i < 3; i++)
    {
        thisMagCal.finvW[i] = thisMagCal.xfinvW[i];
        thisMagCal.ftrinvW[i] = thisMagCal.xftrinvW[i];
        thisMagCal.fA[i] = thisMagCal.xfA[i];
        thisMagCal.finvA[i] = thisMagCal.xinvA[i];
        thisSV6DOF.fR6DOFn[i] = thisSV6DOF.xfR6DOFn[i];
        thisSV6DOF.fR6DOFnm1[i] = thisSV6DOF.xfR6DOFnm1[i];
        thisSV6DOF.fR6DOFnm2[i] = thisSV6DOF.xfR6DOFnm2[i];
        thisSV6DOF.fLPR6DOFn[i] = thisSV6DOF.xfLPR6DOFn[i];
        thisSV6DOF.fLPR6DOFnm1[i] = thisSV6DOF.xfLPR6DOFnm1[i];
        thisSV6DOF.fLPR6DOFnm2[i] = thisSV6DOF.xfLPR6DOFnm2[i];
        ftmpA3x3[i] = xftmpA3x3[i];
        ftmpA3x1[i] = xftmpA3x1[i];
    }
    // 4 row arrays
    for (i = 0; i < 4; i++)
    {
        ftmpA4x4[i] = xftmpA4x4[i];
        ftmpB4x4[i] = xftmpB4x4[i];
        ftmpA4x1[i] = xftmpA4x1[i];
        ftmpB4x1[i] = xftmpB4x1[i];
    }

    // 7 row arrays
    for (i = 0; i < 7; i++)
    {
        ftmpA7x7[i] = xftmpA7x7[i];
        ftmpB7x7[i] = xftmpB7x7[i];
        ftmpA7x1[i] = xftmpA7x1[i];
    }
    // 10 row arrays
    for (i = 0; i < 10; i++)
    {
        ftmpA10x10[i] = xftmpA10x10[i];
        ftmpB10x10[i] = xftmpB10x10[i];
        ftmpA10x1[i] = xftmpA10x1[i];
    }
    // MAXMATINV row arrays
    for (i = 0; i < MAXMATINV; i++)
    {
        icolind[i] = xicolind[i];
        irowind[i] = xirowind[i];
        ipivot[i] = xipivot[i];
    }

    printf("Initialization of hwtimer1   :");
    if (kHwtimerSuccess != HWTIMER_SYS_Init(&hwtimer, &HWTIMER_LL_DEVIF, HWTIMER_LL_ID, 5, NULL))
    {
        printf("\r\nError: hwtimer initialization.\r\n");
    }
    if (kHwtimerSuccess != HWTIMER_SYS_SetPeriod(&hwtimer, HWTIMER_LL_SRCCLK, HWTIMER_PERIOD))
    {
        printf("\r\nError: hwtimer set period.\r\n");
    }
	if (kHwtimerSuccess != HWTIMER_SYS_RegisterCallback(&hwtimer, hwtimer1_callback, NULL))
    {
        printf("\r\nError: hwtimer callback registration.\r\n");
    }
    if (kHwtimerSuccess != HWTIMER_SYS_Start(&hwtimer))
    {
        printf("\r\nError: hwtimer start.\r\n");
    }

    // reset computed magnetic calibration and magnetometer data buffer
    ResetMagCalibration(&thisMagCal, &thisMagneticBuffer);

    SensorDrivers_Init();

    // control loop terminating with option 99
    printf("\r\nFreescale Magnetic Calibration and eCompass Software v3.00");

    // simple command interpreter to drive the most important algorithms
    printf("\r\n0: Display calibration parameters");
    printf("\r\n1: Run eCompass with 10, 7 or 4 element calibration");

    // display the calibration (calculated) hard iron vector
    printf("\r\nCalibration hard iron (uT) Vx=%9.3f Vy=%9.3f Vz=%9.3f",
        thisMagCal.fVx, thisMagCal.fVy, thisMagCal.fVz);

    // display the computed calibration inverse soft iron matrix
    printf("\r\n\nCalibration inverse soft iron matrix invW (normalized)");
        fmatrixPrintA(thisMagCal.finvW, 0, 2, 0, 2);

    // 6DOF: run FPU eCompass with selected calibration model

    // reset magnetic buffer and computed calibration
    ResetMagCalibration(&thisMagCal, &thisMagneticBuffer);

    // get the remaining run time parameters

    // use NED coordinate system
    iCoordSystem = NED;
    // default magnetic calibration model size is 7
    iSolutionSize = 7;

    // initialize the low pass filters for 6DOF orientation
    fInitLPFOrientationMatrix(&fb0, &fa1, &fa2);

    // main sampling loop (typically 10Hz to 50Hz on real systems)
    // for evaluation purposes, this is written as a loop over a fixed
    // number of iterations. for a real time system, this should be
    // implemented as an infinite loop either called at the sampling
    // frequency by an RTOS or simply with an idle until timing interrupt
    // at the top of each loop
    printf("\r\nAngles: Phi=Roll, Theta=Pitch, Psi=Yaw, Rho=Compass, Delta=inclination");
    loopcounter = 0;
    for (;;)
    {
        if(FSL_Sample_Event_Flag == 1)
        {
            printf("\r\nIteration: %6d", loopcounter);
            FSL_Sample_Event_Flag = 0;

            // call 6DOF sensor driver to get sensor data
            fSixDOFSensorDrivers(&thisAccel, &thisMag);


            // display the sensor data on the console
            printf("\r\nf6DOF: Gpx %6d %6.2f Gpy %6d %6.2f Gpz %6d %6.2f",
                thisAccel.iGpx, thisAccel.fGpx, thisAccel.iGpy, thisAccel.fGpy, thisAccel.iGpz, thisAccel.fGpz);
            printf("\r\nf6DOF: Bpx %6d %6.2f Bpy %6d %6.2f Bpz %6d %6.2f",
                thisMag.iBpx, thisMag.fBpx, thisMag.iBpy, thisMag.fBpy, thisMag.iBpz, thisMag.fBpz);

            // update the magnetometer measurement buffer integer magnetometer data
            fUpdateMagnetometerBuffer(&thisMagneticBuffer, &thisMag, &thisAccel, loopcounter);

            // remove hard and soft iron terms from Bp (uT) to get calibrated data Bc (uT)
            fInvertMagCal(&thisMag, &thisMagCal);

            // pass the accel and calibrated mag data to the eCompass
            if (iCoordSystem == NED)
                feCompassDirectNED(&thisSV6DOF, &thisMag, &thisAccel);
            else if (iCoordSystem == ANDROID)
                feCompassDirectAndroid(&thisSV6DOF, &thisMag, &thisAccel);
            else
                feCompassDirectWin8(&thisSV6DOF, &thisMag, &thisAccel);

            // display the computed Euler angles
            printf("\r\nf6DOFECOM: Phi %7.2f The %7.2f Psi %7.2f Rho %7.2f Delta %7.2f",
                thisSV6DOF.fPhi6DOF, thisSV6DOF.fThe6DOF, thisSV6DOF.fPsi6DOF, thisSV6DOF.fRho6DOF, thisSV6DOF.fDelta6DOFn);

             // low pass filter the orientation matrix and get low pass quaternion and Euler angles
            fLPFOrientationMatrix(&thisSV6DOF, iCoordSystem, loopcounter, fb0, fa1, fa2);
            // display the low pass filtered Euler angles
            // note that the default value for the low pass filter cutoff
            // is set to all pass (#define ANGLE_LPF_FPU 1.0F) since it makes no sense
            // to low pass filter a sequence of random test orientations.
            printf("\r\nf6DOFLP  : Phi %7.2f The %7.2f Psi %7.2f Rho %7.2f Delta %7.2f",
                thisSV6DOF.fLPPhi6DOF, thisSV6DOF.fLPThe6DOF, thisSV6DOF.fLPPsi6DOF, thisSV6DOF.fLPRho6DOF, thisSV6DOF.fLPDelta6DOFn);

            // shuffle the rotation matrix low pass filter delay lines
            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                {
                    thisSV6DOF.fLPR6DOFnm2[i][j] = thisSV6DOF.fLPR6DOFnm1[i][j];
                    thisSV6DOF.fLPR6DOFnm1[i][j] = thisSV6DOF.fLPR6DOFn[i][j];
                    thisSV6DOF.fR6DOFnm2[i][j] = thisSV6DOF.fR6DOFnm1[i][j];
                    thisSV6DOF.fR6DOFnm1[i][j] = thisSV6DOF.fR6DOFn[i][j];
                }

            // update the inclination angle low pass filter delay lines
            thisSV6DOF.fLPDelta6DOFnm2 = thisSV6DOF.fLPDelta6DOFnm1;
            thisSV6DOF.fLPDelta6DOFnm1 = thisSV6DOF.fLPDelta6DOFn;
            thisSV6DOF.fDelta6DOFnm2 = thisSV6DOF.fDelta6DOFnm1;
            thisSV6DOF.fDelta6DOFnm1 = thisSV6DOF.fDelta6DOFn;

            // the following section of code executes the calibration algorithms
            // and decides if the new calibration should be accepted. this code
            // is compute-intensive and is best implemented under an RTOS as a low priority
            // task which might execute every minute or so. here it is implemented
            // in a single process which is a simpler way to demonstrate.

            // decision as to whether to calibrate is the logical OR of these conditions
            // line 1: if we don't yet have a calibration
            // line 2: if we have limited measurements (MEDEQUATIONS or less) and INITIALCALINTERVAL iterations have passed
            // line 3: every FINALCALINTERVAL iterations
            if (thisMagneticBuffer.iMagBufferCount >= MINEQUATIONS)
            {
                // calibrate if this will be the first calibration
                // or initially ever INITIALCALINTERVAL iterations or
                // ultimately every FINALCALINTERVAL iterations
                if ((!thisMagCal.iValidMagCal) ||
                    (thisMagCal.iValidMagCal && !(loopcounter % INITIALCALINTERVAL) && (thisMagneticBuffer.iMagBufferCount <= MEDEQUATIONS)) ||
                    (thisMagCal.iValidMagCal && !(loopcounter % FINALCALINTERVAL)))
                {
                    // 10 point eigenpair calibration
                    if (iSolutionSize == 10)
                    {
                        printf("\n\nCalling 10 element EIG calibration at iteration %d with %d in Magnetic Buffer", loopcounter, thisMagneticBuffer.iMagBufferCount);
                        fUpdateCalibration10EIG(&thisMagCal, &thisMagneticBuffer, ftmpA10x10, ftmpB10x10, ftmpA10x1, ftmpA3x3, ftmpA3x1);
                    }
                    // 7 point eigenpair calibration
                    else if (iSolutionSize == 7)
                    {
                        printf("\n\nCalling 7 element EIG calibration at iteration %d with %d in Magnetic Buffer", loopcounter, thisMagneticBuffer.iMagBufferCount);
                        fUpdateCalibration7EIG(&thisMagCal, &thisMagneticBuffer, ftmpA7x7, ftmpB7x7, ftmpA7x1);
                    }
                    // 4 point INV calibration
                    else
                    {
                        printf("\n\nCalling 4 element INV calibration at iteration %d with %d in Magnetic Buffer", loopcounter, thisMagneticBuffer.iMagBufferCount);
                        fUpdateCalibration4INV(&thisMagCal, &thisMagneticBuffer, ftmpA4x4, ftmpB4x4,
                            ftmpA4x1, ftmpB4x1, icolind, irowind, ipivot);
                    }

                    // accept new calibration if:
                    // a) number of measurements is MEDEQUATIONS or less or
                    // b) fit error is reduced and geomagnetic field is in range
                    // (actual range of geomagnetic field strength B on earth varies 22uT to 67uT)
                    if ((thisMagneticBuffer.iMagBufferCount <= MEDEQUATIONS) ||
                        ((thisMagCal.ftrFitErrorpc <= thisMagCal.fFitErrorpc) &&
                        (thisMagCal.ftrB >= MINBFIT) && (thisMagCal.ftrB <= MAXBFIT)))
                    {
                        printf("\n\nAccepting new calibration solution");
                        thisMagCal.fFitErrorpc = thisMagCal.ftrFitErrorpc;
                        thisMagCal.fB = thisMagCal.ftrB;
                        thisMagCal.fVx = thisMagCal.ftrVx;
                        thisMagCal.fVy = thisMagCal.ftrVy;
                        thisMagCal.fVz = thisMagCal.ftrVz;
                        fmatrixAeqB(thisMagCal.finvW, thisMagCal.ftrinvW, 3, 3);
                    }
                    else
                    {
                        printf("\n\nRejecting new calibration solution");
                    }

                    // age (increase) the calibration fit to avoid a good calibration preventing future updates
                    // FITERRORAGING is the reciprocal of the time (s) for the fit error to increase by e=2.718
                    // FINALCALINTERVAL * DELTAT is the interval in seconds between each aging update of fFitErrorpc
                    // (1 + FITERRORAGING * FINALCALINTERVAL * DELTAT)^n=e defines n, the number of updates for e fold increase
                    // approx n * FINALCALINTERVAL * DELTAT = 1. / FITERRORAGING
                    // so as required FITERRORAGING is the reciprocal of the time in secs for e fold increase
                    thisMagCal.fFitErrorpc += thisMagCal.fFitErrorpc * FITERRORAGING * (float) FINALCALINTERVAL * DELTAT;
                } // end of test whether to call calibration functions
            }
            else // still too few entries in magnetic buffer for calibration
                printf("\n%d entries in magnetometer buffer is too few for calibration", thisMagneticBuffer.iMagBufferCount);

            loopcounter ++;
        }
    } // infinite loops
}
