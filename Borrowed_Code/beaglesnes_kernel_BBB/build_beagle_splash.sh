#!/bin/sh
pngtopnm beaglesnes_kernel_splash.png | ppmquant -fs 223 | pnmtoplainpnm > KERNEL/drivers/video/logo/logo_linux_clut224.ppm

