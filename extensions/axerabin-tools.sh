#!/usr/bin/env bash
function fetch_sources_axera_tools__axerabin() {
	local AXERA_BOARD_TYPE="${BOARD##*-}"
	fetch_from_repo "${AXERABIN_GIT_URL:-"https://github.com/dianjixz/axerabin"}" "axerabin-tools" "branch:${AXERABIN_GIT_BRANCH:-"main"}"

	declare -g AXERA_TOOLS_PATH=${SRC}/cache/sources/axerabin-tools/tools/bin
	declare -g AXERA_TOOLS_SIGN_SCRIPT=${AXERA_TOOLS_PATH}/imgsign/sec_boot_AX620E_sign.py
	declare -g AXERA_TOOLS_PUB_KEY=${AXERA_TOOLS_PATH}/imgsign/public.pem
	declare -g AXERA_TOOLS_PRIV_KEY=${AXERA_TOOLS_PATH}/imgsign/private.pem
	declare -g AXERA_TOOLS_SIGN_PARAMS="-cap 0x54FAFE -key_bit 2048"
	

	mkdir -p ${SRC}/cache/dl
	
	[ -f ${SRC}/cache/dl/rootfs_overlay_${AXERA_BOARD_TYPE}.tar.gz ] || wget --passive-ftp -nd -t 3 -O ${SRC}/cache/dl/rootfs_overlay_${AXERA_BOARD_TYPE}.tar.gz https://github.com/dianjixz/axerabin/releases/download/v0.0.1/rootfs_overlay_${AXERA_BOARD_TYPE}.tar.gz
	if [ ! -d ${SRC}/userpatches/rootfs_overlay_${AXERA_BOARD_TYPE} ] ; then
		mkdir -p  ${SRC}/userpatches/rootfs_overlay_${AXERA_BOARD_TYPE}
		tar zxf ${SRC}/cache/dl/rootfs_overlay_${AXERA_BOARD_TYPE}.tar.gz -C ${SRC}/userpatches/rootfs_overlay_${AXERA_BOARD_TYPE}
	fi
	# fetch_from_repo "${AXERABIN_GIT_URL:-"https://github.com/dianjixz/module_kit_uboot.git"}" "module_kit_uboot" "branch:${AXERABIN_GIT_BRANCH:-"main"}"
	# fetch_from_repo "${AXERABIN_GIT_URL:-"https://github.com/dianjixz/module_kit_linux.git"}" "module_kit_linux" "branch:${AXERABIN_GIT_BRANCH:-"main"}"
}

function build_host_tools__install_axerabin_tools() {
	echo '------------------------------------build_host_tools__install_axerabin_tools------------------------------'

}
