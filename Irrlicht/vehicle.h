#ifndef __VEHICLE_H_INCLUDED__
#define __VEHICLE_H_INCLUDED__

#include <irrlicht/irrlicht.h>

#include <cstdio>

class Vehicle : public irr::IReferenceCounted
{
public:
	
	Vehicle(EffectHandler * effhand, irr::IrrlichtDevice * dev, irr::s32 shad) : ehandler(effhand), device(dev), smgr(dev->getSceneManager()), shader(shad)
	{
		this->device->grab();
		
		this->mesh = this->smgr->getMesh("media/models/FinalBaseMesh.obj")->getMesh(0);
		if(this->mesh) {
			//this->rNode = this->smgr->addMeshSceneNode(this->mesh);
		}
		else {
			std::printf("Could not load mesh in Vehicle class constructor!\n");
			return;
		}
		
		//this->rNode->setScale(irr::core::vector3df(0.05, 0.05, 0.05));
		//this->rNode->addShadowVolumeSceneNode();
		
		this->rNode = this->smgr->addCubeSceneNode(2.0f);
		//this->mesh = this->rNode->getMesh();
		
		this->rNode->setMaterialType(irr::video::EMT_SOLID);//(irr::video::E_MATERIAL_TYPE)this->shader);
		this->rNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		this->rNode->setMaterialFlag(irr::video::EMF_GOURAUD_SHADING, false);
		this->rNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
		this->rNode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
		this->rNode->setPosition(irr::core::vector3df(0.0f, 2.0f, 0.0f));
		
		this->rNode->getMaterial(0).ColorMaterial = irr::video::ECM_NONE;
		this->rNode->getMaterial(0).SpecularColor = irr::video::SColor(0, 0, 0, 0);
		this->rNode->getMaterial(0).AmbientColor = irr::video::SColor(255, 50, 50, 50);
		this->rNode->getMaterial(0).DiffuseColor = irr::video::SColor(255, 50, 50, 250);
		this->rNode->getMaterial(0).ZBuffer = 0;
		this->rNode->getMaterial(0).ZWriteEnable = 0;
		
		this->smgr->getMeshManipulator()->recalculateNormals(this->rNode->getMesh(), true, true);
		
		//this->ehandler->addShadowToNode(this->rNode, EFT_4PCF, ESM_CAST);
		
		//this->ehandler->excludeNodeFromLightingCalculations(this->rNode);
		//this->rNode->addShadowVolumeSceneNode();
	}
	
	irr::scene::IMeshSceneNode * getRNode(void) const { return this->rNode; }
	
	~Vehicle(void)
	{
		if(this->device)
			this->device->drop();
	}
	
private:
	
	EffectHandler * ehandler;
	irr::IrrlichtDevice * device;
	irr::scene::ISceneManager * smgr;
	irr::scene::IMesh * mesh;
	irr::scene::IMeshSceneNode * rNode;
	
	irr::s32 shader;
	
};

#endif
