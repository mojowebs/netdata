// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETDATA_PERFLIB_H
#define NETDATA_PERFLIB_H

#include "libnetdata/libnetdata.h"
#include <windows.h>

const char *RegistryFindNameByID(DWORD id);
const char *RegistryFindHelpByID(DWORD id);
DWORD RegistryFindIDByName(const char *name);
#define PERFLIB_REGISTRY_NAME_NOT_FOUND (DWORD)-1

PERF_DATA_BLOCK *perflibGetPerformanceData(DWORD id);
void perflibFreePerformanceData(void);
PERF_OBJECT_TYPE *perflibFindObjectTypeByName(PERF_DATA_BLOCK *pDataBlock, const char *name);
PERF_INSTANCE_DEFINITION *perflibForEachInstance(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, PERF_INSTANCE_DEFINITION *lastInstance);

typedef struct _rawdata {
    DWORD CounterType;
    DWORD MultiCounterData;  // Second raw counter value for multi-valued counters
    ULONGLONG Data;          // Raw counter data
    LONGLONG Time;           // Is a time value or a base value
    LONGLONG Frequency;
} RAW_DATA, *PRAW_DATA;

typedef struct _counterdata {
    DWORD id;
    bool updated;
    const char *key;
    DWORD OverwriteCounterType; // if set, the counter type will be overwritten once read
    RAW_DATA current;
    RAW_DATA previous;
} COUNTER_DATA;

#define RAW_DATA_EMPTY (RAW_DATA){ 0 }

bool perflibGetInstanceCounter(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, PERF_INSTANCE_DEFINITION *pInstance, COUNTER_DATA *cd);
bool perflibGetObjectCounter(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, COUNTER_DATA *cd);

typedef bool (*perflib_data_cb)(PERF_DATA_BLOCK *pDataBlock, void *data);
typedef bool (*perflib_object_cb)(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, void *data);
typedef bool (*perflib_instance_cb)(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, PERF_INSTANCE_DEFINITION *pInstance, void *data);
typedef bool (*perflib_instance_counter_cb)(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, PERF_INSTANCE_DEFINITION *pInstance, PERF_COUNTER_DEFINITION *pCounter, RAW_DATA *sample, void *data);
typedef bool (*perflib_counter_cb)(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, PERF_COUNTER_DEFINITION *pCounter, RAW_DATA *sample, void *data);

int perflibQueryAndTraverse(DWORD id,
                               perflib_data_cb dataCb,
                               perflib_object_cb objectCb,
                               perflib_instance_cb instanceCb,
                               perflib_instance_counter_cb instanceCounterCb,
                               perflib_counter_cb counterCb,
                               void *data);

bool ObjectTypeHasInstances(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType);

BOOL getInstanceName(PERF_DATA_BLOCK *pDataBlock, PERF_OBJECT_TYPE *pObjectType, PERF_INSTANCE_DEFINITION *pInstance,
                     char *buffer, size_t bufferLen);

BOOL getSystemName(PERF_DATA_BLOCK *pDataBlock, char *buffer, size_t bufferLen);

PERF_OBJECT_TYPE *getObjectTypeByIndex(PERF_DATA_BLOCK *pDataBlock, DWORD ObjectNameTitleIndex);

PERF_INSTANCE_DEFINITION *getInstanceByPosition(
    PERF_DATA_BLOCK *pDataBlock,
    PERF_OBJECT_TYPE *pObjectType,
    DWORD instancePosition);

void PerflibNamesRegistryInitialize(void);
void PerflibNamesRegistryUpdate(void);

#endif //NETDATA_PERFLIB_H
