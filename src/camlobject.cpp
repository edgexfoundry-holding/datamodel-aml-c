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
#include <map>
#include <vector>
#include <cstring>

#include "AMLInterface.h"
#include "AMLException.h"

#include "camlinterface.h"
#include "camlerrorcodes.h"
#include "camlhandlemanager.h"
#include "camlutils.h"

using namespace std;
using namespace AML;

CAMLErrorCode CreateAMLObject(const char* deviceId, const char* timeStamp, amlObjectHandle_t* amlObjHandle)
{
    VERIFY_PARAM_NON_NULL(deviceId);
    VERIFY_PARAM_NON_NULL(timeStamp);
    VERIFY_PARAM_NON_NULL(amlObjHandle);

    AMLObject* amlObj = nullptr;
    try
    {
        amlObj = new AMLObject(deviceId, timeStamp);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    amlObjectHandle_t handle = AddAmlObjHandle(amlObj, true);
    if (!handle)
    {
        delete amlObj;
        return CAML_NO_MEMORY;
    }

    *amlObjHandle = handle;
    return CAML_OK;
}

CAMLErrorCode CreateAMLObjectWithID(const char* deviceId, const char* timeStamp, const char* id, amlObjectHandle_t* amlObjHandle)
{
    VERIFY_PARAM_NON_NULL(deviceId);
    VERIFY_PARAM_NON_NULL(timeStamp);
    VERIFY_PARAM_NON_NULL(id);
    VERIFY_PARAM_NON_NULL(amlObjHandle);

    AMLObject* amlObj = nullptr;
    try
    {
        amlObj = new AMLObject(deviceId, timeStamp, id);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    amlObjectHandle_t handle = AddAmlObjHandle(amlObj, true);
    if (!handle)
    {
        delete amlObj;
        return CAML_NO_MEMORY;
    }

    *amlObjHandle = handle;
    return CAML_OK;
}

CAMLErrorCode DestroyAMLObject(amlObjectHandle_t amlObjHandle)
{
    VERIFY_PARAM_NON_NULL(amlObjHandle);

    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    RemoveOwnedAmlDataHandles(amlObj);
    RemoveAmlObj(amlObjHandle);

    return CAML_OK;
}

CAMLErrorCode CloneAMLObject(amlObjectHandle_t origin, amlObjectHandle_t* clone)
{
    VERIFY_PARAM_NON_NULL(origin);
    VERIFY_PARAM_NON_NULL(clone);

    AMLObject* originObj = FindAmlObj(origin);
    if (!originObj)
    {
        return CAML_INVALID_HANDLE;
    }

    AMLObject* cloneObj = nullptr;
    try
    {
        cloneObj = new AMLObject(*originObj);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    amlObjectHandle_t cloneHandle = AddAmlObjHandle(cloneObj, true);
    if (!cloneHandle)
    {
        delete cloneObj;
        return CAML_NO_MEMORY;
    }

    *clone = cloneHandle;
    return CAML_OK;
}

CAMLErrorCode AMLObject_AddData(amlObjectHandle_t amlObjHandle, const char* name, const amlDataHandle_t amlDataHandle)
{
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(name);
    VERIFY_PARAM_NON_NULL(amlDataHandle);

    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    AMLData* amlData = FindAmlData(amlDataHandle);
    if (!amlObj || !amlData)
    {
        return CAML_INVALID_HANDLE;
    }

    try
    {
        amlObj->addData(name, *amlData);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    return CAML_OK;
}

CAMLErrorCode AMLObject_GetData(amlObjectHandle_t amlObjHandle, const char* name, amlDataHandle_t* amlDataHandle)
{
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(name);
    VERIFY_PARAM_NON_NULL(amlDataHandle);

    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    try
    {
        const AMLData& amlData = amlObj->getData(name);

        amlDataHandle_t handle = FindAmlDataHandle(const_cast<AMLData*>(&amlData));
        if (NULL == handle)
        {
            handle = AddAmlDataHandle(const_cast<AMLData*>(&amlData), false);
            if (NULL == handle)
            {
                return CAML_NO_MEMORY;
            }
        }

        *amlDataHandle = handle;
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    return CAML_OK;
}

CAMLErrorCode AMLObject_GetDataNames(amlObjectHandle_t amlObjHandle, char*** names, size_t* namesSize)
{
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(names);
    VERIFY_PARAM_NON_NULL(namesSize);

    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    vector<string> strvec = amlObj->getDataNames();
    char** strarr = ConvertVectorToCharStrArr(strvec);
    if (nullptr == strarr)
    {
        return CAML_NO_MEMORY;;
    }

    *names = strarr;
    *namesSize = strvec.size();

    return CAML_OK;
}

CAMLErrorCode AMLObject_GetDeviceId(amlObjectHandle_t amlObjHandle, char** deviceId)
{
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(deviceId);

    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    string deviceIdStr = amlObj->getDeviceId();
    *deviceId = ConvertStringToCharStr(deviceIdStr);
    if (nullptr == *deviceId)
    {
        return CAML_NO_MEMORY;
    }

    return CAML_OK;
}

CAMLErrorCode AMLObject_GetTimeStamp(amlObjectHandle_t amlObjHandle, char** timeStamp)
{
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(timeStamp);

    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    string timeStampStr = amlObj->getTimeStamp();
    *timeStamp = ConvertStringToCharStr(timeStampStr);
    if (nullptr == *timeStamp)
    {
        return CAML_NO_MEMORY;
    }

    return CAML_OK;
}

CAMLErrorCode AMLObject_GetId(amlObjectHandle_t amlObjHandle, char** id)
{
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(id);

    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    string idStr = amlObj->getId();
    *id = ConvertStringToCharStr(idStr);
    if (nullptr == *id)
    {
        return CAML_NO_MEMORY;
    }

    return CAML_OK;
}
