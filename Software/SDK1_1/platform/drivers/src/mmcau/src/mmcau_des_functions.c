/*
 * Copyright (c) 2011, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "cau2_defines.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
* @brief DES: Checks key parity
*
*   calling convention
*   int     mmcau_des_chk_parity (const unsigned char *key)
*
* @param key pointer to 64-bit DES key with parity bits
* @return returns 0 if no error, -1 in case of parity error
*/
int mmcau_des_chk_parity(unsigned int key[])
{
/* load the 64-bit key into the CAU's CA0/CA1 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA0)) = key[0];    /* load key[i]   -> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = key[1];    /* load key[i+1] -> CA1*/

/* perform the key schedule and check the parity bits*/
    *(MMCAU_PPB_DIRECT + 0) = mmcau_1_cmd(DESK+CP); /* key setup + parity check*/

/* the CAUSR[DPE] reflects the DES key parity check*/
    return ((*(MMCAU_PPB_INDIRECT + (STR+CASR)) & 0x2) ? -1 : 0);
}

/*!
* @brief DES: Encrypts a single 8-byte block
*
*   NOTE    Input and output blocks may overlap
*
*   calling convention
*   mmcau_des_encrypt (const unsigned char *in,
*                      const unsigned char *key,
*                      unsigned char       *out)
*
* @param in pointer to 8-byte block of input plaintext
* @param key pointer to 64-bit DES key with parity bits
* @param out pointer to 8-byte block of output ciphertext
*/
void mmcau_des_encrypt(unsigned int in[], unsigned int key[],
                       unsigned int out[])
{

/* load the 64-bit plaintext input block into CAU's CA2/CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA2)) = byterev(in[0]);  /* ld plain[i]  -> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA3)) = byterev(in[1]);  /* ld plain[i+1]-> CA3*/

/* load the 64-bit key into the CAU's CA0/CA1 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA0)) = byterev(key[0]); /* load key[i]   -> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = byterev(key[1]); /* load key[i+1] -> CA1*/

/* send a series of 17 direct cau commands to perform the DES round operations*/
    *(MMCAU_PPB_DIRECT + 0) = mmcau_3_cmds(DESK,DESR+IP+KSL1,DESR+KSL2);
    *(MMCAU_PPB_DIRECT + 1) = mmcau_3_cmds(DESR+KSL2,DESR+KSL2,DESR+KSL2);
    *(MMCAU_PPB_DIRECT + 2) = mmcau_3_cmds(DESR+KSL2,DESR+KSL2,DESR+KSL1);
    *(MMCAU_PPB_DIRECT + 3) = mmcau_3_cmds(DESR+KSL2,DESR+KSL2,DESR+KSL2);
    *(MMCAU_PPB_DIRECT + 4) = mmcau_3_cmds(DESR+KSL2,DESR+KSL2,DESR+KSL2);
    *(MMCAU_PPB_DIRECT + 5) = mmcau_2_cmds(DESR+KSL1,DESR+FP);

/* store the 64-bit ciphertext output block into memory*/
    out[0] = *(MMCAU_PPB_INDIRECT + (STR+CA2));           /* store 1st 4 bytes*/
    out[0] = byterev(out[0]);                             /* byte reverse*/
    out[1] = *(MMCAU_PPB_INDIRECT + (STR+CA3));           /* store 2nd 4 bytes*/
    out[1] = byterev(out[1]);                             /* byte reverse*/

}

/*!
* @brief DES: Decrypts a single 8-byte block
*
*   NOTE    Input and output blocks may overlap
*
*   calling convention
*   mmcau_des_decrypt (const unsigned char *in,
*                      const unsigned char *key,
*                      unsigned char       *out)
*
* @param in pointer to 8-byte block of input plaintext
* @param key pointer to 64-bit DES key with parity bits
* @param out pointer to 8-byte block of output ciphertext
*/
void mmcau_des_decrypt(unsigned int in[], unsigned int key[],
                       unsigned int out[])
{

/* load the 64-bit ciphertext input block into CAU's CA2/CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA2)) = byterev(in[0]);  /* ld cipher[i]  -> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA3)) = byterev(in[1]);  /* ld cipher[i+1]-> CA3*/

/* load the 64-bit key into the CAU's CA0/CA1 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA0)) = byterev(key[0]); /* load key[i]   -> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = byterev(key[1]); /* load key[i+1] -> CA1*/

/* send a series of 17 direct cau commands to perform the DES round operations*/
    *(MMCAU_PPB_DIRECT + 0) = mmcau_3_cmds(DESK+DC,DESR+IP+KSR1,DESR+KSR2);
    *(MMCAU_PPB_DIRECT + 1) = mmcau_3_cmds(DESR+KSR2,DESR+KSR2,DESR+KSR2);
    *(MMCAU_PPB_DIRECT + 2) = mmcau_3_cmds(DESR+KSR2,DESR+KSR2,DESR+KSR1);
    *(MMCAU_PPB_DIRECT + 3) = mmcau_3_cmds(DESR+KSR2,DESR+KSR2,DESR+KSR2);
    *(MMCAU_PPB_DIRECT + 4) = mmcau_3_cmds(DESR+KSR2,DESR+KSR2,DESR+KSR2);
    *(MMCAU_PPB_DIRECT + 5) = mmcau_2_cmds(DESR+KSR1,DESR+FP);

/* store the 64-bit plaintext output block into memory*/
    out[0] = *(MMCAU_PPB_INDIRECT + (STR+CA2));           /* store 1st 4 bytes*/
    out[0] = byterev(out[0]);                             /* byte reverse*/
    out[1] = *(MMCAU_PPB_INDIRECT + (STR+CA3));           /* store 2nd 4 bytes*/
    out[1] = byterev(out[1]);                             /* byte reverse*/

}
