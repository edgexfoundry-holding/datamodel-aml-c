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

#ifndef C_AML_HANDLE_MANAGER_H_
#define C_AML_HANDLE_MANAGER_H_

#include "AMLInterface.h"
#include "Representation.h"
#include "camlinterface.h"
#include "camlrepresentation.h"

amlObjectHandle_t AddAmlObjHandle(AML::AMLObject* amlObj, bool needsDelete);
void RemoveAmlObj(amlObjectHandle_t handle);
AML::AMLObject* FindAmlObj(amlObjectHandle_t handle);

amlDataHandle_t AddAmlDataHandle(AML::AMLData* amlData, bool needsDelete);
void RemoveAmlData(amlDataHandle_t handle);
void RemoveAmlData(AML::AMLData* amlData);
AML::AMLData* FindAmlData(amlDataHandle_t handle);
amlDataHandle_t* FindAmlDataHandle(AML::AMLData* amlData);

representation_t AddRepresentationHandle(AML::Representation* cppRep);
void RemoveRepresentation(representation_t handle);
AML::Representation* FindRepresentation(representation_t handle);

void RemoveOwnedAmlDataHandles(AML::AMLData* amlData);
void RemoveOwnedAmlDataHandles(AML::AMLObject* amlObj);

#endif // C_AML_HANDLE_MANAGER_H_
