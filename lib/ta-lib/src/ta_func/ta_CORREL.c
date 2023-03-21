/* TA-LIB Copyright (c) 1999-2007, Mario Fortier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * - Neither name of author nor the names of its contributors
 *   may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* List of contributors:
 *
 *  Initial  Name/description
 *  -------------------------------------------------------------------
 *  MF       Mario Fortier
 *
 *
 * Change history:
 *
 *  MMDDYY BY   Description
 *  -------------------------------------------------------------------
 *  120802 MF   Template creation.
 *  101003 MF   Initial Coding
 *  062804 MF   Resolve div by zero bug on limit case.
 *
 */

/**** START GENCODE SECTION 1 - DO NOT DELETE THIS LINE ****/
/* All code within this section is automatically
 * generated by gen_code. Any modification will be lost
 * next time gen_code is run.
 */
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */    #include "TA-Lib-Core.h"
/* Generated */    #define TA_INTERNAL_ERROR(Id) (RetCode::InternalError)
/* Generated */    namespace TicTacTec { namespace TA { namespace Library {
/* Generated */ #elif defined( _JAVA )
/* Generated */    #include "ta_defs.h"
/* Generated */    #include "ta_java_defs.h"
/* Generated */    #define TA_INTERNAL_ERROR(Id) (RetCode.InternalError)
/* Generated */ #else
/* Generated */    #include <string.h>
/* Generated */    #include <math.h>
/* Generated */    #include "ta_func.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #ifndef TA_UTILITY_H
/* Generated */    #include "ta_utility.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #ifndef TA_MEMORY_H
/* Generated */    #include "../ta_common/ta_memory.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #define TA_PREFIX(x) TA_##x
/* Generated */ #define INPUT_TYPE   double
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */ int Core::CorrelLookback( int           optInTimePeriod )  /* From 1 to 100000 */
/* Generated */ 
/* Generated */ #elif defined( _JAVA )
/* Generated */ public int correlLookback( int           optInTimePeriod )  /* From 1 to 100000 */
/* Generated */ 
/* Generated */ #else
/* Generated */ int TA_CORREL_Lookback( int           optInTimePeriod )  /* From 1 to 100000 */
/* Generated */ 
/* Generated */ #endif
/**** END GENCODE SECTION 1 - DO NOT DELETE THIS LINE ****/
{
   /* insert local variable here */

/**** START GENCODE SECTION 2 - DO NOT DELETE THIS LINE ****/
/* Generated */ #ifndef TA_FUNC_NO_RANGE_CHECK
/* Generated */    /* min/max are checked for optInTimePeriod. */
/* Generated */    if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
/* Generated */       optInTimePeriod = 30;
/* Generated */    else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
/* Generated */       return -1;
/* Generated */ 
/* Generated */ #endif /* TA_FUNC_NO_RANGE_CHECK */
/**** END GENCODE SECTION 2 - DO NOT DELETE THIS LINE ****/

   /* insert lookback code here. */
   return optInTimePeriod-1;
}

/**** START GENCODE SECTION 3 - DO NOT DELETE THIS LINE ****/
/*
 * TA_CORREL - Pearson's Correlation Coefficient (r)
 * 
 * Input  = double, double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
/* Generated */ 
/* Generated */ #if defined( _MANAGED ) && defined( USE_SUBARRAY )
/* Generated */ enum class Core::RetCode Core::Correl( int    startIdx,
/* Generated */                                        int    endIdx,
/* Generated */                                        SubArray^    inReal0,
/* Generated */                                        SubArray^    inReal1,
/* Generated */                                        int           optInTimePeriod, /* From 1 to 100000 */
/* Generated */                                        [Out]int%    outBegIdx,
/* Generated */                                        [Out]int%    outNBElement,
/* Generated */                                        cli::array<double>^  outReal )
/* Generated */ #elif defined( _MANAGED )
/* Generated */ enum class Core::RetCode Core::Correl( int    startIdx,
/* Generated */                                        int    endIdx,
/* Generated */                                        cli::array<double>^ inReal0,
/* Generated */                                        cli::array<double>^ inReal1,
/* Generated */                                        int           optInTimePeriod, /* From 1 to 100000 */
/* Generated */                                        [Out]int%    outBegIdx,
/* Generated */                                        [Out]int%    outNBElement,
/* Generated */                                        cli::array<double>^  outReal )
/* Generated */ #elif defined( _JAVA )
/* Generated */ public RetCode correl( int    startIdx,
/* Generated */                        int    endIdx,
/* Generated */                        double       inReal0[],
/* Generated */                        double       inReal1[],
/* Generated */                        int           optInTimePeriod, /* From 1 to 100000 */
/* Generated */                        MInteger     outBegIdx,
/* Generated */                        MInteger     outNBElement,
/* Generated */                        double        outReal[] )
/* Generated */ #else
/* Generated */ TA_RetCode TA_CORREL( int    startIdx,
/* Generated */                       int    endIdx,
/* Generated */                       const double inReal0[],
/* Generated */                       const double inReal1[],
/* Generated */                       int           optInTimePeriod, /* From 1 to 100000 */
/* Generated */                       int          *outBegIdx,
/* Generated */                       int          *outNBElement,
/* Generated */                       double        outReal[] )
/* Generated */ #endif
/**** END GENCODE SECTION 3 - DO NOT DELETE THIS LINE ****/
{
	/* insert local variable here */
    double sumXY, sumX, sumY, sumX2, sumY2, x, y, trailingX, trailingY;
    double tempReal;
    int lookbackTotal, today, trailingIdx, outIdx;

/**** START GENCODE SECTION 4 - DO NOT DELETE THIS LINE ****/
/* Generated */ 
/* Generated */ #ifndef TA_FUNC_NO_RANGE_CHECK
/* Generated */ 
/* Generated */    /* Validate the requested output range. */
/* Generated */    if( startIdx < 0 )
/* Generated */       return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_START_INDEX,OutOfRangeStartIndex);
/* Generated */    if( (endIdx < 0) || (endIdx < startIdx))
/* Generated */       return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_END_INDEX,OutOfRangeEndIndex);
/* Generated */ 
/* Generated */    #if !defined(_JAVA)
/* Generated */    if( !inReal0 ) return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */    if( !inReal1 ) return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */    #endif /* !defined(_JAVA)*/
/* Generated */    /* min/max are checked for optInTimePeriod. */
/* Generated */    if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
/* Generated */       optInTimePeriod = 30;
/* Generated */    else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
/* Generated */       return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */ 
/* Generated */    #if !defined(_JAVA)
/* Generated */    if( !outReal )
/* Generated */       return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */ 
/* Generated */    #endif /* !defined(_JAVA) */
/* Generated */ #endif /* TA_FUNC_NO_RANGE_CHECK */
/* Generated */ 
/**** END GENCODE SECTION 4 - DO NOT DELETE THIS LINE ****/

   /* Insert TA function code here. */

   /* Move up the start index if there is not
    * enough initial data.
    */
   lookbackTotal = optInTimePeriod-1;
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      VALUE_HANDLE_DEREF_TO_ZERO(outBegIdx);
      VALUE_HANDLE_DEREF_TO_ZERO(outNBElement);
      return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
   }

   VALUE_HANDLE_DEREF(outBegIdx)  = startIdx;
   trailingIdx = startIdx - lookbackTotal;

   /* Calculate the initial values. */
   sumXY = sumX = sumY = sumX2 = sumY2 = 0.0;
   for( today=trailingIdx; today <= startIdx; today++ )
   {
      x = inReal0[today];
      sumX  += x;
      sumX2 += x*x;

      y = inReal1[today];
      sumXY += x*y;
      sumY  += y;
      sumY2 += y*y;
   }

   /* Write the first output. 
    * Save first the trailing values since the input
    * and output might be the same array,
    */
   trailingX = inReal0[trailingIdx];
   trailingY = inReal1[trailingIdx++];
   tempReal = (sumX2-((sumX*sumX)/optInTimePeriod)) * (sumY2-((sumY*sumY)/optInTimePeriod));
   if( !TA_IS_ZERO_OR_NEG(tempReal) )
      outReal[0] = (sumXY-((sumX*sumY)/optInTimePeriod)) / std_sqrt(tempReal);
   else 
      outReal[0] = 0.0;

   /* Tight loop to do subsequent values. */
   outIdx = 1;
   while( today <= endIdx )
   {
      /* Remove trailing values */
      sumX  -= trailingX;
      sumX2 -= trailingX*trailingX;

      sumXY -= trailingX*trailingY;
      sumY  -= trailingY;
      sumY2 -= trailingY*trailingY;

      /* Add new values */
      x = inReal0[today];
      sumX  += x;
      sumX2 += x*x;

      y = inReal1[today++];
      sumXY += x*y;
      sumY  += y;
      sumY2 += y*y;

      /* Output new coefficient.
       * Save first the trailing values since the input
       * and output might be the same array,
       */
      trailingX = inReal0[trailingIdx];
      trailingY = inReal1[trailingIdx++];
      tempReal = (sumX2-((sumX*sumX)/optInTimePeriod)) * (sumY2-((sumY*sumY)/optInTimePeriod));
      if( !TA_IS_ZERO_OR_NEG(tempReal) )
         outReal[outIdx++] = (sumXY-((sumX*sumY)/optInTimePeriod)) / std_sqrt(tempReal);
      else
         outReal[outIdx++] = 0.0;
   }  

   VALUE_HANDLE_DEREF(outNBElement) = outIdx;

   return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
}

/**** START GENCODE SECTION 5 - DO NOT DELETE THIS LINE ****/
/* Generated */ 
/* Generated */ #define  USE_SINGLE_PRECISION_INPUT
/* Generated */ #if !defined( _MANAGED ) && !defined( _JAVA )
/* Generated */    #undef   TA_PREFIX
/* Generated */    #define  TA_PREFIX(x) TA_S_##x
/* Generated */ #endif
/* Generated */ #undef   INPUT_TYPE
/* Generated */ #define  INPUT_TYPE float
/* Generated */ #if defined( _MANAGED )
/* Generated */ enum class Core::RetCode Core::Correl( int    startIdx,
/* Generated */                                        int    endIdx,
/* Generated */                                        cli::array<float>^ inReal0,
/* Generated */                                        cli::array<float>^ inReal1,
/* Generated */                                        int           optInTimePeriod, /* From 1 to 100000 */
/* Generated */                                        [Out]int%    outBegIdx,
/* Generated */                                        [Out]int%    outNBElement,
/* Generated */                                        cli::array<double>^  outReal )
/* Generated */ #elif defined( _JAVA )
/* Generated */ public RetCode correl( int    startIdx,
/* Generated */                        int    endIdx,
/* Generated */                        float        inReal0[],
/* Generated */                        float        inReal1[],
/* Generated */                        int           optInTimePeriod, /* From 1 to 100000 */
/* Generated */                        MInteger     outBegIdx,
/* Generated */                        MInteger     outNBElement,
/* Generated */                        double        outReal[] )
/* Generated */ #else
/* Generated */ TA_RetCode TA_S_CORREL( int    startIdx,
/* Generated */                         int    endIdx,
/* Generated */                         const float  inReal0[],
/* Generated */                         const float  inReal1[],
/* Generated */                         int           optInTimePeriod, /* From 1 to 100000 */
/* Generated */                         int          *outBegIdx,
/* Generated */                         int          *outNBElement,
/* Generated */                         double        outReal[] )
/* Generated */ #endif
/* Generated */ {
/* Generated */     double sumXY, sumX, sumY, sumX2, sumY2, x, y, trailingX, trailingY;
/* Generated */     double tempReal;
/* Generated */     int lookbackTotal, today, trailingIdx, outIdx;
/* Generated */  #ifndef TA_FUNC_NO_RANGE_CHECK
/* Generated */     if( startIdx < 0 )
/* Generated */        return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_START_INDEX,OutOfRangeStartIndex);
/* Generated */     if( (endIdx < 0) || (endIdx < startIdx))
/* Generated */        return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_END_INDEX,OutOfRangeEndIndex);
/* Generated */     #if !defined(_JAVA)
/* Generated */     if( !inReal0 ) return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */     if( !inReal1 ) return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */     #endif 
/* Generated */     if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
/* Generated */        optInTimePeriod = 30;
/* Generated */     else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
/* Generated */        return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */     #if !defined(_JAVA)
/* Generated */     if( !outReal )
/* Generated */        return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */     #endif 
/* Generated */  #endif 
/* Generated */    lookbackTotal = optInTimePeriod-1;
/* Generated */    if( startIdx < lookbackTotal )
/* Generated */       startIdx = lookbackTotal;
/* Generated */    if( startIdx > endIdx )
/* Generated */    {
/* Generated */       VALUE_HANDLE_DEREF_TO_ZERO(outBegIdx);
/* Generated */       VALUE_HANDLE_DEREF_TO_ZERO(outNBElement);
/* Generated */       return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
/* Generated */    }
/* Generated */    VALUE_HANDLE_DEREF(outBegIdx)  = startIdx;
/* Generated */    trailingIdx = startIdx - lookbackTotal;
/* Generated */    sumXY = sumX = sumY = sumX2 = sumY2 = 0.0;
/* Generated */    for( today=trailingIdx; today <= startIdx; today++ )
/* Generated */    {
/* Generated */       x = inReal0[today];
/* Generated */       sumX  += x;
/* Generated */       sumX2 += x*x;
/* Generated */       y = inReal1[today];
/* Generated */       sumXY += x*y;
/* Generated */       sumY  += y;
/* Generated */       sumY2 += y*y;
/* Generated */    }
/* Generated */    trailingX = inReal0[trailingIdx];
/* Generated */    trailingY = inReal1[trailingIdx++];
/* Generated */    tempReal = (sumX2-((sumX*sumX)/optInTimePeriod)) * (sumY2-((sumY*sumY)/optInTimePeriod));
/* Generated */    if( !TA_IS_ZERO_OR_NEG(tempReal) )
/* Generated */       outReal[0] = (sumXY-((sumX*sumY)/optInTimePeriod)) / std_sqrt(tempReal);
/* Generated */    else 
/* Generated */       outReal[0] = 0.0;
/* Generated */    outIdx = 1;
/* Generated */    while( today <= endIdx )
/* Generated */    {
/* Generated */       sumX  -= trailingX;
/* Generated */       sumX2 -= trailingX*trailingX;
/* Generated */       sumXY -= trailingX*trailingY;
/* Generated */       sumY  -= trailingY;
/* Generated */       sumY2 -= trailingY*trailingY;
/* Generated */       x = inReal0[today];
/* Generated */       sumX  += x;
/* Generated */       sumX2 += x*x;
/* Generated */       y = inReal1[today++];
/* Generated */       sumXY += x*y;
/* Generated */       sumY  += y;
/* Generated */       sumY2 += y*y;
/* Generated */       trailingX = inReal0[trailingIdx];
/* Generated */       trailingY = inReal1[trailingIdx++];
/* Generated */       tempReal = (sumX2-((sumX*sumX)/optInTimePeriod)) * (sumY2-((sumY*sumY)/optInTimePeriod));
/* Generated */       if( !TA_IS_ZERO_OR_NEG(tempReal) )
/* Generated */          outReal[outIdx++] = (sumXY-((sumX*sumY)/optInTimePeriod)) / std_sqrt(tempReal);
/* Generated */       else
/* Generated */          outReal[outIdx++] = 0.0;
/* Generated */    }  
/* Generated */    VALUE_HANDLE_DEREF(outNBElement) = outIdx;
/* Generated */    return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
/* Generated */ }
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */ }}} // Close namespace TicTacTec.TA.Lib
/* Generated */ #endif
/**** END GENCODE SECTION 5 - DO NOT DELETE THIS LINE ****/

