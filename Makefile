.PHONY: all run clean

all:
	cd src && qmake icp.pro && $(MAKE)

run: all
	open icp-pn.app

clean:
	cd src && qmake icp.pro && $(MAKE) clean
	rm -f src/Makefile src/.qmake.stash
	rm -rf icp-pn.app