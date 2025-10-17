# Rockchip RK3568 quad core 4-32GB SoC 1GBe eMMC USB3	# 这行会在menuconfig中显示，即 [配置文件名 无扩展名] (csc) [这行 # 后面的内容]
BOARD_NAME="Nlnet Xiguapi V3"				# 开发板名称
BOARDFAMILY="rk35xx"					# 在 config/sources/families/rk35xx.conf 里面有该架构详细配置
BOARD_FIRMWARE_INSTALL="-full"				# 安装闭源固件驱动最全版本
BOARD_MAINTAINER="leux"					# 开发板维护者
BOOTCONFIG="radxa-zero-3-rk3566_defconfig"		# 使用的U-Boot的配置文件，有的配置文件会导致卡在u-boot启动时
KERNEL_TARGET="edge"					# 内核支持哪些版本：vendor,current,edge
FULL_DESKTOP="yes"
BOOT_LOGO="desktop"
BOOT_FDT_FILE="rockchip/rk3568-xiguapi-v3.dtb"		# 这里指定在 armbianEnv.txt 中要加载的设备树文件
BOOT_SCENARIO="spl-blobs"
IMAGE_PARTITION_TABLE="gpt"				# 镜像只分一个区，若后面有 BOOTFS_TYPE="fat" 行则分两区存放rootfs与boot

# Mainline U-Boot
function post_family_config_branch_edge__lubancat_2_use_mainline_uboot() {
	display_alert "$BOARD" "Using mainline U-Boot for $BOARD / $BRANCH" "info"

	declare -g BOOTSOURCE="https://github.com/u-boot/u-boot.git" # We ❤️ Mainline U-Boot
	declare -g BOOTBRANCH="tag:v2025.04"
	declare -g BOOTPATCHDIR="v2025.04"
	# Don't set BOOTDIR, allow shared U-Boot source directory for disk space efficiency

	declare -g UBOOT_TARGET_MAP="BL31=${RKBIN_DIR}/${BL31_BLOB} ROCKCHIP_TPL=${RKBIN_DIR}/${DDR_BLOB};;u-boot-rockchip.bin"

	# Disable stuff from rockchip64_common; we're using binman here which does all the work already
	unset uboot_custom_postprocess write_uboot_platform write_uboot_platform_mtd

	# Just use the binman-provided u-boot-rockchip.bin, which is ready-to-go
	function write_uboot_platform() {
		dd "if=$1/u-boot-rockchip.bin" "of=$2" bs=32k seek=1 conv=notrunc status=none
	}
}
