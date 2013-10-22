#!/bin/bash -e
#
# Copyright (c) 2009-2013 Robert Nelson <robertcnelson@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

DIR=$PWD

function make_kernel {
	cd ${DIR}/KERNEL/
	echo "-----------------------------"
	echo "make -j${CORES} ARCH=arm LOCALVERSION=-${BUILD} CROSS_COMPILE=\"${CCACHE} ${CC}\" ${CONFIG_DEBUG_SECTION} uImage modules (AWH - Was zImage)"
	echo "-----------------------------"
	time make -j${CORES} ARCH=arm LOCALVERSION=-${BUILD} CROSS_COMPILE="${CCACHE} ${CC}" ${CONFIG_DEBUG_SECTION} clean
	cd ${DIR}
	rm -rf deploy
}

/bin/bash -e ${DIR}/tools/host_det.sh || { exit 1 ; }

if [ ! -f ${DIR}/system.sh ] ; then
	cp ${DIR}/system.sh.sample ${DIR}/system.sh
fi

unset CC
unset DEBUG_SECTION
unset LINUX_GIT
unset LOCAL_PATCH_DIR
source ${DIR}/system.sh
/bin/bash -e "${DIR}/scripts/gcc.sh" || { exit 1 ; }
source ${DIR}/.CC
echo "debug: CC=${CC}"

source ${DIR}/version.sh
export LINUX_GIT

unset CONFIG_DEBUG_SECTION
if [ "${DEBUG_SECTION}" ] ; then
	CONFIG_DEBUG_SECTION="CONFIG_DEBUG_SECTION_MISMATCH=y"
fi

make_kernel

