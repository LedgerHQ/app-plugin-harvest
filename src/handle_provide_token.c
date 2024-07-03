#include "plugin.h"

// EDIT THIS: Adapt this function to your needs! Remember, the information for tokens are held in
// `msg->token1` and `msg->token2`. If those pointers are `NULL`, this means the ethereum app didn't
// find any info regarding the requested tokens!
void handle_provide_token(ethPluginProvideInfo_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    selector_t selectorIndex = context->selectorIndex;

    if (selectorIndex == PORTAL) {
        PRINTF("Portal router token: 0x%p, 0x%p\n", msg->item1, msg->item2);
        if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address)) {
            sent_network_token(context);
            context->tokens_sent_found = true;
        } else if (msg->item1 != NULL) {
            context->underlying_decimals = msg->item1->token.decimals;
            strlcpy(context->underlying_ticker,
                    (char *) msg->item1->token.ticker,
                    sizeof(context->underlying_ticker));
            context->tokens_sent_found = true;
        } else {
            // CAL did not find the token and token is not ETH.
            context->underlying_decimals = DEFAULT_DECIMAL;
            strlcpy(context->underlying_ticker, DEFAULT_TICKER, sizeof(context->underlying_ticker));
            context->tokens_sent_found = false;
            // We will need an additional screen to display a warning message.
            msg->additionalScreens++;
        }

        if (ADDRESS_IS_NETWORK_TOKEN(context->to_address)) {
            received_network_token(context);
        } else if (msg->item2 != NULL) {
            context->to_decimals = msg->item2->token.decimals;
            strlcpy(context->to_ticker,
                    (char *) msg->item2->token.ticker,
                    sizeof(context->to_ticker));
        } else {
            // CAL did not find the token and token is not ETH.
            context->to_decimals = DEFAULT_DECIMAL;
            strlcpy(context->to_ticker, DEFAULT_TICKER, sizeof(context->to_ticker));
            // We will need an additional screen to display a warning message.
            msg->additionalScreens++;
        }
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
