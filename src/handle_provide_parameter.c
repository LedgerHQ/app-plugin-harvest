#include "harvest_plugin.h"

static void handle_amount(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));

            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_execute_order(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != OFFSET_FROM_ADDRESS + SELECTOR_SIZE) {
            // We still haven't reached the offset...
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case INIT_EXECUTE:
            context->go_to_offset = true;
            context->next_param = FROM_ADDRESS;
            break;
        case FROM_ADDRESS:
            copy_address(context->from_address, msg->parameter, sizeof(context->from_address));
            context->next_param = FROM_AMOUNT;
            printf_hex_array("FROM_ADDRESS: ", ADDRESS_LENGTH, context->from_address);
            break;
        case FROM_AMOUNT:
            copy_parameter(context->from_amount, msg->parameter, sizeof(context->from_amount));
            printf_hex_array("FROM_AMOUNT: ", INT256_LENGTH, context->from_amount);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
        case VAULT_WITHDRAW:
        case POOL_STAKE:
            handle_amount(msg, context);
            break;
        case WIDO_EXECUTE_ORDER:
            handle_execute_order(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
