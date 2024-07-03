from pathlib import Path
import json
import os

from web3 import Web3
from eth_typing import ChainId

from ledger_app_clients.ethereum.client import EthAppClient
import ledger_app_clients.ethereum.response_parser as ResponseParser
from ledger_app_clients.ethereum.utils import get_selector_from_data, recover_transaction
from ragger.navigator import NavInsID

from .utils import get_appname_from_makefile, DERIVATION_PATH

# https://etherscan.io/tx/0xe69b8185aba6f5e89597b75e087792c0e0f886e50c13a8d3341ad3143a01d548

ROOT_SCREENSHOT_PATH = Path(__file__).parent
ABIS_FOLDER = "%s/abis" % (os.path.dirname(__file__))

PLUGIN_NAME = get_appname_from_makefile()
# Define the amount
amount = 91568528066239998771706

with open("%s/0x15d3a64b2d5ab9e152f16593cdebc4bb165b5b4a.abi.json" % (ABIS_FOLDER)) as file:
    contract = Web3().eth.contract(
        abi=json.load(file),
        # Get address from filename
        address=bytes.fromhex(os.path.basename(file.name).split(".")[0].split("x")[-1])
    )

def test_pool_stake(backend, firmware, navigator, test_name, wallet_addr):
    client = EthAppClient(backend)
    data = contract.encodeABI("stake", [
        amount
    ])

    # first setup the external plugin
    client.set_external_plugin(PLUGIN_NAME,
                               contract.address,
                               # Extract function selector from the encoded data
                               get_selector_from_data(data))
    tx_params = {
        "nonce": 1021,
        "maxFeePerGas": Web3.to_wei(145, "gwei"),
        "maxPriorityFeePerGas": Web3.to_wei(2, "gwei"),
        "gas": 186817,
        "to": contract.address,
        "value": Web3.to_wei(0.1, "ether"),
        "chainId": ChainId.ETH,
        "data": data
    }

    with client.sign(DERIVATION_PATH, tx_params):
        # Validate the on-screen request by performing the navigation appropriate for this device
        if firmware.device.startswith("nano"):
            navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
                                                      [NavInsID.BOTH_CLICK],
                                                      "Accept",
                                                      ROOT_SCREENSHOT_PATH,
                                                      test_name)
        else:
            navigator.navigate_until_text_and_compare(NavInsID.USE_CASE_REVIEW_TAP,
                                                      [NavInsID.USE_CASE_REVIEW_CONFIRM,
                                                       NavInsID.USE_CASE_STATUS_DISMISS],
                                                      "Hold to sign",
                                                      ROOT_SCREENSHOT_PATH,
                                                      test_name)
    # verify signature
    vrs = ResponseParser.signature(client.response().data)
    addr = recover_transaction(tx_params, vrs)
    assert addr == wallet_addr.get()
