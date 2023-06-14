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
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    amountToString(context->amount,
                   sizeof(context->amount),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}

static void set_destination_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
        case VAULT_WITHDRAW:
            strlcpy(msg->title, "Vault", msg->titleLength);
            break;
        default:
            strlcpy(msg->title, "Pool", msg->titleLength);
            break;
    }

    // Prefix the address with `0x`.
    char *m = msg->msg;
    m[0] = '0';
    m[1] = 'x';

    uint64_t chainId = 0;

    getEthAddressStringFromBinary(msg->pluginSharedRO->txContent->destination,
                                  m + 2,  // +2 here because we've already prefixed with '0x'.
                                  msg->pluginSharedRW->sha3,
                                  chainId);
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
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
