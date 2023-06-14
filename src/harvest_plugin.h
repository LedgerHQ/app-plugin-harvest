#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
#define NUM_SELECTORS      5
#define STR_ADDRESS_LENGTH 43

// Name of the plugin.
#define PLUGIN_NAME "Harvest"

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in contract.c
typedef enum {
    VAULT_DEPOSIT = 0,
    VAULT_WITHDRAW,
    POOL_STAKE,
    POOL_GET_REWARD,
    POOL_EXIT
} selector_t;

// Enumeration used to parse the smart contract data.
typedef enum { AMOUNT = 0, UNEXPECTED_PARAMETER } parameter;

extern const uint32_t HARVEST_SELECTORS[NUM_SELECTORS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct context_t {
    // For display.
    uint8_t amount[INT256_LENGTH];

    char contract_address[STR_ADDRESS_LENGTH];

    char underlying_ticker[MAX_TICKER_LEN];
    uint8_t underlying_decimals;

    char vault_ticker[MAX_TICKER_LEN];
    uint8_t vault_decimals;

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);

typedef struct contract_info_t {
    char *address;
    // underlying
    char *underlying_ticker;
    uint8_t underlying_decimals;
    // vault
    char *vault_ticker;
    uint8_t vault_decimals;
} contract_info_t;
