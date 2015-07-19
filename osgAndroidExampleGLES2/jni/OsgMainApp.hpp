/*
 * OsgMainApp.hpp
 *
 *  Created on: 29/05/2011
 *      Author: Jorge Izquierdo Ciges
 */

#ifndef OSGMAINAPP_HPP_
#define OSGMAINAPP_HPP_

//Android log
#include <android/log.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

//Standard libraries
#include <string>

//osg
#include <osg/GL>
#include <osg/GLExtensions>
#include <osg/Depth>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Node>
#include <osg/Notify>
//osgText
#include <osgText/Text>
//osgDB
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
//osg_viewer
#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osgViewer/ViewerEventHandlers>
//osgGA
#include <osgGA/GUIEventAdapter>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/SphericalManipulator>
//Self headers
#include "OsgAndroidNotifyHandler.hpp"

#include <osg/MatrixTransform>

//Static plugins Macro
USE_OSGPLUGIN(ive)
USE_OSGPLUGIN(osg)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(terrain)
USE_OSGPLUGIN(rgb)
USE_OSGPLUGIN(OpenFlight)
USE_OSGPLUGIN(dds)
USE_OSGPLUGIN(bmp)
USE_OSGPLUGIN(logo)
USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(png)
//Static DOTOSG
USE_DOTOSGWRAPPER_LIBRARY(osg)
USE_DOTOSGWRAPPER_LIBRARY(osgFX)
USE_DOTOSGWRAPPER_LIBRARY(osgParticle)
USE_DOTOSGWRAPPER_LIBRARY(osgTerrain)
USE_DOTOSGWRAPPER_LIBRARY(osgText)
USE_DOTOSGWRAPPER_LIBRARY(osgViewer)
USE_DOTOSGWRAPPER_LIBRARY(osgVolume)
//Static serializer
USE_SERIALIZER_WRAPPER_LIBRARY(osg)
USE_SERIALIZER_WRAPPER_LIBRARY(osgAnimation)
USE_SERIALIZER_WRAPPER_LIBRARY(osgFX)
USE_SERIALIZER_WRAPPER_LIBRARY(osgManipulator)
USE_SERIALIZER_WRAPPER_LIBRARY(osgParticle)
USE_SERIALIZER_WRAPPER_LIBRARY(osgTerrain)
USE_SERIALIZER_WRAPPER_LIBRARY(osgText)
USE_SERIALIZER_WRAPPER_LIBRARY(osgVolume)

#define  LOG_TAG    "osgNativeLib"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

struct Model{
    std::string filename;
    std::string name;
};

static const char gVertexShader[] =
    "varying vec4 color;                                                    \n"
    "const vec3 lightPos      =vec3(0.0, 0.0, 10.0);                        \n"
    "const vec4 cessnaColor   =vec4(0.8, 0.8, 0.8, 1.0);                    \n"
    "const vec4 lightAmbient  =vec4(0.1, 0.1, 0.1, 1.0);                    \n"
    "const vec4 lightDiffuse  =vec4(0.4, 0.4, 0.4, 1.0);                    \n"
    "const vec4 lightSpecular =vec4(0.8, 0.8, 0.8, 1.0);                    \n"
	"varying vec2 v_texCoord;                                         \n"
	"varying vec4 kOutBaseTCoord; \n"
//	"uniform vec4 uModelViewMatrixInverse;                                 \n"


    "void DirectionalLight(in vec3 normal,                                  \n"
    "                      in vec3 ecPos,                                   \n"
    "                      inout vec4 ambient,                              \n"
    "                      inout vec4 diffuse,                              \n"
    "                      inout vec4 specular)                             \n"
    "{                                                                      \n"
    "     float nDotVP;                                                     \n"
    "     vec3 L = normalize(gl_ModelViewMatrix*vec4(lightPos, 0.0)).xyz;   \n"
    "     nDotVP = max(0.0, dot(normal, L));                                \n"
    "                                                                       \n"
    "     if (nDotVP > 0.0) {                                               \n"
    "       vec3 E = normalize(-ecPos);                                     \n"
    "       vec3 R = normalize(reflect( L, normal ));                       \n"
    "       specular = pow(max(dot(R, E), 0.0), 16.0) * lightSpecular;      \n"
    "     }                                                                 \n"
    "     ambient  = lightAmbient;                                          \n"
    "     diffuse  = lightDiffuse * nDotVP;                                 \n"
    "}                                                                      \n"

	"	void fswap(inout float a, inout float b)                                 \n"
	"	{                                 \n"
	"		float thetemp = a;                                 \n"
	"		a = b;                                 \n"
	"		b = thetemp;                                 \n"
	"	}                                 \n"

	"bool invert_4x4 (in mat4 matSrc, out mat4 matDst)					\n"
	"{                                                                      \n"
	"	int indxc[4], indxr[4], ipiv[4];                            \n"
	"	int i,j,k,l,ll;                            \n"
	"	int icol = 0;                            \n"
	"	int irow = 0;                            \n"
	"	float temp, pivinv, dum, big;                            \n"

	"	matDst = matSrc                    \n"
	"	for (j=0; j<4; j++)  \n"
	"   {										 \n"
	"		ipiv[j]=0;										 \n"
	"   }                            										 \n"
	"	 for(i=0;i<4;i++)                            \n"
	"	    {                            \n"
	"	       big=0.0;                            \n"
	"	       for (j=0; j<4; j++)                            \n"
	"	          if (ipiv[j] != 1)                            \n"
	"	           {  for (k=0; k<4; k++)                            \n"
	"	             {                            \n"
	"	                if (ipiv[k] == 0)                            \n"
	"	                {                            \n"
	"	                   if (abs(matDst[j][k]) >= big)                            \n"
	"	                   {                            \n"
	"	                      big = abs(matDst[j][k]);                            \n"
	"	                      irow=j;                            \n"
	"	                      icol=k;                            \n"
	"	                   }                            \n"
	"	                }                            \n"
	"	                else if (ipiv[k] > 1)                            \n"
	"	                   return false;                            \n"
	"	             }    \n"
	"			}                         \n"
	"	       ++(ipiv[icol]);                            \n"
	"	       if (irow != icol)                            \n"
	"	          for (l=0; l<4; l++)  {fswap (matDst[irow][l], matDst[icol][l]); }                           \n"


	"	       indxr[i]=irow;                            \n"
	"	       indxc[i]=icol;                            \n"
	"	       if (matDst[icol][icol] == 0)                            \n"
	"	          return false;                            \n"

	"	       pivinv = 1.0/matDst[icol][icol];                            \n"
	"	       matDst[icol][icol] = 1;                            \n"
	"	       for (l=0; l<4; l++) matDst[icol][l] *= pivinv;                            \n"
	"	       for (ll=0; ll<4; ll++)                            \n"
	"	          if (ll != icol)                            \n"
	"	          {                            \n"
	"	             dum= matDst[ll][icol];                            \n"
	"	             matDst[ll][icol] = 0;                            \n"
	" 	             for (l=0; l<4; l++) matDst[ll][l] -= matDst[icol][l]*dum;                            \n"
	"	          }                            \n"
	"	    }                            \n"
	"	    for (int lx=4; lx>0; --lx)                            \n"
	"	    {                            \n"
	"	       if (indxr[lx-1] != indxc[lx-1])                            \n"
	"	          for (k=0; k<4; k++) "
	"				{  														\n"
	"					fswap(matDst[k][indxr[lx-1] ], matDst[k][indxc[lx-1] ]); "
	"				}       \n"
	"	    }                            \n"
	"	                            \n"
	"	    return true;                            \n"
 	"}                                                                      \n"
    "void main() {                                                          \n"

    "    vec4 ambiCol = vec4(0.0);                                          \n"
    "    vec4 diffCol = vec4(0.0);                                          \n"
    "    vec4 specCol = vec4(0.0);                                          \n"
    "    gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;          \n"
    "    vec3 normal   = normalize(gl_NormalMatrix * gl_Normal);            \n"
    "    vec4 ecPos    = gl_ModelViewMatrix * gl_Vertex;                    \n"
    "    DirectionalLight(normal, ecPos.xyz, ambiCol, diffCol, specCol);    \n"
    "    color = cessnaColor * (ambiCol + diffCol + specCol);               \n"
	"    v_texCoord   = gl_MultiTexCoord0.xy;                           \n"
//	"	 vec4 sPlane = vec4(1.0, 0.0, 0.0, 0.0);                         \n"
//	"	  vec4 tPlane = vec4(0.0, 1.0, 0.0, 0.0);                         \n"
//	"     vec4 rPlane = vec4(0.0, 0.0, 0.0, 0.0);                         \n"
//	"	  vec4 qPlane = vec4(0.0, 0.0, 0.0, 0.0);                         \n"
//	"	  kOutBaseTCoord.s = dot(vec4(gl_Vertex.xyz, 1.0), sPlane);                         \n"
//	"	  kOutBaseTCoord.t = dot(vec4(gl_Vertex.xyz, 1.0), tPlane);                         \n"



		"vec4 myEyeVertex = normalize(gl_ModelViewMatrix * gl_Vertex);                       \n"

//		"vec4 myEyeNormal =  vec4 (gl_Normal, 1)  *    inverse(gl_ModelViewMatrix) ;                       \n"
// 	"vec4 myEyeNormal =  vec4 (gl_Normal, 1)  *   gl_ModelViewMatrixInverse ;                       \n"

		"vec4 mModelViewMatrixInverse;														\n"
		"invert_4x4 (gl_ModelViewMatrix, mModelViewMatrixInverse);							\n"
		"vec4 myEyeNormal =  vec4 (gl_Normal, 1)  *  mModelViewMatrixInverse; \n"

		"vec4 reflectionVector = myEyeVertex - myEyeNormal * 2.0 * dot(myEyeVertex, myEyeNormal);                       \n"
		"reflectionVector.z += 1.0;                       \n"
		"float m = 1.0 / (2.0 * sqrt(dot(reflectionVector, reflectionVector)));                       \n"
		"kOutBaseTCoord.s = reflectionVector.x * m + 0.5;                       \n"
		"kOutBaseTCoord.t = reflectionVector.y * m + 0.5;                       \n"
    "}                                                                      \n";

static const char gFragmentShader[] =
    "precision mediump float;                  \n"
    "varying mediump vec4 color;               \n"
	"uniform sampler2D sam;                                 \n"

	"varying  mediump vec2 v_texCoord;                      \n"
	"varying mediump vec4 kOutBaseTCoord;  \n"
    "void main() {                             \n"
//    "  gl_FragColor = color;                   \n"
//	"    gl_FragColor = texture2D(sam, v_texCoord);         \n"
//		"gl_FragColor = clamp(color, 0.0, 1.0);                   \n"
	"	gl_FragColor = texture2D(sam, kOutBaseTCoord.st); \n"
    "}                                         \n";


class OsgMainApp{
private:
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::StateSet> _state;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> _manipulator;

    osg::ref_ptr<osg::MatrixTransform> _trans;



    float _xTrans;
    float _yTrans;

    float _xLastTrans;
    float _yLastTrans;


    float _xFactor;

    float _xAngle;
    float _yAngle;


    float _lodScale;
    float _lodLastScale;
    unsigned int _prevFrame;

    bool _initialized;
    bool _clean_scene;

    OsgAndroidNotifyHandler *_notifyHandler;

    std::vector<Model> _vModels;
    std::vector<Model> _vModelsToLoad;
    std::vector<Model> _vModelsToDelete;

    void loadModels();
    void deleteModels();

public:
    OsgMainApp();
    ~OsgMainApp();

    //Initialization function
    void initOsgWindow(int x,int y,int width,int height);
    //Draw
    void draw();
    //Events
    void mouseButtonPressEvent(float x,float y,int button);
    void mouseButtonReleaseEvent(float x,float y,int button);
    void mouseMoveEvent(float x,float y,int button);
    void keyboardDown(int key);
    void keyboardUp(int key);
    //Loading and unloading
    void loadObject(std::string filePath);
    void loadObject(std::string name,std::string filePath);
    void unLoadObject(int number);
    void clearScene();
    //Other functions
    int  getNumberObjects();
    std::string getObjectName(int nunmber);

    void setClearColor(osg::Vec4f color);
    osg::Vec4f getClearColor();

    osg::ref_ptr<osg::Node> createNode();
};


#endif /* OSGMAINAPP_HPP_ */
