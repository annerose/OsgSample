#include "OsgMainApp.hpp"
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexEnv>

OsgMainApp::OsgMainApp(){

    _lodScale = 1.0f;
    _lodLastScale = 1.0f;
    _prevFrame = 0;

    _initialized = false;
    _clean_scene = false;

    _xTrans = 0;
    _yTrans = 0;

    _xLastTrans = 0;
    _yLastTrans = 0;

    _xAngle = 0;
    _yAngle = 0;

    _xFactor = 1.0;

    _trans = NULL;

}
OsgMainApp::~OsgMainApp(){

}


//创建一个四边形节点
osg::ref_ptr<osg::Node> OsgMainApp::createNode() {

    //创建一个叶节点对象
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    //创建一个几何体对象
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    //添加顶点数据 注意顶点的添加顺序是逆时针
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    //添加数据
    v->push_back(osg::Vec3(0, 0, 0));
    v->push_back(osg::Vec3(1, 0, 0));
    v->push_back(osg::Vec3(1, 0, 1));
    v->push_back(osg::Vec3(0, 0, 1));

    //设置顶点数据
    geom->setVertexArray(v.get());

    //创建纹理订点数据
    osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
    //添加纹理坐标
    vt->push_back(osg::Vec2(0, 0));
    vt->push_back(osg::Vec2(1, 0));
    vt->push_back(osg::Vec2(1, 1));
    vt->push_back(osg::Vec2(0, 1));

    //设置纹理坐标
    geom->setTexCoordArray(0, vt.get());

    //创建颜色数组
    osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
    //添加数据
    vc->push_back(osg::Vec4(1, 0, 0, 1));
    vc->push_back(osg::Vec4(0, 1, 0, 1));
    vc->push_back(osg::Vec4(0, 0, 1, 1));
    vc->push_back(osg::Vec4(1, 1, 0, 1));

    //设置颜色数组
    geom->setColorArray(vc.get());
    //设置颜色的绑定方式为单个顶点
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    //创建法线数组
    osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
    //添加法线
    nc->push_back(osg::Vec3(0, -1, 0));
    //设置法线
    geom->setNormalArray(nc.get());
    //设置法绑定为全部顶点
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    //添加图元
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    //添加到叶子节点
    geode->addDrawable(geom.get());
    osg::StateSet* stateset = new osg::StateSet;
    osg::Texture2D* texture = new osg::Texture2D();
    texture->setDataVariance(osg::Object::DYNAMIC);
    texture->setImage(osgDB::readImageFile("/sdcard/Present3D/sunset.bmp"));
    osg::Uniform* samUniform = new osg::Uniform(osg::Uniform::SAMPLER_2D, "sam");
    samUniform->set(0);//设置纹理单元
    stateset->addUniform(samUniform);
    stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

    geode->setStateSet(stateset);

    return geode.get();
}



/***********************************************************************
* Function :	读取文件全部内容
*
* Parameter: const TCHAR * pFileName	文件名
* Parameter: int & iLen					文件长度
*
* Returns:   BYTE*	file buffer
***********************************************************************/
char* GetFileBuffer(const char* pFileName, long& iLen)
{

    FILE* fs =  fopen(pFileName,  "rb" );


	if (fs == NULL)
	{
		LOGE ("file open error %s", pFileName);
		return NULL;
	}

	fseek(fs, 0, SEEK_END);
	long lFileSize = ftell(fs);

	fseek(fs, 0, SEEK_SET);

	char* pBuffer = new char[lFileSize + 1];

	pBuffer[lFileSize] = 0;
	long iReadNum = fread (pBuffer, sizeof(char), lFileSize, fs);

	fclose(fs);

	if(iReadNum != lFileSize)
	{
		LOGE ("fread %s error " , pFileName );
		delete [] pBuffer;
		pBuffer = NULL;
	}
	else
	{
		// OK
		iLen = lFileSize;
	}

	return pBuffer;

}



void OsgMainApp::loadModels(){

	//LOGI ("OsgMainApp::loadModels 1111");
    if(_vModelsToLoad.size()==0) return;
	//LOGI ("OsgMainApp::loadModels 22222");
    osg::notify(osg::ALWAYS)<<"There are "<<_vModelsToLoad.size()<<" models to load"<<std::endl;

	LOGI ("OsgMainApp::loadModels _vModelsToLoad.size() %d", _vModelsToLoad.size());
    Model newModel;
    for(unsigned int i=0; i<_vModelsToLoad.size(); i++){
        newModel = _vModelsToLoad[i];
        osg::notify(osg::ALWAYS)<<"Loading: "<<newModel.filename<<std::endl;

        osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(newModel.filename);
        if (loadedModel == 0) {
            osg::notify(osg::ALWAYS)<<"Model not loaded"<<std::endl;
        } else {
            osg::notify(osg::ALWAYS)<<"Model loaded"<<std::endl;
            _vModels.push_back(newModel);

            loadedModel->setName(newModel.name);



          	osg::Uniform* samUniform = new osg::Uniform(osg::Uniform::SAMPLER_2D,
            			"sam");
            	samUniform->set(0);
           	loadedModel->getOrCreateStateSet()->addUniform(samUniform);

//
//        	osg::Uniform* pUnModelViewProjection = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "gl_ModelViewProjectionMatrix");
//        	loadedModel->getOrCreateStateSet()->addUniform(pUnModelViewProjection);
//
//        	osg::Matrixf  mModelView;
//        	pUnModelViewProjection->get(mModelView);
//
//        	osg::Matrixf  mModelViewInverse;
//        	mModelViewInverse.invert(mModelView);
//
//          	osg::Uniform* pUnModelViewProjectionInverse = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "uModelViewMatrixInverse");
//          	loadedModel->getOrCreateStateSet()->addUniform(pUnModelViewProjectionInverse);

           	long iFileLen = 0;
           	char* pszVertexShader = GetFileBuffer("/sdcard/osg/a.vs", iFileLen);
           	char* pszFragmentShader = GetFileBuffer("/sdcard/osg/a.fs", iFileLen);

            osg::Shader * vshader = new osg::Shader(osg::Shader::VERTEX, pszVertexShader );
            osg::Shader * fshader = new osg::Shader(osg::Shader::FRAGMENT, pszFragmentShader );

            osg::Program * prog = new osg::Program;
            prog->addShader ( vshader );
            prog->addShader ( fshader );

            loadedModel->getOrCreateStateSet()->setAttribute ( prog );

            delete [] pszVertexShader;
            delete [] pszFragmentShader;



//            osg::ref_ptr<osg::Image> image = osgDB::readImageFile("/sdcard/Present3D/sunset.bmp");
//
////        	osg::ref_ptr<osg::Image> image = new osg::Image;
////        	image->allocateImage(256, 256, 4, GL_RGBA, GL_UNSIGNED_BYTE);
////        	image->setInternalTextureFormat(GL_RGBA);
//
//            if (image.get())
//                {
//
////            	unsigned char * pData = image->data();
////            			memset(pData, 255, 256 * 256 * 4);
//
//                    osg::ref_ptr<osg::Texture2D> texture=new osg::Texture2D();
//                    texture->setDataVariance(osg::Object::DYNAMIC);
//                    texture->setImage(image.get());
//
//                    //设置自动生成纹理坐标
//                    osg::ref_ptr<osg::TexGen> texgen=new osg::TexGen();
//                    texgen->setMode(osg::TexGen::SPHERE_MAP);
//
//                    //设置纹理环境，模式为BLEND
//                    osg::ref_ptr<osg::TexEnv> texenv=new osg::TexEnv;
//                    texenv->setMode(osg::TexEnv::ADD);
//                    texenv->setColor(osg::Vec4(0.6,0.6,0.6,0.0));
//
//                    //启动单元一自动生成纹理坐标，并使用纹理
//                    osg::ref_ptr<osg::StateSet> state=new osg::StateSet;
//                    state->setTextureAttributeAndModes(1,texture.get(),osg::StateAttribute::ON);
//                    state->setTextureAttributeAndModes(1,texgen.get(),osg::StateAttribute::ON);
//                    state->setTextureAttribute(1,texenv.get());
//
//                    loadedModel->setStateSet(state.get());
//
//                    LOGE("osgDB::readImageFile ok");
//                }
//            else
//            {
//            	LOGE("osgDB::readImageFile err bmp 3");
//            }



//            osg::ref_ptr<osg::Node> theNode = createNode();

            _trans =new osg::MatrixTransform;
            _trans->setMatrix(osg::Matrix::rotate(0,1,0,0));
           _trans->addChild(loadedModel);
           // _trans->addChild(theNode);





            // 取模型的包围盒
            const osg::BoundingSphere bs = _trans->getBound();
            float radius = bs.radius();

            _xFactor = radius / 100;







           // _root->addChild(loadedModel);
	           _root->addChild(_trans);



        }
    }

    osgViewer::Viewer::Windows windows;
    _viewer->getWindows(windows);
    for(osgViewer::Viewer::Windows::iterator itr = windows.begin();itr != windows.end();++itr)
    {
      (*itr)->getState()->setUseModelViewAndProjectionUniforms(true);
      (*itr)->getState()->setUseVertexAttributeAliasing(true);
    }

    _viewer->setSceneData(NULL);
    _viewer->setSceneData(_root.get());
    _manipulator->getNode();
    _viewer->home();

    _viewer->getDatabasePager()->clear();
    _viewer->getDatabasePager()->registerPagedLODs(_root.get());
    _viewer->getDatabasePager()->setUpThreads(3, 1);
    _viewer->getDatabasePager()->setTargetMaximumNumberOfPageLOD(2);
    _viewer->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true, true);

    _vModelsToLoad.clear();

}

void OsgMainApp::deleteModels(){
    if(_vModelsToDelete.size()==0) return;

    osg::notify(osg::ALWAYS)<<"There are "<<_vModelsToDelete.size()<<" models to delete"<<std::endl;

    Model modelToDelete;
    for(unsigned int i=0; i<_vModelsToDelete.size(); i++){
        modelToDelete = _vModelsToDelete[i];
        osg::notify(osg::ALWAYS)<<"Deleting: "<<modelToDelete.name<<std::endl;

        for(unsigned int j=_root->getNumChildren(); j>0; j--){
            osg::ref_ptr<osg::Node> children = _root->getChild(j-1);
            if(children->getName() == modelToDelete.name){
                _root->removeChild(children);
            }
        }

    }

    _vModelsToDelete.clear();
    osg::notify(osg::ALWAYS)<<"finished"<<std::endl;
}
//Initialization function
void OsgMainApp::initOsgWindow(int x,int y,int width,int height){

    __android_log_write(ANDROID_LOG_INFO, "OSGANDROID",
            "Initializing geometry");

    //Pending
    _notifyHandler = new OsgAndroidNotifyHandler();
    _notifyHandler->setTag("Osg Viewer");
    osg::setNotifyHandler(_notifyHandler);

    osg::notify(osg::ALWAYS)<<"Testing"<<std::endl;

    _viewer = new osgViewer::Viewer();
    _viewer->setUpViewerAsEmbeddedInWindow(x, y, width, height);
    _viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);

    _root = new osg::Group();

    _viewer->realize();

    _viewer->addEventHandler(new osgViewer::StatsHandler);
    _viewer->addEventHandler(new osgGA::StateSetManipulator(_viewer->getCamera()->getOrCreateStateSet()));
    _viewer->addEventHandler(new osgViewer::ThreadingHandler);
    _viewer->addEventHandler(new osgViewer::LODScaleHandler);

    _manipulator = new osgGA::KeySwitchMatrixManipulator;

    _manipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
    _manipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
    _manipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
    _manipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );
    _manipulator->addMatrixManipulator( '5', "Orbit", new osgGA::OrbitManipulator() );
    _manipulator->addMatrixManipulator( '6', "FirstPerson", new osgGA::FirstPersonManipulator() );
    _manipulator->addMatrixManipulator( '7', "Spherical", new osgGA::SphericalManipulator() );

    _viewer->setCameraManipulator( _manipulator.get() );

    _viewer->getViewerStats()->collectStats("scene", true);

    _initialized = true;

}
//Draw
void OsgMainApp::draw(){
    //Every load o remove has to be done before any drawing

	//LOGI ("OsgMainApp::draw 1");
    loadModels();
    deleteModels();



    if(_trans)
    {
    	static double angle = 0;
        _trans->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(angle),1,0,0) *
         osg::Matrix::scale(_lodScale, _lodScale, _lodScale) *
         osg::Matrix::translate(_xTrans * _xFactor, 0,  -_yTrans * _xFactor)
        );



        angle += 0.3;

		LOGI ("angle = %f, _xFactor = %f", angle, _xFactor);
    }
//
//    // 创建矩阵，指定到视点的距离。
//    osg::Matrix trans;
//    trans.makeTranslate( 0., 0., -12. );



    _viewer->frame();
}
//Events
void OsgMainApp::mouseButtonPressEvent(float x,float y,int button){
   // _viewer->getEventQueue()->mouseButtonPress(x, y, button);
    LOGI ("mouseButtonPressEvent %f,%f, %d", x, y, button);

    if(button == 1)
    {


    }
    else if(button == 2 )
    {
    	_lodLastScale = _lodScale;
    }
    else if(button == 3)
    {

    	_xLastTrans = _xTrans;
    	_yLastTrans = _yTrans;
    }

}
void OsgMainApp::mouseButtonReleaseEvent(float x,float y,int button){
    //_viewer->getEventQueue()->mouseButtonRelease(x, y, button);
    LOGI ("mouseButtonReleaseEvent %f,%f, %d", x, y, button);


}
void OsgMainApp::mouseMoveEvent(float x,float y,int button){
   // _viewer->getEventQueue()->mouseMotion(x, y);
    LOGI ("mouseMoveEvent %f,%f, %d ", x, y, button);
	if(button == 1)
	{
		// 旋转

	}
	else if(button == 2)
	{
		// 缩放
		_lodScale = x * _lodLastScale;
	}
	else if(button == 3)
	{
		// 平移
		_xTrans = _xLastTrans + x;
		_yTrans = _yLastTrans + y;

	}

   // _viewer->getEventQueue()->mouseButtonPress(x, y, 2);
}
void OsgMainApp::keyboardDown(int key){
    _viewer->getEventQueue()->keyPress(key);
    LOGI ("keyboardDown   %d",   key);
}
void OsgMainApp::keyboardUp(int key){
    _viewer->getEventQueue()->keyRelease(key);
    LOGI ("keyboardUp   %d",   key);

    if(key = ' ')
    {
    	_lodScale = 1.0f;
    	_lodLastScale = 1.0f;

    	_xTrans = 0;
    	_yTrans = 0;

    	_xLastTrans = 0;
    	_yLastTrans = 0;

        _xAngle = 0;
        _yAngle = 0;
    }
}
//Loading and unloading
void OsgMainApp::loadObject(std::string filePath){
    Model newModel;
    newModel.filename = filePath;
    newModel.name = filePath;

    int num = 0;
    for(unsigned int i=0;i<_vModels.size();i++){
        if(_vModels[i].name==newModel.name)
            return;
    }

    _vModelsToLoad.push_back(newModel);

}
void OsgMainApp::loadObject(std::string name,std::string filePath){

    Model newModel;
    newModel.filename = filePath;
    newModel.name = name;

    for(unsigned int i=0;i<_vModels.size();i++){
        if(_vModels[i].name==newModel.name){
            osg::notify(osg::ALWAYS)<<"Name already used"<<std::endl;
            return;
        }
    }

    _vModelsToLoad.push_back(newModel);
}
void OsgMainApp::unLoadObject(int number){
    if(_vModels.size() <= number){
        osg::notify(osg::FATAL)<<"Index number error"<<std::endl;
        return;
    }

    Model modelToDelete = _vModels[number];
    _vModels.erase(_vModels.begin()+number);
    _vModelsToDelete.push_back(modelToDelete);
}
void OsgMainApp::clearScene(){
    _vModelsToDelete = _vModels;
    _vModels.clear();
}
//Other Functions
int OsgMainApp::getNumberObjects(){
    return _vModels.size();
}
std::string OsgMainApp::getObjectName(int number){
    return _vModels[number].name;
}
void OsgMainApp::setClearColor(osg::Vec4f color){
    osg::notify(osg::ALWAYS)<<"Setting Clear Color"<<std::endl;
    _viewer->getCamera()->setClearColor(color);
}
osg::Vec4f OsgMainApp::getClearColor(){
    osg::notify(osg::ALWAYS)<<"Getting Clear Color"<<std::endl;
    return _viewer->getCamera()->getClearColor();
}
