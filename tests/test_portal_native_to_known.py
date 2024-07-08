from pathlib import Path
import json
import os

from web3 import Web3
from eth_typing import ChainId

from ledger_app_clients.ethereum.client import EthAppClient
import ledger_app_clients.ethereum.response_parser as ResponseParser
from ledger_app_clients.ethereum.utils import get_selector_from_data, recover_transaction
from ragger.navigator import NavInsID
from . import token_metadata_database as tokens
from .utils import get_appname_from_makefile, DERIVATION_PATH

# https://etherscan.io/getRawTx?tx=0x7c50b4f325a73642625b81fc5af99c659a51bc8ffed329da89767462f433090f

ROOT_SCREENSHOT_PATH = Path(__file__).parent
ABIS_FOLDER = "%s/abis" % (os.path.dirname(__file__))

PLUGIN_NAME = get_appname_from_makefile()

with open("%s/portal.abi.json" % (ABIS_FOLDER)) as file:
    contract = Web3().eth.contract(
        abi=json.load(file),
        # Get address from filename
        address=bytes.fromhex('bf5a7f3629fb325e2a8453d595ab103465f75e62')
    )


def convert_to_bytes(hex_string):
    return bytes.fromhex(hex_string.replace("0x", ""))

def test_portal_native_to_known(backend, firmware, navigator, test_name, wallet_addr):
    client = EthAppClient(backend)
    data = contract.encodeABI("portal", [
        [
            (
                convert_to_bytes('0x0000000000000000000000000000000000000000'),
                10000000000000000,
                convert_to_bytes('0x7f39C581F595B53c5cb19bD0b3f8dA6c935E2Ca0'),
                8502896310583579,
                convert_to_bytes('0x4aEc4e67F62d5EDA2D7d8438b557F5D79bF844fF')
            ),
            [(
                convert_to_bytes('0xdAC17F958D2ee523a2206206994597C13D831ec7'),
                convert_to_bytes('0xdAC17F958D2ee523a2206206994597C13D831ec7'),
                convert_to_bytes('0x095ea7b3000000000000000000000000390f3595bca2df7d23783dfd126427cceb997bf40000000000000000000000000000000000000000000000000000000000000000'),
                1
            ),
            (
                convert_to_bytes('0xdAC17F958D2ee523a2206206994597C13D831ec7'),
                convert_to_bytes('0x390f3595bCa2Df7d23783dFd126427CCeb997BF4'),
                convert_to_bytes('0x0b4c7e4d000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000'),
                0
            ),
            (
                convert_to_bytes('0x390f3595bCa2Df7d23783dFd126427CCeb997BF4'),
                convert_to_bytes('0x390f3595bCa2Df7d23783dFd126427CCeb997BF4'),
                convert_to_bytes('0x095ea7b3000000000000000000000000241add131b9aaa7527132b752252b99420937adc0000000000000000000000000000000000000000000000000000000000000000'),
                1
            ),
            (
                convert_to_bytes('0x390f3595bCa2Df7d23783dFd126427CCeb997BF4'),
                convert_to_bytes('0x241AdD131B9aaa7527132b752252b99420937ADc'),
                convert_to_bytes('0xa9059cbb0000000000000000000000004aec4e67f62d5eda2d7d8438b557f5d79bf844ff0000000000000000000000000000000000000000000000000000000000000000'),
                0
            ),
            (
                convert_to_bytes('0x7f39C581F595B53c5cb19bD0b3f8dA6c935E2Ca0'),
                convert_to_bytes('0x7f39C581F595B53c5cb19bD0b3f8dA6c935E2Ca0'),
                convert_to_bytes('0xa9059cbb0000000000000000000000004aec4e67f62d5eda2d7d8438b557f5d79bf844ff0000000000000000000000000000000000000000000000000000000000000000'),
                1
            )]
        ],
        convert_to_bytes('0xFBD4C3D8bE6B15b7cf428Db2838bb44C0054fCd2')
    ])

    # first setup the external plugin
    client.set_external_plugin(PLUGIN_NAME,
                               contract.address,
                               # Extract function selector from the encoded data
                               get_selector_from_data(data))
    tx_params = {
        "nonce": 2,
        "maxFeePerGas": Web3.to_wei(3.5, "gwei"),
        "maxPriorityFeePerGas": Web3.to_wei(3.5, "gwei"),
        "gas": 2200303,
        "to": contract.address,
        "value": Web3.to_wei(0.1, "ether"),
        "chainId": ChainId.ETH,
        "data": data
    }
    client.provide_token_metadata(tokens.wstETH.ticker, bytes.fromhex(tokens.wstETH.address), tokens.wstETH.decimals, tokens.wstETH.chain_id)

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
