import {testRawTx, testTx} from "./tools";
import {addressbook} from "./constants";

const method = 'exit';

// Test from replayed transaction:
// https://etherscan.io/tx/0xd995a624c785281c144a92be3b5ba5e5cadb9cb715b9363b5be5684385ab5370

const rawTx = "0x02f872018201eb84773594008502cb4178008303c7879415d3a64b2d5ab9e152f16593cdebc4bb165b5b4a8084e9fad8eec080a08575a13b70de80eaa9e63d5fcaea163d6b42a2fa28b6645c4da240a823eac3e8a04725a51ba4c54fe432b22158e19f8ccb4d061c0d78f602dfeda9a4973dc737f1";
testRawTx(method, rawTx, 6, 4);

// Test constructed tx
const abi = require('../harvest/abis/harvest_pool.json');
testTx(addressbook.fDAI_POOL, abi, method, [], 6, 4);
