.ONESHELL:
SHELL = bash
.SHELLFLAGS = -ec

CROSS_COMPILE ?=$(HOME)/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
PYTHON3?=python3
ARCH=arm64

ifeq ($(IMX8_TARGET),NXPEVK_iMX8M_4GB)
U_BOOT_CONFIG=imx8mq_evk_nt_defconfig
U_BOOT_DTS=fsl-imx8mq-evk
U_BOOT_SOC=iMX8M
ATF_CONFIG=imx8mq
EDK2_PLATFORM=MCIMX8M_EVK_4GB
OPTEE_PLATFORM=mx8mqevk
endif

ifeq ($(IMX8_TARGET),NXPEVK_iMX8M_Mini_2GB)
U_BOOT_CONFIG=imx8mm_evk_nt_defconfig
U_BOOT_DTS=fsl-imx8mm-evk
U_BOOT_SOC=iMX8MM
ATF_CONFIG=imx8mm
EDK2_PLATFORM=MCIMX8M_MINI_EVK_2GB
OPTEE_PLATFORM=mx8mmevk
endif

ifeq ($(IMX8_TARGET),EVK_iMX8MN_2GB)
U_BOOT_CONFIG=imx8mn_ddr4_evk_nt_defconfig
U_BOOT_DTS=fsl-imx8mn-ddr4-evk
U_BOOT_SOC=iMX8MN
ATF_CONFIG=imx8mn
EDK2_PLATFORM=EVK_iMX8MN_2GB
OPTEE_PLATFORM=mx8mnevk
endif

ifeq (,$(IMX8_TARGET))
$(error IMX8_TARGET not set)
endif

IMX8_REPO_ROOT?=$(abspath ../../../)
CST_ROOT?=$(IMX8_REPO_ROOT)/cst
KEY_ROOT?=$(IMX8_REPO_ROOT)/imx-iotcore/build/firmware/test_keys_no_security
MKIMAGE_WORK_DIR=$(IMX8_REPO_ROOT)/imx-mkimage/iMX8M
SRKH_FUSE_BIN=$(KEY_ROOT)/crts/SRK_1_2_3_4_fuse.bin

# Requires paths to spl_uboot.log and firmware binary as parameters.
define generate_spl_csf
	awk -v binary_path="$2" -v indentation=4 '/spl hab block/ { printf "%*sBlocks = %#x %#x %#x \"%s\"\n", indentation, "", $$4, $$5, $$6, binary_path }' $1 \
		| cat $(IMX8_REPO_ROOT)/imx-iotcore/build/firmware/csf_templates/mx8/csf_spl.txt.template - \
		| sed -e 's#{KEY_ROOT}#${KEY_ROOT}#g' > $(MKIMAGE_WORK_DIR)/csf_spl.txt
	$(CST_ROOT)/linux64/bin/cst --o $(MKIMAGE_WORK_DIR)/csf_spl.bin --i $(MKIMAGE_WORK_DIR)/csf_spl.txt
endef

# Requires paths to spl_uboot.log, fit_hab.log, and firmware binary as parameters.
define generate_fit_csf
	{ \
		awk -v binary_path="$3" -v indentation=4 \
				'/sld hab block/ { printf "%*sBlocks = %0#10x %0#8x %0#7x \"%s\"", indentation, "", $$4, $$5, $$6, binary_path }' $1 \
		&& awk -v binary_path="$3" -v indentation=13 'BEGIN { ORS="" } \
				/print_fit_hab/ { x=NR+4;next}  (NR<=x) { printf ", \\\n%*s", indentation, "" ; print $$1 " " $$2 " " $$3 " \"" binary_path  "\"" }' $2 \
		&& echo ;
	} \
		| cat $(IMX8_REPO_ROOT)/imx-iotcore/build/firmware/csf_templates/mx8/csf_fit.txt.template - \
		| sed -e 's#{KEY_ROOT}#${KEY_ROOT}#g' > $(MKIMAGE_WORK_DIR)/csf_fit.txt
	$(CST_ROOT)/linux64/bin/cst --o $(MKIMAGE_WORK_DIR)/csf_fit.bin --i $(MKIMAGE_WORK_DIR)/csf_fit.txt
endef

# Requires paths to UEFI firmware binary as parameter.
define generate_uefi_csf
	cat $(IMX8_REPO_ROOT)/imx-iotcore/build/firmware/csf_templates/mx8/csf_uefi.txt.template \
		| sed -e 's#{KEY_ROOT}#${KEY_ROOT}#g' > csf_uefi.txt
	UEFI_SIZE=$$(ls -lct $1 | awk '{ print $$5 }') ;\
	INDENTATION=4 ;\
	printf "%*sBlocks = 0x40480000 0x00000 0x%x \"$1\"\n" $$INDENTATION "" $$UEFI_SIZE >> csf_uefi.txt
endef

# Requires paths to spl_uboot.log and u-boot firmware binary as parameters.
define sign_uboot_binary
	SIGNED_UBOOT_BIN=$$(dirname $2)/signed_$$(basename $2) ;\
	cp $2 $$SIGNED_UBOOT_BIN ;\
	SPL_CSF_OFF=$$(awk '/ csf_off/ { print $$2 }' $1) ;\
	FIT_CSF_OFF=$$(awk '/ sld_csf_off/ { print $$2 }' $1) ;\
	dd if=$(MKIMAGE_WORK_DIR)/csf_spl.bin of=$$SIGNED_UBOOT_BIN seek=$$(printf '%d' $$SPL_CSF_OFF) bs=1 conv=notrunc ;\
	dd if=$(MKIMAGE_WORK_DIR)/csf_fit.bin of=$$SIGNED_UBOOT_BIN seek=$$(printf '%d' $$FIT_CSF_OFF) bs=1 conv=notrunc
endef

# Requires UEFI size as parameters.
define generate_ivt_for_uefi
	cat $(IMX8_REPO_ROOT)/imx-iotcore/build/firmware/csf_templates/mx8/genIVT-template.pl | sed -e "s#{UEFI_PADDED_SIZE}#$1#g" > genIVT.pl
	perl genIVT.pl
endef

# Requires paths to UEFI firmware binary as parameter.
define sign_uefi_binary
	SIGNED_UEFI_BIN=$$(dirname $1)/signed_$$(basename $1) ;\
	cp $1 $$SIGNED_UEFI_BIN ;\
	UEFI_SIZE=$$(ls -lct $$SIGNED_UEFI_BIN | awk '{ print $$5 }') ;\
	UEFI_PADDED_SIZE=$$(printf "0x%x" $$(( ( $$UEFI_SIZE + 0xfff ) & ~0xfff ))) ;\
	objcopy -I binary -O binary --pad-to $$UEFI_PADDED_SIZE --gap-fill=0x00 $$SIGNED_UEFI_BIN ;\
	$(call generate_ivt_for_uefi,$$UEFI_PADDED_SIZE) ;\
	cat ivt.bin >> $$SIGNED_UEFI_BIN ;\
	$(call generate_uefi_csf,$$SIGNED_UEFI_BIN) ;\
	$(CST_ROOT)/linux64/bin/cst -i csf_uefi.txt -o csf_uefi.bin ;\
	cat csf_uefi.bin >> $$SIGNED_UEFI_BIN
endef

# Requires path to u-boot config file as a parameter.
define update_srkh_in_uboot_config
	SRKH_VALUES=$$(hexdump -e '/4 "0x"' -e '/4 "%X""\n"' $(SRKH_FUSE_BIN)) ; SRKH_VALUES=$$(echo -n $$SRKH_VALUES) ;\
	tmpfile=$$(mktemp) ;\
	cp $1 $$tmpfile ;\
	sed 's/\<CONFIG_SPL_HAB_SRKH\>/<\0>/' $$tmpfile \
		| awk -v config_value="$$SRKH_VALUES" 'BEGIN { config_line = "CONFIG_SPL_HAB_SRKH=\"" config_value "\""} \
			/<CONFIG_SPL_HAB_SRKH>/ { print config_line ; pattern_found = 1 ; next } \
			{ print } \
			END { if (!pattern_found) { print config_line } }' > $1 ;\
	rm $$tmpfile
endef

.PHONY: imx8_u-boot imx8_optee imx8_atf imx8_uefi imx8_tas imx8_mkimage imx8_update-ffu imx8_build

imx8_u-boot:
	cd $(IMX8_REPO_ROOT)/u-boots/u-boot-imx_arm64
	export ARCH=$(ARCH)
	export CROSS_COMPILE=$(CROSS_COMPILE)
	
	$(call update_srkh_in_uboot_config,./configs/$(U_BOOT_CONFIG))
	$(MAKE) $(U_BOOT_CONFIG) 
	$(MAKE)
	
imx8_atf:
	cd $(IMX8_REPO_ROOT)/imx-atf
	export ARCH=$(ARCH)
	export CROSS_COMPILE=$(CROSS_COMPILE)
	rm -rf build
	$(MAKE) PLAT=$(ATF_CONFIG) SPD=opteed bl31
	
imx8_optee:
	cd $(IMX8_REPO_ROOT)/optee_os/optee_os_arm64
ifeq ($(IMX8_TARGET),NXPEVK_iMX8M_4GB)
	$(MAKE) PLATFORM=imx PLATFORM_FLAVOR=$(OPTEE_PLATFORM) \
     CFG_TEE_CORE_DEBUG=n CFG_TEE_CORE_LOG_LEVEL=2  CFG_UART_BASE=0x30890000 \
     CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n  \
     CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=131072 \
	 CROSS_COMPILE64=$(CROSS_COMPILE) \
	 CROSS_COMPILE= \

else
	$(MAKE) PLATFORM=imx PLATFORM_FLAVOR=$(OPTEE_PLATFORM) \
     CFG_TEE_CORE_DEBUG=n CFG_TEE_CORE_LOG_LEVEL=2 \
     CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n  \
     CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=131072 \
	 CROSS_COMPILE64=$(CROSS_COMPILE) \
	 CROSS_COMPILE= \

endif
	 
	$(CROSS_COMPILE)objcopy -O binary ./out/arm-plat-imx/core/tee.elf ./out/arm-plat-imx/tee.bin
	
imx8_tas: imx8_optee

IMX8_MKIMAGE_DEPS_LPDDR4=$(wildcard $(IMX8_REPO_ROOT)/firmware-imx-8.1/firmware/ddr/synopsys/lpddr4_pmu_train_*.bin)
IMX8_MKIMAGE_DEPS_DDR4=$(wildcard $(IMX8_REPO_ROOT)/firmware-imx-8.1/firmware/ddr/synopsys/ddr4_*.bin)
IMX8_MKIMAGE_DEPS_HDMI=$(wildcard $(IMX8_REPO_ROOT)/firmware-imx-8.1/firmware/hdmi/cadence/signed_hdmi_imx8m.bin)
IMX8_MKIMAGE_DEPS_OPTEE=$(IMX8_REPO_ROOT)/optee_os/optee_os_arm64/out/arm-plat-imx/tee.bin
IMX8_MKIMAGE_DEPS_ATF=$(IMX8_REPO_ROOT)/imx-atf/build/$(ATF_CONFIG)/release/bl31.bin
IMX8_MKIMAGE_DEPS_U-BOOT=$(IMX8_REPO_ROOT)/u-boots/u-boot-imx_arm64/u-boot-nodtb.bin $(IMX8_REPO_ROOT)/u-boots/u-boot-imx_arm64/spl/u-boot-spl.bin $(IMX8_REPO_ROOT)/u-boots/u-boot-imx_arm64/arch/arm/dts/$(U_BOOT_DTS).dtb $(IMX8_REPO_ROOT)/u-boots/u-boot-imx_arm64/tools/mkimage

imx8_mkimage: imx8_optee imx8_u-boot imx8_atf $(IMX8_MKIMAGE_DEPS_LPDDR4) $(IMX8_MKIMAGE_DEPS_DDR4) $(IMX8_MKIMAGE_DEPS_HDMI)
	cd $(IMX8_REPO_ROOT)/imx-mkimage/iMX8M
	rm -f *.bin *.dtb
ifeq ($(IMX8_TARGET),NXPEVK_iMX8M_Mini_2GB)
	cp $(IMX8_MKIMAGE_DEPS_LPDDR4) .
endif
ifeq ($(IMX8_TARGET),EVK_iMX8MN_2GB)
	cp $(IMX8_MKIMAGE_DEPS_DDR4) .
endif
ifeq ($(IMX8_TARGET),NXPEVK_iMX8M_4GB)
	cp $(IMX8_MKIMAGE_DEPS_HDMI) .
	cp $(IMX8_MKIMAGE_DEPS_LPDDR4) .
endif
	cp $(IMX8_MKIMAGE_DEPS_OPTEE) .
	cp $(IMX8_MKIMAGE_DEPS_ATF) .
	cp $(IMX8_MKIMAGE_DEPS_U-BOOT) .
	mv mkimage mkimage_uboot	
	cd ..
ifeq ($(IMX8_TARGET),NXPEVK_iMX8M_Mini_2GB)
	$(MAKE) SOC=$(U_BOOT_SOC) flash_spl_uboot
else ifeq ($(IMX8_TARGET),EVK_iMX8MN_2GB)
	$(MAKE) SOC=$(U_BOOT_SOC) flash_ddr4_evk
else
	$(MAKE) SOC=$(U_BOOT_SOC) flash_hdmi_spl_uboot 2>&1 | tee $(MKIMAGE_WORK_DIR)/spl_uboot.log
	$(MAKE) SOC=$(U_BOOT_SOC) print_fit_hab > $(MKIMAGE_WORK_DIR)/fit_hab.log
	$(call generate_spl_csf,$(MKIMAGE_WORK_DIR)/spl_uboot.log,$(MKIMAGE_WORK_DIR)/flash.bin)
	$(call generate_fit_csf,$(MKIMAGE_WORK_DIR)/spl_uboot.log,$(MKIMAGE_WORK_DIR)/fit_hab.log,$(MKIMAGE_WORK_DIR)/flash.bin)
	$(call sign_uboot_binary,$(MKIMAGE_WORK_DIR)/spl_uboot.log,$(MKIMAGE_WORK_DIR)/flash.bin)
endif

imx8_uefi: imx8_u-boot imx8_tas its/uefi_imx8_unsigned.its
	export GCC5_AARCH64_PREFIX=$(CROSS_COMPILE)
	
	cd $(IMX8_REPO_ROOT)/mu_platform_nxp
	$(PYTHON3) -m pip install -r requirements.txt --upgrade
	$(PYTHON3) NXP/$(EDK2_PLATFORM)/PlatformBuild.py --setup

	cd MU_BASECORE
	$(MAKE) -C BaseTools 
	cd ..
	
	$(PYTHON3) NXP/$(EDK2_PLATFORM)/PlatformBuild.py -V TARGET=RELEASE \
     PROFILE=DEV MAX_CONCURRENT_THREAD_NUMBER=20
	
	cd Build/$(EDK2_PLATFORM)/RELEASE_GCC5/FV
	cp $(CURDIR)/its/uefi_imx8_unsigned.its .
	$(IMX8_REPO_ROOT)/u-boots/u-boot-imx_arm64/tools/mkimage -f uefi_imx8_unsigned.its -r uefi.fit
	$(call sign_uefi_binary,uefi.fit)

imx8_build: imx8_uefi imx8_mkimage

imx8_update-ffu: imx8_build
	cp $(IMX8_REPO_ROOT)/imx-mkimage/iMX8M/flash.bin $(CURDIR)/../board/$(IMX8_TARGET)/Package/BootLoader/flash.bin
	cp $(IMX8_REPO_ROOT)/mu_platform_nxp/Build/$(EDK2_PLATFORM)/RELEASE_GCC5/FV/uefi.fit $(CURDIR)/../board/$(IMX8_TARGET)/Package/BootFirmware/uefi.fit

imx8_commit-firmware: imx8_update-ffu
	git add $(CURDIR)/../board/$(IMX8_TARGET)/Package/BootLoader
	git add $(CURDIR)/../board/$(IMX8_TARGET)/Package/BootFirmware
	@echo "Successfully copied files to package and staged for commit"
