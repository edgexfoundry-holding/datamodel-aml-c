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

#include <string.h>
#include <string>

#include "Representation.h"
#include "AMLInterface.h"
#include "AMLException.h"

#include "camlrepresentation.h"
#include "camlinterface.h"
#include "camlerrorcodes.h"
#include "camlhandlemanager.h"
#include "camlutils.h"

using namespace std;
using namespace AML;

CAMLErrorCode CreateRepresentation(const char* filePath, representation_t* repHandle)
{
    VERIFY_PARAM_NON_NULL(filePath);
    VERIFY_PARAM_NON_NULL(repHandle);

    Representation* rep = nullptr;
    try
    {
        rep = new Representation(filePath);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    representation_t handle = AddRepresentationHandle(rep);
    if (!handle)
    {
        delete rep;
        return CAML_NO_MEMORY;
    }

    *repHandle = handle;
    return CAML_OK;
}

CAMLErrorCode DestroyRepresentation(representation_t repHandle)
{
    VERIFY_PARAM_NON_NULL(repHandle);

    Representation* rep = FindRepresentation(repHandle);
    if (!rep)
    {
        return CAML_INVALID_HANDLE;
    }

    RemoveRepresentation(repHandle);

    return CAML_OK;
}

CAMLErrorCode Representation_GetRepId(representation_t repHandle, char** repId)
{
    VERIFY_PARAM_NON_NULL(repHandle);
    VERIFY_PARAM_NON_NULL(repId);

    Representation* rep = FindRepresentation(repHandle);
    if (!rep)
    {
        return CAML_INVALID_HANDLE;
    }

    string repIdStr = rep->getRepresentationId();
    *repId = ConvertStringToCharStr(repIdStr);
    if (nullptr == *repId)
    {
        return CAML_NO_MEMORY;
    }

    return CAML_OK;
}

CAMLErrorCode Representation_GetConfigInfo(representation_t repHandle, amlObjectHandle_t* amlObjHandle)
{
    VERIFY_PARAM_NON_NULL(repHandle);
    VERIFY_PARAM_NON_NULL(amlObjHandle);

    Representation* rep = FindRepresentation(repHandle);
    if (!rep)
    {
        return CAML_INVALID_HANDLE;
    }

    AMLObject* amlObj = nullptr;
    try
    {
        amlObj = rep->getConfigInfo();
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    amlObjectHandle_t amlObjHandleNew = AddAmlObjHandle(amlObj, true);
    if (!amlObjHandleNew)
    {
        delete amlObj;
        return CAML_NO_MEMORY;
    }

    *amlObjHandle = amlObjHandleNew;
    return CAML_OK;
}

CAMLErrorCode Representation_DataToAml(const representation_t repHandle, const amlObjectHandle_t amlObjHandle, char** amlStr)
{
    VERIFY_PARAM_NON_NULL(repHandle);
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(amlStr);

    Representation* rep = FindRepresentation(repHandle);
    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!rep || !amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    char* amlChar = NULL;
    try
    {
        string amlString = rep->DataToAml(*amlObj);
        amlChar = ConvertStringToCharStr(amlString);
        if (NULL == amlChar)
        {
            return CAML_NO_MEMORY;
        }
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    *amlStr = amlChar;

    return CAML_OK;
}

CAMLErrorCode Representation_AmlToData(const representation_t repHandle, const char* amlStr, amlObjectHandle_t* amlObjHandle)
{
    VERIFY_PARAM_NON_NULL(repHandle);
    VERIFY_PARAM_NON_NULL(amlStr);
    VERIFY_PARAM_NON_NULL(amlObjHandle);

    Representation* rep = FindRepresentation(repHandle);
    if (!rep)
    {
        return CAML_INVALID_HANDLE;
    }

    AMLObject* amlObj = nullptr;
    string amlString(amlStr, strlen(amlStr));

    try
    {
        amlObj = rep->AmlToData(amlString);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    amlObjectHandle_t amlObjHandleNew = AddAmlObjHandle(amlObj, true);
    if (!amlObjHandleNew)
    {
        delete amlObj;
        return CAML_NO_MEMORY;
    }

    *amlObjHandle = amlObjHandleNew;
    return CAML_OK;
}

CAMLErrorCode Representation_DataToByte(const representation_t repHandle, const amlObjectHandle_t amlObjHandle, uint8_t** byte, size_t* size)
{
    VERIFY_PARAM_NON_NULL(repHandle);
    VERIFY_PARAM_NON_NULL(amlObjHandle);
    VERIFY_PARAM_NON_NULL(byte);
    VERIFY_PARAM_NON_NULL(size);

    Representation* rep = FindRepresentation(repHandle);
    AMLObject* amlObj = FindAmlObj(amlObjHandle);
    if (!rep || !amlObj)
    {
        return CAML_INVALID_HANDLE;
    }

    try
    {
        string amlString = rep->DataToByte(*amlObj);
        char* temp = ConvertStringToCharStr(amlString);
        if (nullptr == temp)
        {
            return CAML_NO_MEMORY;
        }
        *byte = reinterpret_cast<uint8_t*>(temp);
        *size = amlString.size();
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    return CAML_OK;
}

CAMLErrorCode Representation_ByteToData(const representation_t repHandle, const uint8_t* byte, const size_t size, amlObjectHandle_t* amlObjHandle)
{
    VERIFY_PARAM_NON_NULL(repHandle);
    VERIFY_PARAM_NON_NULL(byte);
    VERIFY_PARAM_NON_NULL(size);
    VERIFY_PARAM_NON_NULL(amlObjHandle);

    Representation* rep = FindRepresentation(repHandle);
    if (!rep)
    {
        return CAML_INVALID_HANDLE;
    }

    AMLObject* amlObj = nullptr;
    string byteString((char*)byte, size);

    try
    {
        amlObj = rep->ByteToData(byteString);
    }
    catch (const AMLException& e)
    {
        return ExceptionCodeToErrorCode(e.code());
    }

    amlObjectHandle_t amlObjHandleNew = AddAmlObjHandle(amlObj, true);
    if (!amlObjHandleNew)
    {
        delete amlObj;
        return CAML_NO_MEMORY;
    }

    *amlObjHandle = amlObjHandleNew;

    return CAML_OK;
}
