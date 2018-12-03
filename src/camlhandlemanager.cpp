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

#include <assert.h>
#include <string>
#include <vector>
#include <mutex>

#include "camlhandlemanager.h"
#include "utlist.h"

using namespace std;
using namespace AML;

typedef struct amlObject_t {
    AML::AMLObject* cppObj;
    bool needsDelete;
    struct amlObject_t *next;
} amlObject_t;

typedef struct amlData_t {
    AML::AMLData* cppObj;
    bool needsDelete;
    struct amlData_t *next;
} amlData_t;

typedef struct amlRep_t {
    AML::Representation* cppObj;
    struct amlRep_t *next;
} amlRep_t;

static amlObject_t *g_amlObjectHead = NULL;
static amlData_t *g_amlDataHead = NULL;
static amlRep_t *g_amlRepHead = NULL;

static mutex g_amlObjectMtx;
static mutex g_amlDataMtx;
static mutex g_amlRepMtx;

amlObjectHandle_t AddAmlObjHandle(AMLObject* amlObj, bool needsDelete)
{
    amlObject_t* node = (amlObject_t*) malloc(sizeof(amlObject_t));
    if (NULL == node)
    {
        return NULL;
    }

    node->cppObj = amlObj;
    node->needsDelete = needsDelete;

    g_amlObjectMtx.lock();
    LL_APPEND(g_amlObjectHead, node);
    g_amlObjectMtx.unlock();

    return (amlObjectHandle_t)node;
}

void RemoveAmlObj(amlObjectHandle_t handle)
{
    assert(handle);

    amlObject_t *amlObj = (amlObject_t*)handle;

    g_amlObjectMtx.lock();
    LL_DELETE(g_amlObjectHead, amlObj);
    g_amlObjectMtx.unlock();

    if (amlObj->needsDelete)
    {
        delete amlObj->cppObj;
    }
    free(amlObj);
    amlObj = NULL;
}

AMLObject* FindAmlObj(amlObjectHandle_t handle)
{
    assert(handle);

    amlObject_t *target = (amlObject_t*)handle;
    amlObject_t *node = NULL;

    g_amlObjectMtx.lock();
    LL_FOREACH(g_amlObjectHead, node)
    {
        if (node == target)
        {
            g_amlObjectMtx.unlock();
            return node->cppObj;
        }
    }
    g_amlObjectMtx.unlock();

    return NULL;
}

amlDataHandle_t AddAmlDataHandle(AMLData* amlData, bool needsDelete)
{
    amlData_t* node = (amlData_t*) malloc(sizeof(amlData_t));
    if (NULL == node)
    {
        return NULL;
    }

    node->cppObj = amlData;
    node->needsDelete = needsDelete;

    g_amlDataMtx.lock();
    LL_APPEND(g_amlDataHead, node);
    g_amlDataMtx.unlock();

    return (amlDataHandle_t)node;
}

void RemoveAmlData(amlDataHandle_t handle)
{
    assert(handle);

    amlData_t* amlData = (amlData_t*)handle;

    g_amlDataMtx.lock();
    LL_DELETE(g_amlDataHead, amlData);
    g_amlDataMtx.unlock();

    if (amlData->needsDelete)
    {
        delete amlData->cppObj;
    }
    free(amlData);
    amlData = NULL;
}

void RemoveAmlData(AMLData* amlData)
{
    assert(amlData);

    amlData_t *target = NULL;

    g_amlDataMtx.lock();
    LL_FOREACH(g_amlDataHead, target)
    {
        if (target->cppObj == amlData)
        {
            if (target->needsDelete)
            {
                delete target->cppObj;
            }

            LL_DELETE(g_amlDataHead, target);
            free(target);
            target = NULL;

            g_amlDataMtx.unlock();
            return;
        }
    }
    g_amlDataMtx.unlock();
}

AMLData* FindAmlData(amlDataHandle_t handle)
{
    amlData_t *target = (amlData_t*)handle;
    amlData_t *node = NULL;

    g_amlDataMtx.lock();
    LL_FOREACH(g_amlDataHead, node)
    {
        if (node == target)
        {
            g_amlDataMtx.unlock();
            return node->cppObj;
        }
    }
    g_amlDataMtx.unlock();

    return NULL;
}

amlDataHandle_t* FindAmlDataHandle(AMLData* amlData)
{
    amlData_t *target = NULL;

    g_amlDataMtx.lock();
    LL_FOREACH(g_amlDataHead, target)
    {
        if (target->cppObj == amlData)
        {
            g_amlDataMtx.unlock();
            return (amlDataHandle_t*)target;
        }
    }
    g_amlDataMtx.unlock();
    return NULL;
}

representation_t AddRepresentationHandle(Representation* rep)
{
    amlRep_t* node = (amlRep_t*) malloc(sizeof(amlRep_t));
    if (NULL == node)
    {
        return NULL;
    }

    node->cppObj = rep;

    g_amlRepMtx.lock();
    LL_APPEND(g_amlRepHead, node);
    g_amlRepMtx.unlock();

    return (representation_t)node;
}

void RemoveRepresentation(representation_t handle)
{
    assert(handle);

    amlRep_t* rep = (amlRep_t*)handle;

    g_amlRepMtx.lock();
    LL_DELETE(g_amlRepHead, rep);
    g_amlRepMtx.unlock();

    delete rep->cppObj;
    free(rep);
    rep = NULL;
}

Representation* FindRepresentation(representation_t handle)
{
    amlRep_t *target = (amlRep_t*)handle;
    amlRep_t *node = NULL;

    g_amlRepMtx.lock();
    LL_FOREACH(g_amlRepHead, node)
    {
        if (node == target)
        {
            g_amlRepMtx.unlock();
            return node->cppObj;
        }
    }
    g_amlRepMtx.unlock();

    return NULL;
}

void RemoveOwnedAmlDataHandles(AMLData* amlData)
{
    vector<string> keys = amlData->getKeys();
    for (string key : keys)
    {
        AMLValueType type = amlData->getValueType(key);
        if (AMLValueType::AMLData == type)
        {
            AMLData ownedAmlData = amlData->getValueToAMLData(key);
            RemoveOwnedAmlDataHandles(&ownedAmlData);
            RemoveAmlData(&ownedAmlData);
        }
    }
}

void RemoveOwnedAmlDataHandles(AMLObject* amlObj)
{
    vector<string> dataNames = amlObj->getDataNames();

    for (string name : dataNames)
    {
        AMLData ownedAmlData = amlObj->getData(name);
        RemoveOwnedAmlDataHandles(&ownedAmlData);
        RemoveAmlData(&ownedAmlData);
    }
}
