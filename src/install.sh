cat "WARNING: this will remove all data in your USB !!!"
sleep 1
sudo dd if=/dev/zero of=/dev/sdb count=204800 status=progress
sudo dd if="./OS.img" of=/dev/sdb status=progress
