qemu-system-x86_64  -smp 4 \
		    -device ich9-ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0 \
		    -drive id=disk,file="OS.img",format=raw,if=none \
		    -machine q35   -serial stdio -d cpu_reset,pcall
		    #-usb -device usb-ehci -device usb-storage,drive=usbdisk \
                    #-hda OS.img -hdb hdb.img 
		    #-drive id=disk,file="OS.img",format=raw,if=none \
		    #-drive id=usbdisk,file=hdd.img,if=none,format=raw -no-reboot -no-shutdown -m 128M
  		    #-hda OS.img -hdb hdd.img -hdc hdc.img
#-drive format=raw,file="Binaries/OS.bin",index=0 ,if=floppy,
#-machine q35
#qemu-system-x86_64 -machine q35 -enable-kvm -smp 4 -m 256M \
#  -drive id=disk,file="Binaries/OS.bin",format=raw,if=none \
#  -device ich9-ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0 \
#  -usb -device pci-ehci,id=ehci \
#  -device usb-storage,drive=usbdisk \
#  -drive id=usbdisk,file=hdd.img,if=none,format=raw

