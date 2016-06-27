#ifndef __VEHICLE_H_INCLUDED__
#define __VEHICLE_H_INCLUDED__

#include <Irrlicht/irrlicht.h>

#include <cstdio>

class Vehicle : public irr::IReferenceCounted
{
public:
	
	Vehicle(irr::IrrlichtDevice * dev, irr::s32 shad) : device(dev), smgr(dev->getSceneManager()), shader(shad)
	{
		this->device->grab();
		
		this->mesh = this->smgr->getMesh("media/models/FinalBaseMesh.obj");
		if(this->mesh) {
			this->rNode = this->smgr->addAnimatedMeshSceneNode(this->mesh);
		}
		else {
			std::printf("Could not load mesh in Vehicle class constructor!\n");
			return;
		}
		
		this->rNode->setMaterialType((irr::video::E_MATERIAL_TYPE)this->shader);
		this->rNode->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	}
	
	~Vehicle(void)
	{
		this->device->drop();
	}
	
private:
	
	irr::IrrlichtDevice * device;
	irr::scene::ISceneManager * smgr;
	irr::scene::IAnimatedMesh * mesh;
	irr::scene::IAnimatedMeshSceneNode * rNode;
	
	irr::s32 shader;
	
};

#endif
