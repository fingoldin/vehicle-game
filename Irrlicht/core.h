#ifndef __CORE_H_INCLUDED__
#define __CORE_H_INCLUDED__


#include <irrlicht/irrlicht.h>

#include <cstdlib>
#include <cstdio>
#include <string>
#include <ctime>

#include "xeffects/Source/XEffects.h"

#include "corereceiver.h"
#include "coreshadercallback.h"

// #include "coreshadowlight.h"

#include "config.h"

#include "vehicle.h"

class Core
{
public:
	
	void begin(const char * winName);
	
	void end(void);
	
	bool run(void);
	
	bool active(void);
	
	void yield(void);
	
	void update(void);
	
	void render(void);
	
	Core() { }
	~Core() { }
	
private:
	
	void build_keyboard(int x, int y, float s);
	
	void init_device(const char * winName);
	
	void load_shaders(void);
	
	void load_lights(void);
	
	void load_cameras(void);
	
	irr::scene::IAnimatedMesh * getMeshIrrlicht(const irr::io::path& filename);
	
	Vehicle * curr_vehicle;
	
	irr::core::stringc win_name;
	
	irr::IrrlichtDevice * device;
	irr::video::IVideoDriver * driver;
	irr::scene::ISceneManager * scene_manager;
	irr::gui::IGUIEnvironment * gui_env;
	CoreEventReceiver * event_receiver;
	
	EffectHandler * effect_handler;
	
	irr::scene::ICameraSceneNode * camera;
	
	irr::scene::ILightSceneNode * sun;
	
	irr::scene::ISceneNode * ground;
	
	irr::s32 shader;
	irr::io::path vshader_file;
	irr::io::path fshader_file;
};

void Core::begin(const char * winName)
{
	this->init_device(winName);
	
	this->load_shaders();
	
	this->load_lights();
	
	this->load_cameras();
	
	this->curr_vehicle = new Vehicle(this->effect_handler, this->device, this->shader);
	
	const irr::scene::IGeometryCreator * creator = this->scene_manager->getGeometryCreator();
	//this->ground = this->scene_manager->addMeshSceneNode(creator->createPlaneMesh(irr::core::dimension2d<irr::f32>(100.0f, 100.0f)));
	//this->ground->setPosition(irr::core::vector3df(0, -100, 0));
	
	//((irr::scene::IMeshSceneNode*)this->ground)->addShadowVolumeSceneNode();
	
	//this->effect_handler->addShadowToNode(this->ground, EFT_4PCF, ESM_RECEIVE);
}

void Core::end(void)
{
	if(this->effect_handler)
		delete this->effect_handler;
	if(this->event_receiver)
		this->event_receiver->drop();
	if(this->curr_vehicle)
		this->curr_vehicle->drop();
	if(this->device)
		this->device->drop();
	
	// this->driver->removeAllTextures();
	//printf("got here\n");
}

bool Core::run(void)
{
	return this->device->run();
}

bool Core::active(void)
{
	return this->device->isWindowActive();
}

void Core::yield(void)
{
	this->device->yield();
}

void Core::update(void)
{
	this->device->setWindowCaption(irr::core::stringw(this->win_name + "  FPS: " + irr::core::stringc(this->driver->getFPS())).c_str());
	
	// irr::core::stringw FPS = irr::core::stringw(this->driver->getFPS());
	// this->gui_env->addStaticText(FPS.c_str(), irr::core::rect<irr::s32>(10, 10, FPS.size() * 20, 30), true);
	
	/*irr::core::vector3df vnodepos = this->curr_vehicle->getRNode()->getPosition();
	
	if(this->event_receiver->keyDown(irr::KEY_KEY_W))
		this->curr_vehicle->getRNode()->setPosition(vnodepos + irr::core::vector3df(0.3f, 0.0f, 0.0f));
	if(this->event_receiver->keyDown(irr::KEY_KEY_S))
		this->curr_vehicle->getRNode()->setPosition(vnodepos + irr::core::vector3df(-0.3f, 0.0f, 0.0f));
	if(this->event_receiver->keyDown(irr::KEY_KEY_A))
		this->curr_vehicle->getRNode()->setPosition(vnodepos + irr::core::vector3df(0.0f, 0.0f, 0.3f));
	if(this->event_receiver->keyDown(irr::KEY_KEY_D))
		this->curr_vehicle->getRNode()->setPosition(vnodepos + irr::core::vector3df(0.0f, 0.0f, -0.3f));*/
	
	//this->effect_handler->getShadowLight(0).setPosition(-20.0f * (this->sun->getLightData().Direction.normalize()) + irr::core::vector3df(vnodepos.X, -6.0f, vnodepos.Z));
	//this->effect_handler->getShadowLight(0).setTarget(irr::core::vector3df(vnodepos.X, 0.0f, vnodepos.Z));
	
	//this->camera->setPosition(irr::core::vector3df(vnodepos.X + 2.0f, 4.0f, vnodepos.Z + 2.0f));
	//this->camera->setTarget(irr::core::vector3df(vnodepos.X, 0.0f, vnodepos.Z));
	
	//this->effect_handler->getShadowLight(0).getPosition());
	
	//this->ground->setPosition(irr::core::vector3df(vnodepos.X, 0.0f, vnodepos.Z));
}

void Core::render(void)
{
	this->driver->beginScene(true, true, irr::video::SColor(255, 5, 5, 250));
	
	//this->effect_handler->update();
	this->scene_manager->drawAll();
	
	this->gui_env->drawAll();
	
	this->driver->endScene();
}

void Core::init_device(const char * winName)
{
	irr::SIrrlichtCreationParameters params;
	params.AntiAlias = CORE_ANTI_ALIAS;
	params.DriverType  = irr::video::EDT_OPENGL;
	params.DeviceType = irr::EIDT_BEST;
	params.WindowSize = irr::core::dimension2d<irr::u32>(CORE_WINDOW_WIDTH, CORE_WINDOW_HEIGHT);
	params.Stencilbuffer = CORE_STENCIL_BUFFER;
	params.Fullscreen = CORE_FULLSCREEN;
	params.Vsync = CORE_VSYNC;
	
	this->device = irr::createDeviceEx(params);
	if(!device) {
		std::printf("Could not create device!\n");
		return;
	}
	
	this->device->setWindowCaption(irr::core::stringw(winName).c_str());
	this->win_name = winName;
	
	this->driver = this->device->getVideoDriver();
	this->scene_manager = this->device->getSceneManager();
	this->gui_env = this->device->getGUIEnvironment();
	
	this->event_receiver = new CoreEventReceiver(this->device);
	
	this->device->setEventReceiver(this->event_receiver);
	
	this->effect_handler = 0;
	
	//this->effect_handler = new EffectHandler(this->device, this->driver->getScreenSize(), false, false, true);
	
	//this->effect_handler->setClearColour(irr::video::SColor(255, 100, 100, 100));
	//this->effect_handler->setAmbientColor(irr::video::SColor(255, 10, 10, 10));
	
	this->scene_manager->setShadowColor(irr::video::SColor(200, 30, 30, 30));
}

void Core::load_shaders(void)
{
	this->vshader_file = "shader.vs";
	this->fshader_file = "shader.frag";
	
	if (!this->driver->queryFeature(irr::video::EVDF_PIXEL_SHADER_1_1) &&
            !this->driver->queryFeature(irr::video::EVDF_ARB_FRAGMENT_PROGRAM_1))
        {
        	this->device->getLogger()->log("WARNING: Pixel shaders disabled "\
           	 "because of missing driver/hardware support.");
        	fshader_file = "";
        }

        if (!this->driver->queryFeature(irr::video::EVDF_VERTEX_SHADER_1_1) &&
            !this->driver->queryFeature(irr::video::EVDF_ARB_VERTEX_PROGRAM_1))
    	{
        	this->device->getLogger()->log("WARNING: Vertex shaders disabled "\
           	 "because of missing driver/hardware support.");
        	vshader_file = "";
        }
	
	if (!this->driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
	{
		this->device->getLogger()->log("WARNING: Render to texture disabled "\
		 "because of missing driver/harware support.");
	}
	
	irr::video::IGPUProgrammingServices * gpu = this->driver->getGPUProgrammingServices();
	
	this->shader = 0;
	
	CoreShaderCallback * scallback = new CoreShaderCallback(this->device);
	
	this->shader = gpu->addHighLevelShaderMaterialFromFiles(
			vshader_file, "main", irr::video::EVST_VS_1_1,
			fshader_file, "main", irr::video::EPST_PS_1_1,
			scallback, irr::video::EMT_SOLID);
	
	if(this->shader < 0) {
		std::printf("Could not add shader!\n");
		this->device->closeDevice();
	}
	
	scallback->drop();
}

void Core::load_lights(void)
{
	this->sun = this->scene_manager->addLightSceneNode();
	if(this->sun) {
		irr::video::SLight lightData;
		this->sun->setLightType(irr::video::ELT_DIRECTIONAL);
		
		lightData.AmbientColor = irr::video::SColorf(0.4f, 0.4f, 0.4f);
		lightData.SpecularColor = irr::video::SColorf(0.0f, 0.0f, 0.0f);
		lightData.Direction = irr::core::vector3df(-1.0f, -1.0f, -1.0f);
		lightData.Attenuation = irr::core::vector3df(1.0f, 0.0f, 0.0f);
		lightData.Radius = 50.0f;
		lightData.CastShadows = true;
		
		this->sun->setLightData(lightData);
		this->sun->setName("sun");
		
		this->sun->setPosition(irr::core::vector3df(10, 10, 10));
	}
	
	/*this->effect_handler->addShadowLight(SShadowLight(2048, irr::core::vector3df(0.0f, 5.0f, 0.0f), 
								irr::core::vector3df(0.0f, 0.0f, 0.0f), 
								irr::video::SColorf(1.0f, 1.0f, 1.0f), 
							  10.0f, 23.0f, 20.0f, true));*/
}

void Core::load_cameras(void)
{	
	this->camera = this->scene_manager->addCameraSceneNodeFPS(0, 100, 0.02);
	if(this->camera) {
		this->camera->setFarValue(1000.0f);
		this->camera->setNearValue(0.1f);
		this->camera->setFOV(70.0f * irr::core::DEGTORAD);
		this->camera->setPosition(irr::core::vector3df(10.0f, 10.0f, 10.0f));
		this->camera->setTarget(irr::core::vector3df(0, 0, 0));
	}
	
	this->device->getCursorControl()->setVisible(false);
}

irr::scene::IAnimatedMesh * Core::getMeshIrrlicht(const irr::io::path& filename)
{
	irr::scene::IAnimatedMesh * mesh = this->scene_manager->getMesh(filename);
	if(!mesh) {
		std::printf("Could not load mesh with Irrlicht!");
	}
	
	return mesh;
}

#endif
