/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#include <string.h>

#include "freemaster.h"
#include "freemaster_example.h"

/****************************************************************************/

/* Test variables, will be displayed in the FreeMASTER application */
volatile unsigned char  var8;
volatile unsigned char  var8rw;
volatile unsigned char  var8inc = 1;
volatile unsigned short var16;
volatile unsigned short var16rw;
volatile unsigned short var16inc = 1;
volatile unsigned long  var32;
volatile unsigned long  var32rw;
volatile unsigned long  var32inc = 100;
volatile unsigned long long var64;
volatile unsigned long long var64rw;
volatile unsigned long long var64inc = 10000;

volatile float varFLT;
volatile float varFLTrw;
volatile float varFLTinc = 1.0;

volatile double varDBL;
volatile double varDBLrw;
volatile double varDBLinc = 1.0;

#define ARR_SIZE 10
volatile unsigned short arr_size = ARR_SIZE;
volatile unsigned char  arr8[ARR_SIZE];
volatile unsigned short arr16[ARR_SIZE];
volatile unsigned long  arr32[ARR_SIZE];
volatile unsigned long long arr64[ARR_SIZE];
volatile float arrFLT[ARR_SIZE];
volatile double arrDBL[ARR_SIZE];

volatile unsigned char  nAppCmdCounter;

/****************************************************************************/

/*
 * Test structure types - demonstrates the "TSA" feature thanks to which the 
 * FreeMASTER is able to load a variable and type information directly from 
 * the embedded application. 
 *
 */

typedef struct {
  
  unsigned short  aa;
  unsigned long   bb[2];
  unsigned short  cc;
  unsigned long   dd[3];
  unsigned char   ee;
  unsigned char   ff[5];
} INNER_STRUCT;

typedef struct {
  
  unsigned short  a;
  unsigned long   b;
  INNER_STRUCT    inA[4];
  INNER_STRUCT    inB;
} OUTER_STRUCT;

/* Structure type information will be available in the FreeMASTER application (TSA demonstration) */
volatile OUTER_STRUCT so1, so2;
volatile INNER_STRUCT si1, si2;

/****************************************************************************/

/* Variables which hold correct size and offset values to be compared with the ones retrieved from symbols.
   The demo application does not make use of any of these variables, but they may be used for testing.
*/

#define DEFINE_SYM_INFO(var) \
  unsigned long var##_addr = (unsigned long)&var; \
  unsigned long var##_size = (unsigned long)sizeof(var)
  
DEFINE_SYM_INFO(so1);
DEFINE_SYM_INFO(so2);
DEFINE_SYM_INFO(si1);
DEFINE_SYM_INFO(si2);
DEFINE_SYM_INFO(var8);
DEFINE_SYM_INFO(var16);
DEFINE_SYM_INFO(var32);
DEFINE_SYM_INFO(var64);
DEFINE_SYM_INFO(varFLT);
DEFINE_SYM_INFO(varDBL);
DEFINE_SYM_INFO(arr8);
DEFINE_SYM_INFO(arr16);
DEFINE_SYM_INFO(arr32);
DEFINE_SYM_INFO(arr64);
DEFINE_SYM_INFO(arrFLT);
DEFINE_SYM_INFO(arrDBL);

#define DEFINE_MEMB_INFO(type, memb) \
  unsigned long type##_##memb##_offset = (unsigned long)&(((type*)0)->memb); \
  unsigned long type##_##memb##_size = (unsigned long)sizeof(((type*)0)->memb)

DEFINE_MEMB_INFO(INNER_STRUCT, aa);    
DEFINE_MEMB_INFO(INNER_STRUCT, bb);    
DEFINE_MEMB_INFO(INNER_STRUCT, cc);    
DEFINE_MEMB_INFO(INNER_STRUCT, dd);    
DEFINE_MEMB_INFO(INNER_STRUCT, ee);    
DEFINE_MEMB_INFO(INNER_STRUCT, ff);    

DEFINE_MEMB_INFO(OUTER_STRUCT, a);    
DEFINE_MEMB_INFO(OUTER_STRUCT, b);
DEFINE_MEMB_INFO(OUTER_STRUCT, inA);    
DEFINE_MEMB_INFO(OUTER_STRUCT, inB);    

/****************************************************************************/

/*
 * With TSA enabled, the user describes the global and static variables using 
 * so-called TSA tables. There can be any number of tables defined in 
 * the project files. Each table does have the identifier which should be
 * unique across the project. 
 *
 * Note that you can declare variables as Read-Only or Read-Write.
 * The FreeMASTER driver denies any write access to the Read-Only variables
 * when TSA_SAFETY is enabled.
 */

FMSTR_TSA_TABLE_BEGIN(first_table)

    FMSTR_TSA_RO_VAR(var8,     FMSTR_TSA_UINT8)
    FMSTR_TSA_RW_VAR(arr8,     FMSTR_TSA_UINT8)
    FMSTR_TSA_RW_VAR(var8rw,   FMSTR_TSA_UINT8)
    FMSTR_TSA_RW_VAR(var8inc,  FMSTR_TSA_UINT8)
      
    FMSTR_TSA_RO_VAR(var16,    FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(arr16,    FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(var16rw,  FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(var16inc, FMSTR_TSA_UINT16)
      
    FMSTR_TSA_RO_VAR(var32,    FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(arr32,    FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(var32rw,  FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(var32inc, FMSTR_TSA_UINT32)
      
    FMSTR_TSA_RO_VAR(var64,    FMSTR_TSA_UINT64)
    FMSTR_TSA_RW_VAR(arr64,    FMSTR_TSA_UINT64)
    FMSTR_TSA_RW_VAR(var64rw,  FMSTR_TSA_UINT64)
    FMSTR_TSA_RW_VAR(var64inc, FMSTR_TSA_UINT64)
      
    FMSTR_TSA_RO_VAR(varFLT,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(varFLTrw,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(arrFLT,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(varFLTinc, FMSTR_TSA_FLOAT)
      
    FMSTR_TSA_RO_VAR(varDBL,    FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RW_VAR(arrDBL,    FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RW_VAR(varDBLrw,  FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RW_VAR(varDBLinc, FMSTR_TSA_DOUBLE)

    FMSTR_TSA_RO_VAR(arr_size, FMSTR_TSA_UINT16)

    FMSTR_TSA_RO_VAR(nAppCmdCounter, FMSTR_TSA_UINT8)
      
    FMSTR_TSA_RW_VAR(so1,      FMSTR_TSA_USERTYPE(OUTER_STRUCT))
    FMSTR_TSA_RW_VAR(si1,      FMSTR_TSA_USERTYPE(INNER_STRUCT))

    FMSTR_TSA_STRUCT(OUTER_STRUCT)
    FMSTR_TSA_MEMBER(OUTER_STRUCT, a,   FMSTR_TSA_UINT16)
    FMSTR_TSA_MEMBER(OUTER_STRUCT, b,   FMSTR_TSA_UINT32)
    FMSTR_TSA_MEMBER(OUTER_STRUCT, inA, FMSTR_TSA_USERTYPE(INNER_STRUCT))
    FMSTR_TSA_MEMBER(OUTER_STRUCT, inB, FMSTR_TSA_USERTYPE(INNER_STRUCT))
    
    FMSTR_TSA_STRUCT(INNER_STRUCT)
    FMSTR_TSA_MEMBER(INNER_STRUCT, aa, FMSTR_TSA_UINT16)
    FMSTR_TSA_MEMBER(INNER_STRUCT, bb, FMSTR_TSA_UINT32)
    FMSTR_TSA_MEMBER(INNER_STRUCT, cc, FMSTR_TSA_SINT16)
    FMSTR_TSA_MEMBER(INNER_STRUCT, dd, FMSTR_TSA_SINT32)
    FMSTR_TSA_MEMBER(INNER_STRUCT, ee, FMSTR_TSA_UINT8)
    FMSTR_TSA_MEMBER(INNER_STRUCT, ff, FMSTR_TSA_SINT8)
FMSTR_TSA_TABLE_END()

/*
 * This is an example of another TSA table. Typically, you put one table
 * to each .c file where your global or static variables are instantiated.
 */

FMSTR_TSA_TABLE_BEGIN(next_table)
    FMSTR_TSA_RO_VAR(so2, FMSTR_TSA_USERTYPE(OUTER_STRUCT))
    FMSTR_TSA_RO_VAR(si2, FMSTR_TSA_USERTYPE(INNER_STRUCT))
FMSTR_TSA_TABLE_END()

/****************************************************************************/

/*
 * This list describes all TSA tables which should be exported to the 
 * FreeMASTER application.
 */
#ifndef FMSTR_PE_USED
FMSTR_TSA_TABLE_LIST_BEGIN()
    FMSTR_TSA_TABLE(first_table)
    FMSTR_TSA_TABLE(next_table)
FMSTR_TSA_TABLE_LIST_END()
#endif

/****************************************************************************/

/*
 * This function is registerred as a application command handler (see 
 * main() below. It gets automatically invoked when the FreeMASTER
 * application sends appropriate application command.
 *
 */

static FMSTR_APPCMD_RESULT myhandler(FMSTR_APPCMD_CODE nAppcmd, FMSTR_APPCMD_PDATA pData, FMSTR_SIZE nDataLen)
{
    // the return value is used as the application command result code
    return 0x10;
}

/****************************************************************************/
/* General initialization of FreeMASTER example */

void FMSTR_Example_Init(void)
{
    int i;

    var8 = 0;
    var8rw = 0;
    var8inc = 1;
    var16 = 0;
    var16rw = 0;
    var16inc = 1;
    var32 = 0;
    var32rw = 0;
    var32inc = 100;
    var64 = 0;
    var64rw = 0;
    var64inc = 10000;

    varFLT = 0;
    varFLTrw = 0;
    varFLTinc = 1.0;

    varDBL = 0;
    varDBLrw = 0;
    varDBLinc = 1.0;

    arr_size = ARR_SIZE;

    so1.a = 1;
    so2.a = 2;
    si1.aa = 1;
    si2.aa = 2;

    for(i=0; i<ARR_SIZE; i++)
    {
       arr8[i] = i;
       arr16[i] = i*10;
       arr32[i] = i*100;
       arr64[i] = i*1000;
       arrFLT[i] = ((float)i)/10;
       arrDBL[i] = ((double)i)/10;
    }
 
#ifndef FMSTR_PE_USED
	/* FreeMASTER initialization */
    (void)FMSTR_Init();
#endif
    
    /* registering the App.Command handler */
    (void)FMSTR_RegisterAppCmdCall(10, myhandler);
}

/****************************************************************************/
/* Poll function of the FreeMASTER example, called periodically from the 
   main application */

void FMSTR_Example_Poll(void)
{
    static unsigned short div;
    unsigned short nAppCmdCode;
    
    // scope variables, increment once a while
    if(++div > 20)
    {
        var8 += var8inc;
        var16 += var16inc;
        var32 += var32inc;
        var64 += var64inc;
        varFLT = varFLT + varFLTinc;
        varDBL = varDBL + varDBLinc;

        div = 0;
    }

    // the application commands not registered with callback handlers
    // can be detected and processed using the API calls below
        
    // first, check if a new command has been received
    nAppCmdCode = FMSTR_GetAppCmd();

    // when a new command arrives, the nAppCmdCode contains the application 
    // command code. In other case, the "NOCMD" special value is returned
    if (nAppCmdCode != FMSTR_APPCMDRESULT_NOCMD)
    {
        nAppCmdCounter++;
            
        // each command may have different processing and different 
        // result code. The command processing is finished by 
        // calling FMSTR_AppCmdAck() with the result code value
        switch(nAppCmdCode)
        {
            case 1: FMSTR_AppCmdAck(var8); break;
            case 2: FMSTR_AppCmdAck((unsigned char) ~var8); break;
            default: FMSTR_AppCmdAck(0); break;
        }
    }

    // This call should rather be placed in the timer interrupt or anywhere 
    // where the recorder sampling should occur.
    FMSTR_Recorder();

    // The FreeMASTER poll call must be called in the main application loop
    // to handle the communication interface and protocol. 
    // In LONG_INTR FreeMASTER interrupt mode, all the processing is done 
    // during the communication interrupt routine and the FMSTR_Poll() is 
    // compiled empty.
    FMSTR_Poll();
}
