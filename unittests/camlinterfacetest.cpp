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

#include <iostream>
#include <string>
#include <fstream>

#include "camlinterface.h"
#include "camlerrorcodes.h"
#include "gtest/gtest.h"

using namespace std;

namespace camlinterfacetest
{
    bool isEqual(const char* str1, const char* str2)
    {
        size_t len1 = strlen(str1), len2 = strlen(str2);
        if (len1 != len2) return false;
        return (0 == strncmp(str1, str2, len1));
    }

    bool isEqualArr(const char** strArr1, size_t size1, const char** strArr2, size_t size2)
    {
        if (size1 != size2)    return false;

        for (size_t i = 0; i < size2; i++)
        {
            if (false == isEqual(strArr1[i], strArr2[i])) return false;
        }

        return true;
    }

    bool isEqualAMLData(amlDataHandle_t data1, amlDataHandle_t data2)
    {
        char** keys1 = NULL;
        char** keys2 = NULL;
        size_t size1, size2;

        AMLData_GetKeys(data1, &keys1, &size1);
        AMLData_GetKeys(data2, &keys2, &size2);

        if (false == isEqualArr((const char**)keys1, size1, (const char**)keys2, size2)) return false;

        for (size_t i = 0; i < size1; i++)
        {
            char* key = keys1[i];

            CAMLValueType type1, type2;
            AMLData_GetValueType(data1, key, &type1);
            AMLData_GetValueType(data2, key, &type2);

            if (type1 != type2)     return false;

            if (AMLVALTYPE_STRING == type1)
            {
                char* ret1 = NULL;
                char* ret2 = NULL;
                AMLData_GetValueStr(data1, key, &ret1);
                AMLData_GetValueStr(data2, key, &ret2);
                
                if (false == isEqual(ret1, ret2)) return false;
            }
            else if (AMLVALTYPE_STRINGARRAY == type1)
            {
                char** ret1 = NULL;
                char** ret2 = NULL;
                size_t ret_size1, ret_size2;
                AMLData_GetValueStrArr(data1, key, &ret1, &ret_size1);
                AMLData_GetValueStrArr(data2, key, &ret2, &ret_size2);

                if (false == isEqualArr((const char**)ret1, ret_size1, (const char**)ret2, ret_size2)) return false;
            }
            else if (AMLVALTYPE_AMLDATA == type1)
            {
                amlDataHandle_t ret1, ret2;
                AMLData_GetValueAMLData(data1, key, &ret1);
                AMLData_GetValueAMLData(data2, key, &ret2);
                
                if (false == isEqualAMLData(ret1, ret2)) return false;
            }
        }
        return true;
    }

    bool isEqualAMLObject(amlObjectHandle_t obj1, amlObjectHandle_t obj2)
    {
        char* str1 = NULL;
        char* str2 = NULL;

        AMLObject_GetDeviceId(obj1, &str1);
        AMLObject_GetDeviceId(obj2, &str2);

        if (false == isEqual(str1, str2)) return false;

        AMLObject_GetTimeStamp(obj1, &str1);
        AMLObject_GetTimeStamp(obj2, &str2);

        if (false == isEqual(str1, str2)) return false;

        AMLObject_GetId(obj1, &str1);
        AMLObject_GetId(obj2, &str2);

        if (false == isEqual(str1, str2)) return false;

        char** dataNames1 = NULL;
        char** dataNames2 = NULL;

        size_t size1;
        size_t size2;

        AMLObject_GetDataNames(obj1, &dataNames1, &size1);
        AMLObject_GetDataNames(obj2, &dataNames2, &size2);

        if (false == isEqualArr((const char**)dataNames1, size1, (const char**)dataNames2, size2)) return false;

        amlDataHandle_t data1;
        amlDataHandle_t data2;
        for (size_t i = 0; i < size1; i++)
        {
            AMLObject_GetData(obj1, dataNames1[i], &data1);
            AMLObject_GetData(obj2, dataNames1[i], &data2);

            if (false == isEqualAMLData(data1, data2)) return false;
        }

        return true;
    }

    bool isPresent(const char* str, char** strArr, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (isEqual(str, strArr[i])) return true;
        }

        return false;
    }

    // Test
    // AMLData Test
    TEST(AMLData_CreateTest, Valid)
    {
        amlDataHandle_t amlData;

        EXPECT_EQ(CreateAMLData(&amlData), CAML_OK);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_DestroyTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        EXPECT_EQ(DestroyAMLData(amlData), CAML_OK);
    }

    TEST(AMLData_DestroyTest, Invalid_handle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        EXPECT_EQ(DestroyAMLData(amlData), CAML_OK);

        EXPECT_EQ(DestroyAMLData(amlData), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_CloneAMLData, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        AMLData_SetValueStr(amlData, "key1", "value1");
        
        amlDataHandle_t value2;
        CreateAMLData(&value2);
        
        AMLData_SetValueAMLData(amlData, "key2", value2);


        amlDataHandle_t cloneData;

        EXPECT_EQ(CloneAMLData(amlData, &cloneData), CAML_OK);
        EXPECT_TRUE(isEqualAMLData(amlData, cloneData));

        DestroyAMLData(value2);
        EXPECT_EQ(DestroyAMLData(cloneData), CAML_OK);
        EXPECT_EQ(DestroyAMLData(amlData), CAML_OK);
    }

    TEST(AMLData_CloneAMLData, Invalid_handle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        amlDataHandle_t cloneData;

        EXPECT_EQ(CloneAMLData(amlData, &cloneData), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_SetValueStrTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value = "value";
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, key, value), CAML_OK);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_SetValueStrTest, Invalid_handle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        const char* key = "key";
        const char* value = "value";

        EXPECT_EQ(AMLData_SetValueStr(amlData, key, value), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_SetValueStrTest, Invalid_DuplicatedKey)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value = "value";
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, key, value), CAML_OK);
        EXPECT_EQ(AMLData_SetValueStr(amlData, key, value), CAML_KEY_ALREADY_EXIST);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_SetValueStrArrTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value[2] = {"value1", "value2"};
        
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key, value, 2), CAML_OK);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_SetValueStrArrTest, Invalid_handle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        const char* key = "key";
        const char* value[2] = {"value1", "value2"};
        
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key, value, 2), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_SetValueStrArrTest, Invalid_DuplicatedKey)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value[2] = {"value1", "value2"};
        
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key, value, 2), CAML_OK);
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key, value, 2), CAML_KEY_ALREADY_EXIST);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_SetValueAMLDataTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        amlDataHandle_t value;
        CreateAMLData(&value);
        
        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key, value), CAML_OK);

        DestroyAMLData(amlData);
        DestroyAMLData(value);
    }

    TEST(AMLData_SetValueAMLDataTest, Invalid_handle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        amlDataHandle_t dummy;
        CreateAMLData(&dummy);

        const char* key = "key";
        amlDataHandle_t value;
        CreateAMLData(&value);

        DestroyAMLData(dummy);

        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key, value), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_SetValueAMLDataTest, Invalid_DuplicatedKey)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        amlDataHandle_t value;
        CreateAMLData(&value);
        
        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key, value), CAML_OK);
        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key, value), CAML_KEY_ALREADY_EXIST);

        DestroyAMLData(amlData);
        DestroyAMLData(value);
    }

    TEST(AMLData_GetValueStrTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value = "value";
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, key, value), CAML_OK);

        char* ret;
        EXPECT_EQ(AMLData_GetValueStr(amlData, key, &ret), CAML_OK);
        EXPECT_TRUE(isEqual(value, ret));

        DestroyAMLData(amlData);
    }

    TEST(AMLData_GetValueStrTest, InvalidHandle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        const char* key = "key";
        char* ret;

        EXPECT_EQ(AMLData_GetValueStr(amlData, key, &ret), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_GetValueStrTest, InvalidGetterType)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value[2] = {"value1", "value2"};
        
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key, value, 2), CAML_OK);

        char* ret;
        EXPECT_EQ(AMLData_GetValueStr(amlData, key, &ret), CAML_WRONG_GETTER_TYPE);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_GetValueStrArrTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value[2] = {"value1", "value2"};
        
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key, value, 2), CAML_OK);

        char** ret;
        size_t size;
        EXPECT_EQ(AMLData_GetValueStrArr(amlData, key, &ret, &size), CAML_OK);
        EXPECT_TRUE(isEqualArr(value, 2, (const char**)ret, size));

        DestroyAMLData(amlData);
    }

    TEST(AMLData_GetValueStrArrTest, InvalidHandle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        const char* key = "key";
        char** ret;
        size_t size;

        EXPECT_EQ(AMLData_GetValueStrArr(amlData, key, &ret, &size), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_GetValueStrArrTest, InvalidGetterType)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value = "value";
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, key, value), CAML_OK);

        char** ret;
        size_t size;
        EXPECT_EQ(AMLData_GetValueStrArr(amlData, key, &ret, &size), CAML_WRONG_GETTER_TYPE);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_GetValueAMLDataTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        amlDataHandle_t value;
        CreateAMLData(&value);
        
        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key, value), CAML_OK);

        amlDataHandle_t ret;
        EXPECT_EQ(AMLData_GetValueAMLData(amlData, key, &ret), CAML_OK);
        EXPECT_TRUE(isEqualAMLData(value, ret));

        DestroyAMLData(amlData);
        DestroyAMLData(value);
    }

    TEST(AMLData_GetValueAMLDataTest, InvalidHandle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        const char* key = "key";
        amlDataHandle_t value;

        EXPECT_EQ(AMLData_GetValueAMLData(amlData, key, &value), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_GetValueAMLDataTest, InvalidGetterType)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key = "key";
        const char* value = "value";
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, key, value), CAML_OK);

        amlDataHandle_t ret;
        EXPECT_EQ(AMLData_GetValueAMLData(amlData, key, &ret), CAML_WRONG_GETTER_TYPE);

        DestroyAMLData(amlData);
    }

    TEST(AMLData_GetKeysTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key1 = "key1";
        const char* value1 = "value1";

        const char* key2 = "key2";
        const char* value2[1] = {"value2"};

        const char* key3 = "key3";
        amlDataHandle_t value3;
        CreateAMLData(&value3);

        EXPECT_EQ(AMLData_SetValueStr(amlData, key1, value1), CAML_OK);
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key2, value2, 1), CAML_OK);
        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key3, value3), CAML_OK);

        char** keys = NULL;
        size_t size;

        EXPECT_EQ(AMLData_GetKeys(amlData, &keys, &size), CAML_OK);

        EXPECT_TRUE(isPresent(key1, keys, size));
        EXPECT_TRUE(isPresent(key2, keys, size));
        EXPECT_TRUE(isPresent(key3, keys, size));
        EXPECT_FALSE(isPresent("key4", keys, size));

        DestroyAMLData(amlData);
        DestroyAMLData(value3);
    }

    TEST(AMLData_GetKeysTest, InvalidHandle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        char** keys = NULL;
        size_t size;

        EXPECT_EQ(AMLData_GetKeys(amlData, &keys, &size), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_GetValueTypeTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key1 = "key1";
        const char* value1 = "value1";

        const char* key2 = "key2";
        const char* value2[1] = {"value2"};

        const char* key3 = "key3";
        amlDataHandle_t value3;
        CreateAMLData(&value3);        

        EXPECT_EQ(AMLData_SetValueStr(amlData, key1, value1), CAML_OK);
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key2, value2, 1), CAML_OK);
        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key3, value3), CAML_OK);

        CAMLValueType type1;
        CAMLValueType type2;
        CAMLValueType type3;

        EXPECT_EQ(AMLData_GetValueType(amlData, key1, &type1), CAML_OK);
        EXPECT_EQ(AMLData_GetValueType(amlData, key2, &type2), CAML_OK);
        EXPECT_EQ(AMLData_GetValueType(amlData, key3, &type3), CAML_OK);

        EXPECT_TRUE(type1 == AMLVALTYPE_STRING);
        EXPECT_TRUE(type2 == AMLVALTYPE_STRINGARRAY);
        EXPECT_TRUE(type3 == AMLVALTYPE_AMLDATA);
        
        DestroyAMLData(amlData);
        DestroyAMLData(value3);
    }

    TEST(AMLData_GetValueTypeTest, InvalidHandle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        DestroyAMLData(amlData);

        const char* key = "key";
        CAMLValueType type;

        EXPECT_EQ(AMLData_GetValueType(amlData, key, &type), CAML_INVALID_HANDLE);
    }

    TEST(AMLData_GetValueTypeTest, InvalidKey)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        const char* key1 = "key1";
        const char* value1 = "value1";

        const char* key2 = "key2";
        const char* value2[1] = {"value2"};

        const char* key3 = "key3";
        amlDataHandle_t value3;
        CreateAMLData(&value3);        

        EXPECT_EQ(AMLData_SetValueStr(amlData, key1, value1), CAML_OK);
        EXPECT_EQ(AMLData_SetValueStrArr(amlData, key2, value2, 1), CAML_OK);
        EXPECT_EQ(AMLData_SetValueAMLData(amlData, key3, value3), CAML_OK);

        CAMLValueType type;

        EXPECT_EQ(AMLData_GetValueType(amlData, "key4", &type), CAML_KEY_NOT_EXIST);
        
        DestroyAMLData(amlData);
        DestroyAMLData(value3);
    }

    //AMLObject Test
    TEST(AMLObject_CreateTest, Valid)
    {
        const char* deviceId = "deviceId";
        const char* timeStamp = "timeStamp";
        const char* id = "id";

        amlObjectHandle_t amlObj;
        EXPECT_EQ(CreateAMLObjectWithID(deviceId, timeStamp, id, &amlObj), CAML_OK);

        DestroyAMLObject(amlObj);
    }

    TEST(AMLObject_CreateTest, Invalid_Parameter1)
    {
        amlObjectHandle_t amlObj;
        EXPECT_EQ(CreateAMLObject("", "timeStamp", &amlObj), CAML_INVALID_PARAM);
    }

    TEST(AMLObject_CreateTest, Invalid_Parameter2)
    {
        amlObjectHandle_t amlObj;
        EXPECT_EQ(CreateAMLObject("deviceId", "", &amlObj), CAML_INVALID_PARAM);
    }

    TEST(AMLObjectTest_CreateWithoutIdTest, Valid)
    {
        const char* deviceId = "deviceId";
        const char* timeStamp = "timeStamp";

        amlObjectHandle_t amlObj;
        EXPECT_EQ(CreateAMLObject(deviceId, timeStamp, &amlObj), CAML_OK);

        DestroyAMLObject(amlObj);
    }

    TEST(AMLObject_CreateWithoutIdTest, Invalid_Parameter)
    {
        amlObjectHandle_t amlObj;
        EXPECT_EQ(CreateAMLObjectWithID("deviceId", "timeStamp", "", &amlObj), CAML_INVALID_PARAM);
    }

    TEST(AMLObject_DestroyTest, DestroyObject)
    {
        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);

        EXPECT_EQ(DestroyAMLObject(amlObj), CAML_OK);
    }

    TEST(AMLObject_DestroyTest, DestroyObject_InvalidHandle)
    {
        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);
        EXPECT_EQ(DestroyAMLObject(amlObj), CAML_OK);

        EXPECT_EQ(DestroyAMLObject(amlObj), CAML_INVALID_HANDLE);
    }

    TEST(AMLObject_CloneAMLObject, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        AMLData_SetValueStr(amlData, "key1", "value1");
        
        amlDataHandle_t value2;
        CreateAMLData(&value2);
        
        AMLData_SetValueAMLData(amlData, "key2", value2);

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);

        AMLObject_AddData(amlObj, "dataName", amlData);

        amlObjectHandle_t cloneObj;
        
        EXPECT_EQ(CloneAMLObject(amlObj, &cloneObj), CAML_OK);
        EXPECT_TRUE(isEqualAMLObject(amlObj, cloneObj));

        DestroyAMLData(amlData);
        DestroyAMLData(value2);

        EXPECT_EQ(DestroyAMLObject(amlObj), CAML_OK);
        EXPECT_EQ(DestroyAMLObject(cloneObj), CAML_OK);
    }

    TEST(AMLObject_CloneAMLObject, InvalidHandle)
    {
        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);
        DestroyAMLObject(amlObj);

        amlObjectHandle_t cloneObj;

        EXPECT_EQ(CloneAMLObject(amlObj, &cloneObj), CAML_INVALID_HANDLE);
    }

    TEST(AMLObject_AddDataTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, "key", "value"), CAML_OK);

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);

        EXPECT_EQ(AMLObject_AddData(amlObj, "dataName", amlData), CAML_OK);

        DestroyAMLData(amlData);
        DestroyAMLObject(amlObj);
    }

    TEST(AMLObject_AddDataTest, InvalidHandle)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);

        EXPECT_EQ(AMLData_SetValueStr(amlData, "key", "value"), CAML_OK);

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);
        DestroyAMLObject(amlObj);

        EXPECT_EQ(AMLObject_AddData(amlObj, "dataName", amlData), CAML_INVALID_HANDLE);

        DestroyAMLData(amlData);
    }

    TEST(AMLObject_AddDataTest, Invalid_DuplicatedKey)
    {
        amlDataHandle_t amlData1;
        CreateAMLData(&amlData1);
        
        EXPECT_EQ(AMLData_SetValueStr(amlData1, "key1", "value1"), CAML_OK);

        amlDataHandle_t amlData2;
        CreateAMLData(&amlData2);

        EXPECT_EQ(AMLData_SetValueStr(amlData2, "key2", "value2"), CAML_OK);

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);

        EXPECT_EQ(AMLObject_AddData(amlObj, "dataName", amlData1), CAML_OK);
        EXPECT_EQ(AMLObject_AddData(amlObj, "dataName", amlData2), CAML_KEY_ALREADY_EXIST);

        DestroyAMLData(amlData1);
        DestroyAMLData(amlData2);
        DestroyAMLObject(amlObj);
    }

    TEST(AMLObject_GetDataTest, Valid)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, "key", "value"), CAML_OK);

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);

        EXPECT_EQ(AMLObject_AddData(amlObj, "dataName", amlData), CAML_OK);

        amlDataHandle_t res;
        EXPECT_EQ(AMLObject_GetData(amlObj, "dataName", &res), CAML_OK);

        EXPECT_TRUE(isEqualAMLData(amlData, res));

        DestroyAMLData(amlData);
        DestroyAMLObject(amlObj);
    }

    TEST(AMLObject_GetDataTest, InvalidHandle)
    {
        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);
        DestroyAMLObject(amlObj);

        amlDataHandle_t res;
        EXPECT_EQ(AMLObject_GetData(amlObj, "dataName", &res), CAML_INVALID_HANDLE);
    }

    TEST(AMLObject_GetDataTest, InvalidKey)
    {
        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        
        EXPECT_EQ(AMLData_SetValueStr(amlData, "key", "value"), CAML_OK);

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);

        EXPECT_EQ(AMLObject_AddData(amlObj, "dataName", amlData), CAML_OK);

        amlDataHandle_t res;
        EXPECT_EQ(AMLObject_GetData(amlObj, "DataName", &res), CAML_KEY_NOT_EXIST);

        DestroyAMLData(amlData);
        DestroyAMLObject(amlObj);
    }    

    TEST(AMLObject_GetDataNamesTest, Valid)
    {
        amlDataHandle_t amlData1;
        CreateAMLData(&amlData1);

        amlDataHandle_t amlData2;
        CreateAMLData(&amlData2);

        const char* dataName1 = "dataName1";
        const char* dataName2 = "dataName2";

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);

        EXPECT_EQ(AMLObject_AddData(amlObj, dataName1, amlData1), CAML_OK);
        EXPECT_EQ(AMLObject_AddData(amlObj, dataName2, amlData2), CAML_OK);

        char** dataNames = NULL;
        size_t size;

        EXPECT_EQ(AMLObject_GetDataNames(amlObj, &dataNames, &size), CAML_OK);

        EXPECT_TRUE(isPresent(dataName1, dataNames, size));
        EXPECT_TRUE(isPresent(dataName2, dataNames, size));
        EXPECT_FALSE(isPresent("dataName3", dataNames, size));

        DestroyAMLData(amlData1);
        DestroyAMLData(amlData2);
        DestroyAMLObject(amlObj);
    }

    TEST(AMLObject_GetDataNamesTest, InvalidHandle)
    {
        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "timeStamp", &amlObj);
        DestroyAMLObject(amlObj);

        char** dataNames = NULL;
        size_t size;

        EXPECT_EQ(AMLObject_GetDataNames(amlObj, &dataNames, &size), CAML_INVALID_HANDLE);
    }
}