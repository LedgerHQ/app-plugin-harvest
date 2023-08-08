#include "harvest_plugin.h"

void handle_provide_token(void *parameters) {
    ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    selector_t selectorIndex = context->selectorIndex;
    
    if (selectorIndex == WIDO_EXECUTE_ORDER) {
        PRINTF("Wido router token: 0x%p\n", msg->item1);
        if (ADDRESS_IS_NETWORK_TOKEN(context->from_address)) {
            sent_network_token(context);
        } else if (msg->item1 != NULL) {
            context->from_address_decimals = msg->item1->token.decimals;
            strlcpy(context->from_address_ticker, (char *) msg->item1->token.ticker, sizeof(context->from_address_ticker));
        } else {
            // CAL did not find the token and token is not ETH.
            context->from_address_decimals = DEFAULT_DECIMAL;
            strlcpy(context->from_address_ticker, DEFAULT_TICKER, sizeof(context->from_address_ticker));
            // We will need an additional screen to display a warning message.
            msg->additionalScreens++;
        }
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
