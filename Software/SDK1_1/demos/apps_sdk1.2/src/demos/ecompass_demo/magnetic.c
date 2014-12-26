// -----------------------------------------------------------------------
// Copyright (c) 2011, 2012, 2013 Freescale Semiconductor, Inc.
// ----------------------------------------------------------------------------
// FILENAME:     magnetic.c
// CONTENTS:     MAGNETIC CALIBRATION ROUTINES
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

// magnetometer calibration reset function
void ResetMagCalibration(struct MagCalibration *pthisMagCal, struct MagneticBuffer *pthisMagneticBuffer)
{
    int32 j, k, l; // loop counters

    // initialize the calibration hard and soft iron estimate to null
    fmatrixAeqI(pthisMagCal->finvW, 3);
    pthisMagCal->fVx = pthisMagCal->fVy = pthisMagCal->fVz = 0.0F;
    pthisMagCal->fB = 0.0F;
    pthisMagCal->fFitErrorpc = 1000.0F;
    pthisMagCal->iValidMagCal = 0;

    // set magnetic buffer index to invalid value -1 to denote invalid
    pthisMagneticBuffer->iMagBufferCount = 0;
    for (j = 0; j < MAGBUFFSIZE; j++)
    {
        for (k = 0; k < MAGBUFFSIZE; k++)
        {
            for (l = 0; l < MAGBUFFSIZE; l++)
            {
                pthisMagneticBuffer->index[j][k][l] = -1;
            }
        }
    }

    return;
}

// map the uncalibrated magnetometer data Bp (uT) onto calibrated data Bc (uT)
void fInvertMagCal(struct MagSensor *pthisMag, struct MagCalibration *pthisMagCal)
{
    // local variables
    float ftmpx, ftmpy, ftmpz;

    // remove the computed hard iron offset
    pthisMag->fBcx = pthisMag->fBpx - pthisMagCal->fVx;
    pthisMag->fBcy = pthisMag->fBpy - pthisMagCal->fVy;
    pthisMag->fBcz = pthisMag->fBpz - pthisMagCal->fVz;

    // remove the computed soft iron offset
    ftmpx = pthisMagCal->finvW[0][0] * pthisMag->fBcx + pthisMagCal->finvW[0][1] * pthisMag->fBcy +
        pthisMagCal->finvW[0][2] * pthisMag->fBcz;
    ftmpy = pthisMagCal->finvW[1][0] * pthisMag->fBcx + pthisMagCal->finvW[1][1] * pthisMag->fBcy +
        pthisMagCal->finvW[1][2] * pthisMag->fBcz;
    ftmpz = pthisMagCal->finvW[2][0] * pthisMag->fBcx + pthisMagCal->finvW[2][1] * pthisMag->fBcy +
        pthisMagCal->finvW[2][2] * pthisMag->fBcz;
    pthisMag->fBcx = ftmpx;
    pthisMag->fBcy = ftmpy;
    pthisMag->fBcz = ftmpz;

    return;
}

// update the magnetic measurement buffer with most recent data
void fUpdateMagnetometerBuffer(struct MagneticBuffer *pthisMagneticBuffer,
    struct MagSensor *pthisMag, struct AccelSensor *pthisAccel, int32 loopcounter)
{
    // local variables
    int32 j, k, l;                      // magnetic buffer indices
    int32 previndex;                    // previous time index in the bin being over-written
    int32 oldestj, oldestk, oldestl;    // indices of bin with oldest data
    int32 oldestindex;                  // time index of bin with oldest data
    float ftmp;                         // scratch variable

    // map -90 to 90 degrees onto range 0 to MAGBUFFSIZE - 1
    ftmp = (float) MAGBUFFSIZE * FRECIP180;
    j = (int32) ((atan2(pthisAccel->fGpx, fabs(pthisAccel->fGpy)) * FRADTODEG + 90.0F) * ftmp);
    k = (int32) ((atan2(pthisAccel->fGpy, fabs(pthisAccel->fGpz)) * FRADTODEG + 90.0F) * ftmp);
    l = (int32) ((atan2(pthisAccel->fGpz, fabs(pthisAccel->fGpx)) * FRADTODEG + 90.0F) * ftmp);

    // bounds safety check in case exactly on boundary
    if (j >= MAGBUFFSIZE) j = MAGBUFFSIZE - 1;
    if (k >= MAGBUFFSIZE) k = MAGBUFFSIZE - 1;
    if (l >= MAGBUFFSIZE) l = MAGBUFFSIZE - 1;

    // save the previous time index in this bin for later use
    previndex = pthisMagneticBuffer->index[j][k][l];

    // store the raw short integer magnetometer reading
    pthisMagneticBuffer->iBx[j][k][l] = pthisMag->iBpx;
    pthisMagneticBuffer->iBy[j][k][l] = pthisMag->iBpy;
    pthisMagneticBuffer->iBz[j][k][l] = pthisMag->iBpz;

    // set the time index to the loop counter (valid value 0 at start of first pass)
    pthisMagneticBuffer->index[j][k][l] = loopcounter;

    // no additional action needed if current bin was active
    // but action is needed if bin was inactive (value -1)
    if (previndex == -1)
    {
        if (pthisMagneticBuffer->iMagBufferCount < MAXEQUATIONS)
        {
            // simply increase the count of active measurements for calibration
            (pthisMagneticBuffer->iMagBufferCount)++;
        }
        else
        {
            // we need to find and de-activate the oldest reading
            oldestindex = loopcounter;
            oldestj = oldestk = oldestl = 0; // to avoid compiler error
            for (j = 0; j < MAGBUFFSIZE; j++)
            {
                for (k = 0; k < MAGBUFFSIZE; k++)
                {
                    for (l = 0; l < MAGBUFFSIZE; l++)
                    {
                        if ((pthisMagneticBuffer->index[j][k][l] != -1) &&
                            (pthisMagneticBuffer->index[j][k][l] < oldestindex))
                        {
                            oldestj = j;
                            oldestk = k;
                            oldestl = l;
                            oldestindex = pthisMagneticBuffer->index[oldestj][oldestk][oldestl];
                        }
                    }
                }
            }
            // deactivate the oldest reading without bothering to zero the measurement data
            pthisMagneticBuffer->index[oldestj][oldestk][oldestl] = -1;
        }
    }

    return;
}

// 10 element calibration using direct eigen-decomposition
void fUpdateCalibration10EIG(struct MagCalibration *pthisMagCal, struct MagneticBuffer *pthisMagneticBuffer,
    float **ftmpA10x10, float **ftmpB10x10, float **ftmpA10x1, float **ftmpA3x3, float **ftmpA3x1)
{
    // Freescale does not release the source code for the 10 element magnetic
    // calibration solver but may release the object code in ARM Thumb2 format
    // on email request to sfusion@freescale.com.
    // Other object code formats may be supported on request.
    printf("\nThe 10 element calibration solver is not supplied as source code");
    printf("\nReverting to 7 element solver");
    fUpdateCalibration7EIG(pthisMagCal, pthisMagneticBuffer, ftmpA10x10, ftmpB10x10, ftmpA10x1);

    return;
}

// 7 element calibration using direct eigen-decomposition
void fUpdateCalibration7EIG(struct MagCalibration *pthisMagCal,
    struct MagneticBuffer *pthisMagneticBuffer,
    float **ftmpA7x7, float **ftmpB7x7, float **ftmpA7x1)
{
    // local variables
    int32 i, j, k, l, m, n;                 // loop counters
    int32 ilocalMagBufferCount;             // local count of measurements for this process
    float fOffsetx, fOffsety, fOffsetz;     // offset to remove large DC hard iron bias
    float det;                              // matrix determinant
    float fscaling;                         // set to FUTPERCOUNT * FMATRIXSCALING

    // compute fscaling to reduce multiplications later
    fscaling = FUTPERCOUNT * FMATRIXSCALING;

    // the offsets are guaranteed to be set from the first element but to avoid compiler error
    fOffsetx = fOffsety = fOffsetz = 0.0F;

    // zero the on and above diagonal elements of the 7x7 symmetric measurement matrix ftmpA7x7
    for (m = 0; m < 7; m++)
    {
        for (n = m; n < 7; n++)
        {
            ftmpA7x7[m][n] = 0.0F;
        }
    }

    // last entry of vector ftmpA7x1 is always 1.0F so move assignment outside the loop
    ftmpA7x1[6][0] = 1.0F;

    // place from MINEQUATIONS to MAXEQUATIONS entries into product matrix ftmpA7x7
    i = 0;
    for (j = 0; j < MAGBUFFSIZE; j++)
    {
        for (k = 0; k < MAGBUFFSIZE; k++)
        {
            for (l = 0; l < MAGBUFFSIZE; l++)
            {
                if (pthisMagneticBuffer->index[j][k][l] != -1)
                {
                    // use first valid magnetic buffer entry as offset estimate (bit counts)
                    if (i == 0)
                    {
                        fOffsetx = (float)pthisMagneticBuffer->iBx[j][k][l];
                        fOffsety = (float)pthisMagneticBuffer->iBy[j][k][l];
                        fOffsetz = (float)pthisMagneticBuffer->iBz[j][k][l];
                    }

                    // apply the offset, scaling and conversion to uT
                    ftmpA7x1[3][0] = ((float)pthisMagneticBuffer->iBx[j][k][l] - fOffsetx) * fscaling;
                    ftmpA7x1[4][0] = ((float)pthisMagneticBuffer->iBy[j][k][l] - fOffsety) * fscaling;
                    ftmpA7x1[5][0] = ((float)pthisMagneticBuffer->iBz[j][k][l] - fOffsetz) * fscaling;

                    // compute the remaining members of the measurement vector ftmpA7x1
                    ftmpA7x1[0][0] = ftmpA7x1[3][0] * ftmpA7x1[3][0];
                    ftmpA7x1[1][0] = ftmpA7x1[4][0] * ftmpA7x1[4][0];
                    ftmpA7x1[2][0] = ftmpA7x1[5][0] * ftmpA7x1[5][0];

                    // accumulate the on-and above-diagonal terms of ftmpA7x7
                    // = X^T*X = Sigma{ftmpA7x1^T * ftmpA7x1}
                    for (m = 0; m < 7; m++)
                    {
                        for (n = m; n < 7; n++)
                        {
                            ftmpA7x7[m][n] += ftmpA7x1[m][0] * ftmpA7x1[n][0];
                        }
                    }

                    // increment the counter for the next iteration
                    i++;
                }
            }
        }
    }

    // store the number of measurements accumulated
    ilocalMagBufferCount = i;

    // copy the above diagonal elements of ftmpA7x7 to below the diagonal
    for (m = 1; m < 7; m++)
    {
        for (n = 0; n < m; n++)
        {
            ftmpA7x7[m][n] = ftmpA7x7[n][m];
        }
    }

    // set tmpA7x1 to the unsorted eigenvalues and ftmpB7x7 to the unsorted eigenvectors of ftmpA7x7
    eigencompute(ftmpA7x7, 7, ftmpA7x1, ftmpB7x7);

    // set ellipsoid matrix A to the solution vector column j with smallest eigenvalue
    j = 0;
    for (i = 1; i < 7; i++)
    {
        if (ftmpA7x1[i][0] < ftmpA7x1[j][0])
        {
            j = i;
        }
    }
    pthisMagCal->fA[0][0] = ftmpB7x7[0][j];
    pthisMagCal->fA[1][1] = ftmpB7x7[1][j];
    pthisMagCal->fA[2][2] = ftmpB7x7[2][j];
    pthisMagCal->fA[0][1] = pthisMagCal->fA[0][2] = pthisMagCal->fA[1][0] = 0.0F;
    pthisMagCal->fA[1][2] = pthisMagCal->fA[2][0] = pthisMagCal->fA[2][1] = 0.0F;

    // compute the trial hard iron vector in offset and scaled uT
    pthisMagCal->ftrVx = -0.5F * ftmpB7x7[3][j] / pthisMagCal->fA[0][0];
    pthisMagCal->ftrVy = -0.5F * ftmpB7x7[4][j] / pthisMagCal->fA[1][1];
    pthisMagCal->ftrVz = -0.5F * ftmpB7x7[5][j] / pthisMagCal->fA[2][2];

    // negate A if it has negative determinant
    det = pthisMagCal->fA[0][0] * pthisMagCal->fA[1][1] * pthisMagCal->fA[2][2];
    if (det < 0.0F)
    {
        fmatrixAeqMinusA(pthisMagCal->fA, 3, 3);
        ftmpB7x7[6][j] = -ftmpB7x7[6][j];
        det = -det;
    }

    // compute the trial geomagnetic field strength B in bit counts times FMATRIXSCALING
    pthisMagCal->ftrB = (float)sqrt(fabs(pthisMagCal->fA[0][0] * pthisMagCal->ftrVx * pthisMagCal->ftrVx +
        pthisMagCal->fA[1][1] * pthisMagCal->ftrVy * pthisMagCal->ftrVy +
        pthisMagCal->fA[2][2] * pthisMagCal->ftrVz * pthisMagCal->ftrVz - ftmpB7x7[6][j]));

    // calculate the trial normalized fit error as a percentage
    pthisMagCal->ftrFitErrorpc = 100.0F * (float) sqrt(fabs(ftmpA7x1[j][0]) /
        (double) ilocalMagBufferCount) / (2.0F * pthisMagCal->ftrB * pthisMagCal->ftrB);
    printf("\n\nTrial new calibration fit error=%9.4f%% versus previous %9.4f%%",
        pthisMagCal->ftrFitErrorpc, pthisMagCal->fFitErrorpc);

    // correct for the measurement matrix offset and scaling and get the computed trial hard iron offset in uT
    pthisMagCal->ftrVx = pthisMagCal->ftrVx * FINVMATRIXSCALING + fOffsetx * FUTPERCOUNT;
    pthisMagCal->ftrVy = pthisMagCal->ftrVy * FINVMATRIXSCALING + fOffsety * FUTPERCOUNT;
    pthisMagCal->ftrVz = pthisMagCal->ftrVz * FINVMATRIXSCALING + fOffsetz * FUTPERCOUNT;
    printf("\n\nTrial new calibration hard iron (uT) Vx=%9.3f Vy=%9.3f Vz=%9.3f",
        pthisMagCal->ftrVx, pthisMagCal->ftrVy, pthisMagCal->ftrVz);

    // convert the geomagnetic field strength B into uT for current soft iron matrix A
    pthisMagCal->ftrB *= FINVMATRIXSCALING;

    // normalize the ellipsoid matrix A to unit determinant and correct B by root of this multiplicative factor
    fmatrixAeqAxScalar(pthisMagCal->fA, (float)pow((double)det, (double) (-1.0F / 3.0F)), 3, 3);
    pthisMagCal->ftrB *= (float)pow((double)det, (double) (-1.0F / 6.0F));
    printf("\n\nTrial new calibration geomagnetic field (uT) B=%9.3f", pthisMagCal->ftrB);
    printf("\n\nTrial new calibration ellipsoid matrix A (normalized)");
    fmatrixPrintA(pthisMagCal->fA, 0, 2, 0, 2);

    // compute trial invW from the square root of A also with normalized determinant
    pthisMagCal->ftrinvW[0][0] = (float)sqrt(fabs(pthisMagCal->fA[0][0]));
    pthisMagCal->ftrinvW[1][1] = (float)sqrt(fabs(pthisMagCal->fA[1][1]));
    pthisMagCal->ftrinvW[2][2] = (float)sqrt(fabs(pthisMagCal->fA[2][2]));
    pthisMagCal->ftrinvW[0][1] = pthisMagCal->ftrinvW[0][2] = pthisMagCal->ftrinvW[1][0] = 0.0F;
    pthisMagCal->ftrinvW[1][2] = pthisMagCal->ftrinvW[2][0] = pthisMagCal->ftrinvW[2][1] = 0.0F;
    printf("\n\nTrial new calibration inverse soft iron matrix invW (normalized)");
    fmatrixPrintA(pthisMagCal->ftrinvW, 0, 2, 0, 2);

    // set the valid calibration flag to true
    pthisMagCal->iValidMagCal = 1;

    return;
}

// 4 element calibration using 4x4 matrix inverse
void fUpdateCalibration4INV(struct MagCalibration *pthisMagCal,
    struct MagneticBuffer *pthisMagneticBuffer,
    float **ftmpA4x4, float **ftmpB4x4, float **ftmpA4x1,
    float **ftmpB4x1, int32 **icolind, int32 **irowind, int32 **ipivot)
{
    int32 i, j, k, l, m, n;                 // loop counters
    int32 ilocalMagBufferCount;             // local count of measurements for this process
    float fOffsetx, fOffsety, fOffsetz;     // offset to remove large DC hard iron bias in matrix
    float ftmpBpx, ftmpBpy, ftmpBpz;        // x, y, z magnetometer readings (uT)
    float ftmpBpxsq, ftmpBpysq, ftmpBpzsq;  // squares of x, y, z magnetometer readings (uT)
    float fy;                               // dependent variable
    float fYTY;                             // scalar equal to Y^T.Y
    float fscaling;                         // set to FUTPERCOUNT * FMATRIXSCALING
    float fP;                               // performance function = r^T.r

    // compute fscaling to reduce multiplications later
    fscaling = FUTPERCOUNT * FMATRIXSCALING;

    // set trial inverse soft iron matrix invW to the identity matrix for 4 element calibration
    pthisMagCal->ftrinvW[0][0] = pthisMagCal->ftrinvW[1][1] = pthisMagCal->ftrinvW[2][2] = 1.0F;
    pthisMagCal->ftrinvW[0][1] = pthisMagCal->ftrinvW[0][2] = pthisMagCal->ftrinvW[1][0] = 0.0F;
    pthisMagCal->ftrinvW[1][2] = pthisMagCal->ftrinvW[2][0] = pthisMagCal->ftrinvW[2][1] = 0.0F;

    // zero fYTY=Y^T.Y, ftmpA4x1=X^T.Y and on and above diagonal elements of ftmpA4x4=X^T*X
    fYTY = 0.0F;
    for (m = 0; m < 4; m++)
    {
        ftmpA4x1[m][0] = 0.0F;
        for (n = m; n < 4; n++)
        {
            ftmpA4x4[m][n] = 0.0F;
        }
    }

    // the offsets are guaranteed to be set from the first element but to avoid compiler error
    fOffsetx = fOffsety = fOffsetz = 0.0F;

    // use from MINEQUATIONS up to MAXEQUATIONS entries from magnetic buffer to compute matrices
    i = 0;
    for (j = 0; j < MAGBUFFSIZE; j++)
    {
        for (k = 0; k < MAGBUFFSIZE; k++)
        {
            for (l = 0; l < MAGBUFFSIZE; l++)
            {
                if (pthisMagneticBuffer->index[j][k][l] != -1)
                {
                    // use first valid magnetic buffer entry as estimate (in counts) for offset
                    if (i == 0)
                    {
                        fOffsetx = (float)pthisMagneticBuffer->iBx[j][k][l];
                        fOffsety = (float)pthisMagneticBuffer->iBy[j][k][l];
                        fOffsetz = (float)pthisMagneticBuffer->iBz[j][k][l];
                    }

                    // calculate offset and scaled magnetic buffer vector data Bx, By, Bz (scaled uT)
                    ftmpBpx = ((float)pthisMagneticBuffer->iBx[j][k][l] - fOffsetx) * fscaling;
                    ftmpBpy = ((float)pthisMagneticBuffer->iBy[j][k][l] - fOffsety) * fscaling;
                    ftmpBpz = ((float)pthisMagneticBuffer->iBz[j][k][l] - fOffsetz) * fscaling;

                    // calculate y = Bx^2 + By^2 + Bz^2 (scaled uT^2) and accumulate Y^T.Y
                    ftmpBpxsq = ftmpBpx * ftmpBpx;
                    ftmpBpysq = ftmpBpy * ftmpBpy;
                    ftmpBpzsq = ftmpBpz * ftmpBpz;
                    fy = ftmpBpxsq + ftmpBpysq + ftmpBpzsq;
                    fYTY += fy * fy;

                    // accumulate ftmpA4x1 = X^T.Y
                    ftmpA4x1[0][0] += ftmpBpx * fy;
                    ftmpA4x1[1][0] += ftmpBpy * fy;
                    ftmpA4x1[2][0] += ftmpBpz * fy;
                    ftmpA4x1[3][0] += fy;

                    // accumulate on and above-diagonal terms of ftmpA4x4 = X^T.X
                    ftmpA4x4[0][0] += ftmpBpxsq;
                    ftmpA4x4[0][1] += ftmpBpx * ftmpBpy;
                    ftmpA4x4[0][2] += ftmpBpx * ftmpBpz;
                    ftmpA4x4[0][3] += ftmpBpx;
                    ftmpA4x4[1][1] += ftmpBpysq;
                    ftmpA4x4[1][2] += ftmpBpy * ftmpBpz;
                    ftmpA4x4[1][3] += ftmpBpy;
                    ftmpA4x4[2][2] += ftmpBpzsq;
                    ftmpA4x4[2][3] += ftmpBpz;

                    // increment the counter for next iteration
                    i++;
                }
            }
        }
    }

    // store the number of measurements accumulated
    ilocalMagBufferCount = i;

    // set the last element of the measurement matrix to the number of buffer elements used
    ftmpA4x4[3][3] = (float) i;

    // use above diagonal elements of symmetric matrix ftmpA4x4 to create ftmpB4x4 = ftmpA4x4 = X^T.X
    for (m = 0; m < 4; m++)
    {
        for (n = m; n < 4; n++)
        {
        ftmpB4x4[m][n] = ftmpB4x4[n][m] = ftmpA4x4[n][m] = ftmpA4x4[m][n];
        }
    }

    // calculate in situ inverse of ftmpB4x4 = inv(X^T.X) (4x4)
    fmatrixAeqInvA(ftmpB4x4, 4, icolind, irowind, ipivot);

    // calculate ftmpB4x1 = solution vector beta (4x1) = inv(X^T.X).X^T.Y = ftmpB4x4 * ftmpA4x1
    fmatrixAeqBxC(ftmpB4x1, ftmpB4x4, ftmpA4x1, 4, 4, 1);

    // calculate P = r^T.r = Y^T.Y - 2 * beta^T.(X^T.Y) + beta^T.(X^T.X).beta
    // = fYTY - 2 * ftmpB4x1^T.ftmpA4x1 + ftmpB4x1^T.ftmpA4x4.ftmpB4x1
    // first set P = Y^T.Y - 2 * beta^T.(X^T.Y) = fYTY - 2 * ftmpB4x1^T.ftmpA4x1
    fP = fYTY - 2.0F * (ftmpA4x1[0][0] * ftmpB4x1[0][0] + ftmpA4x1[1][0] * ftmpB4x1[1][0] +
        ftmpA4x1[2][0] * ftmpB4x1[2][0] + ftmpA4x1[3][0] * ftmpB4x1[3][0]);
    // set ftmpA4x1 = (X^T.X).beta = ftmpA4x4.ftmpB4x1
    fmatrixAeqBxC(ftmpA4x1, ftmpA4x4, ftmpB4x1, 4, 4, 1);
    // add beta^T.(X^T.X).beta = ftmpB4x1^T * ftmpA4x1 to P
    fP += ftmpA4x1[0][0] * ftmpB4x1[0][0] + ftmpA4x1[1][0] * ftmpB4x1[1][0] +
        ftmpA4x1[2][0] * ftmpB4x1[2][0] + ftmpA4x1[3][0] * ftmpB4x1[3][0];

    // compute the hard iron vector (in uT but offset and scaled by FMATRIXSCALING)
    pthisMagCal->ftrVx = 0.5F * ftmpB4x1[0][0];
    pthisMagCal->ftrVy = 0.5F * ftmpB4x1[1][0];
    pthisMagCal->ftrVz = 0.5F * ftmpB4x1[2][0];

    // compute the scaled geomagnetic field strength B (in uT but scaled by FMATRIXSCALING)
    pthisMagCal->ftrB = (float)sqrt(ftmpB4x1[3][0] + pthisMagCal->ftrVx * pthisMagCal->ftrVx +
        pthisMagCal->ftrVy * pthisMagCal->ftrVy + pthisMagCal->ftrVz * pthisMagCal->ftrVz);

    // calculate the trial fit error (percent) normalized to number of measurements and scaled geomagnetic field strength
    pthisMagCal->ftrFitErrorpc = (float)sqrt(fP / (float) ilocalMagBufferCount) * 100.0F /
        (2.0F * pthisMagCal->ftrB * pthisMagCal->ftrB);
    printf("\n\nTrial new calibration fit error=%9.4f%% versus previous %9.4f%%", pthisMagCal->ftrFitErrorpc, pthisMagCal->fFitErrorpc);

    // correct the hard iron estimate for FMATRIXSCALING and the offsets applied (result in uT)
    pthisMagCal->ftrVx = pthisMagCal->ftrVx * FINVMATRIXSCALING + fOffsetx * FUTPERCOUNT;
    pthisMagCal->ftrVy = pthisMagCal->ftrVy * FINVMATRIXSCALING + fOffsety * FUTPERCOUNT;
    pthisMagCal->ftrVz = pthisMagCal->ftrVz * FINVMATRIXSCALING + fOffsetz * FUTPERCOUNT;
    printf("\n\nTrial new calibration hard iron (uT) Vx=%9.3f Vy=%9.3f Vz=%9.3f", pthisMagCal->ftrVx, pthisMagCal->ftrVy, pthisMagCal->ftrVz);

    // correct the geomagnetic field strength B to correct scaling (result in uT)
    pthisMagCal->ftrB *= FINVMATRIXSCALING;
    printf("\n\nTrial new calibration geomagnetic field (uT) B=%9.3f", pthisMagCal->ftrB);

    // set the valid calibration flag to true
    pthisMagCal->iValidMagCal = 1;

    return;
}
