LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_PRELINK_MODULE := false
LOCAL_SRC_FILES := src/ti_omx_interface.cpp

LOCAL_MODULE := libVendor_ti_omx
LOCAL_MODULE_TAGS:= optional

PV_TOP := external/opencore

PV_COPY_HEADERS_TO := libpv

PV_INCLUDES := \
	$(PV_TOP)/android \
	$(PV_TOP)/extern_libs_v2/khronos/openmax/include \
	$(PV_TOP)/engines/common/include \
	$(PV_TOP)/engines/player/config/android \
	$(PV_TOP)/engines/player/include \
	$(PV_TOP)/nodes/pvmediaoutputnode/include \
	$(PV_TOP)/nodes/pvdownloadmanagernode/config/opencore \
	$(PV_TOP)/pvmi/pvmf/include \
	$(PV_TOP)/fileformats/mp4/parser/config/opencore \
	$(PV_TOP)/engines/author/include \
	$(PV_TOP)/android/drm/oma1/src \
	$(PV_TOP)/codecs_v2/omx/omx_mastercore/include \
	$(PV_TOP)/codecs_v2/omx/omx_common/include \
	$(PV_TOP)/codecs_v2/omx/omx_queue/src \
	$(PV_TOP)/codecs_v2/omx/omx_proxy/src \
	$(PV_TOP)/oscl/oscl/config/android \
	$(PV_TOP)/oscl/oscl/config/shared \
	$(PV_TOP)/oscl/oscl/osclbase/src \
	$(PV_TOP)/oscl/oscl/osclerror/src \
	$(PV_TOP)/oscl/oscl/osclio/src \
	$(PV_TOP)/oscl/oscl/oscllib/src \
	$(PV_TOP)/oscl/oscl/osclmemory/src \
	$(PV_TOP)/oscl/oscl/osclproc/src \
	$(PV_TOP)/oscl/oscl/osclregcli/src \
	$(PV_TOP)/oscl/oscl/osclregsrv/src \
	$(PV_TOP)/oscl/oscl/osclutil/src \
	$(PV_TOP)/oscl/pvlogger/src \
	$(PV_TOP)/build_config/opencore_dynamic \
	hardware/ti/omap4/omx/ducati/domx/system/omx_core/inc \
	$(TARGET_OUT_HEADERS)/$(PV_COPY_HEADERS_TO)

LOCAL_CFLAGS :=   $(PV_CFLAGS)
ifeq ($(TARGET_BOARD_PLATFORM),omap4)
LOCAL_CFLAGS += -DTARGET_OMAP4
endif
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := \
    $(PV_INCLUDES)

LOCAL_SHARED_LIBRARIES := libOMX_Core

-include $(PV_TOP)/Android_platform_extras.mk

-include $(PV_TOP)/Android_system_extras.mk

include $(BUILD_SHARED_LIBRARY)

