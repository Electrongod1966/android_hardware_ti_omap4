/*
 * Copyright (C) Texas Instruments - http://www.ti.com/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */



#ifndef TI_CAMERA_PARAMETERS_H
#define TI_CAMERA_PARAMETERS_H

#include <utils/KeyedVector.h>
#include <utils/String8.h>

namespace android {

///TI Specific Camera Parameters
class TICameraParameters
{
public:

// Supported Camera indexes
// Example value: "0,1,2,3", where 0-primary, 1-secondary1, 2-secondary2, 3-sterocamera
static const  char KEY_SUPPORTED_CAMERAS[];
// Select logical Camera index
static const char KEY_CAMERA[];
static const char KEY_CAMERA_MOTO[];
static const char KEY_CAMERA_MOTO_VALUES[];
static const char KEY_CAMERA_NAME[];
static const  char KEY_S3D_SUPPORTED[];
static const char  KEY_FACE_DETECTION_ENABLE[];
static const char  KEY_FACE_DETECTION_DATA[];
static const char  KEY_FACE_DETECTION_THRESHOLD[];
static const char  KEY_BURST[];
static const  char KEY_CAP_MODE[];
static const  char KEY_VSTAB[];
static const  char KEY_VSTAB_VALUES[];
static const  char KEY_VNF[];
static const  char KEY_VNF_VALUES[];
static const  char KEY_SATURATION[];
static const  char KEY_BRIGHTNESS[];
static const  char KEY_EXPOSURE_MODE[];
static const  char KEY_SUPPORTED_EXPOSURE[];
static const  char KEY_CONTRAST[];
static const  char KEY_SHARPNESS[];
static const  char KEY_ISO[];
static const  char KEY_SUPPORTED_ISO_VALUES[];
static const  char KEY_SUPPORTED_IPP[];
static const  char KEY_IPP[];
static const  char KEY_MAN_EXPOSURE[];
static const  char KEY_METERING_MODE[];
static const  char KEY_PADDED_WIDTH[];
static const  char KEY_PADDED_HEIGHT[];
static const char  KEY_EXP_BRACKETING_RANGE[];
static const char  KEY_TEMP_BRACKETING[];
static const char  KEY_TEMP_BRACKETING_RANGE_POS[];
static const char  KEY_TEMP_BRACKETING_RANGE_NEG[];
static const char  KEY_SHUTTER_ENABLE[];
static const char  KEY_TOUCH_FOCUS_POS[];
static const char  KEY_MEASUREMENT_ENABLE[];
static const char  KEY_INITIAL_VALUES[];
static const char  KEY_GBCE[];
static const char  KEY_GLBCE[];
static const char  KEY_MINFRAMERATE[];
static const char  KEY_MAXFRAMERATE[];

static const char  KEY_CURRENT_ISO[];

//TI extensions for zoom
static const char ZOOM_SUPPORTED[];
static const char ZOOM_UNSUPPORTED[];

//TI extensions for camera capabilies
static const char INITIAL_VALUES_TRUE[];
static const char INITIAL_VALUES_FALSE[];

//TI extensions for enabling/disabling measurements
static const char MEASUREMENT_ENABLE[];
static const char MEASUREMENT_DISABLE[];

//  TI extensions to add values for ManualConvergence and AutoConvergence mode
static const char KEY_AUTOCONVERGENCE[];
static const char KEY_AUTOCONVERGENCE_MODE[];
static const char KEY_MANUALCONVERGENCE_VALUES[];


//TI extensions for enabling/disabling GLBCE
static const char GLBCE_ENABLE[];
static const char GLBCE_DISABLE[];

//TI extensions for enabling/disabling GBCE
static const char GBCE_ENABLE[];
static const char GBCE_DISABLE[];

// TI extensions to add Min frame rate Values
static const char VIDEO_MINFRAMERATE_5[];
static const char VIDEO_MINFRAMERATE_10[];
static const char VIDEO_MINFRAMERATE_15[];
static const char VIDEO_MINFRAMERATE_20[];
static const char VIDEO_MINFRAMERATE_24[];
static const char VIDEO_MINFRAMERATE_25[];
static const char VIDEO_MINFRAMERATE_30[];
static const char VIDEO_MINFRAMERATE_33[];

//  TI extensions for Manual Gain and Manual Exposure
static const char KEY_MANUAL_EXPOSURE_LEFT[];
static const char KEY_MANUAL_EXPOSURE_RIGHT[];
static const char KEY_MANUAL_EXPOSURE_MODES[];
static const char KEY_MANUAL_GAIN_EV_RIGHT[];
static const char KEY_MANUAL_GAIN_EV_LEFT[];
static const char KEY_MANUAL_GAIN_ISO_RIGHT[];
static const char KEY_MANUAL_GAIN_ISO_LEFT[];
static const char KEY_MANUAL_GAIN_MODES[];

//TI extensions for setting EXIF tags
static const char KEY_EXIF_MODEL[];
static const char KEY_EXIF_MAKE[];

//TI extensions for additional GPS data
static const char  KEY_GPS_ALTITUDE_REF[];
static const char  KEY_GPS_MAPDATUM[];
static const char  KEY_GPS_VERSION[];
static const char  KEY_GPS_DATESTAMP[];

//TI extensions for enabling/disabling shutter sound
static const char SHUTTER_ENABLE[];
static const char SHUTTER_DISABLE[];

//TI extensions for Temporal bracketing
static const char BRACKET_ENABLE[];
static const char BRACKET_DISABLE[];

//TI extensions to Image post-processing
static const char IPP_LDCNSF[];
static const char IPP_LDC[];
static const char IPP_NSF[];
static const char IPP_NONE[];

//TI extensions to camera mode
static const char HIGH_PERFORMANCE_MODE[];
static const char HIGH_QUALITY_MODE[];
static const char VIDEO_MODE[];
static const char HIGH_QUALITY_NONZSL_MODE[];


// TI extensions to standard android pixel formats
static const char PIXEL_FORMAT_RAW[];
static const char PIXEL_FORMAT_JPS[];
static const char PIXEL_FORMAT_MPO[];
static const char PIXEL_FORMAT_RAW_JPEG[];
static const char PIXEL_FORMAT_RAW_MPO[];

// TI extensions to standard android scene mode settings
static const  char SCENE_MODE_SPORT[];
static const  char SCENE_MODE_CLOSEUP[];
static const  char SCENE_MODE_AQUA[];
static const  char SCENE_MODE_SNOWBEACH[];
static const  char SCENE_MODE_MOOD[];
static const  char SCENE_MODE_NIGHT_INDOOR[];
static const  char SCENE_MODE_DOCUMENT[];
static const  char SCENE_MODE_BARCODE[];
static const  char SCENE_MODE_VIDEO_SUPER_NIGHT[];
static const  char SCENE_MODE_VIDEO_CINE[];
static const  char SCENE_MODE_VIDEO_OLD_FILM[];

// TI extensions for face detecition
static const  char FACE_DETECTION_ENABLE[];
static const  char FACE_DETECTION_DISABLE[];

// TI extensions to standard android white balance settings.
static const  char WHITE_BALANCE_TUNGSTEN[];
static const  char WHITE_BALANCE_HORIZON[];
static const  char WHITE_BALANCE_SUNSET[];
static const  char WHITE_BALANCE_FACE[];

// TI extensions to add exposure preset modes to android api
static const  char EXPOSURE_MODE_OFF[];
static const  char EXPOSURE_MODE_AUTO[];
static const  char EXPOSURE_MODE_NIGHT[];
static const  char EXPOSURE_MODE_BACKLIGHT[];
static const  char EXPOSURE_MODE_SPOTLIGHT[];
static const  char EXPOSURE_MODE_SPORTS[];
static const  char EXPOSURE_MODE_SNOW[];
static const  char EXPOSURE_MODE_BEACH[];
static const  char EXPOSURE_MODE_APERTURE[];
static const  char EXPOSURE_MODE_SMALL_APERTURE[];
static const  char EXPOSURE_MODE_FACE[];

// TI extensions to standard android focus presets.
static const  char FOCUS_MODE_PORTRAIT[];
static const  char FOCUS_MODE_EXTENDED[];
static const  char FOCUS_MODE_CAF[];
static const  char FOCUS_MODE_TOUCH[];
static const char  FOCUS_MODE_FACE[];

// TI extensions to add iso values
static const char ISO_MODE_AUTO[];
static const char ISO_MODE_100[];
static const char ISO_MODE_200[];
static const char ISO_MODE_400[];
static const char ISO_MODE_800[];
static const char ISO_MODE_1000[];
static const char ISO_MODE_1200[];
static const char ISO_MODE_1600[];

//  TI extensions to add  values for effect settings.
static const char EFFECT_NATURAL[];
static const char EFFECT_VIVID[];
static const char EFFECT_COLOR_SWAP[];
static const char EFFECT_BLACKWHITE[];

static const char KEY_S3D2D_PREVIEW[];
static const char KEY_S3D2D_PREVIEW_MODE[];

//  TI extensions to add values for AutoConvergence settings.
static const char AUTOCONVERGENCE_MODE_DISABLE[];
static const char AUTOCONVERGENCE_MODE_FRAME[];
static const char AUTOCONVERGENCE_MODE_CENTER[];
static const char AUTOCONVERGENCE_MODE_FFT[];
static const char AUTOCONVERGENCE_MODE_MANUAL[];

// Motorola specific - begin

// TI extensions to add Motorola test command inerface (TCMD)

static const char KEY_TESTPATTERN1_COLORBARS[];

static const char KEY_TESTPATTERN1_ENMANUALEXPOSURE[];
static const char KEY_DEBUGATTRIB_EXPOSURETIME[];
static const char KEY_DEBUGATTRIB_EXPOSUREGAIN[];

static const char KEY_TESTPATTERN1_TARGETEDEXPOSURE[];
static const char KEY_DEBUGATTRIB_TARGETEXPVALUE[];

static const char KEY_DEBUGATTRIB_CALIBRATIONSTATUS[];

static const char KEY_DEBUGATTRIB_ENLENSPOSGETSET[];
static const char KEY_DEBUGATTRIB_LENSPOSITION[];
static const char KEY_DEBUGATTRIB_AFSHARPNESSSCORE[];

static const char KEY_DEBUGATTRIB_MIPIFRAMECOUNT[];
static const char KEY_DEBUGATTRIB_MIPIECCERRORS[];
static const char KEY_DEBUGATTRIB_MIPICRCERRORS[];
static const char KEY_DEBUGATTRIB_MIPIRESET[];

// enable/disable values
static const char TESTPATTERN1_ENABLE[];
static const char TESTPATTERN1_DISABLE[];

static const char KEY_MOT_LEDFLASH[];
static const char KEY_MOT_LEDTORCH[];

static const char KEY_MANUAL_EXPOSURE_TIME_MS[];

// Current mot-hdr mode.
// Example value: "on" or MOT_HDR_MODE_XXX constants. Read/write.
static const char KEY_MOT_HDR_MODE[];
// Supported mot-hdr modes.
// Example value: "on,off". Read only.
static const char KEY_SUPPORTED_MOT_HDR_MODES[];

// Values for mot-hdr mode settings.
// Mot HDR will be off.
static const char MOT_HDR_MODE_OFF[];
// Mot HDR will be on
static const char MOT_HDR_MODE_ON[];

// Red, Green, Blue Tint Effects
static const char EFFECT_RED_TINT[];
static const char EFFECT_GREEN_TINT[];
static const char EFFECT_BLUE_TINT[];

// Motorola specific - end

//TI extensions for flash mode settings
static const char FLASH_MODE_FILL_IN[];

//TI values for camera direction
static const char FACING_FRONT[];
static const char FACING_BACK[];

};

};

#endif
