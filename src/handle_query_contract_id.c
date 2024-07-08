#include "plugin.h"

void set_msg(ethQueryContractID_t *msg, const char *text) {
    strlcpy(msg->version, text, msg->versionLength);
}

// Sets the first screen to display.
void handle_query_contract_id(ethQueryContractID_t *msg) {
    const context_t *context = (const context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, APPNAME, msg->nameLength);

    msg->result = ETH_PLUGIN_RESULT_OK;
    selector_t selectorIndex = context->selectorIndex;

    switch (selectorIndex) {
        case VAULT_DEPOSIT:
            set_msg(msg, "Deposit");
            break;
        case VAULT_WITHDRAW:
            set_msg(msg, "Withdraw");
            break;
        case POOL_STAKE:
            set_msg(msg, "Stake");
            break;
        case POOL_EXIT:
            set_msg(msg, "Exit");
            break;
        case POOL_GET_REWARD:
            set_msg(msg, "Claim");
            break;
        case PORTAL:
            set_msg(msg, "Portal");
            break;
        default:
            PRINTF("Selector index: %d not supported\n", selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
