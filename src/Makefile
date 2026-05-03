iso:
	@dd if=/dev/zero of=OS.img count=160000 status=progress

run:
	@./be.sh
	@sh run.sh
brun: build
	@bochs -f bochsrc.txt -q

build:
	@./be.sh

install:
	@echo "WRNING: YOU WILL LOST ALL YOUR DATA IN THE PROCES IN THE FIRST USB"
	@read -p "continu?[y,N]:" conf;\
	if [ $$conf != "y" ];\
	then \
		echo "Operatin has ben ignored, All your data in safe";\
		exit 1;\
	fi;
	@echo "It look that you are sore, it will be writen to /dev/sdb";
	@read -p "Press any key to continu ..."
	#echo "Writing to |$$dev|";
	@sudo dd if=OS.img of=/dev/sdb status=progress bs=4M

root:iso
	@./be.sh
	@echo "Monting ..."
	@sudo mount OS.img ../mnt -t vfat -o offset=0
	@echo "Copying ..."
	@sudo cp -rf ../root/* ../mnt
	@echo "Unmounting ..."
	@sudo umount OS.img
mount:
	@sudo mount OS.img ../mnt
	@echo "Mounted."
unmount:
	@sudo umount OS.img
	@echo "Unmounted."
