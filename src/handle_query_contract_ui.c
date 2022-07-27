#include "harvest_plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.

// Set UI for the "Send" screen.
// EDIT THIS: Adapt / remove this function to your needs.
/* static void set_send_ui(ethQueryContractUI_t *msg) {
    strlcpy(msg->title, "Send", msg->titleLength);

    const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
    uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;

    // Converts the uint256 number located in `eth_amount` to its string representation and
    // copies this to `msg->msg`.
    amountToString(eth_amount, eth_amount_size, WEI_TO_ETHER, "ETH ", msg->msg, msg->msgLength);
} */

static void set_amount(ethQueryContractUI_t *msg, const context_t *context) {

    strlcpy(msg->title, "Amount", msg->titleLength);

    uint8_t decimals;
    const char *ticker;

    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
            decimals = context->underlying_decimals;
            ticker = context->underlying_ticker;
            break;
        case VAULT_WITHDRAW:
            decimals = context->vault_decimals;
            ticker = context->vault_ticker;
            break;
        default:
            decimals = WEI_TO_ETHER;
            ticker = "???";
//             PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
//             msg->result = ETH_PLUGIN_RESULT_ERROR;
//             return;
    }

    amountToString(context->amount,
                   sizeof(context->amount),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}

static void set_destination_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Vault", msg->titleLength);



    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    uint64_t chainid = 0;

    getEthAddressStringFromBinary(
        msg->pluginSharedRO->txContent->destination,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        msg->pluginSharedRW->sha3,
        chainid);
    PRINTF("MSG Address: %s\n", msg->msg); // TODO remove

    // make short representation of the address
    char *m = msg->msg;
    m[6] = '.';
    m[7] = '.';
    m[8] = '.';
    m[9] = m[38];
    m[10] = m[39];
    m[11] = m[40];
    m[12] = m[41];
    m[13] = '\0';
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
