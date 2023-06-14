import {testRawTx, testTx} from "./tools";
import {addressbook} from "./constants";

const method = 'getReward';

// Test from replayed transaction:
// https://etherscan.io/getRawTx?tx=0xbe94d3757e487ea613bac9d4f96a22c5a6aee49c325f58b8239ae8393f46ec1c

const rawTx = "0x02f87101428506abd89a828506abd89a828302261d9415d3a64b2d5ab9e152f16593cdebc4bb165b5b4a80843d18b912c080a0157e207829a5723a6353782912bb31a64c98384186911b6833d00a5b9c2b190ba07a30c1dc7cd84fdd6050f0051ff8f1b51276c91740e43313ca817e261f3446e4";
testRawTx(method, rawTx, 8, 4);

// Test constructed tx
const abi = require('../harvest/abis/harvest_pool.json');
testTx(addressbook.fDAI_POOL, abi, method, [], 6, 4);
