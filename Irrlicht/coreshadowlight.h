#ifndef __CORE_SHADOW_H_INCLUDED__
#define __CORE_SHADOW_H_INCLUDED__

#include <irrlicht/irrlicht.h>

class ShadowLightNode : public irr::scene::ILightSceneNode
{
public:

	ShadowLightNode(EffectHandler * effhand, irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id, 
			const irr::core::vector3df& position = irr::core::vector3df(0, 0, 0)) : ILightSceneNode(parent, mgr, id, position),
			ehandler(effhand)
	{
		
	}
	
	virtual void OnAnimate(irr::u32 timeMs)
	{
		if(this->getLightType() == irr::video::ELT_POINT)
		{
			for(int i = 0; i < 6; i++)
				this->ehandler->getShadowLight(this->shadowLightIndices[i]).setPosition(this->getPosition());
			
			this->ehandler->getShadowLight(this->shadowLightIndices[0]).setTarget
		irr::scene::ISceneNode::OnAnimate();
	}

private:

};

#endif
