/*******************************************************************************
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *******************************************************************************/

#include <string>
#include <string.h>
#include <vector>

#include "AMLInterface.h"
#include "AMLException.h"

#include "camlinterface.h"
#include "camlerrorcodes.h"
#include "camlhandlemanager.h"
#include "camlutils.h"

using namespace std;
using namespace AML;

CAMLErrorCode CreateAMLData(amlDataHandle_t* amlDataHandle)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);

    AMLData* amlData = new AMLData();
    if (nullptr == amlData)
    {
        return CAML_NO_MEMORY;
    }

    amlDataHandle_t handle = AddAmlDataHandle(amlData, true);
    if (!handle)
    {
        delete amlData;
        return CAML_NO_MEMORY;
    }

    *amlDataHandle = handle;
    return CAML_OK;
}

CAMLErrorCode DestroyAMLData(amlDataHandle_t amlDataHandle)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    RemoveOwnedAmlDataHandles(amlData);
    RemoveAmlData(amlDataHandle);

    return CAML_OK;
}

CAMLErrorCode CloneAMLData(amlDataHandle_t origin, amlDataHandle_t* clone)
{
    VERIFY_PARAM_NON_NULL(origin);
    VERIFY_PARAM_NON_NULL(clone);

    AMLData* originAmlData = FindAmlData(origin);
    if (!originAmlData)
    {
        return CAML_INVALID_HANDLE;
    }

    AMLData* cloneAmlData = new AMLData(*originAmlData);
    if (nullptr == cloneAmlData)
    {
        return CAML_NO_MEMORY;
    }

    amlDataHandle_t cloneHandle = AddAmlDataHandle(cloneAmlData, true);
    if (!cloneHandle)
    {
        delete cloneAmlData;
        return CAML_NO_MEMORY;
    }

    *clone = cloneHandle;
    return CAML_OK;
}

CAMLErrorCode AMLData_SetValueStr(amlDataHandle_t amlDataHandle, const char* key, const char* value)
{   
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(key);
    VERIFY_PARAM_NON_NULL(value);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    string keyStr(key, strlen(key));
    string valueStr(value, strlen(value));

    try
    {
        amlData->setValue(keyStr, valueStr);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    return CAML_OK;
}

CAMLErrorCode AMLData_SetValueStrArr(amlDataHandle_t amlDataHandle, const char* key, const char** value, const size_t valueSize)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(key);
    VERIFY_PARAM_NON_NULL(value);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    string keyStr(key, strlen(key));
    vector<string> valueStrArr;
    for (size_t i = 0; i< valueSize; i++)
    {
        string valueStr(value[i], strlen(value[i]));
        valueStrArr.push_back(valueStr);
    }

    try
    {
        amlData->setValue(keyStr, valueStrArr);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    return CAML_OK;
}

CAMLErrorCode AMLData_SetValueAMLData(amlDataHandle_t amlDataHandle, const char* key, const amlDataHandle_t value)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(key);
    VERIFY_PARAM_NON_NULL(value);

    AMLData* amlData = FindAmlData(amlDataHandle);
    AMLData* valueData = FindAmlData(value);
    if (!amlData || !valueData)
    {
        return CAML_INVALID_HANDLE;
    }

    string keyStr(key, strlen(key));

    try
    {
        amlData->setValue(keyStr, *valueData);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    return CAML_OK;
}

CAMLErrorCode AMLData_GetValueStr(amlDataHandle_t amlDataHandle, const char* key, char** value)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(key);
    VERIFY_PARAM_NON_NULL(value);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    string keyStr(key, strlen(key));
    string valueStr;

    try
    {
        valueStr = amlData->getValueToStr(keyStr);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    char* valueArr = ConvertStringToCharStr(valueStr);
    if (NULL == valueArr)
    {
        return CAML_NO_MEMORY;
    }

    *value = valueArr;

    return CAML_OK;
}

CAMLErrorCode AMLData_GetValueStrArr(amlDataHandle_t amlDataHandle, const char* key, char*** value, size_t* valueSize)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(key);
    VERIFY_PARAM_NON_NULL(value);
    VERIFY_PARAM_NON_NULL(valueSize);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    string keyStr(key, strlen(key));
    vector<string> valueStrArr;

    try
    {
        valueStrArr = amlData->getValueToStrArr(keyStr);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    char** strarr = ConvertVectorToCharStrArr(valueStrArr);
    if (NULL == strarr)
    {
        return CAML_NO_MEMORY;
    }

    *valueSize = valueStrArr.size();
    *value = strarr;

    return CAML_OK;
}

CAMLErrorCode AMLData_GetValueAMLData(amlDataHandle_t amlDataHandle, const char* key, amlDataHandle_t* value)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(key);
    VERIFY_PARAM_NON_NULL(value);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    string keyStr(key, strlen(key));

    try
    {
        const AMLData& valueData = amlData->getValueToAMLData(keyStr);

        amlDataHandle_t valueHandle = FindAmlDataHandle(const_cast<AMLData*>(&valueData));
        if (NULL == valueHandle)
        {
            valueHandle = AddAmlDataHandle(const_cast<AMLData*>(&valueData), false);
            if (NULL == valueHandle)
            {
                return CAML_NO_MEMORY;
            }
        }

        *value = valueHandle;
    }
    catch (const AMLException e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    return CAML_OK;
}

CAMLErrorCode AMLData_GetKeys(amlDataHandle_t amlDataHandle, char*** keys, size_t* keysSize)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(keys);
    VERIFY_PARAM_NON_NULL(keysSize);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    vector<string> keysVec = amlData->getKeys();

    char** strarr = ConvertVectorToCharStrArr(keysVec);
    if (NULL == strarr)
    {
        return CAML_NO_MEMORY;;
    }

    *keysSize = keysVec.size();
    *keys = strarr;

    return CAML_OK;
}

CAMLErrorCode AMLData_GetValueType(amlDataHandle_t amlDataHandle, const char* key, CAMLValueType* type)
{
    VERIFY_PARAM_NON_NULL(amlDataHandle);
    VERIFY_PARAM_NON_NULL(key);
    VERIFY_PARAM_NON_NULL(type);

    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    string keyStr(key, strlen(key));
    AMLValueType cpptype;

    try
    {
        cpptype = amlData->getValueType(keyStr);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    switch (cpptype)
    {
        case AMLValueType::String :
            *type = AMLVALTYPE_STRING;
            break;
        case AMLValueType::StringArray :
            *type = AMLVALTYPE_STRINGARRAY;
            break;
        case AMLValueType::AMLData :
            *type = AMLVALTYPE_AMLDATA;
            break;
        default:
            break;
    }

    return CAML_OK;
}
