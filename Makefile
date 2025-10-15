MODNAME = hello
SRCDIR = src
OBJDIR = bin
KERNELDIR = /usr/lib/modules/$(shell uname -r)/build

obj-m := $(SRCDIR)/hello.o

build:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
	mv $(SRCDIR)/$(MODNAME).ko $(OBJDIR)/

load: unload
	sudo insmod $(OBJDIR)/$(MODNAME).ko

unload:
	sudo rmmod $(MODNAME) 2> /dev/null || true

dev:
	sudo mknod -m 666 hello c 300 0

clean:
	rm -f $(SRCDIR)/.*.d $(SRCDIR)/*.mod* \
		$(SRCDIR)/.*.cmd $(SRCDIR)/*.o .*.cmd

clean-all: clean
	rm -f $(OBJDIR)/*.ko modules.order Module.symvers
