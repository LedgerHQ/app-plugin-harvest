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

static void set_ui(ethQueryContractUI_t *msg, const context_t *context, int ui_type) {
    uint8_t decimals;
    const char *ticker;

    if (ui_type == 0) {
        strlcpy(msg->title, "Swap From", msg->titleLength);
        decimals = context->underlying_decimals;
        ticker = context->underlying_ticker;
        if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address)) {
            strlcpy(context->underlying_ticker,
                    msg->network_ticker,
                    sizeof(context->underlying_ticker));
        }
        amountToString(context->amount,
                       sizeof(context->amount),
                       decimals,
                       ticker,
                       msg->msg,
                       msg->msgLength);
    } else {
        strlcpy(msg->title, "Swap To", msg->titleLength);
        decimals = context->to_decimals;
        ticker = context->to_ticker;
        if (ADDRESS_IS_NETWORK_TOKEN(context->to_address)) {
            strlcpy(context->to_ticker, msg->network_ticker, sizeof(context->to_ticker));
        }
        amountToString(context->to_amount,
                       sizeof(context->to_amount),
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

static void set_warn_ui(ethQueryContractUI_t *msg) {
    strlcpy(msg->title, "WARNING", msg->titleLength);
    strlcpy(msg->msg, "Unknown Token", msg->msgLength);
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    selector_t selectorIndex = context->selectorIndex;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    if (selectorIndex != WIDO_EXECUTE_ORDER) {
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
    } else {
        switch (msg->screenIndex) {
            case 0:
                set_ui(msg, context, 0);  // set swap from token ui
                break;
            case 1:
                if (context->tokens_sent_found) {
                    set_ui(msg, context, 1);  // set swap to token ui
                } else {
                    set_warn_ui(msg);
                }
                break;
            case 2:
                if (context->tokens_sent_found) {
                    set_warn_ui(msg);
                } else {
                    set_ui(msg, context, 1);  // set swap to token ui
                }
                break;
            case 3:
                if (!context->tokens_sent_found) {
                    set_warn_ui(msg);
                }
                break;
            // Keep this
            default:
                PRINTF("Received an invalid screenIndex\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
        }
    }
}
