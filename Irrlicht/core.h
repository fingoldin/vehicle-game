#ifndef __CORE_H_INCLUDED__
#define __CORE_H_INCLUDED__


#include <Irrlicht/irrlicht.h>

#include <cstdlib>
#include <cstdio>
#include <string>
#include <ctime>

#include "corereciever.h"
#include "coreshadercallback.h"
#include "corezshadercallback.h"

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
	
	void add_shadowMap(const char * name);
	
	void generate_shadowMaps(void);
	
	irr::scene::IAnimatedMesh * getMeshIrrlicht(const irr::io::path& filename);
	
	Vehicle * vehicle;
	
	irr::core::stringc win_name;
	
	irr::IrrlichtDevice * device;
	irr::video::IVideoDriver * driver;
	irr::scene::ISceneManager * scene_manager;
	irr::gui::IGUIEnvironment * gui_env;
	CoreEventReciever * event_reciever;
	
	irr::scene::ICameraSceneNode * camera;
	
	irr::s32 shader;
	irr::s32 zshader;
	irr::io::path vshader_file;
	irr::io::path fshader_file;
	irr::io::path vzshader_file;
	irr::io::path fzshader_file;
	
	irr::core::array<irr::video::ITexture*> shadowMaps;
};

void Core::begin(const char * winName)
{
	this->init_device(winName);
	
	this->load_shaders();
	
	this->load_lights();
	
	this->load_cameras();
	
	this->vehicle = new Vehicle(this->device, this->zshader);
	
	this->scene_manager->setShadowColor(irr::video::SColor(150,0,0,0));
}

void Core::end(void)
{
	if(this->event_reciever)
		this->event_reciever->drop();
	if(this->vehicle)
		this->vehicle->drop();
	if(this->device)
		this->device->drop();
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
	
	if(this->event_reciever->keyDown(irr::KEY_ESCAPE)) {
		this->device->closeDevice();
	}
}

void Core::render(void)
{
	this->driver->beginScene(true, true, irr::video::SColor(255, 5, 5, 250));
	
	this->generate_shadowMaps();
	
	this->scene_manager->drawAll();
	
	this->gui_env->drawAll();
	
	this->driver->endScene();
}

void Core::generate_shadowMaps(void)
{
	irr::core::array<irr::scene::ILightSceneNode*> lights;
	this->scene_manager->getSceneNodesFromType(irr::scene::ESNT_LIGHT, lights);
	
	for(int i = 0; i < this->shadowMaps.size(), i < MAX_SHADOW_MAPS, i < lights.size(); i++)
	{
		driver->setRenderTarget(this->shadowMaps[i]
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
	
	this->driver = device->getVideoDriver();
	this->scene_manager = device->getSceneManager();
	this->gui_env = device->getGUIEnvironment();
	
	this->event_reciever = new CoreEventReciever(this->device);
	
	this->device->setEventReceiver(this->event_reciever);
}

void Core::load_shaders(void)
{
	this->vshader_file = "shader.vs";
	this->fshader_file = "shader.frag";
	this->vzshader_file = "zbuffer.vs";
	this->fzshader_file = "zbuffer.frag";
	
	if (!this->driver->queryFeature(irr::video::EVDF_PIXEL_SHADER_1_1) &&
            !this->driver->queryFeature(irr::video::EVDF_ARB_FRAGMENT_PROGRAM_1))
        {
        	this->device->getLogger()->log("WARNING: Pixel shaders disabled "\
           	 "because of missing driver/hardware support.");
        	fshader_file = "";
		fzshader_file = "";
        }

        if (!this->driver->queryFeature(irr::video::EVDF_VERTEX_SHADER_1_1) &&
            !this->driver->queryFeature(irr::video::EVDF_ARB_VERTEX_PROGRAM_1))
    	{
        	this->device->getLogger()->log("WARNING: Vertex shaders disabled "\
           	 "because of missing driver/hardware support.");
        	vshader_file = "";
		vzshader_file = "";
        }
	
	if (!this->driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
	{
		this->device->getLogger()->log("WARNING: Render to texture disabled "\
		 "because of missing driver/harware support.");
		fzshader_file = "";
		vzshader_file = "";
	}
	
	irr::video::IGPUProgrammingServices * gpu = this->driver->getGPUProgrammingServices();
	
	this->shader = 0;
	this->zshader = 0;
	
	CoreShaderCallback * scallback = new CoreShaderCallback(this->device);
	CoreZShaderCallback * zcallback = new CoreZShaderCallback(this->device);
	
	this->shader = gpu->addHighLevelShaderMaterialFromFiles(
			vshader_file, "main", irr::video::EVST_VS_1_1,
			fshader_file, "main", irr::video::EPST_PS_1_1,
			scallback, irr::video::EMT_SOLID);
	
	this->zshader = gpu->addHighLevelShaderMaterialFromFiles(
			vzshader_file, "main", irr::video::EVST_VS_1_1,
			fzshader_file, "main", irr::video::EPST_PS_1_1,
			zcallback, irr::video::EMT_SOLID);
	
	if(this->shader < 0 || this->zshader < 0) {
		std::printf("Could not add shaders!\n");
		this->device->closeDevice();
	}
	
	scallback->drop();
	zcallback->drop();
}

void Core::load_lights(void)
{
	irr::scene::ILightSceneNode * light0 = this->scene_manager->addLightSceneNode(0, irr::core::vector3df(150, 150, 150), irr::video::SColorf(1.0f, 1.0f, 1.0f), 1000.0f);
	if(light0) {
		light0->getLightData().AmbientColor = irr::video::SColorf(0.4f, 0.4f, 0.4f);
		light0->getLightData().SpecularColor = irr::video::SColorf(1.0f, 1.0f, 1.0f);
		light0->setLightType(irr::video::ELT_DIRECTIONAL);
		
		this->add_shadowMap("light0");
		
		/*irr::scene::ISceneNodeAnimator * lightAnimator = this->scene_manager->createFlyCircleAnimator(irr::core::vector3df(0, 10, 0), 50, 0.001);
		if(lightAnimator) {
			light0->addAnimator(lightAnimator);
			lightAnimator->drop();
		}*/
	}
	
	//this->scene_manager->setAmbientLight(irr::video::SColor(200, 20, 20, 20));
}

void Core::load_cameras(void)
{
	this->camera = this->scene_manager->addCameraSceneNodeFPS(0, 100.0f, 0.02f);
	if(camera) {
		this->camera->setFarValue(50000.0f);
		this->camera->setNearValue(0.5f);
		this->camera->setFOV(1.0f);
	}
	
	this->device->getCursorControl()->setVisible(false);
}

irr::video::ITexture * Core::add_shadowMap(const char * name)
{
	irr::video::ITexture * RTT = this->driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE), 
										name, irr::video::ECF_G32R32F);
	this->shadowMaps.push_back(RTT);
	
	return RTT;
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
