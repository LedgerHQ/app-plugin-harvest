#include "harvest_plugin.h"

static void set_amount(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Amount", msg->titleLength);

    uint8_t decimals;
    const char *ticker;

    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
        case POOL_STAKE:
            decimals = context->underlying_decimals;
            ticker = context->underlying_ticker;
            break;
        case VAULT_WITHDRAW:
            decimals = context->vault_decimals;
            ticker = context->vault_ticker;
            break;
        case WIDO_EXECUTE_ORDER:
            decimals = context->from_address_decimals;
            ticker = context->from_address_ticker;
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    if (context->selectorIndex == WIDO_EXECUTE_ORDER) {
        amountToString(context->from_amount,
                       sizeof(context->from_amount),
                       decimals,
                       ticker,
                       msg->msg,
                       msg->msgLength);
    } else {
        amountToString(context->amount,
                       sizeof(context->amount),
                       decimals,
                       ticker,
                       msg->msg,
                       msg->msgLength);
    }
}

static void set_destination_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
        case VAULT_WITHDRAW:
            strlcpy(msg->title, "Vault", msg->titleLength);
            break;
        case WIDO_EXECUTE_ORDER:
            strlcpy(msg->title, "From Token", msg->titleLength);
            break;
        default:
            strlcpy(msg->title, "Pool", msg->titleLength);
            break;
    }

    if (context->selectorIndex != WIDO_EXECUTE_ORDER) {
        // Prefix the address with `0x`.
        char *m = msg->msg;
        m[0] = '0';
        m[1] = 'x';

        uint64_t chainId = 0;

        getEthAddressStringFromBinary(msg->pluginSharedRO->txContent->destination,
                                    m + 2,  // +2 here because we've already prefixed with '0x'.
                                    msg->pluginSharedRW->sha3,
                                    chainId);
    } else {
        strlcpy(msg->msg, context->from_address_ticker, sizeof(context->from_address_ticker));
    }
}

static void set_warn_ui(ethQueryContractUI_t *msg) {
    strlcpy(msg->title, "WARNING", msg->titleLength);
    strlcpy(msg->msg, "Unknown token", msg->msgLength);
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (msg->screenIndex) {
        case 0:
            set_destination_ui(msg, context);
            break;
        case 1:
            set_amount(msg, context);
            break;
        case 2:
            set_warn_ui(msg);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
