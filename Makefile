.PHONY: all run clean

all:
	cd src && qmake icp.pro && $(MAKE)

run: all
	./icp-pn

clean:
	cd src && qmake icp.pro && $(MAKE) clean
	rm -f src/Makefile src/.qmake.stash
	#rm -rf icp-pn.app
	rm -rf icp-pn

doxygen:
	doxygen doc/Doxyfile

clean-doxy:
	rm -rf doc/html
	rm -rf doc/latex

pack: clean clean-doxy
	zip -r xdurec00-xpertod00.zip \
		src \
		examples \
		doc \
		README.md \
		Makefile