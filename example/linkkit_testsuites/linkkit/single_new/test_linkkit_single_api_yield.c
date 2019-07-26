#if defined(DEVICE_MODEL_ENABLED)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(DEPRECATED_LINKKIT)

#include "iot_import.h"
#include "iot_export.h"
#include "cut.h"
#include "single_config_new.h"

DATA(test_linkkit_single_api_yield)
{
    user_example_ctx_t *user_example_ctx;
    iotx_linkkit_dev_meta_info_t master_meta_info;
};

SETUP(test_linkkit_single_api_yield)
{
    data->user_example_ctx = user_example_get_ctx();
    memset(data->user_example_ctx, 0, sizeof(user_example_ctx_t));

    cJSON_Hooks cjson_hooks = {(void *)test_malloc, (void *)test_free};
    cJSON_InitHooks(&cjson_hooks);

    IOT_SetLogLevel(IOT_LOG_DEBUG);

    /* Register Callback */
    IOT_RegisterCallback(ITE_CONNECT_SUCC, user_connected_event_handler);
    IOT_RegisterCallback(ITE_DISCONNECTED, user_disconnected_event_handler);
    IOT_RegisterCallback(ITE_RAWDATA_ARRIVED, user_down_raw_data_arrived_event_handler);
    IOT_RegisterCallback(ITE_SERVICE_REQUEST, user_service_request_event_handler);
    IOT_RegisterCallback(ITE_PROPERTY_SET, user_property_set_event_handler);
    IOT_RegisterCallback(ITE_PROPERTY_GET, user_property_get_event_handler);
    IOT_RegisterCallback(ITE_REPORT_REPLY, user_report_reply_event_handler);
    IOT_RegisterCallback(ITE_INITIALIZE_COMPLETED, user_initialized);

    TEST_REPLACE_DEVCERT(&TEST_PRODUCT_KEY, &TEST_PRODUCT_SECRET, &TEST_DEVICE_NAME, &TEST_DEVICE_SECRET);

    memset(&data->master_meta_info, 0, sizeof(iotx_linkkit_dev_meta_info_t));
    memcpy(data->master_meta_info.product_key, TEST_PRODUCT_KEY, strlen(TEST_PRODUCT_KEY));
    memcpy(data->master_meta_info.product_secret, TEST_PRODUCT_SECRET, strlen(TEST_PRODUCT_SECRET));
    memcpy(data->master_meta_info.device_name, TEST_DEVICE_NAME, strlen(TEST_DEVICE_NAME));
    memcpy(data->master_meta_info.device_secret, TEST_DEVICE_SECRET, strlen(TEST_DEVICE_SECRET));

    data->user_example_ctx->master_devid = IOT_Linkkit_Open(IOTX_LINKKIT_DEV_TYPE_MASTER, &data->master_meta_info);

    IOT_Linkkit_Connect(data->user_example_ctx->master_devid);
    wait_for_connected();
}

TEARDOWN(test_linkkit_single_api_yield)
{
    IOT_Linkkit_Close(data->user_example_ctx->master_devid);
}

// 正常调用
CASE2(test_linkkit_single_api_yield, case_01)
{
    int count = 10;
    while (count--) {
        IOT_Linkkit_Yield(100);
    }
}

// timeout = 0;
CASE2(test_linkkit_single_api_yield, case_02)
{
    IOT_Linkkit_Yield(0);
}

// timeout = -1;
CASE2(test_linkkit_single_api_yield, case_03)
{
    IOT_Linkkit_Yield(-1);
}

// 启动前yield
CASE(test_linkkit_single_api_yield, case_04)
{
    IOT_Linkkit_Yield(100);
}

SUITE(test_linkkit_single_api_yield) = {
    ADD_CASE(test_linkkit_single_api_yield, case_01),
    ADD_CASE(test_linkkit_single_api_yield, case_02),
    ADD_CASE(test_linkkit_single_api_yield, case_03),
    ADD_CASE(test_linkkit_single_api_yield, case_04),
    ADD_CASE_NULL
};
#endif  /* !defined(DEPRECATED_LINKKIT) */
#endif  /* #if defined(DEVICE_MODEL_ENABLED) */