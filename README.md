# Evofox-phantom

This is a small python script that can be used to toggle LED
modes of an Amkette Evofox Phantom Gaming mouse. 

I wrote this because the configuration tool that shipped with the 
mouse was only available (in binary form) for Microsoft Windows.

```
$ evofox-phantom -h
usage: evofox-phantom [-h] [--vid VID] [--pid PID]
					  [-l [{off,static,slow,fast}]]

Toggle LED modes in Amkette Evofox Gaming Mouse

options:
  -h, --help            show this help message and exit
  --vid VID             Vendor ID; lsusb(8)
  --pid PID             Product ID; lsusb(8)
  -l [{off,static,slow,fast}], --led [{off,static,slow,fast}]
```

Phantom supports four modes: off, static, slow and fast. This program,
at present only supports toggling LEDs.

Here's a demo:

![](out.mkv)

# Install

**Dependencies**: hidapi

```
$ sudo pip install hidpi
```

or alternatively on arch linux systems, 

```
$ sudo pacman -S python-hidapi
```

Install 

```
$ git clone https://github.com/bojle/evofox-phantom.git
$ cd evofox-phantom
$ make install
```

# TODO

* Add toggling DPI values
* Add re-configuring mouse buttons.
* Hack and see if the LEDs can be illuminated individually to a color
of choice.

