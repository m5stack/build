# Axera AX630c quad core 4GB SoC 32GBe eMMC USB2
# 这行注释会在menuconfig中显示，即 [配置文件名 无扩展名] (csc) [这行 # 后面的内容]
# 例如作为标识和说明该配置适用的硬件平台信息

BOARD_NAME="M5stack LLM630 Compute Kit"
# 开发板名称，用于标识构建的目标硬件（例如显示给用户看的名字）

BOARDFAMILY="ax630c"
# 指定开发板的Soc架构系列，通常用来选择特定的编译参数和补丁
# 所有针对ax630c系列的通用设置会应用于该板

# BOARD_FIRMWARE_INSTALL="-full"
# 如果取消注释，表示固件安装为最完整版本（含闭源驱动）
# 一般用于需要最大兼容性的情况

BOARD_MAINTAINER="dianjixz"
# 维护人员标识，有问题可以联系此人，便于后续维护和追溯

# BOOTCONFIG="radxa-zero-3-rk3566_defconfig"
# 指定U-Boot的默认配置文件，控制U-Boot功能和启动流程
# 不同的配置文件可能导致U-Boot启动状态不同，请根据硬件选择

KERNEL_TARGET="edge"
# 内核支持的版本分支范围：vendor为官方内核，current为主线稳定内核，edge为主线测试/开发分支
# edge通常包含最新特性但也可能存在稳定性风险

FULL_DESKTOP="yes"
# 如果取消注释则安装完整桌面环境
# 当前注释掉，表示不安装桌面，仅为命令行系统

# BOOT_LOGO="desktop"
# 启动时显示的Logo图片，可以自定义用户体验

# BOOT_FDT_FILE="rockchip/rk3568-xiguapi-v3.dtb"
# 指定启动加载的设备树文件（.dtb），用于设备的硬件资源映射和驱动加载
# 在armbianEnv.txt中设置，必须与目标板兼容，否则启动失败

BOOT_SCENARIO="spl-blobs"
# 启动流程方案，比如是否需要使用spl、闭源blobs等

IMAGE_PARTITION_TABLE="gpt"
# 磁盘分区方案，采用GPT
# 比MBR支持更多分区和更大容量，目前Linux常用

BOOTFS_TYPE="ext4"
# boot分区的文件系统类型，这里指定为ext4，有更好的性能和兼容性

declare -g EXT_ROOTFS_SIZE=30606884864

BOOTPATCHDIR="u-boot-axera/kit"
BOOTCONFIG="AX630C_m5stack_LITE_defconfig"


LINUXCONFIG="linux-ax630c-kit"
LINUXDTS="m5stack-ax630c-lite"

CUSTOM_ROOTFS_SCRIPT=axera-image-kit.sh

# SRC_EXTLINUX
# SRC_CMDLINE
# compile: ./compile.sh build BOARD=m5stack-ax630c-kit BRANCH=edge BUILD_DESKTOP=no BUILD_MINIMAL=yes KERNEL_CONFIGURE=no RELEASE=trixie