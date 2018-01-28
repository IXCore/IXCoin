IXCoin Core integration/staging tree
=====================================

[![Build Status](https://travis-ci.org/bitcoin/bitcoin.svg?branch=master)](https://travis-ci.org/bitcoin/bitcoin)

https://bitcoincore.org

What is IXCoin?
----------------

IXCoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. IXCoin uses the same peer-to-peer technology as Bitcoin 
to operate with no central authority: managing transactions and issuing money are 
carried out collectively by the network. IXCoin Core is the name of open source
software which enables the use of this currency. IXCoin is merged mined with 
Bitcoin for added security and use as a Bitcoin side chain.

For more information, as well as an immediately useable, binary version of
the IXCoin Core software, see https://ixcoin.net, or read the
[original whitepaper](https://bitcoincore.org/bitcoin.pdf).

License
-------

IXCoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/IXCore/IXCoin/tags) are created
regularly to indicate new official, stable release versions of IXCoin Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).


Testing
-------

Testing and code review is the bottleneck for development. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](/doc/unit-tests.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[IXCoin Core's Transifex page](https://www.transifex.com/projects/p/IXCoin/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.

