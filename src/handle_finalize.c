#include "harvest_plugin.h"

#define _HARVEST(address, underlying_ticker, underlying_decimals, vault_ticker, vault_decimals) \
    {address, underlying_ticker, underlying_decimals, vault_ticker, vault_decimals},

const contract_info_t contracts[] = {
// this contracts-info.txt file is autogenerated by script:
// /tests/harvest/update-contracts-info-b2c.mjs
// or type: cd tests && npm run update-harvest

#ifdef DEBUG
// Include vaults used in the tests
#include "contracts-info-tests.txt"
#else
// this contracts-info.txt file is autogenerated by script:
// /tests/harvest/update-contracts-info-b2c.mjs
// or type: cd tests && npm run harvest-update
#include "contracts-info.txt"
#endif
};

contract_info_t *find_contract_info(const char *address) {
    int len = sizeof(contracts) / sizeof(contracts[0]);
    PRINTF("Contracts length: %d\n", len);
    for (int i = 0; i < len; i++) {
        contract_info_t *ci = &contracts[i];
        if (memcmp(address, (char *) PIC(ci->address), 42) == 0) return ci;
    }
    // when contract is not found
    return NULL;
}

void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    selector_t selectorIndex = context->selectorIndex;
    msg->numScreens = selectorIndex == POOL_GET_REWARD || selectorIndex == POOL_EXIT ? 1 : 2;

    if (selectorIndex != WIDO_EXECUTE_ORDER) {
        // Fill context underlying and vault ticker/decimals
        char *addr = context->contract_address;
        addr[0] = '0';
        addr[1] = 'x';

        uint64_t chainId = 0;
        if (!getEthAddressStringFromBinary(
                msg->pluginSharedRO->txContent->destination,
                addr + 2,  // +2 here because we've already prefixed with '0x'.
                msg->pluginSharedRW->sha3,
                chainId)) {
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
        }
        PRINTF("MSG Address: %s\n", addr);

        contract_info_t *info = find_contract_info(addr);

        if (info == NULL) {  // if contract info is not found
            msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;

        } else {
            strlcpy(context->underlying_ticker,
                    (char *) PIC(info->underlying_ticker),
                    sizeof(context->underlying_ticker));
            context->underlying_decimals = info->underlying_decimals;
            strlcpy(context->vault_ticker,
                    (char *) PIC(info->vault_ticker),
                    sizeof(context->vault_ticker));
            context->vault_decimals = info->vault_decimals;

            msg->uiType = ETH_UI_TYPE_GENERIC;
            msg->result = ETH_PLUGIN_RESULT_OK;
        }
    } else {
        if (!ADDRESS_IS_NETWORK_TOKEN(context->contract_address)) {
            // Address is not network token (0x000...) so we will need to look up the token in the
            // CAL.
            msg->tokenLookup1 = context->contract_address;
        } else {
            sent_network_token(context);
            msg->tokenLookup1 = NULL;
        }

        if (!ADDRESS_IS_NETWORK_TOKEN(context->to_address)) {
            // Address is not network token (0x000...) so we will need to look up the token in the
            // CAL.
            msg->tokenLookup2 = context->to_address;
        } else {
            received_network_token(context);
            msg->tokenLookup2 = NULL;
        }
        msg->uiType = ETH_UI_TYPE_GENERIC;
        msg->result = ETH_PLUGIN_RESULT_OK;
    }
}
