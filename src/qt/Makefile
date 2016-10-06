.PHONY: FORCE
all: FORCE
	$(MAKE) -C .. ixcoin_qt test_bitcoin_qt
clean: FORCE
	$(MAKE) -C .. ixcoin_qt_clean test_bitcoin_qt_clean
check: FORCE
	$(MAKE) -C .. test_bitcoin_qt_check
ixcoin-qt ixcoin-qt.exe: FORCE
	 $(MAKE) -C .. ixcoin_qt
