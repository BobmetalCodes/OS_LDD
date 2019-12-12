-----------------------------------------------------------------------------------------
                        For Internal Dynamic Modules
-----------------------------------------------------------------------------------------
-> Prepare hello.c and hello1.c
-> Prepare Makefile and Kconfig (which resides in your directory)
-> Edit Makefile and Kconfig of Parent's directory
-> Compile and Make the *.ko files
    $ make -j 8 (from /usr/src/linux-source.4.18.20/)
        --> Press y/m/n to select
        --> let us select 'm' first for internal dynamic.
    $ make modules_install
-> Check the created *.ko files in the following directory
    $ cd /lib/modules/4.18.20kernel-dev/kernel/drivers/char/custom
    $ ls -la
-> Install modules
    $ make install
-> reboot

-> Load the instances of the modules using
    $ modprobe -v hello
    $ modprobe -v hello1

-> Check the debug messages
    $ dmesg | less

-> Unload the instances of the modules using
    $ modprobe -r -v hello
    $ modprobe -r -v hello1

-> To check the dependencies of hello and hello1,
   goto the following directory and open the following file:
    $ cd /lib/modules/4.18.20kernel-dev
    $ vim modules.dep
        --> Search for hello and hello1 and observe their dependencies...!!
    for eg. you can observe something like this
        kernel/drivers/char/custom/hello.ko:
        kernel/drivers/char/custom/hello1.ko: kernel/drivers/char/custom/hello.ko

-----------------------------------------------------------------------------------------
                        For Internal Static Modules
-----------------------------------------------------------------------------------------
-> Prepare hello.c and hello1.c
    --> Change the global variables and symbols' values.
-> Prepare Makefile and Kconfig (which resides in your directory)
    --> Change these (name) if there is a static module present also.
    --> can change 'tristate' to 'bool' (or) 'm' to 'y' accordingly and as per your need
-> Edit Makefile and Kconfig of Parent's directory
-> You can change the configuration options using "make menuconfig"
    $ cd /usr/src/linux-source-4.18.0/
    $ make menuconfig
        --> Goto Device Drivers -> Character and find your Config parameter.
        --> Change accordingly (using Y,M,N)
        --> Save and exit
-> To see the changes in the 'dmesg' then add the following in grub.cfg
   See the flow below:
    $ cd /boot/grub/
    $ sudo vim grub.cfg
    --> Find "Ubuntu, with Linux 4.18.20kernel-dev" and inside,
    --> Find 
"linux   /boot/vmlinuz-4.18.20kernel-dev root=UUID=9fbae4ef-4d47-4b85-9613-4b3b32248c0d ro  quiet splash $vt_handoff"
    --> Add "initcall_debug" between splash and $vt_handoff.
    --> Save and exit

    --> You can also add this line during boot time by entering into Command Line (c)

-> Compile and Install the modules (from /usr/src/linux-source.4.18.20/)
    $ make -j 8 
    $ make modules_install
    $ make install
-> reboot

-> Open terminal ; search 'hello' and 'hello1' in dmesg
    $ dmesg | less

-> To check the memory location of the symbols/variables
    $ sudo cat /proc/kallsyms | less
-> Check the Object Dumps
    $ objdump -t -f hello.ko
    $ objdump -t -f hello1.ko
-----------------------------------------------------------------------------------------
