#
# SPDX-License-Identifier: GPL-2.0
#
# Copyright (c) 2013-2023 Igor Pecovnik, igor@armbian.com
#
# This file is a part of the Armbian Build Framework
# https://github.com/armbian/build/
# Author: dianjixz


enable_extension "axerabin-tools"
ARCH=arm64
ATF_COMPILE='no'
# BOOTDELAY=1
# BOOTBRANCH="${BOOTBRANCH:-"tag:v2025.04"}"
# BOOTENV_FILE='sunxi.txt'
# UBOOT_TARGET_MAP="u-boot-dtb.bin"
# BOOTSCRIPT='boot-sun50i-next.cmd:boot.cmd'
# OVERLAY_DIR="/boot/dtb/allwinner/overlay"

cp $SRC/config/templates/${CUSTOM_ROOTFS_SCRIPT} $SRC/userpatches/customize-image.sh


family_tweaks() {
	echo '-----------family_tweaks-----------------'
	# execute specific tweaks function if present
	# [[ $(type -t family_tweaks_s) == function ]] && family_tweaks_s
	# cp $SRC/packages/blobs/splash/armbian-u-boot-24.bmp $SDCARD/boot/boot.bmp
}

write_uboot_platform() {
	echo '-----------family_tweaks-----------------'
	# dd if=/dev/zero of=$2 bs=1k count=1023 seek=1 status=noxfer > /dev/null 2>&1
	# dd if=$1/u-boot-sunxi-with-spl.bin of=$2 bs=1024 seek=8 status=noxfer > /dev/null 2>&1
}

setup_write_uboot_platform() {
	echo '-----------setup_write_uboot_platform-----------------'

}


# 安装 axerabin_tools 工具包
function host_dependencies_ready__axera() {
	call_extension_method "fetch_sources_axera_tools" <<- 'FETCH_SOURCES_AXERA_TOOLS'
		*run after all host dependencies are installed*
		At this point we can read `${FINAL_HOST_DEPS}`, but changing won't have any effect.
		All the dependencies, including the default/core deps and the ones added via `${EXTRA_BUILD_DEPS}`
		are installed at this point. The system clock has not yet been synced.
	FETCH_SOURCES_AXERA_TOOLS
}

function custom_kernel_config__unset_armbian_ext_config() {
	# kernel_config_set_n 
	# kernel_config_set_m 
	# kernel_config_set_y 
	echo '-------custom_kernel_config_unset_armbian_ext_config-----------------'
	# kernel_config_set_n CONFIG_POSIX_MQUEUE
	# kernel_config_set_n CONFIG_CFS_BANDWIDTH
	
	# kernel_config_set_n CONFIG_CGROUP_HUGETLB
	# kernel_config_set_n CONFIG_CPUSETS
	# kernel_config_set_n CONFIG_USER_NS
	# kernel_config_set_n CONFIG_BLK_DEV_THROTTLING
	# kernel_config_set_n CONFIG_NF_CONNTRACK_ZONES
	# kernel_config_set_n CONFIG_NF_CONNTRACK_EVENTS

	# kernel_config_set_n CONFIG_IP_VS_PROTO_TCP
	# kernel_config_set_n CONFIG_IP_VS_PROTO_UDP
	# kernel_config_set_n CONFIG_IP_VS_NFCT
	# kernel_config_set_n CONFIG_CGROUP_NET_PRIO
	# kernel_config_set_n CONFIG_BPF_JIT
	# kernel_config_set_n CONFIG_ZRAM_WRITEBACK
	# kernel_config_set_n CONFIG_ZRAM_MEMORY_TRACKING
	# kernel_config_set_n CONFIG_EXT4_FS_SECURITY
	# kernel_config_set_n CONFIG_OVERLAY_FS_REDIRECT_ALWAYS_FOLLOW
	# kernel_config_set_n CONFIG_PERSISTENT_KEYRINGS
	# kernel_config_set_n CONFIG_ENCRYPTED_KEYS
	# kernel_config_set_n CONFIG_KEY_DH_OPERATIONS
	kernel_config_set_n CONFIG_RT_GROUP_SCHED
	kernel_config_set_n CONFIG_FTRACE_SYSCALLS
}



function pre_umount_final_image__export_rootfs() {
	sleep 360
	source "$SRC/config/templates/${CUSTOM_ROOTFS_SCRIPT}" host_build
	pre_export_rootfs_final_image


	mkdir -p ${SRC}/cache/sources/rootfs-worktree
	pushd ${SRC}/cache/sources/axerabin-tools/tools/bin
	${SRC}/cache/sources/axerabin-tools/tools/bin/make_ext4fs -l ${EXT_ROOTFS_SIZE} -s ${DEST}/axera_image/rootfs_sparse.ext4 ${MOUNT}
	popd
}



function run_after_build__creat_axera_axp() {
	case $BOARDFAMILY in

		ax630c)
			creat_axera_ax630c_axp
			;;
		ax650n)
			echo "none"
			;;
	esac
}



function creat_axera_ax630c_axp() {
	# ${SRC} ${DEST}
	display_alert "Building axp package" "Building axp package" "info"

	# rootfs 文件处理
	cp ${SRC}/cache/sources/axerabin-tools/ax630c/* ${DEST}/axera_image/

	# uboot 文件处理
	BINARIES_DIR=$SRC/cache/sources/$BOOTSOURCEDIR
	
	python3 ${AXERA_TOOLS_SIGN_SCRIPT} -i ${BINARIES_DIR}/u-boot-dtb.bin \
		-o ${BINARIES_DIR}/fdl2_signed.bin -pub ${AXERA_TOOLS_PUB_KEY} -prv ${AXERA_TOOLS_PRIV_KEY} ${AXERA_TOOLS_SIGN_PARAMS}
	${AXERA_TOOLS_PATH}/ax_gzip -9 ${BINARIES_DIR}/u-boot-dtb.bin
	python3 ${AXERA_TOOLS_SIGN_SCRIPT} -i ${BINARIES_DIR}/u-boot-dtb_axgzip.bin \
		-o ${BINARIES_DIR}/u-boot_signed.bin -pub ${AXERA_TOOLS_PUB_KEY} -prv ${AXERA_TOOLS_PRIV_KEY} ${AXERA_TOOLS_SIGN_PARAMS}
	cp ${BINARIES_DIR}/u-boot_signed.bin ${BINARIES_DIR}/u-boot_b_signed.bin

	cp ${BINARIES_DIR}/u-boot_signed.bin ${DEST}/axera_image/
	cp ${BINARIES_DIR}/u-boot_b_signed.bin ${DEST}/axera_image/


	# 内核文件处理
	BINARIES_DIR=${SRC}/cache/sources/linux-kernel-worktree/${KERNEL_MAJOR_MINOR}__${LINUXFAMILY}__${ARCH}/arch/arm64/boot

	${AXERA_TOOLS_PATH}/ax_gzip -9 ${BINARIES_DIR}/Image
	python3 ${AXERA_TOOLS_SIGN_SCRIPT} -i ${BINARIES_DIR}/Image.axgzip \
		-o ${BINARIES_DIR}/boot_signed.bin -pub ${AXERA_TOOLS_PUB_KEY} -prv ${AXERA_TOOLS_PRIV_KEY} ${AXERA_TOOLS_SIGN_PARAMS}
	cp ${BINARIES_DIR}/boot_signed.bin ${BINARIES_DIR}/boot_signed.bin.1


	${AXERA_TOOLS_PATH}/ax_gzip -9 ${BINARIES_DIR}/dts/axera/${LINUXDTS}.dtb
	python3 ${AXERA_TOOLS_SIGN_SCRIPT} -i ${BINARIES_DIR}/dts/axera/${LINUXDTS}.dtb.axgzip \
		-o ${BINARIES_DIR}/dts/axera/AX630C_emmc_arm64_k419_signed.dtb -pub ${AXERA_TOOLS_PUB_KEY} -prv ${AXERA_TOOLS_PRIV_KEY} ${AXERA_TOOLS_SIGN_PARAMS}
	cp ${BINARIES_DIR}/dts/axera/AX630C_emmc_arm64_k419_signed.dtb ${BINARIES_DIR}/dts/axera/AX630C_emmc_arm64_k419_signed.dtb.1


	cp ${BINARIES_DIR}/boot_signed.bin ${DEST}/axera_image/
	cp ${BINARIES_DIR}/boot_signed.bin.1 ${DEST}/axera_image/
	cp ${BINARIES_DIR}/dts/axera/AX630C_emmc_arm64_k419_signed.dtb ${DEST}/axera_image/
	cp ${BINARIES_DIR}/dts/axera/AX630C_emmc_arm64_k419_signed.dtb.1 ${DEST}/axera_image/

	
	sed -i "s/V2\.0\.0_P7_20240513101106_[0-9]\{14\}/V2.0.0_P7_20240513101106_$(date +%Y%m%d%H%M%S)/g" ${DEST}/axera_image/AX630C_emmc_arm64_k419.xml

	# 打包 axp
	pushd ${DEST}/axera_image/
	zip -r ${DEST}/images/ax630c.zip .
	mv ${DEST}/images/ax630c.zip ${DEST}/images/${version}.axp
	popd
}


mkdir -p ${DEST}/axera_image
rm -rf ${DEST}/axera_image/*