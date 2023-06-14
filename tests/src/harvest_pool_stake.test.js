import {testAmount1_18, testRawTx, testTx} from "./tools";
import {addressbook} from "./constants";

const method = 'stake';

// Test from replayed transaction:
// https://etherscan.io/tx/0xe69b8185aba6f5e89597b75e087792c0e0f886e50c13a8d3341ad3143a01d548

const rawTx = "0x02f893018203fd85070e0d686585070e0d68658302d9c19415d3a64b2d5ab9e152f16593cdebc4bb165b5b4a80a4a694fc3a000000000000000000000000000000000000000000001363f09a356415c0c1fac001a0fb6ce111e21a33431686417814939b6e49f59c26a74859a70ec4a9bef22c8c31a002e8ce7fbf9d071bfb9210a8c489ee468476830c88103e5438d0853b2bcf6cc1";
testRawTx(method, rawTx, 11, 5);

// Test constructed tx
const abi = require('../harvest/abis/harvest_pool.json');
testTx(addressbook.fDAI_POOL, abi, method, [testAmount1_18], 7, 5);
