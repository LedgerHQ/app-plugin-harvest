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

# https://etherscan.io/getRawTx?tx=0xeba3e61b83aaae80f7d94321805fb550a86fb4f75ef8aa072d71cb95fd3b42c8

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

def test_portal_known_to_known(backend, firmware, navigator, test_name, wallet_addr):
    client = EthAppClient(backend)
    data = contract.encodeABI("portal", [
        [
            (
                convert_to_bytes('0xae7ab96520de3a18e5e111b5eaab095312d7fe84'),
                424914404154433277,
                convert_to_bytes('0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c'),
                1098760785922679689803,
                convert_to_bytes('0x8b8d8ac0c40fe881487509451da1720d2dec8d22')
            ),
            [(
                convert_to_bytes('0xae7ab96520de3a18e5e111b5eaab095312d7fe84'),
                convert_to_bytes('0xae7ab96520de3a18e5e111b5eaab095312d7fe84'),
                convert_to_bytes('0x095ea7b30000000000000000000000001111111254eeb25477b68fb85ed929f73a9605820000000000000000000000000000000000000000000000000000000000000000'),
                1
            ),
            (
                convert_to_bytes('0xae7ab96520de3a18e5e111b5eaab095312d7fe84'),
                convert_to_bytes('0x1111111254eeb25477b68fb85ed929f73a960582'),
                convert_to_bytes('0x12aa3caf000000000000000000000000e37e799d5077682fa0a244d46e5649f71457bd09000000000000000000000000ae7ab96520de3a18e5e111b5eaab095312d7fe840000000000000000000000006b175474e89094c44da98b954eedeac495271d0f000000000000000000000000e37e799d5077682fa0a244d46e5649f71457bd0900000000000000000000000089c30e3af15d210736b2918fbd655c9842fd74f700000000000000000000000000000000000000000000000005ed25afb02b6c4600000000000000000000000000000000000000000000004042da2c9405f0807d000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000001400000000000000000000000000000000000000000000000000000000000000160000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ff0000000000000000000000000000000000000000000000000000000001e100a007e5c0d20000000000000000000000000000000000000001bd00015a00006a00005051207f39c581f595b53c5cb19bd0b3f8da6c935e2ca0ae7ab96520de3a18e5e111b5eaab095312d7fe840004ea598cb000000000000000000000000000000000000000000000000000000000000000000020d6bdbf787f39c581f595b53c5cb19bd0b3f8da6c935e2ca051324370e48e610d2e02d3d091a9d79c8eb9a54c5b1c7f39c581f595b53c5cb19bd0b3f8da6c935e2ca0004475d39ecb000000000000000000000000e37e799d5077682fa0a244d46e5649f71457bd090000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000276a40000000000000000000000000000000000000000000000000554622a19ae4b7c00000000000000000000000000000000000000000000000000000000664e566202a000000000000000000000000000000000000000000000004042da2c9405f0807dee63c1e58060594a405d53811d3bc4766596efd80fd545a270c02aaa39b223fe8d0a0e5c4f27ead9083c756cc21111111254eeb25477b68fb85ed929f73a960582007cadafe8'),
                5
            ),
            (
                convert_to_bytes('0x6b175474e89094c44da98b954eedeac495271d0f'),
                convert_to_bytes('0x6b175474e89094c44da98b954eedeac495271d0f'),
                convert_to_bytes('0x095ea7b3000000000000000000000000ab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c0000000000000000000000000000000000000000000000000000000000000000'),
                1
            ),
            (
                convert_to_bytes('0x6b175474e89094c44da98b954eedeac495271d0f'),
                convert_to_bytes('0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c'),
                convert_to_bytes('0xb6b55f250000000000000000000000000000000000000000000000000000000000000000'),
                0
            ),
            (
                convert_to_bytes('0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c'),
                convert_to_bytes('0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c'),
                convert_to_bytes('0xa9059cbb0000000000000000000000008b8d8ac0c40fe881487509451da1720d2dec8d220000000000000000000000000000000000000000000000000000000000000000'),
                1
            )]
        ],
        convert_to_bytes('0xf066789028fe31d4f53b69b81b328b8218cc0641')
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

    client.provide_token_metadata(tokens.stETH.ticker, bytes.fromhex(tokens.stETH.address), tokens.stETH.decimals, tokens.stETH.chain_id)
    client.provide_token_metadata(tokens.fDAI.ticker, bytes.fromhex(tokens.fDAI.address), tokens.fDAI.decimals, tokens.fDAI.chain_id)

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
