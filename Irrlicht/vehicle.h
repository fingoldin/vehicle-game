#ifndef __VEHICLE_H_INCLUDED__
#define __VEHICLE_H_INCLUDED__

#include <irrlicht/irrlicht.h>

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
		
		irr::video::SMaterial mat = this->rNode->getMaterial(0);
		mat.AmbientColor = irr::video::SColor(255, 40, 40, 40);
		mat.DiffuseColor = irr::video::SColor(255, 40, 40, 200);
		mat.SpecularColor = irr::video::SColor(255, 150, 150, 150);
		mat.Shininess = 128.0f;
		
		printf("%d, %d, %d\n", this->rNode->getMaterial(0).AmbientColor.getRed(), this->rNode->getMaterial(0).AmbientColor.getGreen(), this->rNode->getMaterial(0).AmbientColor.getBlue());
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
