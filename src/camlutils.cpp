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
#include <cstring>
#include <vector>

#include "camlutils.h"

using namespace std;

char* ConvertStringToCharStr(std::string str)
{
    size_t size = str.size();
    char* cstr = (char*)malloc(sizeof(char) * (size + 1));
    if (nullptr == cstr) 
    {
        return nullptr;
    }

    memcpy(cstr, str.c_str(), size);
    cstr[size] = '\0';

    return cstr;
}

char** ConvertVectorToCharStrArr(std::vector<std::string>& list)
{
    unsigned long size = list.size();
    char** cstr = (char**)malloc(sizeof(char*) * size);
    if (nullptr == cstr)
    {
        return nullptr;
    }

    for (unsigned long i = 0; i < size; i++)
    {
        cstr[i] = ConvertStringToCharStr(list[i]);
        if (nullptr == cstr[i])
        {
            for (unsigned long j = 0; j < i; j++)
            {
                free(cstr[j]);
            }
            free(cstr);
            return nullptr;
        }
    }

    return cstr;
}

CAMLErrorCode ExceptionCodeToErrorCode(AML::ResultCode result)
{
    switch (result)
    {
        case AML::INVALID_PARAM :           return CAML_INVALID_PARAM;
        case AML::INVALID_FILE_PATH :       return CAML_INVALID_FILE_PATH;
        case AML::INVALID_AML_SCHEMA :      return CAML_INVALID_AML_SCHEMA;
        case AML::INVALID_XML_STR :         return CAML_INVALID_XML_STR;
        case AML::NOT_MATCH_TO_AML_MODEL :  return CAML_NOT_MATCH_TO_AML_MODEL;
        case AML::INVALID_BYTE_STR :        return CAML_INVALID_BYTE_STR;
        case AML::SERIALIZE_FAIL :          return CAML_SERIALIZE_FAIL;
        case AML::NO_MEMORY :               return CAML_NO_MEMORY;
        case AML::KEY_NOT_EXIST :           return CAML_KEY_NOT_EXIST;
        case AML::KEY_ALREADY_EXIST :       return CAML_KEY_ALREADY_EXIST;
        case AML::WRONG_GETTER_TYPE :       return CAML_WRONG_GETTER_TYPE;
        case AML::API_NOT_ENABLED :         return CAML_API_NOT_ENABLED;
        default : /* AML::NO_ERROR */       return CAML_OK;
    }
}