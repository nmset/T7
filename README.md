#T7

HID foot pedal controlled transcription application.

## Build

$ cd T7

$ cmake -DCMAKE_BUILD_TYPE:STRING=Release ../

$ make -j4

## Usage

Udev's facilities are used here. A group with read/write access to the 
device must be created, with users added to that group.

$ groupadd transcript
$ usermod -a -G transcript user

Create /etc/udev/rules.d/99-mypedal.rules with (one single line) :
KERNEL=="hidraw[0-9]*", ATTRS{idVendor}=="abcd", ATTRS{idProduct}=="efgh",  
MODE="0660", GROUP="transcript"

Get 'abcd' and 'efgh' with 'lsusb' command or 'dmesg |tail' commands 
after plugging the foot pedal.

Remove the pedal device and plug it again. On next login, users in group 
'transcript' should have read access to the device.

##### T7 configuration:

Open the two nested collapsible panes at the bottom of the main window.

Select the foot pedal device.

Identify each of the three pedals in turn.

Restart the application.

Declare pedal roles.

Choose an automatic rewind duration on pause.

Select a root media directory.

Refresh the list.

Double click on an item.

Use the pedals.

## Disclaimer

Programming is just my hobby, use at your own risks.

T7 does not claim to be fit for any purpose.

## Limitations

Not designed for devices with less or more than three pedals.
Developed on Linux, other operating systems are not a target.

