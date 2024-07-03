/*******************************************************************************
 *   Plugin Boilerplate
 *   (c) 2023 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#pragma once

#include <string.h>
#include "eth_plugin_interface.h"

// All possible selectors of your plugin.
// EDIT THIS: Enter your selectors here, in the format X(NAME, value)
// A Xmacro below will create for you:
//     - an enum named selector_t with every NAME
//     - a map named SELECTORS associating each NAME with it's value
#define SELECTORS_LIST(X)          \
    X(VAULT_DEPOSIT, 0xb6b55f25)   \
    X(VAULT_WITHDRAW, 0x2e1a7d4d)  \
    X(POOL_STAKE, 0xa694fc3a)      \
    X(POOL_GET_REWARD, 0x3d18b912) \
    X(POOL_EXIT, 0xe9fad8ee)       \
    X(PORTAL, 0xa2e42c65)

// Xmacro helpers to define the enum and map
// Do not modify !
#define TO_ENUM(selector_name, selector_id)  selector_name,
#define TO_VALUE(selector_name, selector_id) selector_id,

// This enum will be automatically expanded to hold all selector names.
// The value SELECTOR_COUNT can be used to get the number of defined selectors
// Do not modify !
typedef enum selector_e {
    SELECTORS_LIST(TO_ENUM) SELECTOR_COUNT,
} selector_t;

// This array will be automatically expanded to map all selector_t names with the correct value.
// Do not modify !
extern const uint32_t HARVEST_SELECTORS[SELECTOR_COUNT];

// Enumeration used to parse the smart contract data.
// EDIT THIS: Adapt the parameter names here.
typedef enum {
    AMOUNT = 0,
    UNEXPECTED_PARAMETER,
} parameter;

#define INIT_EXECUTE        2
#define FROM_ADDRESS        10  // Address of the token the user is sending.
#define FROM_AMOUNT         11  // Amount sent by the user to the contract
#define OUTPUT_LENGTH       12  // Output length
#define TO_ADDRESS          13
#define TO_AMOUNT           14
#define OFFSET_FROM_ADDRESS 64

#define DEFAULT_TICKER  "???"
#define DEFAULT_DECIMAL WEI_TO_ETHER

extern const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH];

// Returns 1 if corresponding address is the Portal Router address for the chain token.
#define ADDRESS_IS_NETWORK_TOKEN(_addr) (!memcmp(_addr, NULL_ETH_ADDRESS, ADDRESS_LENGTH))

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
// EDIT THIS: This struct is used by your plugin to save the parameters you parse. You
// will need to adapt this struct to your plugin.
typedef struct context_s {
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

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.
    bool tokens_sent_found;

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

typedef struct contract_info_t {
    const char *address;
    // underlying
    const char *underlying_ticker;
    uint8_t underlying_decimals;
    // vault
    const char *vault_ticker;
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

// Check if the context structure will fit in the RAM section ETH will prepare for us
// Do not remove!
ASSERT_SIZEOF_PLUGIN_CONTEXT(context_t);
