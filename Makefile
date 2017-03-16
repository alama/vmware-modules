

%.tar:
	tar --owner=root --group=root -cf $*.tar $*-only/

all: vmblock.tar vmci.tar vmmon.tar vmnet.tar vsock.tar

install: all vmblock.tar vmci.tar vmmon.tar vmnet.tar vsock.tar
	cp vmblock.tar vmci.tar vmmon.tar vmnet.tar vsock.tar /usr/lib/vmware/modules/source

clean:
	$(RM) vmmon.tar vmnet.tar vmblock.tar vmci.tar vsock.tar
