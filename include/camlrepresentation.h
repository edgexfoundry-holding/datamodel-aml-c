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

#ifndef C_AML_REPRESENTATION_H_
#define C_AML_REPRESENTATION_H_

#include <stdint.h>

#include "camlinterface.h"
#include "camlerrorcodes.h"

#define AML_EXPORT __attribute__ ((visibility("default")))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Representation handle
 */
typedef void * representation_t;


/**
 * @brief       Create an instance of Representation.
 * @param       filePath        [in] path of an AML file that contains data model information.
 * @param       repHandle       [out] handle of created Representation.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_FILE_PATH Invalid file path.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory.
 * @note        Representation instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use DestroyRepresentation().
 */
AML_EXPORT CAMLErrorCode CreateRepresentation(const char* filePath,
                                              representation_t* repHandle);

/**
 * @brief       Destroy an instance of Representation.
 * @param       repHandle       [in] handle of Representation that will be destroyed.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 */
AML_EXPORT CAMLErrorCode DestroyRepresentation(representation_t repHandle);

/**
 * @brief       This function returns AutomationML SystemUnitClassLib's unique ID.
 * @param       repHandle       [in] handle of Representation.
 * @param       repId           [out] ID of Representation.
 * @retval      #CAML_OK                Successful.
 * @retval      #CAML_INVALID_PARAM     Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE    Invalid handle.
 * @retval      #CAML_NO_MEMORY         Failed to alloc memory.
 * @note        Characters will be allocated to 'repId', so it should be freed after use. (See the below example)
 *              ex) free(repId);
 */
AML_EXPORT CAMLErrorCode Representation_GetRepId(const representation_t repHandle,
                                                 char** repId);

/**
 * @brief       This function returns AMLObject that contains configuration data which is present in RoleClassLib.
 * @param       repHandle       [in] handle of Representation.
 * @param       amlObjHandle    [out] handle of AMLObject.
 * @retval      #CAML_OK                 Successful.
 * @retval      #CAML_INVALID_PARAM      Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE     Invalid handle.
 * @retval      #CAML_INVALID_AML_SCHEMA The AML, which is set by CreateRepresentation, has a invalid schema.
 * @note        AMLObject instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLObject().
 * @see         DestroyAMLObject
 */
AML_EXPORT CAMLErrorCode Representation_GetConfigInfo(const representation_t repHandle,
                                                      amlObjectHandle_t* amlObjHandle);

/**
 * @brief       This function converts AMLObject to AML(XML) string to match the AML model information which is set on CreateRepresentation().
 * @param       repHandle       [in] handle of Representation.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       amlStr          [out] AML(XML) string.
 * @retval      #CAML_OK                 Successful.
 * @retval      #CAML_INVALID_PARAM      Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE     Invalid handle.
 * @retval      #CAML_INVALID_AML_SCHEMA The AML, which is set by CreateRepresentation, has a invalid schema.
 * @note        Characters will be allocated to 'amlStr', so it should be freed after use. (See the below example)
 *              ex) free(amlStr);
 */
AML_EXPORT CAMLErrorCode Representation_DataToAml(const representation_t repHandle,
                                                  const amlObjectHandle_t amlObjHandle,
                                                  char** amlStr);

/**
 * @brief       This function converts AML(XML) string to AMLObject to match the AML model information which is set on CreateRepresentation().
 * @param       repHandle       [in] handle of Representation.
 * @param       amlStr          [in] AML(XML) string.
 * @param       amlObjHandle    [out] handle of AMLObject.
 * @retval      #CAML_OK                 Successful.
 * @retval      #CAML_INVALID_PARAM      Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE     Invalid handle.
 * @retval      #CAML_INVALID_AML_SCHEMA The AML, which is set by CreateRepresentation, has a invalid schema.
 * @note        AMLObject instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLObject().
 * @see         DestroyAMLObject
 */
AML_EXPORT CAMLErrorCode Representation_AmlToData(const representation_t repHandle,
                                                  const char* amlStr,
                                                  amlObjectHandle_t* amlObjHandle);

/**
 * @brief       This function converts AMLObject to Protobuf byte data to match the AML model information which is set on CreateRepresentation().
 * @param       repHandle       [in] handle of Representation.
 * @param       amlObjHandle    [in] handle of AMLObject.
 * @param       byte            [out] byte data serialized by Protobuf.
 * @param       size            [out] size of byte data.
 * @retval      #CAML_OK                 Successful.
 * @retval      #CAML_INVALID_PARAM      Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE     Invalid handle.
 * @retval      $CAML_INVALID_BYTE_STR   Invalid protobuf byte string.
 * @retval      #CAML_INVALID_AML_SCHEMA The AML, which is set by CreateRepresentation, has a invalid schema.
 * @retval      #CAML_API_NOT_ENABLED    If datamodel-aml-cpp library is built with 'disable_protobuf' option, this API will be disabled.
 * @note        Characters will be allocated to 'amlStr', so it should be freed after use. (See the below example)
 *              ex) free(amlStr);
 */
AML_EXPORT CAMLErrorCode Representation_DataToByte(const representation_t repHandle,
                                                   const amlObjectHandle_t amlObjHandle,
                                                   uint8_t** byte, 
                                                   size_t* size);

/**
 * @brief       This function converts Protobuf byte data to AMLObject to match the AML model information which is set on CreateRepresentation().
 * @param       repHandle       [in] handle of Representation.
 * @param       byte            [in] byte data serialized by Protobuf.
 * @param       size            [in] size of byte data.
 * @param       amlObjHandle    [out] handle of AMLObject.
 * @retval      #CAML_OK                 Successful.
 * @retval      #CAML_INVALID_PARAM      Invalid parameter.
 * @retval      #CAML_INVALID_HANDLE     Invalid handle.
 * @retval      #CAML_INVALID_AML_SCHEMA The AML, which is set by CreateRepresentation, has a invalid schema.
 * @retval      #CAML_API_NOT_ENABLED    If datamodel-aml-cpp library is built with 'disable_protobuf' option, this API will be disabled.
 * @note        AMLObject instance will be allocated, so it should be deleted after use.
 *              To destroy an instance, use DestroyAMLObject().
 * @see         DestroyAMLObject
 */
AML_EXPORT CAMLErrorCode Representation_ByteToData(const representation_t repHandle,
                                                   const uint8_t* byte,
                                                   const size_t size,
                                                   amlObjectHandle_t* amlObjHandle);
#ifdef __cplusplus
}
#endif

#endif // C_AML_REPRESENTATION_H_