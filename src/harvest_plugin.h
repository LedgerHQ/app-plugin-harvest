#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
#define NUM_SELECTORS 6

// Name of the plugin.
#define PLUGIN_NAME "Harvest"

// Wido uses 0x00000 as a dummy address to reprecent ETH.
extern const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH];

// Returns 1 if corresponding address is the Wido Router address for the chain token.
#define ADDRESS_IS_NETWORK_TOKEN(_addr) (!memcmp(_addr, NULL_ETH_ADDRESS, ADDRESS_LENGTH))

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in contract.c
typedef enum {
    VAULT_DEPOSIT = 0,
    VAULT_WITHDRAW,
    POOL_STAKE,
    POOL_GET_REWARD,
    POOL_EXIT,
    WIDO_EXECUTE_ORDER
} selector_t;

// Enumeration used to parse the smart contract data.
typedef enum { AMOUNT = 0, UNEXPECTED_PARAMETER } parameter;

#define INIT_EXECUTE        2
#define FROM_ADDRESS        10  // Address of the token the user is sending.
#define FROM_AMOUNT         11  // Amount sent by the user to the contract
#define OUTPUT_LENGTH       12  // Output length
#define TO_ADDRESS          13
#define TO_AMOUNT           14
#define OFFSET_FROM_ADDRESS 320

extern const uint32_t HARVEST_SELECTORS[NUM_SELECTORS];

// Number of decimals used when the token wasn't found in the CAL.
#define DEFAULT_DECIMAL WEI_TO_ETHER

// Ticker used when the token wasn't found in the CAL.
#define DEFAULT_TICKER "???"

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct context_t {
    // For display.
    uint8_t amount[INT256_LENGTH];

    uint8_t contract_address[ADDRESS_LENGTH];

    char underlying_ticker[MAX_TICKER_LEN];
    uint8_t underlying_decimals;

    char vault_ticker[MAX_TICKER_LEN];
    uint8_t vault_decimals;

    uint8_t to_address[ADDRESS_LENGTH];
    char to_ticker[MAX_TICKER_LEN];
    uint8_t to_decimals;

    uint8_t to_amount[INT256_LENGTH];

    uint16_t offset;

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.

    // For both parsing and display.
    selector_t selectorIndex;

    bool go_to_offset;

    bool tokens_sent_found;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

typedef struct contract_info_t {
    char *address;
    // underlying
    char *underlying_ticker;
    uint8_t underlying_decimals;
    // vault
    char *vault_ticker;
    uint8_t vault_decimals;
} contract_info_t;

static inline void printf_hex_array(const char *title __attribute__((unused)),
                                    size_t len __attribute__((unused)),
                                    const uint8_t *data __attribute__((unused))) {
    PRINTF(title);
    for (size_t i = 0; i < len; ++i) {
        PRINTF("%02x", data[i]);
    };
    PRINTF("\n");
}

static inline void sent_network_token(context_t *context) {
    context->underlying_decimals = WEI_TO_ETHER;
}

static inline void received_network_token(context_t *context) {
    context->to_decimals = WEI_TO_ETHER;
}
