// -----------------------------------------------------------------------
// Copyright (c) 2011, 2012, 2013 Freescale Semiconductor, Inc.
// ----------------------------------------------------------------------------
// FILENAME:     orientation.c
// CONTENTS:     ECOMPASS, ORIENTATION, EULER ANGLE, QUATERNION FUNCTIONS
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
// V3.0 ENHANCEMENTS:
// - Numerous minor enhancements to improve accuracy
// - Significant reduction in RAM and CPU in the magnetic calibration solvers
// - Users of V2 code need not upgrade to V3 unless RAM and CPU are an issue.
// ----------------------------------------------------------------------------

#include "include.h"


// NED: Direct tilt-compensated e-Compass function
void feCompassDirectNED(struct SV6DOF *pthisSV6DOF, struct MagSensor *pthisMag, struct AccelSensor *pthisAccel)
{
    // local variables
    float fmodB, fmodG, fmodx, fmody;
    float fsinDelta;

    // place the un-normalized gravity and geomagnetic vectors into the rotation matrix z and x axes
    pthisSV6DOF->fR6DOFn[0][2] = pthisAccel->fGpx;
    pthisSV6DOF->fR6DOFn[1][2] = pthisAccel->fGpy;
    pthisSV6DOF->fR6DOFn[2][2] = pthisAccel->fGpz;
    pthisSV6DOF->fR6DOFn[0][0] = pthisMag->fBcx;
    pthisSV6DOF->fR6DOFn[1][0] = pthisMag->fBcy;
    pthisSV6DOF->fR6DOFn[2][0] = pthisMag->fBcz;

    // set y vector to vector product of z and x vectors
    pthisSV6DOF->fR6DOFn[0][1] = pthisSV6DOF->fR6DOFn[1][2] * pthisSV6DOF->fR6DOFn[2][0] -
        pthisSV6DOF->fR6DOFn[2][2] * pthisSV6DOF->fR6DOFn[1][0];
    pthisSV6DOF->fR6DOFn[1][1] = pthisSV6DOF->fR6DOFn[2][2] * pthisSV6DOF->fR6DOFn[0][0] -
        pthisSV6DOF->fR6DOFn[0][2] * pthisSV6DOF->fR6DOFn[2][0];
    pthisSV6DOF->fR6DOFn[2][1] = pthisSV6DOF->fR6DOFn[0][2] * pthisSV6DOF->fR6DOFn[1][0] -
        pthisSV6DOF->fR6DOFn[1][2] * pthisSV6DOF->fR6DOFn[0][0];

    // set x vector to vector product of y and z vectors
    pthisSV6DOF->fR6DOFn[0][0] = pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[2][2] -
        pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[1][2];
    pthisSV6DOF->fR6DOFn[1][0] = pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[0][2] -
        pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[2][2];
    pthisSV6DOF->fR6DOFn[2][0] = pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[1][2] -
        pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[0][2];

    // calculate the vector moduli
    fmodG =  (float) sqrt(pthisAccel->fGpx * pthisAccel->fGpx + pthisAccel->fGpy * pthisAccel->fGpy +
        pthisAccel->fGpz * pthisAccel->fGpz);
    fmodB =  (float) sqrt(pthisMag->fBcx * pthisMag->fBcx + pthisMag->fBcy * pthisMag->fBcy +
        pthisMag->fBcz * pthisMag->fBcz);
    fmodx = (float) sqrt(pthisSV6DOF->fR6DOFn[0][0] * pthisSV6DOF->fR6DOFn[0][0] +
                pthisSV6DOF->fR6DOFn[1][0] * pthisSV6DOF->fR6DOFn[1][0] +
                pthisSV6DOF->fR6DOFn[2][0] * pthisSV6DOF->fR6DOFn[2][0]);
    fmody = (float) sqrt(pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[0][1] +
                pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[1][1] +
                pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[2][1]);

    // normalize the rotation matrix
    if (!((fmodx == 0.0F) || (fmody == 0.0F) || (fmodG == 0.0F)))
    {
        // normalize x axis
        pthisSV6DOF->fR6DOFn[0][0] /= fmodx;
        pthisSV6DOF->fR6DOFn[1][0] /= fmodx;
        pthisSV6DOF->fR6DOFn[2][0] /= fmodx;
        // normalize y axis
        pthisSV6DOF->fR6DOFn[0][1] /= fmody;
        pthisSV6DOF->fR6DOFn[1][1] /= fmody;
        pthisSV6DOF->fR6DOFn[2][1] /= fmody;
        // normalize z axis
        pthisSV6DOF->fR6DOFn[0][2] /= fmodG;
        pthisSV6DOF->fR6DOFn[1][2] /= fmodG;
        pthisSV6DOF->fR6DOFn[2][2] /= fmodG;

        // estimate the declination angle Delta (90 minus angle between the vectors)
        fsinDelta = (pthisAccel->fGpx * pthisMag->fBcx + pthisAccel->fGpy * pthisMag->fBcy +
            pthisAccel->fGpz * pthisMag->fBcz) / fmodG / fmodB;
        pthisSV6DOF->fDelta6DOFn = (float) asin(fsinDelta) * FRADTODEG;
    }
    else
    // no solution is possible to set rotation to identity matrix and delta to 0 degrees
    {
        fmatrixAeqI(pthisSV6DOF->fR6DOFn, 3);
        pthisSV6DOF->fDelta6DOFn = 0.0F;
    }

    // extract the roll, pitch and yaw angles from the rotation matrix fR6DOFn
    fNEDAnglesDegFromRotationMatrix(pthisSV6DOF->fR6DOFn, &(pthisSV6DOF->fPhi6DOF), &(pthisSV6DOF->fThe6DOF),
        &(pthisSV6DOF->fPsi6DOF), &(pthisSV6DOF->fRho6DOF));

    // get the instantaneous orientation quaternion
    fQuaternionFromRotationMatrix(pthisSV6DOF->fR6DOFn, &(pthisSV6DOF->fq6DOFn));

    return;
}

// Android: Direct tilt-compensated e-Compass function
void feCompassDirectAndroid(struct SV6DOF *pthisSV6DOF, struct MagSensor *pthisMag, struct AccelSensor *pthisAccel)
{
    // local variables
    float fmodB, fmodG, fmodx, fmody;
    float fsinDelta;

    // place the un-normalized gravity and geomagnetic vectors into the rotation matrix z and y axes
    pthisSV6DOF->fR6DOFn[0][2] = pthisAccel->fGpx;
    pthisSV6DOF->fR6DOFn[1][2] = pthisAccel->fGpy;
    pthisSV6DOF->fR6DOFn[2][2] = pthisAccel->fGpz;
    pthisSV6DOF->fR6DOFn[0][1] = pthisMag->fBcx;
    pthisSV6DOF->fR6DOFn[1][1] = pthisMag->fBcy;
    pthisSV6DOF->fR6DOFn[2][1] = pthisMag->fBcz;

    // set x vector to vector product of y and z vectors
    pthisSV6DOF->fR6DOFn[0][0] = pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[2][2] -
        pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[1][2];
    pthisSV6DOF->fR6DOFn[1][0] = pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[0][2] -
        pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[2][2];
    pthisSV6DOF->fR6DOFn[2][0] = pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[1][2] -
        pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[0][2];

    // set y vector to vector product of z and x vectors
    pthisSV6DOF->fR6DOFn[0][1] = pthisSV6DOF->fR6DOFn[1][2] * pthisSV6DOF->fR6DOFn[2][0] -
        pthisSV6DOF->fR6DOFn[2][2] * pthisSV6DOF->fR6DOFn[1][0];
    pthisSV6DOF->fR6DOFn[1][1] = pthisSV6DOF->fR6DOFn[2][2] * pthisSV6DOF->fR6DOFn[0][0] -
        pthisSV6DOF->fR6DOFn[0][2] * pthisSV6DOF->fR6DOFn[2][0];
    pthisSV6DOF->fR6DOFn[2][1] = pthisSV6DOF->fR6DOFn[0][2] * pthisSV6DOF->fR6DOFn[1][0] -
        pthisSV6DOF->fR6DOFn[1][2] * pthisSV6DOF->fR6DOFn[0][0];

    // calculate the vector moduli
    fmodG =  (float) sqrt(pthisAccel->fGpx * pthisAccel->fGpx + pthisAccel->fGpy * pthisAccel->fGpy +
        pthisAccel->fGpz * pthisAccel->fGpz);
    fmodB =  (float) sqrt(pthisMag->fBcx * pthisMag->fBcx + pthisMag->fBcy * pthisMag->fBcy +
        pthisMag->fBcz * pthisMag->fBcz);
    fmodx = (float) sqrt(pthisSV6DOF->fR6DOFn[0][0] * pthisSV6DOF->fR6DOFn[0][0] +
                pthisSV6DOF->fR6DOFn[1][0] * pthisSV6DOF->fR6DOFn[1][0] +
                pthisSV6DOF->fR6DOFn[2][0] * pthisSV6DOF->fR6DOFn[2][0]);
    fmody = (float) sqrt(pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[0][1] +
                pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[1][1] +
                pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[2][1]);

    // normalize the rotation matrix
    if (!((fmodx == 0.0F) || (fmody == 0.0F) || (fmodG == 0.0F)))
    {
        // normalize x axis
        pthisSV6DOF->fR6DOFn[0][0] /= fmodx;
        pthisSV6DOF->fR6DOFn[1][0] /= fmodx;
        pthisSV6DOF->fR6DOFn[2][0] /= fmodx;
        // normalize y axis
        pthisSV6DOF->fR6DOFn[0][1] /= fmody;
        pthisSV6DOF->fR6DOFn[1][1] /= fmody;
        pthisSV6DOF->fR6DOFn[2][1] /= fmody;
        // normalize z axis
        pthisSV6DOF->fR6DOFn[0][2] /= fmodG;
        pthisSV6DOF->fR6DOFn[1][2] /= fmodG;
        pthisSV6DOF->fR6DOFn[2][2] /= fmodG;

        // estimate the declination angle Delta (90 minus angle between the vectors)
        fsinDelta = -(pthisAccel->fGpx * pthisMag->fBcx + pthisAccel->fGpy * pthisMag->fBcy +
            pthisAccel->fGpz * pthisMag->fBcz) / fmodG / fmodB;
        pthisSV6DOF->fDelta6DOFn = (float) asin(fsinDelta) * FRADTODEG;
    }
    else
    // no solution is possible to set rotation to identity matrix and delta to 0 degrees
    {
        fmatrixAeqI(pthisSV6DOF->fR6DOFn, 3);
        pthisSV6DOF->fDelta6DOFn = 0.0F;
    }

    // extract the roll, pitch and yaw angles from the rotation matrix fR6DOFn
    fAndroidAnglesDegFromRotationMatrix(pthisSV6DOF->fR6DOFn, &(pthisSV6DOF->fPhi6DOF), &(pthisSV6DOF->fThe6DOF),
        &(pthisSV6DOF->fPsi6DOF), &(pthisSV6DOF->fRho6DOF));

    // get the instantaneous orientation quaternion
    fQuaternionFromRotationMatrix(pthisSV6DOF->fR6DOFn, &(pthisSV6DOF->fq6DOFn));

    return;
}

// Win8: Direct tilt-compensated e-Compass function
void feCompassDirectWin8(struct SV6DOF *pthisSV6DOF, struct MagSensor *pthisMag, struct AccelSensor *pthisAccel)
{
    // local variables
    float fmodB, fmodG, fmodx, fmody;
    float fsinDelta;

    // place the negated un-normalized gravity and un-normalized geomagnetic vectors into the rotation matrix z and y axes
    pthisSV6DOF->fR6DOFn[0][2] = -pthisAccel->fGpx;
    pthisSV6DOF->fR6DOFn[1][2] = -pthisAccel->fGpy;
    pthisSV6DOF->fR6DOFn[2][2] = -pthisAccel->fGpz;
    pthisSV6DOF->fR6DOFn[0][1] = pthisMag->fBcx;
    pthisSV6DOF->fR6DOFn[1][1] = pthisMag->fBcy;
    pthisSV6DOF->fR6DOFn[2][1] = pthisMag->fBcz;

    // set x vector to vector product of y and z vectors
    pthisSV6DOF->fR6DOFn[0][0] = pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[2][2] -
        pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[1][2];
    pthisSV6DOF->fR6DOFn[1][0] = pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[0][2] -
        pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[2][2];
    pthisSV6DOF->fR6DOFn[2][0] = pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[1][2] -
        pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[0][2];

    // set y vector to vector product of z and x vectors
    pthisSV6DOF->fR6DOFn[0][1] = pthisSV6DOF->fR6DOFn[1][2] * pthisSV6DOF->fR6DOFn[2][0] -
        pthisSV6DOF->fR6DOFn[2][2] * pthisSV6DOF->fR6DOFn[1][0];
    pthisSV6DOF->fR6DOFn[1][1] = pthisSV6DOF->fR6DOFn[2][2] * pthisSV6DOF->fR6DOFn[0][0] -
        pthisSV6DOF->fR6DOFn[0][2] * pthisSV6DOF->fR6DOFn[2][0];
    pthisSV6DOF->fR6DOFn[2][1] = pthisSV6DOF->fR6DOFn[0][2] * pthisSV6DOF->fR6DOFn[1][0] -
        pthisSV6DOF->fR6DOFn[1][2] * pthisSV6DOF->fR6DOFn[0][0];

    // calculate the vector moduli
    fmodG =  (float) sqrt(pthisAccel->fGpx * pthisAccel->fGpx + pthisAccel->fGpy * pthisAccel->fGpy +
        pthisAccel->fGpz * pthisAccel->fGpz);
    fmodB =  (float) sqrt(pthisMag->fBcx * pthisMag->fBcx + pthisMag->fBcy * pthisMag->fBcy +
        pthisMag->fBcz * pthisMag->fBcz);
    fmodx = (float) sqrt(pthisSV6DOF->fR6DOFn[0][0] * pthisSV6DOF->fR6DOFn[0][0] +
                pthisSV6DOF->fR6DOFn[1][0] * pthisSV6DOF->fR6DOFn[1][0] +
                pthisSV6DOF->fR6DOFn[2][0] * pthisSV6DOF->fR6DOFn[2][0]);
    fmody = (float) sqrt(pthisSV6DOF->fR6DOFn[0][1] * pthisSV6DOF->fR6DOFn[0][1] +
                pthisSV6DOF->fR6DOFn[1][1] * pthisSV6DOF->fR6DOFn[1][1] +
                pthisSV6DOF->fR6DOFn[2][1] * pthisSV6DOF->fR6DOFn[2][1]);

    // normalize the rotation matrix
    if (!((fmodx == 0.0F) || (fmody == 0.0F) || (fmodG == 0.0F)))
    {
        // normalize x axis
        pthisSV6DOF->fR6DOFn[0][0] /= fmodx;
        pthisSV6DOF->fR6DOFn[1][0] /= fmodx;
        pthisSV6DOF->fR6DOFn[2][0] /= fmodx;
        // normalize y axis
        pthisSV6DOF->fR6DOFn[0][1] /= fmody;
        pthisSV6DOF->fR6DOFn[1][1] /= fmody;
        pthisSV6DOF->fR6DOFn[2][1] /= fmody;
        // normalize z axis
        pthisSV6DOF->fR6DOFn[0][2] /= fmodG;
        pthisSV6DOF->fR6DOFn[1][2] /= fmodG;
        pthisSV6DOF->fR6DOFn[2][2] /= fmodG;

        // estimate the declination angle Delta (90 minus angle between the vectors)
        fsinDelta = (pthisAccel->fGpx * pthisMag->fBcx + pthisAccel->fGpy * pthisMag->fBcy +
            pthisAccel->fGpz * pthisMag->fBcz) / fmodG / fmodB;
        pthisSV6DOF->fDelta6DOFn = (float) asin(fsinDelta) * FRADTODEG;
    }
    else
    // no solution is possible to set rotation to identity matrix and delta to 0 degrees
    {
        fmatrixAeqI(pthisSV6DOF->fR6DOFn, 3);
        pthisSV6DOF->fDelta6DOFn = 0.0F;
    }

    // extract the roll, pitch and yaw angles from the rotation matrix fR6DOFn
    fWin8AnglesDegFromRotationMatrix(pthisSV6DOF->fR6DOFn, &(pthisSV6DOF->fPhi6DOF), &(pthisSV6DOF->fThe6DOF),
        &(pthisSV6DOF->fPsi6DOF), &(pthisSV6DOF->fRho6DOF));

    // get the instantaneous orientation quaternion
    fQuaternionFromRotationMatrix(pthisSV6DOF->fR6DOFn, &(pthisSV6DOF->fq6DOFn));

    return;
}

// function initializes the low pass filter
void fInitLPFOrientationMatrix(float *pfb0, float *pfa1, float *pfa2)
{
    // function computes the Butterworth low pass filter values from ANGLE_LPF_FPU

    // local variables
    float fwc;                      // bilinear transformed frequency
    float ftmp;                     // scratch variable

    // return nothing for the all pass case which is tested for explicitly in the filter
    // ANGLE_LPF_FPU == 1.0F results in pole zero cancellation
    if (ANGLE_LPF_FPU == 1.0F)
        return;

    // calculate the filter coefficients for the general low pass case
    fwc = (float)tan(0.5F * PI / ANGLE_LPF_FPU);
    ftmp = 1.0F + (float)sqrt(2.0F) * fwc + fwc * fwc;
    *pfb0 = fwc * fwc / ftmp;
    *pfa1 = 2.0F * (1.0F - fwc * fwc) / ftmp;
    *pfa2 = -(1.0F - (float) sqrt(2.0F) * fwc + fwc * fwc) / ftmp;

    return;
}

// function low pass filters the orientation matrix
void fLPFOrientationMatrix(struct SV6DOF *pthisSV6DOF, int32 iCoordSystem, int32 loopcounter,
            float fb0, float fa1, float fa2)
{
    // function is called with the instantaneous orientation matrix fR6DOFn
    // and inclination angle fdelta6DOFn and computes the low pass filtered
    // rotation matrix, Euler angles, quaternion and inclination angle

    // local variables
    int32 i, j;                 // loop counters

    // initialize delay lines on first pass
    if (loopcounter == 0)
    {
        // set R[LP,n-2]=R[LP,n-1]=R[n-2]=R[n-1]=R[n]
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                pthisSV6DOF->fLPR6DOFnm2[i][j] = pthisSV6DOF->fLPR6DOFnm1[i][j] =
                pthisSV6DOF->fR6DOFnm2[i][j] = pthisSV6DOF->fR6DOFnm1[i][j] =
                pthisSV6DOF->fR6DOFn[i][j];

        // set delta[LP,n-2]=delta[LP,n-1]=delta[n-2]=delta[n-1]=delta[n]
        pthisSV6DOF->fLPDelta6DOFnm2 = pthisSV6DOF->fLPDelta6DOFnm1 =
            pthisSV6DOF->fDelta6DOFnm2 = pthisSV6DOF->fDelta6DOFnm1 =
            pthisSV6DOF->fDelta6DOFn;
    }

    // low pass filter the nine elements of the rotation matrix
    // the resulting matrix will no longer be perfectly orthonormal
    if (ANGLE_LPF_FPU == 1.0F)
    {
        // all pass case for orientation matrix
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                pthisSV6DOF->fLPR6DOFn[i][j] = pthisSV6DOF->fR6DOFn[i][j];
        // all pass case for the inclination angle delta
        pthisSV6DOF->fLPDelta6DOFn = pthisSV6DOF->fDelta6DOFn;
    }
    else
    {
        // apply the low pass Butterworth filter to the orientation matrix
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                pthisSV6DOF->fLPR6DOFn[i][j] = fb0 * (pthisSV6DOF->fR6DOFn[i][j] +
                        2.0F * pthisSV6DOF->fR6DOFnm1[i][j] + pthisSV6DOF->fR6DOFnm2[i][j]) +
                        fa1 * pthisSV6DOF->fLPR6DOFnm1[i][j] + fa2 * pthisSV6DOF->fLPR6DOFnm2[i][j];
        // apply the low pass Butterworth filter to the inclination angle delta
        pthisSV6DOF->fLPDelta6DOFn = fb0 * (pthisSV6DOF->fDelta6DOFn +
            2.0F * pthisSV6DOF->fDelta6DOFnm1 + pthisSV6DOF->fDelta6DOFnm2) +
            fa1 * pthisSV6DOF->fLPDelta6DOFnm1 + fa2 * pthisSV6DOF->fLPDelta6DOFnm2;
    }

    // renormalize the orientation matrix
    fmatrixAeqRenormRotA(pthisSV6DOF->fLPR6DOFn);

    // calculate Euler angles from the low pass orientation matrix
    if (iCoordSystem == NED) // NED
    {
        fNEDAnglesDegFromRotationMatrix(pthisSV6DOF->fLPR6DOFn, &(pthisSV6DOF->fLPPhi6DOF),
            &(pthisSV6DOF->fLPThe6DOF), &(pthisSV6DOF->fLPPsi6DOF), &(pthisSV6DOF->fLPRho6DOF));
    }
    else if (iCoordSystem == ANDROID) // Android
    {
        fAndroidAnglesDegFromRotationMatrix(pthisSV6DOF->fLPR6DOFn, &(pthisSV6DOF->fLPPhi6DOF),
            &(pthisSV6DOF->fLPThe6DOF), &(pthisSV6DOF->fLPPsi6DOF), &(pthisSV6DOF->fLPRho6DOF));
    }
    else  // Windows 8
    {
        fWin8AnglesDegFromRotationMatrix(pthisSV6DOF->fLPR6DOFn, &(pthisSV6DOF->fLPPhi6DOF),
            &(pthisSV6DOF->fLPThe6DOF), &(pthisSV6DOF->fLPPsi6DOF), &(pthisSV6DOF->fLPRho6DOF));
    }

    // compute a normalized low pass quaternion from the low pass normalized matrix
    fQuaternionFromRotationMatrix(pthisSV6DOF->fLPR6DOFn, &(pthisSV6DOF->fLPq6DOFn));

    return;
}

// extract the NED angles in degrees from the NED rotation matrix
void fNEDAnglesDegFromRotationMatrix(float **R, float *pfPhiDeg, float *pfTheDeg, float *pfPsiDeg, float *pfRhoDeg)
{
    // calculate the pitch angle -90.0 <= Theta <= 90.0 deg
    if (R[0][2] >= 1.0F)
        *pfTheDeg = -90.0F;
    else if (R[0][2] <= -1.0F)
        *pfTheDeg = 90.0F;
    else
        *pfTheDeg = (float) asin(-R[0][2]) * FRADTODEG;

    // calculate the roll angle range -180.0 <= Phi < 180.0 deg
    *pfPhiDeg = (float)atan2(R[1][2], R[2][2]) * FRADTODEG;
    // map +180 roll onto the functionally equivalent -180 deg roll
    if (*pfPhiDeg == 180.0F)
        *pfPhiDeg = -180.0F;

    // calculate the yaw and compass angle 0.0 <= Psi < 360.0 deg
    *pfPsiDeg = (float)atan2(R[0][1], R[0][0]) * FRADTODEG;
    if (*pfPsiDeg < 0.0F)
        *pfPsiDeg += 360.0F;
    // check for rounding errors mapping small negative angle to 360 deg
    if (*pfPsiDeg >= 360.0F)
        *pfPsiDeg = 0.0F;
    // for NED, the compass heading Rho equals the yaw angle Psi
    *pfRhoDeg = *pfPsiDeg;

    return;
}

// extract the Android angles in degrees from the Android rotation matrix
void fAndroidAnglesDegFromRotationMatrix(float **R, float *pfPhiDeg, float *pfTheDeg, float *pfPsiDeg, float *pfRhoDeg)
{
    // calculate the roll angle -90.0 <= Phi <= 90.0 deg
    if (R[0][2] >= 1.0F)
        *pfPhiDeg = 90.0F;
    else if (R[0][2] <= -1.0F)
        *pfPhiDeg = -90.0F;
    else
        *pfPhiDeg = (float) asin(R[0][2]) * FRADTODEG;

    // calculate the pitch angle -180.0 <= The < 180.0 deg
    *pfTheDeg = (float)atan2(-R[1][2], R[2][2]) * FRADTODEG;
    // map +180 pitch onto the functionally equivalent -180 deg pitch
    if (*pfTheDeg == 180.0F)
        *pfTheDeg = -180.0F;

    // calculate the yaw=compass angle 0.0 <= Psi < 360.0 deg
    *pfPsiDeg = (float)atan2(-R[0][1], R[0][0]) * FRADTODEG;
    if (*pfPsiDeg < 0.0F)
        *pfPsiDeg += 360.0F;
    // check for rounding errors mapping small negative angle to 360 deg
    if (*pfPsiDeg >= 360.0F)
        *pfPsiDeg = 0.0F;
    // the compass heading angle Rho equals the yaw angle Psi
    // this definition is compliant with Motorola Xoom tablet behavior
    *pfRhoDeg = *pfPsiDeg;

    return;
}

// extract the Windows 8 angles in degrees from the Windows 8 rotation matrix
void fWin8AnglesDegFromRotationMatrix(float **R, float *pfPhiDeg, float *pfTheDeg, float *pfPsiDeg, float *pfRhoDeg)
{
    // calculate the roll angle -90.0 <= Phi <= 90.0 deg
    // later we correct this to the range -90.0 <= Phi < 90.0 deg
    if (R[2][2] == 0.0F)
    {
        if (R[0][2] >= 0.0F)        // tan(phi) is -infinity
            *pfPhiDeg = -90.0F;
        else                        // tan(phi) is +infinity
            *pfPhiDeg = 90.0F;
    }
    else
        *pfPhiDeg = (float)atan(-R[0][2] / R[2][2]) * FRADTODEG;

    // first calculate the pitch angle in the range -90.0 <= The <= 90.0 deg
    if (R[1][2] >= 1.0F)
        *pfTheDeg = 90.0F;
    else if (R[1][2] <= -1.0F)
        *pfTheDeg = -90.0F;
    else
        *pfTheDeg = (float) asin(R[1][2]) * FRADTODEG;
    // use R[2][2]=cos(Phi)*cos(The) to correct the quadrant of The remembering
    // cos(Phi) is non-negative and that R[2][2] has the same sign as cos(The).
    // the result is -180.0 <= The < 180.0 deg
    if (R[2][2] < 0.0F)
    {
        // The is either in range 90.0 < The < 180.0 deg or -180.0 <= The < -90.0 deg
        if (*pfTheDeg > 0.0F)
            // wrap The around +90 deg
            *pfTheDeg = 180.0F - *pfTheDeg;
        if (*pfTheDeg <= 0.0F)
            // wrap The around -90 deg
            *pfTheDeg = -180.0F - *pfTheDeg;
    }

    // calculate the yaw angle -180.0 <= Psi < 180.0 deg
    if (*pfTheDeg == 90.0F)
    {
        *pfPsiDeg = (float)atan2(R[0][1], R[0][0]) * FRADTODEG - *pfPhiDeg;
    }
    else if (*pfTheDeg == -90.0F)
    {
        *pfPsiDeg = (float)atan2(R[0][1], R[0][0]) * FRADTODEG + *pfPhiDeg;
    }
    else
    {
        // general case where cos(The) is not zero
        *pfPsiDeg = (float)atan2(-cos(*pfTheDeg * FDEGTORAD) * R[1][0],
                    cos(*pfTheDeg * FDEGTORAD) * R[1][1]) * FRADTODEG;
    }

    // need to correct the special case where Phi = +90 deg
    if (*pfPhiDeg == 90.0F)
    {
        *pfPhiDeg = -90.0F;
        *pfTheDeg = 180.0F - *pfTheDeg;
        *pfPsiDeg += 180.0F;
    }

    // map +180 deg pitch onto equivalent -180 deg giving -180.0 <= The < 180.0 deg
    if (*pfTheDeg == 180.0F)
        *pfTheDeg = -180.0F;

    // map yaw angle onto range 0.0 <= Psi < 360.0 deg
    if (*pfPsiDeg < 0.0F)
        *pfPsiDeg += 360.0F;
    // check for rounding errors mapping small negative angle to 360 deg
    if (*pfPsiDeg >= 360.0F)
        *pfPsiDeg = 0.0F;

    // compute the compass angle Rho checking for the special case of 0.0 deg
    if (*pfPsiDeg == 0.0F)
        *pfRhoDeg = 0.0F;
    else
        *pfRhoDeg = 360.0F - *pfPsiDeg;

    return;
}

// compute the orientation quaternion from a rotation matrix
void fQuaternionFromRotationMatrix(float **R, struct fquaternion *pq)
{
    float fq0sq;            // q0^2
    float recip4q0;         // 1/4q0
    float fmag;             // quaternion magnitude
#define SMALLQ0 0.01F       // limit where rounding errors may appear

    // typically 18 floating point operations

    // get q0^2 and q0
    fq0sq = 0.25F * (1.0F + R[0][0] + R[1][1] + R[2][2]);
    pq->q0 = (float)sqrt(fabs(fq0sq));

    // normal case when q0 is not small meaning rotation angle not near 180 deg
    if (pq->q0 > SMALLQ0)
    {
        // calculate q1 to q3
        recip4q0 = 0.25F / pq->q0;
        pq->q1 = recip4q0 * (R[1][2] - R[2][1]);
        pq->q2 = recip4q0 * (R[2][0] - R[0][2]);
        pq->q3 = recip4q0 * (R[0][1] - R[1][0]);
    }
    else
    {
        // special case of near 180 deg corresponds to nearly symmetric matrix
        // which is not numerically well conditioned for division by small q0
        // instead get absolute values of q1 to q3 from leading diagonal
        pq->q1 = (float)sqrt(fabs(0.5F * (1.0F + R[0][0]) - fq0sq));
        pq->q2 = (float)sqrt(fabs(0.5F * (1.0F + R[1][1]) - fq0sq));
        pq->q3 = (float)sqrt(fabs(0.5F * (1.0F + R[2][2]) - fq0sq));

        // first assume q1 is positive and ensure q2 and q3 are consistent with q1
        if ((R[0][1] + R[1][0]) < 0.0F)
        {
            // q1*q2 < 0 so q2 is negative
            pq->q2 = -pq->q2;
            if ((R[1][2] + R[2][1]) > 0.0F)
            {
                // q1*q2 < 0 and q2*q3 > 0 so q3 is also both negative
                pq->q3 = -pq->q3;
            }
        }
        else if ((R[0][1] + R[1][0]) > 0.0F)
        {
            if ((R[1][2] + R[2][1]) < 0.0F)
            {
                // q1*q2 > 0 and q2*q3 < 0 so q3 is negative
                pq->q3 = -pq->q3;
            }
        }

        // negate the vector components if q1 should be negative
        if ((R[1][2] - R[2][1]) < 0.0F)
        {
            pq->q1 = -pq->q1;
            pq->q2 = -pq->q2;
            pq->q3 = -pq->q3;
        }
    }

     // finally re-normalize for safety
     fmag = (float) sqrt(pq->q0 * pq->q0 + pq->q1 * pq->q1 + pq->q2 * pq->q2 + pq->q3 * pq->q3);
     if (fmag > SMALLQ0)
     {
         // normal case
         pq->q0 /= fmag;
         pq->q1 /= fmag;
         pq->q2 /= fmag;
         pq->q3 /= fmag;
      }
      else
      {
          // severe rounding errors so return identity quaternion
          pq->q0 = 1.0F;
          pq->q1 = pq->q2 = pq->q3 = 0.0F;
      }

    return;
}
