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

#ifndef C_AML_INTERFACE_H_
#define C_AML_INTERFACE_H_

#include <stdlib.h>

#include "camlerrorcodes.h"

#define AML_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * AMLObject handle
 */
typedef void * amlObjectHandle_t;

/**
 * AMLData handle
 */
typedef void * amlDataHandle_t;


typedef enum
{
    AMLVALTYPE_STRING = 0,
    AMLVALTYPE_STRINGARRAY,
    AMLVALTYPE_AMLDATA
} CAMLValueType;


/**
 * @brief       Create an instance of AMLObject.
 * @param       deviceId        [in] Device id that source device of AMLObject.
 * @param       timestamp       [in] timestamp value of AMLObject delivered by device.
 * @param       amlObjHandle    [out] handle of created AMLObject.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to AMLObject instance.
 * @note        AMLObject instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLObject().
 */
AML_EXPORT CAMLErrorCode CreateAMLObject(const char* deviceId,
                                         const char* timeStamp,
                                         amlObjectHandle_t* amlObjHandle);

/**
 * @brief       Create an instance of AMLObject.
 * @param       deviceId        [in] Device id that source device of AMLObject.
 * @param       timestamp       [in] timestamp value of AMLObject delivered by device.
 * @param       id              [in] id of AMLObject.
 * @param       amlObjHandle    [out] handle of created AMLObject.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to AMLObject instance.
 * @note        AMLObject instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLObject().
 */
AML_EXPORT CAMLErrorCode CreateAMLObjectWithID(const char* deviceId,
                                               const char* timeStamp,
                                               const char* id,
                                               amlObjectHandle_t* amlObjHandle);

/**
 * @brief       Destroy an instance of AMLObject.
 * @param       amlObjHandle    [in] AMLObject that will be destroyed.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 */
AML_EXPORT CAMLErrorCode DestroyAMLObject(amlObjectHandle_t amlObjHandle);

/**
 * @brief       Clone an instance of AMLObject.
 * @param       origin          [in] AMLObject that will be cloned.
 * @param       clone           [out] Cloned AMLObject.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @note        AMLObject instance will be allocated to 'clone', so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLObject().
 */
AML_EXPORT CAMLErrorCode CloneAMLObject(amlObjectHandle_t origin,
                                        amlObjectHandle_t* clone);

/**
 * @brief       This function adds AMLData to AMLObject using AMLData key that to match AMLData value.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       name            [in] AMLData key.
 * @param       amlDataHandle   [in] handle of AMLData value.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_ALREADY_EXIST Name already exists in AMLObject.
 */
AML_EXPORT CAMLErrorCode AMLObject_AddData(const amlObjectHandle_t amlObjHandle,
                                           const char* name,
                                           const amlDataHandle_t amlDataHandle);

/**
 * @brief       This function returns AMLData which matched input name string with AMLObject's amlDatas key.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       name            [in] AMLData key.
 * @param       amlDataHandle   [out] handle of AMLData value.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_NOT_EXIST     Name does not exists in AMLObject.
 */
AML_EXPORT CAMLErrorCode AMLObject_GetData(const amlObjectHandle_t amlObjHandle,
                                           const char* name,
                                           amlDataHandle_t* amlDataHandle);

/**
 * @brief       This function returns a list of AMLData names that AMLObject has.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       names           [out] character array that contains AMLData names.
 * @param       namesSize       [out] the size of 'names'.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to character array.
 * @note        Character array will be allocated to 'names', so it should be freed after use. (See the below example)
 *              ex) for (size_t i = 0; i < namesSize; i++) free(names[i]);
 *                  free(names);
 */
AML_EXPORT CAMLErrorCode AMLObject_GetDataNames(const amlObjectHandle_t amlObjHandle,
                                                char*** names,
                                                size_t* namesSize);

/**
 * @brief       This function returns the deviceId of AMLObject.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       deviceId        [out] deviceId.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to characters.
 * @note        Characters will be allocated to 'deviceId', so it should be freed after use. (See the below example)
 *              ex) free(deviceId);
 */
AML_EXPORT CAMLErrorCode AMLObject_GetDeviceId(const amlObjectHandle_t amlObjHandle,
                                               char** deviceId);

/**
 * @brief       This function returns the timeStamp of AMLObject.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       timeStamp       [out] timeStamp.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to characters.
 * @note        Characters will be allocated to 'timeStamp', so it should be freed after use. (See the below example)
 *              ex) free(timeStamp);
 */
AML_EXPORT CAMLErrorCode AMLObject_GetTimeStamp(const amlObjectHandle_t amlObjHandle,
                                                char** timeStamp);

/**
 * @brief       This function returns the id of AMLObject.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       id              [out] id.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to characters.
 * @note        Characters will be allocated to 'id', so it should be freed after use. (See the below example)
 *              ex) free(id);
 */
AML_EXPORT CAMLErrorCode AMLObject_GetId(const amlObjectHandle_t amlObjHandle,
                                         char** id);


/**
 * @brief       Create an instance of AMLData.
 * @param       amlDataHandle   [out] handle of created AMLData.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @note        AMLData instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLData().
 */
AML_EXPORT CAMLErrorCode CreateAMLData(amlDataHandle_t* amlDataHandle);

/**
 * @brief       Destroy an instance of AMLData.
 * @param       amlDataHandle   [in] AMLData that will be destroyed.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 */
AML_EXPORT CAMLErrorCode DestroyAMLData(amlDataHandle_t amlDataHandle);

/**
 * @brief       Clone an instance of AMLData.
 * @param       origin          [in] AMLData that will be cloned.
 * @param       clone           [out] Cloned AMLData.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @note        AMLData instance will be allocated to 'clone', so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLData().
 */
AML_EXPORT CAMLErrorCode CloneAMLData(amlDataHandle_t origin,
                                      amlDataHandle_t* clone);
/**
 * @brief       This function set key/value as a string value to AMLData.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       key             [in] key string.
 * @param       value           [in] string value.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_ALREADY_EXIST Key already exists in AMLData.
 */
AML_EXPORT CAMLErrorCode AMLData_SetValueStr(const amlDataHandle_t amlDataHandle,
                                             const char* key,
                                             const char* value);

/**
 * @brief       This function set key/value as a string array value to AMLData.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       key             [in] key string.
 * @param       value           [in] string array value.
 * @param       valueSize       [in] size of value array.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_ALREADY_EXIST Key already exists in AMLData.
 */
AML_EXPORT CAMLErrorCode AMLData_SetValueStrArr(const amlDataHandle_t amlDataHandle,
                                                const char* key,
                                                const char** value,
                                                const size_t valueSize);

/**
 * @brief       This function set key/value as a AMLData value to AMLData.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       key             [in] key string.
 * @param       value           [in] handle of AMLData that will be set as value.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_ALREADY_EXIST Key already exists in AMLData.
 */
AML_EXPORT CAMLErrorCode AMLData_SetValueAMLData(const amlDataHandle_t amlDataHandle,
                                                 const char* key,
                                                 const amlDataHandle_t value);

/**
 * @brief       This function returns a string value which matchs a key in AMLData.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       key             [in] key string.
 * @param       value           [out] string value.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_NOT_EXIST     Key does not exists in AMLData.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to characters.
 * @note        Characters will be allocated to 'value', so it should be freed after use. (See the below example)
 *              ex) free(value);
 */
AML_EXPORT CAMLErrorCode AMLData_GetValueStr(const amlDataHandle_t amlDataHandle,
                                             const char* key,
                                             char** value);

/**
 * @brief       This function returns a string array value which matchs a key in AMLData.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       key             [in] key string.
 * @param       value           [out] string array value.
 * @param       valueSize       [out] size of value array.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_NOT_EXIST     Key does not exists in AMLData.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to character array.
 * @note        Character array will be allocated to 'value', so it should be freed after use. (See the below example)
 *              ex) for (size_t i = 0; i < valueSize; i++) free(value[i]);
 *                  free(value);
 */
AML_EXPORT CAMLErrorCode AMLData_GetValueStrArr(const amlDataHandle_t amlDataHandle,
                                                const char* key,
                                                char*** value,
                                                size_t* valueSize);

/**
 * @brief       This function returns a AMLData value which matchs a key in AMLData.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       key             [in] key string.
 * @param       value           [out] handle of AMLData value.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_NOT_EXIST     Key does not exists in AMLData.
 */
AML_EXPORT CAMLErrorCode AMLData_GetValueAMLData(const amlDataHandle_t amlDataHandle,
                                                 const char* key,
                                                 amlDataHandle_t* value);

/**
 * @brief       This function returns a list of key that AMLData has.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       keys            [out] character array that contains keys.
 * @param       keysSize        [out] the size of 'keys'.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory to character array.
 * @note        Character array will be allocated to 'keys', so it should be freed after use. (See the below example)
 *              ex) for (size_t i = 0; i < keysSize; i++) free(keys[i]);
 *                  free(keys);
 */
AML_EXPORT CAMLErrorCode AMLData_GetKeys(const amlDataHandle_t amlDataHandle,
                                         char*** keys,
                                         size_t* keysSize);

/**
 * @brief       This function returns which type value of a key is.
 * @param       amlDataHandle   [in] handle of AMLData.
 * @param       key             [in] key string.
 * @param       type            [out] type of value of the key.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_KEY_NOT_EXIST     Key does not exists in AMLData.
 */
AML_EXPORT CAMLErrorCode AMLData_GetValueType(const amlDataHandle_t amlDataHandle,
                                              const char* key,
                                              CAMLValueType* type);


#ifdef __cplusplus
}
#endif

#endif // C_AML_INTERFACE_H_