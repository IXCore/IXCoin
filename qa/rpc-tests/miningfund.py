#!/usr/bin/env python3
# Copyright (c) 2017 Daniel Kraft <d@domob.eu>
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# Test tracking of the mining fund (based on "fundmining" transactions)
# through reorgs.

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import *

initial_fund = Decimal('10')

class MiningFundTest(BitcoinTestFramework):

    def __init__(self):
        super().__init__()
        self.setup_clean_chain = True
        self.num_nodes = 1

    def setup_network(self, split=False):
        self.nodes = start_nodes(self.num_nodes, self.options.tmpdir)

    def run_test(self):

        # Test that the initial mining fund is set up.
        self.check_fund(initial_fund)

        # Mine some blocks to get coins.  This shouldn't change the mining
        # fund yet.
        self.nodes[0].generate(150)
        self.check_fund(initial_fund)
        assert self.nodes[0].getbalance() > 0

        # Send coins in a normal transaction, which should not affect the fund.
        addr = self.nodes[0].getnewaddress()
        self.nodes[0].sendtoaddress(addr, 42)
        self.nodes[0].generate(1)
        self.check_fund(initial_fund)

        # Add coins into the mining fund.
        self.nodes[0].fundmining(5)
        self.check_fund(initial_fund)
        blk = self.nodes[0].generate(1)[0]
        self.check_fund(initial_fund + 5)

        # Revert the block, this should revert the fund calculation.
        self.nodes[0].invalidateblock(blk)
        self.check_fund(initial_fund)

        # Add some more funding and remine a fresh block, which should now
        # contain both funding transactions.
        self.nodes[0].fundmining(1)
        self.nodes[0].generate(1)
        self.check_fund(initial_fund + 6)

    def check_fund(self, expected):
        assert_equal(self.nodes[0].getblockchaininfo()['miningfund'], expected)

if __name__ == '__main__':
    MiningFundTest().main()
