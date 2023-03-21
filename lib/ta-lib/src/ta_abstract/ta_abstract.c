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
 *  AC       Angelo Ciceri
 *
 *
 * Change history:
 *
 *  MMDDYY BY   Description
 *  -------------------------------------------------------------------
 *  020101 MF   First version.
 *  110302 MF   Re-design the interface related to TA_CallFunc for being
 *              faster and use less memory allocation.
 *  022904 MF   Add TA_GetLookback
 *  031404 MF   Some function renaming for consistency and better
 *              Perl integration.
 *  110206 AC   Change volume and open interest to double
 */

/* Description:
 *   Provide a way to abstract the call of the TA functions.
 */

/**** Headers ****/
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include "ta_common.h"
#include "../ta_common/ta_memory.h"
#include "ta_abstract.h"
#include "ta_def_ui.h"
#include "ta_frame_priv.h"

#include <limits.h>

/**** External functions declarations. ****/
/* None */

/**** External variables declarations. ****/

/* The interface definition of all functions are accessible
 * through one of the following 26 tables.
 */
extern const TA_FuncDef *TA_DEF_TableA, *TA_DEF_TableB, *TA_DEF_TableC,
    *TA_DEF_TableD, *TA_DEF_TableE, *TA_DEF_TableF,
    *TA_DEF_TableG, *TA_DEF_TableH, *TA_DEF_TableI,
    *TA_DEF_TableJ, *TA_DEF_TableK, *TA_DEF_TableL,
    *TA_DEF_TableM, *TA_DEF_TableN, *TA_DEF_TableO,
    *TA_DEF_TableP, *TA_DEF_TableQ, *TA_DEF_TableR,
    *TA_DEF_TableS, *TA_DEF_TableT, *TA_DEF_TableU,
    *TA_DEF_TableV, *TA_DEF_TableW, *TA_DEF_TableX,
    *TA_DEF_TableY, *TA_DEF_TableZ;

extern const unsigned int TA_DEF_TableASize, TA_DEF_TableBSize,
    TA_DEF_TableCSize, TA_DEF_TableDSize,
    TA_DEF_TableESize, TA_DEF_TableFSize,
    TA_DEF_TableGSize, TA_DEF_TableHSize,
    TA_DEF_TableISize, TA_DEF_TableJSize,
    TA_DEF_TableKSize, TA_DEF_TableLSize,
    TA_DEF_TableMSize, TA_DEF_TableNSize,
    TA_DEF_TableOSize, TA_DEF_TablePSize,
    TA_DEF_TableQSize, TA_DEF_TableRSize,
    TA_DEF_TableSSize, TA_DEF_TableTSize,
    TA_DEF_TableUSize, TA_DEF_TableVSize,
    TA_DEF_TableWSize, TA_DEF_TableXSize,
    TA_DEF_TableYSize, TA_DEF_TableZSize;

#ifndef TA_GEN_CODE
/* In gen_code, these value does not exist (they are generated by
 * gen_code itself!)
 * Consequently, the code, when being used to make the gen_code
 * executable, must provide the same functionality without using
 * these shortcuts.
 */
extern const TA_FuncDef **TA_PerGroupFuncDef[];
extern const unsigned int TA_PerGroupSize[];
#endif

/**** Global variables definitions.    ****/
/* None */

/**** Local declarations.              ****/
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

typedef struct
{
   unsigned int magicNumber;
} TA_StringTablePriv;

/**** Local functions declarations.    ****/

#ifdef TA_GEN_CODE
static TA_RetCode getGroupId(const char *groupString, unsigned int *groupId);
static TA_RetCode getGroupSize(TA_GroupId groupId, unsigned int *groupSize);
static TA_RetCode getFuncNameByIdx(TA_GroupId groupId,
                                   unsigned int idx,
                                   const char **stringPtr);
#else
static TA_RetCode getGroupId(const char *groupString, unsigned int *groupId);
static TA_RetCode getGroupSize(TA_GroupId groupId, unsigned int *groupSize);
static TA_RetCode getFuncNameByIdx(TA_GroupId groupId,
                                   unsigned int idx,
                                   const char **stringPtr);
#endif

/**** Local variables definitions.     ****/

static const TA_FuncDef **TA_DEF_Tables[26] =
    {
        &TA_DEF_TableA, &TA_DEF_TableB, &TA_DEF_TableC, &TA_DEF_TableD, &TA_DEF_TableE,
        &TA_DEF_TableF, &TA_DEF_TableG, &TA_DEF_TableH, &TA_DEF_TableI, &TA_DEF_TableJ,
        &TA_DEF_TableK, &TA_DEF_TableL, &TA_DEF_TableM, &TA_DEF_TableN, &TA_DEF_TableO,
        &TA_DEF_TableP, &TA_DEF_TableQ, &TA_DEF_TableR, &TA_DEF_TableS, &TA_DEF_TableT,
        &TA_DEF_TableU, &TA_DEF_TableV, &TA_DEF_TableW, &TA_DEF_TableX, &TA_DEF_TableY,
        &TA_DEF_TableZ};

static const unsigned int *TA_DEF_TablesSize[26] =
    {
        &TA_DEF_TableASize, &TA_DEF_TableBSize, &TA_DEF_TableCSize,
        &TA_DEF_TableDSize, &TA_DEF_TableESize, &TA_DEF_TableFSize,
        &TA_DEF_TableGSize, &TA_DEF_TableHSize, &TA_DEF_TableISize,
        &TA_DEF_TableJSize, &TA_DEF_TableKSize, &TA_DEF_TableLSize,
        &TA_DEF_TableMSize, &TA_DEF_TableNSize, &TA_DEF_TableOSize,
        &TA_DEF_TablePSize, &TA_DEF_TableQSize, &TA_DEF_TableRSize,
        &TA_DEF_TableSSize, &TA_DEF_TableTSize, &TA_DEF_TableUSize,
        &TA_DEF_TableVSize, &TA_DEF_TableWSize, &TA_DEF_TableXSize,
        &TA_DEF_TableYSize, &TA_DEF_TableZSize};

/**** Global functions definitions.   ****/
TA_RetCode TA_GroupTableAlloc(TA_StringTable **table)
{
   TA_StringTable *stringTable;
   TA_StringTablePriv *stringTablePriv;

   if (table == NULL)
   {
      return TA_BAD_PARAM;
   }

   stringTable = (TA_StringTable *)TA_Malloc(sizeof(TA_StringTable) + sizeof(TA_StringTablePriv));
   if (!stringTable)
   {
      *table = NULL;
      return TA_ALLOC_ERR;
   }

   memset(stringTable, 0, sizeof(TA_StringTable) + sizeof(TA_StringTablePriv));
   stringTablePriv = (TA_StringTablePriv *)(((char *)stringTable) + sizeof(TA_StringTable));
   stringTablePriv->magicNumber = TA_STRING_TABLE_GROUP_MAGIC_NB;

   stringTable->size = TA_NB_GROUP_ID;
   stringTable->string = &TA_GroupString[0];
   stringTable->hiddenData = stringTablePriv;

   /* From this point, TA_FuncTableFree can be safely called. */

   /* Success. Return the table to the caller. */
   *table = stringTable;

   return TA_SUCCESS;
}

TA_RetCode TA_GroupTableFree(TA_StringTable *table)
{
   TA_StringTablePriv *stringTablePriv;

   if (table)
   {
      stringTablePriv = (TA_StringTablePriv *)table->hiddenData;
      if (!stringTablePriv)
      {
         return TA_INTERNAL_ERROR(1);
      }

      if (stringTablePriv->magicNumber != TA_STRING_TABLE_GROUP_MAGIC_NB)
      {
         return TA_BAD_OBJECT;
      }

      TA_Free(table);
   }

   return TA_SUCCESS;
}

/* Iterate in alphabetical order */
TA_RetCode TA_ForEachFunc(TA_CallForEachFunc functionToCall, void *opaqueData)
{
   const TA_FuncDef **funcDefTable;
   const TA_FuncDef *funcDef;
   const TA_FuncInfo *funcInfo;
   unsigned int i, j, funcDefTableSize;

   if (functionToCall == NULL)
   {
      return TA_BAD_PARAM;
   }

   /* Iterate the tables (each table is for one letter) */
   for (i = 0; i < 26; i++)
   {
      funcDefTable = TA_DEF_Tables[i];

      /* Identify the table size. */
      funcDefTableSize = *TA_DEF_TablesSize[i];
      for (j = 0; j < funcDefTableSize; j++)
      {
         funcDef = funcDefTable[j];
         if (!funcDef || !funcDef->funcInfo)
            return TA_INTERNAL_ERROR(3);

         funcInfo = funcDef->funcInfo;
         if (!funcInfo)
            return TA_INTERNAL_ERROR(4);
         (*functionToCall)(funcInfo, opaqueData);
      }
   }

   return TA_SUCCESS;
}

TA_RetCode TA_FuncTableAlloc(const char *group, TA_StringTable **table)
{
   TA_RetCode retCode;
   unsigned int i;
   TA_StringTable *stringTable;
   unsigned int groupId; /* TA_GroupId */
   unsigned int groupSize;
   const char *stringPtr;
   TA_StringTablePriv *stringTablePriv;

   if ((group == NULL) || (table == NULL))
   {
      return TA_BAD_PARAM;
   }

   *table = NULL;
   stringPtr = NULL;

   /* Get information on the group. */
   retCode = getGroupId(group, &groupId);
   if (retCode != TA_SUCCESS)
   {
      return retCode;
   }

   retCode = getGroupSize((TA_GroupId)groupId, &groupSize);
   if (retCode != TA_SUCCESS)
   {
      return retCode;
   }

   /* Allocate the table. */

   stringTable = (TA_StringTable *)TA_Malloc(sizeof(TA_StringTable) + sizeof(TA_StringTablePriv));
   if (!stringTable)
   {
      *table = NULL;
      return TA_ALLOC_ERR;
   }

   memset(stringTable, 0, sizeof(TA_StringTable) + sizeof(TA_StringTablePriv));
   stringTablePriv = (TA_StringTablePriv *)(((char *)stringTable) + sizeof(TA_StringTable));
   stringTablePriv->magicNumber = TA_STRING_TABLE_FUNC_MAGIC_NB;
   stringTable->hiddenData = stringTablePriv;

   /* From this point, TA_FuncTableFree can be safely called. */
   stringTable->size = groupSize;
   if (groupSize != 0)
   {
      stringTable->string = (const char **)TA_Malloc((stringTable->size) *
                                                     sizeof(const char *));

      if (stringTable->string == NULL)
      {
         *table = NULL;
         TA_FuncTableFree(stringTable);
         return TA_ALLOC_ERR;
      }

      memset((void *)stringTable->string, 0,
             (stringTable->size) * sizeof(const char *));

      for (i = 0; i < stringTable->size; i++)
      {
         retCode = getFuncNameByIdx((TA_GroupId)groupId, i, &stringPtr);

         if (retCode != TA_SUCCESS)
         {
            *table = NULL;
            TA_FuncTableFree(stringTable);
            return TA_ALLOC_ERR;
         }

         (stringTable->string)[i] = stringPtr;
      }
   }

   /* Return the table to the caller. */
   *table = stringTable;

   return TA_SUCCESS;
}

TA_RetCode TA_FuncTableFree(TA_StringTable *table)
{
   TA_StringTablePriv *stringTablePriv;

   if (table)
   {
      stringTablePriv = (TA_StringTablePriv *)table->hiddenData;
      if (!stringTablePriv)
      {
         return TA_INTERNAL_ERROR(3);
      }

      if (stringTablePriv->magicNumber != TA_STRING_TABLE_FUNC_MAGIC_NB)
      {
         return TA_BAD_OBJECT;
      }

      if (table->string)
         TA_Free((void *)table->string);

      TA_Free(table);
   }

   return TA_SUCCESS;
}

TA_RetCode TA_GetFuncHandle(const char *name, const TA_FuncHandle **handle)
{
   char firstChar, tmp;
   const TA_FuncDef **funcDefTable;
   const TA_FuncDef *funcDef;
   const TA_FuncInfo *funcInfo;
   unsigned int i, funcDefTableSize;

   /* A TA_FuncHandle is internally a TA_FuncDef. Let's find it
    * by using the alphabetical tables.
    */
   if ((name == NULL) || (handle == NULL))
   {
      return TA_BAD_PARAM;
   }

   *handle = NULL;

   firstChar = name[0];

   if (firstChar == '\0')
   {
      return TA_BAD_PARAM;
   }

   tmp = (char)tolower(firstChar);

   if ((tmp < 'a') || (tmp > 'z'))
   {
      return TA_FUNC_NOT_FOUND;
   }

   /* Identify the table. */
   tmp -= (char)'a';
   funcDefTable = TA_DEF_Tables[(int)tmp];

   /* Identify the table size. */
   funcDefTableSize = *TA_DEF_TablesSize[(int)tmp];
   if (funcDefTableSize < 1)
   {
      return TA_FUNC_NOT_FOUND;
   }

   /* Iterate all entries of the table and return as soon as found. */
   for (i = 0; i < funcDefTableSize; i++)
   {
      funcDef = funcDefTable[i];
      if (!funcDef || !funcDef->funcInfo)
         return TA_INTERNAL_ERROR(3);

      funcInfo = funcDef->funcInfo;
      if (!funcInfo)
         return TA_INTERNAL_ERROR(4);

      if (strcmp(funcInfo->name, name) == 0)
      {
         *handle = (TA_FuncHandle *)funcDef;
         return TA_SUCCESS;
      }
   }

   return TA_FUNC_NOT_FOUND;
}

TA_RetCode TA_GetFuncInfo(const TA_FuncHandle *handle,
                          const TA_FuncInfo **funcInfo)
{
   const TA_FuncDef *funcDef;

   if (!funcInfo || !handle)
   {
      return TA_BAD_PARAM;
   }

   /* Validate that this is a valid funcHandle. */
   funcDef = (const TA_FuncDef *)handle;
   if (funcDef->magicNumber != TA_FUNC_DEF_MAGIC_NB)
   {
      return TA_INVALID_HANDLE;
   }

   *funcInfo = funcDef->funcInfo;
   if (!funcDef->funcInfo)
      return TA_INVALID_HANDLE;

   return TA_SUCCESS;
}

TA_RetCode TA_GetInputParameterInfo(const TA_FuncHandle *handle,
                                    unsigned int paramIndex,
                                    const TA_InputParameterInfo **info)
{
   const TA_FuncDef *funcDef;
   const TA_FuncInfo *funcInfo;
   const TA_InputParameterInfo **inputTable;

   if ((handle == NULL) || (info == NULL))
   {
      return TA_BAD_PARAM;
   }

   *info = NULL;

   /* Validate that this is a valid funcHandle. */
   funcDef = (const TA_FuncDef *)handle;
   if (funcDef->magicNumber != TA_FUNC_DEF_MAGIC_NB)
   {
      return TA_INVALID_HANDLE;
   }
   funcInfo = funcDef->funcInfo;

   if (!funcInfo)
      return TA_INVALID_HANDLE;

   if (paramIndex >= funcInfo->nbInput)
   {
      return TA_BAD_PARAM;
   }

   inputTable = (const TA_InputParameterInfo **)funcDef->input;

   if (!inputTable)
      return TA_INTERNAL_ERROR(2);

   *info = inputTable[paramIndex];

   if (!(*info))
      return TA_INTERNAL_ERROR(3);

   return TA_SUCCESS;
}

TA_RetCode TA_GetOptInputParameterInfo(const TA_FuncHandle *handle,
                                       unsigned int paramIndex,
                                       const TA_OptInputParameterInfo **info)
{
   const TA_FuncDef *funcDef;
   const TA_FuncInfo *funcInfo;
   const TA_OptInputParameterInfo **inputTable;

   if ((handle == NULL) || (info == NULL))
   {
      return TA_BAD_PARAM;
   }

   *info = NULL;

   /* Validate that this is a valid funcHandle. */
   funcDef = (const TA_FuncDef *)handle;
   if (funcDef->magicNumber != TA_FUNC_DEF_MAGIC_NB)
   {
      return TA_INVALID_HANDLE;
   }

   funcInfo = funcDef->funcInfo;

   if (!funcInfo)
      return TA_INVALID_HANDLE;

   if (paramIndex >= funcInfo->nbOptInput)
   {
      return TA_BAD_PARAM;
   }

   inputTable = (const TA_OptInputParameterInfo **)funcDef->optInput;

   if (!inputTable)
      return TA_INTERNAL_ERROR(3);

   *info = inputTable[paramIndex];

   if (!(*info))
      return TA_INTERNAL_ERROR(4);

   return TA_SUCCESS;
}

TA_RetCode TA_GetOutputParameterInfo(const TA_FuncHandle *handle,
                                     unsigned int paramIndex,
                                     const TA_OutputParameterInfo **info)
{

   const TA_FuncDef *funcDef;
   const TA_FuncInfo *funcInfo;
   const TA_OutputParameterInfo **outputTable;

   if ((handle == NULL) || (info == NULL))
   {
      return TA_BAD_PARAM;
   }

   *info = NULL;

   /* Validate that this is a valid funcHandle. */
   funcDef = (const TA_FuncDef *)handle;
   if (funcDef->magicNumber != TA_FUNC_DEF_MAGIC_NB)
   {
      return TA_INVALID_HANDLE;
   }

   funcInfo = funcDef->funcInfo;

   if (!funcInfo)
   {
      return TA_INVALID_HANDLE;
   }

   if (paramIndex >= funcInfo->nbOutput)
   {
      return TA_BAD_PARAM;
   }

   outputTable = (const TA_OutputParameterInfo **)funcDef->output;

   if (!outputTable)
   {
      return TA_INTERNAL_ERROR(4);
   }

   *info = outputTable[paramIndex];

   if (!(*info))
   {
      return TA_INTERNAL_ERROR(5);
   }

   return TA_SUCCESS;
}

TA_RetCode TA_ParamHolderAlloc(const TA_FuncHandle *handle,
                               TA_ParamHolder **allocatedParams)
{

   TA_FuncDef *funcDef;
   unsigned int allocSize, i;
   TA_ParamHolderInput *input;
   TA_ParamHolderOptInput *optInput;
   TA_ParamHolderOutput *output;

   const TA_FuncInfo *funcInfo;
   TA_ParamHolder *newParams;
   TA_ParamHolderPriv *newParamsPriv;

   const TA_InputParameterInfo **inputInfo;
   const TA_OptInputParameterInfo **optInputInfo;
   const TA_OutputParameterInfo **outputInfo;

   /* Validate the parameters. */
   if (!handle || !allocatedParams)
   {
      return TA_BAD_PARAM;
   }

   /* Validate that this is a valid funcHandle. */
   funcDef = (TA_FuncDef *)handle;
   if (funcDef->magicNumber != TA_FUNC_DEF_MAGIC_NB)
   {
      *allocatedParams = NULL;
      return TA_INVALID_HANDLE;
   }

   /* Get the TA_FuncInfo. */
   funcInfo = funcDef->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;

   /* Allocate the TA_ParamHolder. */
   newParams = (TA_ParamHolder *)TA_Malloc(sizeof(TA_ParamHolder) + sizeof(TA_ParamHolderPriv));
   if (!newParams)
   {
      *allocatedParams = NULL;
      return TA_ALLOC_ERR;
   }

   memset(newParams, 0, sizeof(TA_ParamHolder) + sizeof(TA_ParamHolderPriv));
   newParamsPriv = (TA_ParamHolderPriv *)(((char *)newParams) + sizeof(TA_ParamHolder));
   newParamsPriv->magicNumber = TA_PARAM_HOLDER_PRIV_MAGIC_NB;
   newParams->hiddenData = newParamsPriv;

   /* From this point, TA_ParamHolderFree can be safely called. */

   /* Allocate the array of structure holding the info
    * for each parameter.
    */
   if (funcInfo->nbInput == 0)
      return TA_INTERNAL_ERROR(2);

   allocSize = (funcInfo->nbInput) * sizeof(TA_ParamHolderInput);
   input = (TA_ParamHolderInput *)TA_Malloc(allocSize);

   if (!input)
   {
      TA_ParamHolderFree(newParams);
      *allocatedParams = NULL;
      return TA_ALLOC_ERR;
   }
   memset(input, 0, allocSize);
   newParamsPriv->in = input;

   if (funcInfo->nbOptInput == 0)
      optInput = NULL;
   else
   {
      allocSize = (funcInfo->nbOptInput) * sizeof(TA_ParamHolderOptInput);
      optInput = (TA_ParamHolderOptInput *)TA_Malloc(allocSize);

      if (!optInput)
      {
         TA_ParamHolderFree(newParams);
         *allocatedParams = NULL;
         return TA_ALLOC_ERR;
      }
      memset(optInput, 0, allocSize);
   }
   newParamsPriv->optIn = optInput;

   allocSize = (funcInfo->nbOutput) * sizeof(TA_ParamHolderOutput);
   output = (TA_ParamHolderOutput *)TA_Malloc(allocSize);
   if (!output)
   {
      TA_ParamHolderFree(newParams);
      *allocatedParams = NULL;
      return TA_ALLOC_ERR;
   }
   memset(output, 0, allocSize);
   newParamsPriv->out = output;

   newParamsPriv->funcInfo = funcInfo;

   inputInfo = (const TA_InputParameterInfo **)funcDef->input;
   optInputInfo = (const TA_OptInputParameterInfo **)funcDef->optInput;
   outputInfo = (const TA_OutputParameterInfo **)funcDef->output;

   for (i = 0; i < funcInfo->nbInput; i++)
   {
      input[i].inputInfo = inputInfo[i];
      newParamsPriv->inBitmap <<= 1;
      newParamsPriv->inBitmap |= 1;
   }

   for (i = 0; i < funcInfo->nbOptInput; i++)
   {
      optInput[i].optInputInfo = optInputInfo[i];
      if (optInput[i].optInputInfo->type == TA_OptInput_RealRange)
         optInput[i].data.optInReal = optInputInfo[i]->defaultValue;
      else
         optInput[i].data.optInInteger = (TA_Integer)optInputInfo[i]->defaultValue;
   }

   for (i = 0; i < funcInfo->nbOutput; i++)
   {
      output[i].outputInfo = outputInfo[i];
      newParamsPriv->outBitmap <<= 1;
      newParamsPriv->outBitmap |= 1;
   }

   /* Succcess, return the result to the caller. */
   *allocatedParams = newParams;

   return TA_SUCCESS;
}

TA_RetCode TA_ParamHolderFree(TA_ParamHolder *paramsToFree)
{
   TA_ParamHolderPriv *paramPriv;

   TA_ParamHolderInput *input;
   TA_ParamHolderOptInput *optInput;
   TA_ParamHolderOutput *output;

   if (!paramsToFree)
   {
      return TA_SUCCESS;
   }

   paramPriv = paramsToFree->hiddenData;

   if (!paramPriv)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   if (paramPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   optInput = paramPriv->optIn;
   if (optInput)
      TA_Free(optInput);

   input = paramPriv->in;
   if (input)
      TA_Free(input);

   output = paramPriv->out;
   if (output)
      TA_Free(output);

   TA_Free(paramsToFree);

   return TA_SUCCESS;
}

TA_RetCode TA_SetInputParamIntegerPtr(TA_ParamHolder *param,
                                      unsigned int paramIndex,
                                      const TA_Integer *value)
{

   TA_ParamHolderPriv *paramHolderPriv;
   const TA_InputParameterInfo *paramInfo;
   const TA_FuncInfo *funcInfo;

   if ((param == NULL) || (value == NULL))
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Make sure this index really exist. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;

   if (paramIndex >= funcInfo->nbInput)
   {
      return TA_BAD_PARAM;
   }

   /* Verify the type of the parameter. */
   paramInfo = paramHolderPriv->in[paramIndex].inputInfo;
   if (!paramInfo)
      return TA_INTERNAL_ERROR(2);
   if (paramInfo->type != TA_Input_Integer)
   {
      return TA_INVALID_PARAM_HOLDER_TYPE;
   }

   /* keep a copy of the provided parameter. */
   paramHolderPriv->in[paramIndex].data.inInteger = value;

   /* This parameter is now initialized, clear the corresponding bit. */
   paramHolderPriv->inBitmap &= ~(1 << paramIndex);

   return TA_SUCCESS;
}

TA_RetCode TA_SetInputParamRealPtr(TA_ParamHolder *param,
                                   unsigned int paramIndex,
                                   const TA_Real *value)
{
   TA_ParamHolderPriv *paramHolderPriv;
   const TA_InputParameterInfo *paramInfo;
   const TA_FuncInfo *funcInfo;

   if ((param == NULL) || (value == NULL))
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Make sure this index really exist. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;

   if (paramIndex >= funcInfo->nbInput)
   {
      return TA_BAD_PARAM;
   }

   /* Verify the type of the parameter. */
   paramInfo = paramHolderPriv->in[paramIndex].inputInfo;
   if (!paramInfo)
      return TA_INTERNAL_ERROR(2);
   if (paramInfo->type != TA_Input_Real)
   {
      return TA_INVALID_PARAM_HOLDER_TYPE;
   }

   /* keep a copy of the provided parameter. */
   paramHolderPriv->in[paramIndex].data.inReal = value;

   /* This parameter is now initialized, clear the corresponding bit. */
   paramHolderPriv->inBitmap &= ~(1 << paramIndex);

   return TA_SUCCESS;
}

TA_RetCode TA_SetInputParamPricePtr(TA_ParamHolder *param,
                                    unsigned int paramIndex,
                                    const TA_Real *open,
                                    const TA_Real *high,
                                    const TA_Real *low,
                                    const TA_Real *close,
                                    const TA_Real *volume,
                                    const TA_Real *openInterest)
{

   TA_ParamHolderPriv *paramHolderPriv;
   const TA_InputParameterInfo *paramInfo;
   const TA_FuncInfo *funcInfo;

   if (param == NULL)
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Make sure this index really exist. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;
   if (paramIndex >= funcInfo->nbInput)
   {
      return TA_BAD_PARAM;
   }

   /* Verify the type of the parameter. */
   paramInfo = paramHolderPriv->in[paramIndex].inputInfo;
   if (!paramInfo)
      return TA_INTERNAL_ERROR(2);
   if (paramInfo->type != TA_Input_Price)
   {
      return TA_INVALID_PARAM_HOLDER_TYPE;
   }

/* keep a copy of the provided parameter. */
#define SET_PARAM_INFO(lowerParam, upperParam)                                 \
   {                                                                           \
      if (paramInfo->flags & TA_IN_PRICE_##upperParam)                         \
      {                                                                        \
         if (lowerParam == NULL)                                               \
         {                                                                     \
            return TA_BAD_PARAM;                                               \
         }                                                                     \
         paramHolderPriv->in[paramIndex].data.inPrice.lowerParam = lowerParam; \
      }                                                                        \
   }

   SET_PARAM_INFO(open, OPEN);
   SET_PARAM_INFO(high, HIGH);
   SET_PARAM_INFO(low, LOW);
   SET_PARAM_INFO(close, CLOSE);
   SET_PARAM_INFO(volume, VOLUME);
   SET_PARAM_INFO(openInterest, OPENINTEREST);

#undef SET_PARAM_INFO

   /* This parameter is now initialized, clear the corresponding bit. */
   paramHolderPriv->inBitmap &= ~(1 << paramIndex);

   return TA_SUCCESS;
}

TA_RetCode TA_SetOptInputParamInteger(TA_ParamHolder *param,
                                      unsigned int paramIndex,
                                      TA_Integer value)
{

   TA_ParamHolderPriv *paramHolderPriv;
   const TA_OptInputParameterInfo *paramInfo;
   const TA_FuncInfo *funcInfo;

   if (param == NULL)
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Make sure this index really exist. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;
   if (paramIndex >= funcInfo->nbOptInput)
   {
      return TA_BAD_PARAM;
   }

   /* Verify the type of the parameter. */
   paramInfo = paramHolderPriv->optIn[paramIndex].optInputInfo;
   if (!paramInfo)
      return TA_INTERNAL_ERROR(2);
   if ((paramInfo->type != TA_OptInput_IntegerRange) &&
       (paramInfo->type != TA_OptInput_IntegerList))
   {
      return TA_INVALID_PARAM_HOLDER_TYPE;
   }

   /* keep a copy of the provided parameter. */
   paramHolderPriv->optIn[paramIndex].data.optInInteger = value;

   return TA_SUCCESS;
}

TA_RetCode TA_SetOptInputParamReal(TA_ParamHolder *param,
                                   unsigned int paramIndex,
                                   TA_Real value)
{
   TA_ParamHolderPriv *paramHolderPriv;
   const TA_OptInputParameterInfo *paramInfo;
   const TA_FuncInfo *funcInfo;

   if (param == NULL)
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Make sure this index really exist. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;

   if (paramIndex >= funcInfo->nbOptInput)
   {
      return TA_BAD_PARAM;
   }

   /* Verify the type of the parameter. */
   paramInfo = paramHolderPriv->optIn[paramIndex].optInputInfo;
   if (!paramInfo)
      return TA_INTERNAL_ERROR(2);
   if ((paramInfo->type != TA_OptInput_RealRange) &&
       (paramInfo->type != TA_OptInput_RealList))
   {
      return TA_INVALID_PARAM_HOLDER_TYPE;
   }

   /* keep a copy of the provided parameter. */
   paramHolderPriv->optIn[paramIndex].data.optInReal = value;

   return TA_SUCCESS;
}

/* Setup the parameters indicating where to store the output. */
TA_RetCode TA_SetOutputParamIntegerPtr(TA_ParamHolder *param,
                                       unsigned int paramIndex,
                                       TA_Integer *out)
{
   TA_ParamHolderPriv *paramHolderPriv;
   const TA_OutputParameterInfo *paramInfo;
   const TA_FuncInfo *funcInfo;

   if ((param == NULL) || (out == NULL))
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Make sure this index really exist. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;

   if (paramIndex >= funcInfo->nbOutput)
   {
      return TA_BAD_PARAM;
   }

   /* Verify the type of the parameter. */
   paramInfo = paramHolderPriv->out[paramIndex].outputInfo;
   if (!paramInfo)
      return TA_INTERNAL_ERROR(2);
   if (paramInfo->type != TA_Output_Integer)
   {
      return TA_INVALID_PARAM_HOLDER_TYPE;
   }

   /* keep a copy of the provided parameter. */
   paramHolderPriv->out[paramIndex].data.outInteger = out;

   /* This parameter is now initialized, clear the corresponding bit. */
   paramHolderPriv->outBitmap &= ~(1 << paramIndex);

   return TA_SUCCESS;
}

TA_RetCode TA_SetOutputParamRealPtr(TA_ParamHolder *param,
                                    unsigned int paramIndex,
                                    TA_Real *out)
{
   TA_ParamHolderPriv *paramHolderPriv;
   const TA_OutputParameterInfo *paramInfo;
   const TA_FuncInfo *funcInfo;

   if ((param == NULL) || (out == NULL))
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Make sure this index really exist. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;

   if (paramIndex >= funcInfo->nbOutput)
   {
      return TA_BAD_PARAM;
   }

   /* Verify the type of the parameter. */
   paramInfo = paramHolderPriv->out[paramIndex].outputInfo;
   if (!paramInfo)
      return TA_INTERNAL_ERROR(2);
   if (paramInfo->type != TA_Output_Real)
   {
      return TA_INVALID_PARAM_HOLDER_TYPE;
   }

   /* keep a copy of the provided parameter. */
   paramHolderPriv->out[paramIndex].data.outReal = out;

   /* This parameter is now initialized, clear the corresponding bit. */
   paramHolderPriv->outBitmap &= ~(1 << paramIndex);

   return TA_SUCCESS;
}

TA_RetCode TA_GetLookback(const TA_ParamHolder *param, TA_Integer *lookback)
{
   const TA_ParamHolderPriv *paramHolderPriv;

   const TA_FuncDef *funcDef;
   const TA_FuncInfo *funcInfo;
   TA_FrameLookback lookbackFunction;

   if ((param == NULL) || (lookback == NULL))
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Get the pointer on the lookback function. */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;

   funcDef = (const TA_FuncDef *)funcInfo->handle;
   if (!funcDef)
      return TA_INTERNAL_ERROR(2);
   lookbackFunction = funcDef->lookback;
   if (!lookbackFunction)
      return TA_INTERNAL_ERROR(2);

   /* Perform the function call. */
   *lookback = (*lookbackFunction)(paramHolderPriv);

   return TA_SUCCESS;
}

/* Finally, call a TA function with the parameters. */
TA_RetCode TA_CallFunc(const TA_ParamHolder *param,
                       TA_Integer startIdx,
                       TA_Integer endIdx,
                       TA_Integer *outBegIdx,
                       TA_Integer *outNbElement)
{
   TA_RetCode retCode;
   const TA_ParamHolderPriv *paramHolderPriv;

   const TA_FuncDef *funcDef;
   const TA_FuncInfo *funcInfo;
   TA_FrameFunction function;

   if ((param == NULL) ||
       (outBegIdx == NULL) ||
       (outNbElement == NULL))
   {
      return TA_BAD_PARAM;
   }

   paramHolderPriv = (TA_ParamHolderPriv *)(param->hiddenData);
   if (paramHolderPriv->magicNumber != TA_PARAM_HOLDER_PRIV_MAGIC_NB)
   {
      return TA_INVALID_PARAM_HOLDER;
   }

   /* Check that all parameters are initialize (except the optInput). */
   if (paramHolderPriv->inBitmap != 0)
   {
      return TA_INPUT_NOT_ALL_INITIALIZE;
   }

   if (paramHolderPriv->outBitmap != 0)
   {
      return TA_OUTPUT_NOT_ALL_INITIALIZE;
   }

   /* Get the pointer on the function */
   funcInfo = paramHolderPriv->funcInfo;
   if (!funcInfo)
      return TA_INVALID_HANDLE;
   funcDef = (const TA_FuncDef *)funcInfo->handle;
   if (!funcDef)
      return TA_INTERNAL_ERROR(2);
   function = funcDef->function;
   if (!function)
      return TA_INTERNAL_ERROR(2);

   /* Perform the function call. */
   retCode = (*function)(paramHolderPriv, startIdx, endIdx,
                         outBegIdx, outNbElement);
   return retCode;
}

/**** Local functions definitions.     ****/
static TA_RetCode getGroupId(const char *groupString, unsigned int *groupId)
{
   unsigned int i;

   for (i = 0; i < TA_NB_GROUP_ID; i++)
   {
      if (strcmp(TA_GroupString[i], groupString) == 0)
      {
         *groupId = i;
         return TA_SUCCESS;
      }
   }

   return TA_GROUP_NOT_FOUND;
}

static TA_RetCode getGroupSize(TA_GroupId groupId, unsigned int *groupSize)
{
#ifdef TA_GEN_CODE
   /* Code used only when compiled with gen_code. */

   unsigned int i, j;
   const TA_FuncDef **funcDefTable;
   const TA_FuncDef *funcDef;
   unsigned int tableSize;
   unsigned int nbFuncFound;

   if (groupId >= TA_NB_GROUP_ID)
      return TA_INTERNAL_ERROR(2);
   if (!groupSize)
      return TA_INTERNAL_ERROR(2);

   nbFuncFound = 0;
   for (i = 0; i < 26; i++)
   {
      funcDefTable = TA_DEF_Tables[i];
      tableSize = *(TA_DEF_TablesSize[i]);

      for (j = 0; j < tableSize; j++)
      {
         funcDef = funcDefTable[j];
         if (funcDef && (funcDef->groupId == groupId))
            nbFuncFound++;
      }
   }

   *groupSize = nbFuncFound;

   return TA_SUCCESS;
#else
   /* Optimized code in the final library. */
   *groupSize = TA_PerGroupSize[groupId];

   return TA_SUCCESS;
#endif
}

#ifdef TA_GEN_CODE
static TA_RetCode getFuncNameByIdx(TA_GroupId groupId,
                                   unsigned int idx,
                                   const char **stringPtr)
#else
static TA_RetCode getFuncNameByIdx(TA_GroupId groupId,
                                   unsigned int idx,
                                   const char **stringPtr)
#endif
{
#ifdef TA_GEN_CODE
   /* Code used only when compiled with gen_code. */

   unsigned int curIdx;
   unsigned int i, j, found;
   const TA_FuncDef **funcDefTable;
   unsigned int tableSize;
   const TA_FuncInfo *funcInfo;

   if (!stringPtr)
      return TA_INTERNAL_ERROR(2);

   curIdx = 0;
   found = 0;
   for (i = 0; (i < 26) && !found; i++)
   {
      funcDefTable = TA_DEF_Tables[i];
      tableSize = *(TA_DEF_TablesSize[i]);

      for (j = 0; (j < tableSize) && !found; j++)
      {
         if (funcDefTable[j]->groupId == groupId)
         {
            if (idx == curIdx)
            {
               funcInfo = funcDefTable[j]->funcInfo;
               if (!funcInfo)
                  return TA_INTERNAL_ERROR(2);
               *stringPtr = funcInfo->name;
               found = 1;
            }
            curIdx++;
         }
      }
   }

   if (found != 1)
      return TA_INTERNAL_ERROR(2);
   if (!(*stringPtr))
      return TA_INTERNAL_ERROR(2);

   return TA_SUCCESS;
#else
   /* Optimized code in the final library. */
   const TA_FuncDef **funcDefTable;
   const TA_FuncInfo *funcInfo;

   funcDefTable = TA_PerGroupFuncDef[groupId];
   funcInfo = funcDefTable[idx]->funcInfo;
   *stringPtr = funcInfo->name;

   return TA_SUCCESS;
#endif
}
