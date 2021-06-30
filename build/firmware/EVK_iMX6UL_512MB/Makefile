
UBOOT_CONFIG=mx6ul_14x14_evk_nt_defconfig
UBOOT_ROOT=../../../../u-boots/u-boot-imx_arm

EDK2_DSC=EVK_iMX6UL_512MB
EDK2_PLATFORM=NXP/EVK_iMX6UL_512MB
EDK2_DEBUG_RELEASE=RELEASE

UBOOT_OPTEE_ITS=uboot_optee_unsigned.its
UEFI_ITS=uefi_dtb_unsigned.its

all: firmware_fit.merged firmwareversions.log

include ../Common.mk

.PHONY: $(OPTEE)
$(OPTEE):
	$(MAKE) -C $(OPTEE_ROOT) O=$(OPTEE_OUT) PLATFORM=imx-mx6ulevk \
	  $(OPTEE_FLAGS_IMX6UL)
