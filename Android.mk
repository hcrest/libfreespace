#$(info Begin libfreespace Android.mk)

ifdef NDK_ROOT
	LIBFREESPACE_NDK_BUILD := 1
endif

LOCAL_PATH := $(call my-dir)/

include $(CLEAR_VARS)

ifdef LIBFREESPACE_NDK_BUILD
	LIBFREESPACE_GEN_DIR := $(LOCAL_PATH)../../obj/libfreespace/
else
    LIBFREESPACE_GEN_DIR := $(call local-intermediates-dir)
endif

#$(info    Generated files in: $(LIBFREESPACE_GEN_DIR))

# Generated files directory
$(LIBFREESPACE_GEN_DIR) :
	@echo "libfreespace <= ** Create directory: $@"
	@mkdir -p $@/include/freespace
	@mkdir -p $@/common

# Generate a config.h file.
LIBFREESPACE_CONF_FILE := $(LIBFREESPACE_GEN_DIR)/config.h

$(LIBFREESPACE_CONF_FILE) : $(LIBFREESPACE_GEN_DIR)
	@echo "libfreespace <= ** Creating Config File: $@"
	@echo "#define VERSION \"\"	" > $@	
	
# Generate codecs.
# Respect Android build system and do not run/generate scripts in source directory 

# Copy script to build directory so it can be run from there 
LIBFREESPACE_MSG_GEN_SCRIPT := $(LIBFREESPACE_GEN_DIR)/common/messageCodeGenerator.py

$(LIBFREESPACE_MSG_GEN_SCRIPT) : local_path := $(LOCAL_PATH)
$(LIBFREESPACE_MSG_GEN_SCRIPT) : gen_dir := $(LIBFREESPACE_GEN_DIR)
$(LIBFREESPACE_MSG_GEN_SCRIPT) : $(LOCAL_PATH)/common/messageCodeGenerator.py $(LIBFREESPACE_GEN_DIR)
	@echo "libfreespace <= ** Copy Message Code Generator"
	@cp $(local_path)/common/messageCodeGenerator.py $(gen_dir)/common
	
# Step 2. Setup rules to generate codecs
LIBFREESPACE_MSG_GEN := $(LIBFREESPACE_GEN_DIR)/.msg

$(LIBFREESPACE_MSG_GEN) : libfreespace_gen_dir := $(LIBFREESPACE_GEN_DIR)
$(LIBFREESPACE_MSG_GEN) : localpath := $(LOCAL_PATH)
# TODO Figure out why this runs on every build
$(LIBFREESPACE_MSG_GEN) : $(LIBFREESPACE_MSG_GEN_SCRIPT) $(LIBFREESPACE_GEN_DIR) $(LIBFREESPACE_MSG_GEN_SCRIPT) 
	@echo "libfreespace <= ** Running Message Code Generator"
	@mkdir -p $(libfreespace_gen_dir)/include/freespace;
	@cd $(libfreespace_gen_dir)/common && \
	python messageCodeGenerator.py $(localpath)/common/setupMessages.py $(LIBFREESPACE_EXTRA_MSG_FILE)
	@touch $(LIBFREESPACE_MSG_GEN)

# Outputs:
LIBFREESPACE_MSG_GEN_SRCS := \
	$(LIBFREESPACE_GEN_DIR)/common/freespace_printers.c \
	$(LIBFREESPACE_GEN_DIR)/common/freespace_codecs.c \
	$(LIBFREESPACE_GEN_DIR)/include/freespace_printers.h \
	$(LIBFREESPACE_GEN_DIR)/include/freespace_codecs.h
	
$(LIBFREESPACE_MSG_GEN_SRCS) : $(LIBFREESPACE_MSG_GEN)

## Additional dependencies
$(LOCAL_PATH)/linux/freespace_hidraw.c : $(LIBFREESPACE_MSG_GEN) $(LIBFREESPACE_CONF_FILE)
##$(all_objects): $(LIBFREESPACE_CONF_FILE) $(LIBFREESPACE_MSG_GEN_SCRIPT)
 
LOCAL_MODULE := freespace

LOCAL_MODULE_CLASS := STATIC_LIBRARIES

LOCAL_SRC_FILES := \
	linux/freespace_hidraw.c \
	common/freespace_deviceTable.c

ifdef LIBFREESPACE_NDK_BUILD
LOCAL_SRC_FILES += \
	../../obj/libfreespace/common/freespace_printers.c \
	../../obj/libfreespace/common/freespace_codecs.c	
else
LOCAL_GENERATED_SOURCES := $(LIBFREESPACE_CONF_FILE) $(LIBFREESPACE_MSG_GEN_SRCS)
endif

LOCAL_CFLAGS += -DFREESPACE_LITTLE_ENDIAN

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include \
	$(LIBFREESPACE_GEN_DIR)/ \
	$(LIBFREESPACE_GEN_DIR)/include/ \
	$(LIBFREESPACE_GEN_DIR)/include/freespace

LOCAL_EXPORT_C_INCLUDES := \
	$(LOCAL_PATH)/include \
	$(LIBFREESPACE_GEN_DIR)/include/ \
	$(LIBFREESPACE_GEN_DIR)/include/freespace

include $(BUILD_STATIC_LIBRARY)
