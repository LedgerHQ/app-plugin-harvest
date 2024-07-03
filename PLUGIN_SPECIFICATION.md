# Technical Specification

## About

This documentation describes the smart contracts and functions supported by the plugin.

## Smart Contracts

Smart contracts covered by the plugin shall be described here:

|  Network | Version | Smart Contract | Address |
|   ----   |   ---   |      ----      |   ---   |
| NetworkName   | VX  | PortalRouter  | `0xbf5A7F3629fB325E2a8453D595AB103465F75E62` |

## Functions

For the smart contracts implemented, the functions covered by the plugin shall be described here:

|Contract |    Function   | Selector  | Displayed Parameters |
|   ---   |    ---        | ---       | --- |
| Pool  | stake           | `0xa694fc3a`| <table><tbody> <tr><td><code> uint256 amount </code></td></tr> </tbody></table> |
| Pool  | getReward           | `0x3d18b912`| |
| Pool  | exit           | `0xe9fad8ee`| |
| Vault  | deposit           | `0xb6b55f25`| <table><tbody> <tr><td><code> uint256 amount </code></td></tr> </tbody></table> |
| Vault  | withdraw           | `0x2e1a7d4d`| <table><tbody> <tr><td><code> uint256 shares </code></td></tr> </tbody></table> |
| PortalRouter  | portal           | `0x2e1a7d4d`| <table><tbody> <tr><td><code> tuple orderPayload,address partner </code></td></tr> </tbody></table> |
