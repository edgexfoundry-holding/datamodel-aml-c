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

#include "camlrepresentation.h"
#include "camlinterface.h"
#include "camlerrorcodes.h"
#include "gtest/gtest.h"

using namespace std;

namespace camlrepresentationtest
{
    const char* amlModelFile                = "./TEST_DataModel.aml";
    const char* amlModelFile_invalid_NoCAEX = "./TEST_DataModel_Invalid_NoCAEX.aml";
    const char* amlModelFile_invalid_NoSUCL = "./TEST_DataModel_Invalid_NoSUCL.aml";
    const char* amlDataFile                 = "./TEST_Data.aml";
    const char* dataBinaryFile              = "./TEST_DataBinary";

    const char* amlModelId                    = "SAMPLE_Robot_0.0.1"; 

    //Helper method
    
    amlObjectHandle_t TestAMLObjectHandle()
    {
        // create "Model" data
        amlDataHandle_t model;
        CreateAMLData(&model);

        AMLData_SetValueStr(model, "a", "Model_107.113.97.248");
        AMLData_SetValueStr(model, "b", "SR-P7-970");

        // create "Sample" data
        amlDataHandle_t axis;
        CreateAMLData(&axis);
        AMLData_SetValueStr(axis, "x", "20");
        AMLData_SetValueStr(axis, "y", "110");
        AMLData_SetValueStr(axis, "z", "80");

        amlDataHandle_t info;
        CreateAMLData(&info);
        AMLData_SetValueStr(info, "id", "f437da3b");
        AMLData_SetValueAMLData(info, "axis", axis);

        amlDataHandle_t sample;
        CreateAMLData(&sample);
        AMLData_SetValueAMLData(sample, "info", info);
        const char* appendix[3] = {"52303", "935", "1442"};
        AMLData_SetValueStrArr(sample, "appendix", appendix, 3);


        // set datas to object
        amlObjectHandle_t object;
        CreateAMLObject("SAMPLE001", "123456789", &object);
        
        AMLObject_AddData(object, "Model", model);
        AMLObject_AddData(object, "Sample", sample);

        DestroyAMLData(model);
        DestroyAMLData(axis);
        DestroyAMLData(info);
        DestroyAMLData(sample);

        return object;
    }

    char* TestAML()
    {
        std::ifstream t(amlDataFile);
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        
        size_t size = str.size();
        
        char* cstr = new char[size + 1];
        memcpy(cstr, str.c_str(), size);
        cstr[size] = '\0';

        return cstr;
    }

    void TestBinary(uint8_t** amlByte, size_t* byteSize)
    {
        std::ifstream t(dataBinaryFile);
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        
        size_t size = str.size();
        char* cstr = new char[size + 1];
        memcpy(cstr, str.c_str(), size);
        cstr[size] = '\0';

        *amlByte = (uint8_t*)cstr;
        *byteSize = size;
        
        return;
    }

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

    bool isEqualBinary(uint8_t* binary1, size_t size1, uint8_t* binary2, size_t size2)
    {
        if (size1 != size2) return false;

        char debug1[size1 * 2 + 1];
        memset(debug1, 0x00, sizeof(debug1));
        char debug2[size2 * 2 + 1];
        memset(debug2, 0x00, sizeof(debug2));

        size_t i;
        for (i = 0; i < size1; i++)
        {
            snprintf(debug1 + 2*i, sizeof(debug1), "%02X", binary1[i]);
        }

        for (i = 0; i < size2; i++)
        {
            snprintf(debug2 + 2*i, sizeof(debug2), "%02X", binary2[i]);
        }

         return (0 == strncmp(debug1, debug2, sizeof(debug1)));
    }

    // Test
    TEST(ConstructRepresentationTest, ValidAML)
    {
        representation_t rep;
        EXPECT_EQ(CreateRepresentation(amlModelFile, &rep), CAML_OK);

        DestroyRepresentation(rep);
    }

    TEST(ConstructRepresentationTest, InvalidFilePath)
    {
        representation_t rep;
        EXPECT_EQ(CreateRepresentation("NoExist.aml", &rep), CAML_INVALID_FILE_PATH);
    }

    TEST(ConstructRepresentationTest, AMLWithoutCAEX)
    {
        representation_t rep;
        EXPECT_EQ(CreateRepresentation(amlModelFile_invalid_NoCAEX, &rep), CAML_INVALID_AML_SCHEMA);
    }

    TEST(ConstructRepresentationTest, AMLWithoutSUCL)
    {
        representation_t rep;
        EXPECT_EQ(CreateRepresentation(amlModelFile_invalid_NoSUCL, &rep), CAML_INVALID_AML_SCHEMA);
    }

    TEST(DestroyRepresentationTest, Valid)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        EXPECT_EQ(DestroyRepresentation(rep), CAML_OK);
    }

    TEST(DestroyRepresentationTest, InvalidHandle)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);
        EXPECT_EQ(DestroyRepresentation(rep), CAML_OK);

        EXPECT_EQ(DestroyRepresentation(rep), CAML_INVALID_HANDLE);
    }

    TEST(Representation_AmlToDataTest, ConvertValid)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj;
        char* amlStr = TestAML();

        EXPECT_EQ(Representation_AmlToData(rep, amlStr, &amlObj), CAML_OK);

        amlObjectHandle_t varify = TestAMLObjectHandle();
        EXPECT_TRUE(isEqualAMLObject(amlObj, varify));

        DestroyAMLObject(amlObj);
        DestroyAMLObject(varify);
        DestroyRepresentation(rep);
    }

    TEST(Representation_AmlToDataTest, InvalidHandle)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);
        DestroyRepresentation(rep);

        amlObjectHandle_t amlObj;
        char* amlStr = TestAML();

        EXPECT_EQ(Representation_AmlToData(rep, amlStr, &amlObj), CAML_INVALID_HANDLE);
    }

    TEST(Representation_AmlToDataTest, InvalidAml)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj;
        const char* amlStr = "<invalid />";

        EXPECT_EQ(Representation_AmlToData(rep, amlStr, &amlObj), CAML_INVALID_AML_SCHEMA);

        DestroyAMLObject(amlObj);
        DestroyRepresentation(rep);
    }

    TEST(Representation_DataToAmlTest, ConvertValid)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj;
        amlObj = TestAMLObjectHandle();

        char* amlStr;
        EXPECT_EQ(Representation_DataToAml(rep, amlObj, &amlStr), CAML_OK);

        char* varify = TestAML();
        EXPECT_TRUE(isEqual(amlStr, varify));

        DestroyRepresentation(rep);
    }

    TEST(Representation_DataToAmlTest, InvalidHandle)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);
        DestroyRepresentation(rep);

        amlObjectHandle_t amlObj;
        amlObj = TestAMLObjectHandle();

        char* amlStr;
        EXPECT_EQ(Representation_DataToAml(rep, amlObj, &amlStr), CAML_INVALID_HANDLE);
    }

    TEST(Representation_DataToAmlTest, InvalidDataToModel)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj;
        CreateAMLObject("deviceId", "0", &amlObj);

        amlDataHandle_t amlData;
        CreateAMLData(&amlData);
        AMLData_SetValueStr(amlData, "invalidKey", "invalidValue");
        
        AMLObject_AddData(amlObj, "invalidData", amlData);

        char* amlStr;

        EXPECT_EQ(Representation_DataToAml(rep, amlObj, &amlStr), CAML_NOT_MATCH_TO_AML_MODEL);

        DestroyAMLData(amlData);
        DestroyAMLObject(amlObj);
        DestroyRepresentation(rep);
    }
    
    TEST(Representation_DataToAmlTest, IgnoreDataNotInModel)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj;
        amlObj = TestAMLObjectHandle();

        amlDataHandle_t amlData;
        AMLObject_GetData(amlObj, "Sample", &amlData);

        AMLData_SetValueStr(amlData, "additionalKey", "additionalValue");
        
        char* amlStr;
        EXPECT_EQ(Representation_DataToAml(rep, amlObj, &amlStr), CAML_OK);
        
        amlObjectHandle_t resultObj;
        EXPECT_EQ(Representation_AmlToData(rep, amlStr, &resultObj), CAML_OK);

        amlDataHandle_t sampleResult;
        EXPECT_EQ(AMLObject_GetData(resultObj, "Sample", &sampleResult), CAML_OK);

        char * resultStr;
        EXPECT_EQ(AMLData_GetValueStr(sampleResult, "additionalKey", &resultStr), CAML_KEY_NOT_EXIST);
        
        DestroyAMLObject(resultObj);
        DestroyAMLObject(amlObj);
        DestroyRepresentation(rep);
    }

    TEST(Representation_ByteToDataTest, ConvertValid)
    {   
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj;
        uint8_t* binary;
        size_t size;

        TestBinary(&binary, &size);

#ifndef _DISABLE_PROTOBUF_
        EXPECT_EQ(Representation_ByteToData(rep, binary, size, &amlObj), CAML_OK);
        amlObjectHandle_t varify = TestAMLObjectHandle();
        
        EXPECT_TRUE(isEqualAMLObject(amlObj, varify));

        DestroyAMLObject(amlObj);
        DestroyAMLObject(varify);
#else
        EXPECT_EQ(Representation_ByteToData(rep, binary, size, &amlObj), CAML_API_NOT_ENABLED);
#endif
        DestroyRepresentation(rep);
    }

    TEST(Representation_ByteToDataTest, InvalidHandle)
    {   
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);
        DestroyRepresentation(rep);

        amlObjectHandle_t amlObj;
        uint8_t* binary;
        size_t size;

        TestBinary(&binary, &size);

        EXPECT_EQ(Representation_ByteToData(rep, binary, size, &amlObj), CAML_INVALID_HANDLE);
    }

    TEST(Representation_ByteToDataTest, InvalidByte)
    {   
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj;
        
        std::string invalidBinary("invalidBinary");

        uint8_t* binary = (uint8_t*)invalidBinary.c_str();
        size_t size = invalidBinary.size();

#ifndef _DISABLE_PROTOBUF_
        EXPECT_EQ(Representation_ByteToData(rep, binary, size, &amlObj), CAML_INVALID_BYTE_STR);
#else
        EXPECT_EQ(Representation_ByteToData(rep, binary, size, &amlObj), CAML_API_NOT_ENABLED);
#endif
        DestroyRepresentation(rep);
    }

    TEST(Representation_DataToByteTest, ConvertValid)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t amlObj = TestAMLObjectHandle();

        uint8_t* amlBinary;
        size_t size;
    
#ifndef _DISABLE_PROTOBUF_

        EXPECT_EQ(Representation_DataToByte(rep, amlObj, &amlBinary, &size), CAML_OK);

        uint8_t* varify;
        size_t varify_size;
        TestBinary(&varify, &varify_size);

        EXPECT_TRUE(isEqualBinary(amlBinary, size, varify, varify_size));

        DestroyAMLObject(amlObj);

#else
        EXPECT_EQ(Representation_DataToByte(rep, amlObj, &amlBinary, &size), CAML_API_NOT_ENABLED);
#endif
        DestroyRepresentation(rep);
    }

    TEST(Representation_DataToByteTest, InvalidHandle)
    {
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);
        DestroyRepresentation(rep);

        amlObjectHandle_t amlObj = TestAMLObjectHandle();

        uint8_t* amlBinary;
        size_t size;

        EXPECT_EQ(Representation_DataToByte(rep, amlObj, &amlBinary, &size), CAML_INVALID_HANDLE);
    }

    TEST(GetRepresentationIdTest, GetValid)
    {   
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        char* repId;
        Representation_GetRepId(rep, &repId);
        EXPECT_TRUE(isEqual(amlModelId, repId));

        DestroyRepresentation(rep);
    }

    TEST(GetRepresentationIdTest, InvalidHandle)
    {   
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);
        DestroyRepresentation(rep);

        char* repId;
        EXPECT_EQ(Representation_GetRepId(rep, &repId), CAML_INVALID_HANDLE);
    }

    TEST(GetConfigInfoTest, Valid)
    {   
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);

        amlObjectHandle_t config;
        EXPECT_EQ(Representation_GetConfigInfo(rep, &config), CAML_OK);

        DestroyAMLObject(config);
        DestroyRepresentation(rep);
    }

    TEST(GetConfigInfoTest, InvalidHandle)
    {   
        representation_t rep;
        CreateRepresentation(amlModelFile, &rep);
        DestroyRepresentation(rep);

        amlObjectHandle_t config;
        EXPECT_EQ(Representation_GetConfigInfo(rep, &config), CAML_INVALID_HANDLE);
    }
}

