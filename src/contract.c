#include <stdint.h>
#include "harvest_plugin.h"

// List of selectors supported by this plugin.
static const uint32_t VAULT_DEPOSIT_SELECTOR = 0xb6b55f25;
static const uint32_t VAULT_WITHDRAW_SELECTOR = 0x2e1a7d4d;
static const uint32_t POOL_STAKE_SELECTOR = 0xa694fc3a;
static const uint32_t POOL_GET_REWARD_SELECTOR = 0x3d18b912;
static const uint32_t POOL_EXIT_SELECTOR = 0xe9fad8ee;

// Array of all the different boilerplate selectors. Make sure this follows the same order as the
// enum defined in `harvest_plugin.h`
const uint32_t HARVEST_SELECTORS[NUM_SELECTORS] = {VAULT_DEPOSIT_SELECTOR,
                                                   VAULT_WITHDRAW_SELECTOR,
                                                   POOL_STAKE_SELECTOR,
                                                   POOL_GET_REWARD_SELECTOR,
                                                   POOL_EXIT_SELECTOR};
