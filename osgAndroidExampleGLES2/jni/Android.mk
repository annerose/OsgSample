LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := osgNativeLib
### Main Install dir
OSG_ANDROID_DIR	:= G:/osg/OpenSceneGraph-3.2.1-rc3/OpenSceneGraph/androidbin

NDK_ROOT_PATH := d:/android/android-ndk-r9d
STL_LIBS := -L$(NDK_ROOT_PATH)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a  -lgnustl_static 

LIBDIR 			:= $(OSG_ANDROID_DIR)/obj/local/armeabi

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_ARM_NEON 	:= true
	LIBDIR 			:= $(OSG_ANDROID_DIR)/obj/local/armeabi-v7a
endif

### Add all source file names to be included in lib separated by a whitespace

LOCAL_C_INCLUDES:= $(OSG_ANDROID_DIR)/include
LOCAL_CFLAGS    := -Werror -fno-short-enums
LOCAL_CPPFLAGS  := -DOSG_LIBRARY_STATIC 

LOCAL_LDLIBS    := -llog -lGLESv2 -lz
LOCAL_SRC_FILES := osgNativeLib.cpp OsgMainApp.cpp OsgAndroidNotifyHandler.cpp

LOCAL_LDFLAGS   := -L $(LIBDIR)    \
-losgdb_3dc  \
 -losgdb_ac  \
 -losgdb_bmp  \
 -losgdb_bsp  \
 -losgdb_bvh  \
 -losgdb_cfg  \
 -losgdb_curl  \
 -losgdb_dds  \
 -losgdb_deprecated_osg  \
 -losgdb_deprecated_osganimation  \
 -losgdb_deprecated_osgfx  \
 -losgdb_deprecated_osgparticle  \
 -losgdb_deprecated_osgshadow  \
 -losgdb_deprecated_osgsim  \
 -losgdb_deprecated_osgterrain  \
 -losgdb_deprecated_osgtext  \
 -losgdb_deprecated_osgviewer  \
 -losgdb_deprecated_osgvolume  \
 -losgdb_deprecated_osgwidget  \
 -losgdb_dot  \
 -losgdb_dw  \
 -losgdb_dxf  \
 -losgdb_freetype  \
 -losgdb_gdal  \
 -losgdb_gif  \
 -losgdb_glsl  \
 -losgdb_hdr  \
 -losgdb_ive  \
 -losgdb_jpeg  \
 -losgdb_ktx  \
 -losgdb_logo  \
 -losgdb_lwo  \
 -losgdb_lws  \
 -losgdb_md2  \
 -losgdb_mdl  \
 -losgdb_normals  \
 -losgdb_obj  \
 -losgdb_ogr  \
 -losgdb_openflight  \
 -losgdb_osc  \
 -losgdb_osg  \
 -losgdb_osga  \
 -losgdb_osgshadow  \
 -losgdb_osgterrain  \
 -losgdb_osgtgz  \
 -losgdb_osgviewer  \
 -losgdb_p3d  \
 -losgdb_pic  \
 -losgdb_ply  \
 -losgdb_png  \
 -losgdb_pnm  \
 -losgdb_pov  \
 -losgdb_pvr  \
 -losgdb_revisions  \
 -losgdb_rgb  \
 -losgdb_rot  \
 -losgdb_scale  \
 -losgdb_serializers_osg  \
 -losgdb_serializers_osganimation  \
 -losgdb_serializers_osgfx  \
 -losgdb_serializers_osgga  \
 -losgdb_serializers_osgmanipulator  \
 -losgdb_serializers_osgparticle  \
 -losgdb_serializers_osgshadow  \
 -losgdb_serializers_osgsim  \
 -losgdb_serializers_osgterrain  \
 -losgdb_serializers_osgtext  \
 -losgdb_serializers_osgviewer  \
 -losgdb_serializers_osgvolume  \
 -losgdb_shp  \
 -losgdb_stl  \
 -losgdb_tga  \
 -losgdb_tgz  \
 -losgdb_tiff  \
 -losgdb_trans  \
 -losgdb_trk  \
 -losgdb_txf  \
 -losgdb_txp  \
 -losgdb_vtf  \
 -losgdb_x  \
 -losgViewer  \
  -losgWidget  \
 -losgVolume  \
 -losgTerrain  \
  -losgText  \
  -losgShadow  \
 -losgSim  \
  -losgParticle  \
   -losgManipulator  \
    -losgGA  \
 -losgFX  \
 -losgDB \
  -losgPresentation  \
 -losgAnimation  \
 -losgUtil  \
 -losg \
 -lpng  \
 -ltiff  \
 -lcurl  \
 -lft2  \
 -lgdal  \
 -lgif  \
 -ljpeg  \
 -lzlib  \
 -lOpenThreads  \
 $(STL_LIBS)






#-losgdb_dds \
#-losgdb_openflight \
#-losgdb_tga \
#-losgdb_rgb \
#-losgdb_osgterrain \
#-losgdb_osg \
#-losgdb_ive \
#-losgdb_deprecated_osgviewer \
#-losgdb_deprecated_osgvolume \
#-losgdb_deprecated_osgtext \
#-losgdb_deprecated_osgterrain \
#-losgdb_deprecated_osgsim \
#-losgdb_deprecated_osgshadow \
#-losgdb_deprecated_osgparticle \
#-losgdb_deprecated_osgfx \
#-losgdb_deprecated_osganimation \
#-losgdb_deprecated_osg \
#-losgdb_serializers_osgvolume \
#-losgdb_serializers_osgtext \
#-losgdb_serializers_osgterrain \
#-losgdb_serializers_osgsim \
#-losgdb_serializers_osgshadow \
#-losgdb_serializers_osgparticle \
#-losgdb_serializers_osgmanipulator \
#-losgdb_serializers_osgfx \
#-losgdb_serializers_osganimation \
#-losgdb_serializers_osg \
#-losgViewer \
#-losgVolume \
#-losgTerrain \
#-losgText \
#-losgShadow \
#-losgSim \
#-losgParticle \
#-losgManipulator \
#-losgGA \
#-losgFX \
#-losgDB \
#-losgAnimation \
#-losgUtil \
#-losg \
#-lOpenThreads \
#$(STL_LIBS)

include $(BUILD_SHARED_LIBRARY)
