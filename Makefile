.PHONY: all
all: DH-dlc DH-wad

.PHONY: DH-dlc
DH-dlc:
	cd DH-dlc && make

.PHONY: DH-wad
DH-wad:
	cd DH-wad && make

.PHONY: clean
clean:
	cd DH-dlc && make clean
	cd DH-wad && make clean



