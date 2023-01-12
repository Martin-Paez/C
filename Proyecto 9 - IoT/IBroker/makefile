BOARD?=
PORT?=

.PHONY: c w upload write compile clean style

w: style compile upload clean

c: style compile clean

upload:
	arduino-cli upload --fqbn $(BOARD) -p $(PORT) ./
compile:
	arduino-cli compile --fqbn $(BOARD) ./
clean:
    rm -f .*.hex
#.elf
style:
	cpplint --extensions=ino 
