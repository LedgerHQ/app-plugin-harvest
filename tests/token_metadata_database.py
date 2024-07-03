class TokenMetadata:
	def __init__(self, ticker, address, decimals, chain_id):
		self.ticker = ticker
		self.address = address
		self.decimals = decimals
		self.chain_id = chain_id

stETH = TokenMetadata("stETH", 'ae7ab96520de3a18e5e111b5eaab095312d7fe84', 18, 1)
fDAI = TokenMetadata("fDAI", 'ab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c', 18, 1)
wstETH = TokenMetadata("wstETH", '7f39c581f595b53c5cb19bd0b3f8da6c935e2ca0', 18, 1)