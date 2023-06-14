import {testAmount1_18, testRawTx, testTx} from "./tools";
import {addressbook} from "./constants";

const method = 'withdraw';

// Test from replayed transaction:
// https://etherscan.io/getRawTx?tx=0xf8bd307e00342b3c541b6d24c1bd42ab4fa0c383e3d96fcd77a3a84d8270144b

const rawTx = "0x02f892018201ec847735940085028fa6ae008304e8a994ab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c80a42e1a7d4d00000000000000000000000000000000000000000000000212571dde98c00110c001a01829d911ed640cfbef8755c536b2446a81812fca61afdaa153d8b8ba4cabb47da07104152b639cfefd9ede0ca8e333ed0f7781a3cb769eea240f6d9bda4c4eb366";
testRawTx(method, rawTx, 9, 5);

// Test constructed tx
const abi = require('../harvest/abis/harvest_vault.json');
testTx(addressbook.fDAI_VAULT, abi, method, [testAmount1_18], 7, 5);
