// -----------------------------------------------------------------------
// Copyright (c) 2011, 2012, 2013 Freescale Semiconductor, Inc.
// ----------------------------------------------------------------------------
// FILENAME:     matrix.c
// CONTENTS:     MATRIX ALGEBRA ROUTINES
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

// function calculates the matrix product A = B x C
void fmatrixAeqBxC(float **A, float **B, float **C, int32 rB, int32 cBrC, int32 cC)
{
    // rB = rows in B
    // cBrC = columns in B = rows in C
    // cC = columns in C
    // A has dimension rB rows x cC columns

    int32 i, j, k;	// counters

    for (i = 0; i < rB; i++)
    {
        for (j = 0; j < cC; j++)
        {
            A[i][j] = 0.0F;
            for (k = 0; k < cBrC; k++)
                A[i][j] += B[i][k] * C[k][j];
        }
    }
    return;
}

// function calculates the matrix product A = B^T x C
void fmatrixAeqTrBxC(float **A, float **B, float **C, int32 rBrC, int32 cB, int32 cC)
{
    // rBrC = rows in B before transposing = rows in C
    // cB = columns in B before transposing
    // cC = columns in C
    // A has dimension cB rows x cC columns
    // the matrices B and C can be the same matrix in order to compute B^T x B

    int32 i, j, k;  // counters

    for (i = 0; i < cB; i++)
    {
        for (j = 0; j < cC; j++)
        {
            A[i][j] = 0.0F;
            for (k = 0; k < rBrC; k++)
                A[i][j] += B[k][i] * C[k][j];
        }
    }
    return;
}

// function calculates the symmetric matrix product A = B x B^T since A^T = (B * B^T)^T = B x B^T = A
void fmatrixAeqBxTrB(float **A, float **B, int32 rB, int32 cB)
{
    // rB = rows in B
    // cB = columns in B
    // A has dimension rB rows x rB columns

    int32 i, j, k; // counters

    for (i = 0; i < rB; i++)
    {
        for (j = i; j < rB; j++)
        {
            A[i][j] = 0.0F;
            for (k = 0; k < cB; k++)
            {
                A[i][j] += B[i][k] * B[j][k];
            }
            A[j][i] = A[i][j];
        }
    }
    return;
}

// function sets the matrix A to the identity matrix
void fmatrixAeqI(float **A, int32 rc)
{
    // rc = rows and columns in A

    int32 i, j;     // loop counters

    for (i = 0; i < rc; i++)
    {
        for (j = 0; j < rc; j++)
        {
            A[i][j] = 0.0F;
        }
        A[i][i] = 1.0F;
    }
    return;
}

// function multiplies all elements of matrix A by the specified scalar
void fmatrixAeqAxScalar(float **A, float Scalar, int32 r, int32 c)
{
    // r = rows and c = columns in A

    int32 i, j;     // loop counters

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            A[i][j] *= Scalar;
        }
    }
    return;
}

// function negates all elements of matrix A
void fmatrixAeqMinusA(float **A, int32 r, int32 c)
{
    // r = rows and c = columns in A

    int32 i, j;     // loop counters

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            A[i][j] = -A[i][j];
        }
    }
    return;
}

// function prints the floating point matrix A between rows r1 and r2 and columns c1 and c2 inclusve
void fmatrixPrintA(float **A, int32 r1, int32 r2, int32 c1, int32 c2)
{
    int32 i, j;     // loop counters

    for (i = r1; i <= r2; i++)
    {
        printf("\nRow %d", i);
        for (j = c1; j <= c2; j++)
        {
            printf("%12.5f", (double) A[i][j]);
        }
    }
    return;
}

// function calculates the determinant of a 3x3 matrix
float f3x3matrixDetA(float **A)
{
    float det;     // determinant

    det = A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1])
        + A[0][1] * (A[1][2] * A[2][0] - A[1][0] * A[2][2])
        + A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);

    return(det);
}

// function directly calculates the symmetric inverse of a symmetric 3x3 matrix
// only the on and above diagonal terms in B are used and need to be specified
void f3x3matrixAeqInvSymB(float **A, float **B)
{
    float fB11B22mB12B12;   // B[1][1] * B[2][2] - B[1][2] * B[1][2]
    float fB12B02mB01B22;   // B[1][2] * B[0][2] - B[0][1] * B[2][2]
    float fB01B12mB11B02;   // B[0][1] * B[1][2] - B[1][1] * B[0][2]
    float ftmp;             // determinant and then reciprocal

    // calculate useful products
    fB11B22mB12B12 = B[1][1] * B[2][2] - B[1][2] * B[1][2];
    fB12B02mB01B22 = B[1][2] * B[0][2] - B[0][1] * B[2][2];
    fB01B12mB11B02 = B[0][1] * B[1][2] - B[1][1] * B[0][2];

    // set ftmp to the determinant of the input matrix B
    ftmp = B[0][0] * fB11B22mB12B12 + B[0][1] * fB12B02mB01B22 + B[0][2] * fB01B12mB11B02;

    // set A to the inverse of B for any determinant except zero
    if (ftmp != 0.0F)
    {
        ftmp = 1.0F / ftmp;
        A[0][0] = fB11B22mB12B12 * ftmp;
        A[1][0] = A[0][1] = fB12B02mB01B22 * ftmp;
        A[2][0] = A[0][2] = fB01B12mB11B02 * ftmp;
        A[1][1] = (B[0][0] * B[2][2] - B[0][2] * B[0][2]) * ftmp;
        A[2][1] = A[1][2] = (B[0][2] * B[0][1] - B[0][0] * B[1][2]) * ftmp;
        A[2][2] = (B[0][0] * B[1][1] - B[0][1] * B[0][1]) * ftmp;
    }
    else
    {
        // provide the identity matrix if the determinant is zero
        A[0][0] = A[1][1] = A[2][2] = 1.0F;
        A[0][1] = A[0][2] = A[1][0] = A[1][2] = A[2][0] = A[2][1] = 0.0F;
    }
    return;
}

// function copies matrix B to A
void fmatrixAeqB(float **A, float **B, int32 r, int32 c)
{
    // r = rows in A and B
    // c = columns in A and B

    int32 i, j;     // loop counters

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            A[i][j] = B[i][j];
        }
    }
    return;
}

// function sets symmetric matrix A to the square root of symmetric matrix B
void fmatrixAeqRootSymB(float **A, float **B, float **ftmpmat, float **ftmpvec, int32 rc)
{
    // matrix A is set to the square root of matrix B
    // matrix B is unaltered
    // ftmpmat is a scratch rc x rc matrix and ftmpvec a scratch rc x 1 vector
    // rc is the number of rows and columns in A and B

    // local variables
    int32 i, j;         // loop counters
    float ftmp;         // scratch variable

    // set ftmpvec to the eigenvalues and ftmpmat to the eigenvectors of B
    eigencompute(B, rc, ftmpvec, ftmpmat);

    // set ftmpmat to be eigenvectors . diag(sqrt(sqrt(eigenvalues))) = ftmpmat . diag(sqrt(sqrt(ftmpvec))
    for (j = 0; j < rc; j++) // loop over columns j
    {
        ftmp = (float) sqrt(sqrt(fabs(ftmpvec[j][0])));
        for (i = 0; i < rc; i++) // loop over rows i
        {
            ftmpmat[i][j] *= ftmp;
        }
    }

    // set A to eigenvectors * diag(sqrt(eigenvalues)) * eigenvectors^T = ftmpmat * ftmpmat^T
    // function fmatrixAeqBxTrB forces the resulting matrix to be symmetric
    fmatrixAeqBxTrB(A, ftmpmat, rc, rc);

    return;
}

// function computes all eigenvalues and eigenvectors of a real symmetric matrix m[0..n-1][0..n-1]
// m[][] is unchanged on output.
// eigval[0..n-1] returns the eigenvalues of m[][].
// eigvec[0..n-1][0..n-1] returns the normalized eigenvectors of m[][]
// the eigenvectors are not sorted by value
void eigencompute(float **m, int32 n, float **eigval, float **eigvec)
{
    // maximum number of iterations to achieve convergence: in practice 6 is typical
#define NITERATIONS 15

    // matrix row and column indices
    int32 ir, ic;
    // general loop counter
    int32 j;
    // various trig functions of the jacobi rotation angle phi
    float cot2phi, tanhalfphi, tanphi, sinphi, cosphi;
    // scratch variable to prevent over-writing during rotations
    float ftmp;
    // residue from remaining non-zero above diagonal terms
    float residue;
    // timeout ctr for number of passes of the algorithm
    int32 ctr;

    // initialize eigenvectors matrix and eigenvalues array
    for (ir = 0; ir < n; ir++)
    {
        // loop over all columns
        for (ic = 0; ic < n; ic++)
        {
            // set on diagonal and off-diagonal elements to zero
            eigvec[ir][ic] = 0.0F;
        }

        // correct the diagonal elements to 1.0
        eigvec[ir][ir] = 1.0F;

        // initialize the array of eigenvalues to the diagonal elements of m
        eigval[ir][0] = m[ir][ir];
    }

    // initialize the counter and loop until converged or NITERATIONS reached
    ctr = 0;
    do
    {
        // compute the absolute value of the above diagonal elements as exit criterion
        residue = 0.0F;
        // loop over rows excluding last row
        for (ir = 0; ir < n - 1; ir++)
        {
            // loop over above diagonal columns
            for (ic = ir + 1; ic < n; ic++)
            {
                // accumulate the residual off diagonal terms which are being driven to zero
                residue += (float) fabs(m[ir][ic]);
            }
        }

        // check if we still have work to do
        if (residue > 0.0F)
        {
            // loop over all rows with the exception of the last row
            // (since only rotating above diagonal elements)
            for (ir = 0; ir < n - 1; ir++)
            {
                // loop over columns ic (where ic is always greater than ir since above diagonal)
                for (ic = ir + 1; ic < n; ic++)
                {
                    // only continue with this element if the element is non-zero
                    if (fabs(m[ir][ic]) > 0.0F)
                    {
                        // calculate cot(2*phi) where phi is the Jacobi rotation angle
                        cot2phi = 0.5F * (eigval[ic][0] - eigval[ir][0]) / (m[ir][ic]);

                        // calculate tan(phi) correcting sign to ensure the smaller solution is used
                        tanphi = 1.0F / (float) (fabs(cot2phi) + sqrt(1.0F + cot2phi * cot2phi));
                        if (cot2phi < 0.0F)
                        {
                            tanphi = -tanphi;
                        }

                        // calculate the sine and cosine of the Jacobi rotation angle phi
                        cosphi = 1.0F / (float)sqrt(1.0F + tanphi * tanphi);
                        sinphi = tanphi * cosphi;

                        // calculate tan(phi/2)
                        tanhalfphi = sinphi / (1.0F + cosphi);

                        // set ftmp = tan(phi) * current matrix element
                        // used in update of leading diagonal elements
                        ftmp = tanphi * m[ir][ic];

                        // apply the jacobi rotation to diagonal elements [ir][ir] and [ic][ic]
                        // stored in the eigenvalue array
                        // eigval[ir] = eigval[ir] - tan(phi) * m[ir][ic]
                        eigval[ir][0] -= ftmp;
                        // eigval[ic] = eigval[ic] + tan(phi) * m[ir][ic]
                        eigval[ic][0] += ftmp;

                        // by definition, applying the jacobi rotation on element ir, ic results in 0.0
                        m[ir][ic] = 0.0F;

                        // apply the jacobi rotation to all elements of the eigenvector matrix
                        for (j = 0; j < n; j++)
                        {
                            // store eigvec[j][ir]
                            ftmp = eigvec[j][ir];
                            // eigvec[j][ir] = eigvec[j][ir] - sin(phi) *
                            // (eigvec[j][ic] + tan(phi/2) * eigvec[j][ir])
                            eigvec[j][ir] = ftmp - sinphi * (eigvec[j][ic] + tanhalfphi * ftmp);
                            // eigvec[j][ic] = eigvec[j][ic] + sin(phi) *
                            // (eigvec[j][ir] - tan(phi/2) * eigvec[j][ic])
                            eigvec[j][ic] = eigvec[j][ic] + sinphi * (ftmp - tanhalfphi * eigvec[j][ic]);
                        }

                        // apply the jacobi rotation only to those elements of matrix m that can change
                        for (j = 0; j <= ir - 1; j++)
                        {
                            // store m[j][ir]
                            ftmp = m[j][ir];
                            // m[j][ir] = m[j][ir] - sin(phi) * (m[j][ic] + tan(phi/2) * m[j][ir])
                            m[j][ir] = ftmp - sinphi * (m[j][ic] + tanhalfphi * ftmp);
                            // m[j][ic] = m[j][ic] + sin(phi) * (m[j][ir] - tan(phi/2) * m[j][ic])
                            m[j][ic] = m[j][ic] + sinphi * (ftmp - tanhalfphi * m[j][ic]);
                        }
                        for (j = ir + 1; j <= ic - 1; j++)
                        {
                            // store m[ir][j]
                            ftmp = m[ir][j];
                            // m[ir][j] = m[ir][j] - sin(phi) * (m[j][ic] + tan(phi/2) * m[ir][j])
                            m[ir][j] = ftmp - sinphi * (m[j][ic] + tanhalfphi * ftmp);
                            // m[j][ic] = m[j][ic] + sin(phi) * (m[ir][j] - tan(phi/2) * m[j][ic])
                            m[j][ic] = m[j][ic] + sinphi * (ftmp - tanhalfphi * m[j][ic]);
                        }
                        for (j = ic + 1; j < n; j++)
                        {
                            // store m[ir][j]
                            ftmp = m[ir][j];
                            // m[ir][j] = m[ir][j] - sin(phi) * (m[ic][j] + tan(phi/2) * m[ir][j])
                            m[ir][j] = ftmp - sinphi * (m[ic][j] + tanhalfphi * ftmp);
                            // m[ic][j] = m[ic][j] + sin(phi) * (m[ir][j] - tan(phi/2) * m[ic][j])
                            m[ic][j] = m[ic][j] + sinphi * (ftmp - tanhalfphi * m[ic][j]);
                        }
                    }   // end of test for matrix element already zero
                }   // end of loop over columns
            }   // end of loop over rows
        }  // end of test for non-zero residue
    } while ((residue > 0.0F) && (ctr++ < NITERATIONS)); // end of main loop

    // recover the above diagonal matrix elements from unaltered below diagonal elements
    // this code can be deleted if the input matrix m[][] is not required to be unaltered on exit
    for (ir = 0; ir < n - 1; ir++)
    {
        // loop over above diagonal columns
        for (ic = ir + 1; ic < n; ic++)
        {
            // copy below diagonal element to above diagonal element
            m[ir][ic] = m[ic][ir];
        }
    }
    return;
}

/* function uses Gauss-Jordan elimination to compute the inverse of matrix A in situ */
/* on exit, A is replaced with its inverse */
void fmatrixAeqInvA(float **A, int32 isize, int32 **icolind, int32 **irowind, int32 **ipivot)
{
    int32 i, j, k, l, m;            // index counters
    int32 ipivotrow = 0, ipivotcol = 0;     // row and column of pivot element
    float largest;                  // largest element used for pivoting
    float scaling;                  // scaling factor in pivoting
    float recippiv;                 // reciprocal of pivot element
    float ftmp;                     // temporary variable used in swaps

    // initialize the pivot array to 0
    for (j = 0; j < isize; j++)
    {
        ipivot[j][0] = 0;
    }

    // main loop i over the dimensions of the square matrix A
    for (i = 0; i < isize; i++)
    {
        // zero the largest element found for pivoting
        largest = 0.0F;
        // loop over candidate rows j
        for (j = 0; j < isize; j++)
        {
            // check if row j has been previously pivoted
            if (ipivot[j][0] != 1)
            {
                // loop over candidate columns k
                for (k = 0; k < isize; k++)
                {
                    // check if column k has previously been pivoted
                    if (ipivot[k][0] == 0)
                    {
                        // check if the pivot element is the largest found so far
                        if (fabs(A[j][k]) >= largest)
                        {
                            // and store this location as the current best candidate for pivoting
                            ipivotrow = j;
                            ipivotcol = k;
                            largest = (float) fabs(A[ipivotrow][ipivotcol]);
                        }
                    }
                    else if (ipivot[k][0] > 1)
                    {
                        // zero determinant situation: exit with identity matrix
                        fmatrixAeqI(A, 3);
                        return;
                    }
                }
            }
        }
        // increment the entry in ipivot to denote it has been selected for pivoting
        ipivot[ipivotcol][0]++;

        // check the pivot rows ipivotrow and ipivotcol are not the same before swapping
        if (ipivotrow != ipivotcol)
        {
            // loop over columns l
            for (l = 0; l < isize; l++)
            {
                // and swap all elements of rows ipivotrow and ipivotcol
                ftmp = A[ipivotrow][l];
                A[ipivotrow][l] = A[ipivotcol][l];
                A[ipivotcol][l] = ftmp;
            }
        }

        // record that on the i-th iteration rows ipivotrow and ipivotcol were swapped
        irowind[i][0] = ipivotrow;
        icolind[i][0] = ipivotcol;

        // check for zero on-diagonal element (singular matrix) and return with identity matrix if detected
        if (A[ipivotcol][ipivotcol] == 0.0F)
        {
            // zero determinant situation: exit with identity matrix
            fmatrixAeqI(A, 3);
            return;
        }

        // calculate the reciprocal of the pivot element knowing it's non-zero
        recippiv = 1.0F / A[ipivotcol][ipivotcol];
        // by definition, the diagonal element normalizes to 1
        A[ipivotcol][ipivotcol] = 1.0F;
        // multiply all of row ipivotcol by the reciprocal of the pivot element including the diagonal element
        // the diagonal element A[ipivotcol][ipivotcol] now has value equal to the reciprocal of its previous value
        for (l = 0; l < isize; l++)
        {
            A[ipivotcol][l] *= recippiv;
        }
        // loop over all rows m of A
        for (m = 0; m < isize; m++)
        {
            if (m != ipivotcol)
            {
                // scaling factor for this row m is in column ipivotcol
                scaling = A[m][ipivotcol];
                // zero this element
                A[m][ipivotcol] = 0.0F;
                // loop over all columns l of A and perform elimination
                for (l = 0; l < isize; l++)
                {
                    A[m][l] -= A[ipivotcol][l] * scaling;
                }
            }
        }
    } // end of loop i over the matrix dimensions

    // finally, loop in inverse order to apply the missing column swaps
    for (l = isize - 1; l >= 0; l--)
    {
        // set i and j to the two columns to be swapped
        i = irowind[l][0];
        j = icolind[l][0];

        // check that the two columns i and j to be swapped are not the same
        if (i != j)
        {
            // loop over all rows k to swap columns i and j of A
            for (k = 0; k < isize; k++)
            {
                ftmp = A[k][i];
                A[k][i] = A[k][j];
                A[k][j] = ftmp;
            }
        }
    }

    return;
}

// function re-orthonormalizes a 3x3 rotation matrix
void fmatrixAeqRenormRotA(float **A)
{
    float ftmp;                 // scratch variable
#define CORRUPTMATRIX 0.01F     // threshold for deciding rotation matrix is corrupt

    // normalize the z vector [column 2]
    ftmp = (float) sqrt(A[0][2] * A[0][2] + A[1][2] * A[1][2] + A[2][2] * A[2][2]);
    if (ftmp > CORRUPTMATRIX)
    {
        // general case
        A[0][2] /= ftmp;
        A[1][2] /= ftmp;
        A[2][2] /= ftmp;
    }
    else
    {
        // return with identity matrix since the matrix is corrupted
        A[0][0] = A[1][1] = A[2][2] = 1.0F;
        A[0][1] = A[0][2] = A[1][0] = A[1][2] = A[2][0] = A[2][1] = 0.0F;
        return;
    }

    // set the x vector [column 0] to y [column 1] cross z [column 2]
    A[0][0] = A[1][1] * A[2][2] - A[2][1] * A[1][2];
    A[1][0] = A[2][1] * A[0][2] - A[0][1] * A[2][2];
    A[2][0] = A[0][1] * A[1][2] - A[1][1] * A[0][2];

    // normalize the resulting x vector [column 0]
    ftmp = (float) sqrt(A[0][0] * A[0][0] + A[1][0] * A[1][0] + A[2][0] * A[2][0]);
    if (ftmp > CORRUPTMATRIX)
    {
        // general case
        A[0][0] /= ftmp;
        A[1][0] /= ftmp;
        A[2][0] /= ftmp;
    }
    else
    {
        // return with identity matrix since the matrix is corrupted
        A[0][0] = A[1][1] = A[2][2] = 1.0F;
        A[0][1] = A[0][2] = A[1][0] = A[1][2] = A[2][0] = A[2][1] = 0.0F;
        return;
    }

    // set the y vector [column 1] to z [column 2] cross x [column 0]
    // this will be normalized since z and x already are
    A[0][1] = A[1][2] * A[2][0] - A[2][2] * A[1][0];
    A[1][1] = A[2][2] * A[0][0] - A[0][2] * A[2][0];
    A[2][1] = A[0][2] * A[1][0] - A[1][2] * A[0][0];

    return;
}
