#include "TheCreator.h"

void TheCreator::Init(PxPhysics* physics, PxScene* gScene)
{
	m_gPhysics = physics;
	m_gScene = gScene;
}

void TheCreator::CreateAnchorBall(PxTransform pos, PxMaterial* gMaterial,float radius)
{
	PxRigidStatic* ballBody = PxCreateStatic(*m_gPhysics, pos, PxSphereGeometry(radius), 
		*gMaterial,PxTransform(PxIdentity));
	m_gScene->addActor(*ballBody);
}

void TheCreator::CreateAnchorBox(PxTransform pos, PxMaterial* gMaterial, float halfX,float halfY,float halfZ)
{
	PxRigidStatic* ballBody = PxCreateStatic(*m_gPhysics, pos, PxBoxGeometry(halfX,halfY,halfZ),
		*gMaterial, PxTransform(PxIdentity));
	m_gScene->addActor(*ballBody);
}

void TheCreator::CreateBanister(PxVec3 pos,PxVec3 dir,  PxMaterial* gMaterial,float halfHeight, 
	float halfLength, float density, float force, float torque,BreakType type)
{
	//单位化
	dir = dir.getNormalized();

	//旋转轴
	PxVec3 rotate = PxVec3(0, 0, 1).cross(dir).getNormalized();

	//旋转角度(弧度)
	float angle = Mathf::Angle(PxVec3(0, 0, 1), dir);

	//旋转四元数
	//PxQuat q = PxQuat(Mathf::DegToRad( angle), rotate);
	
	PxRigidDynamic* link1 = PxCreateDynamic(*m_gPhysics,PxTransform(PxVec3(0,halfHeight,0)+pos), PxBoxGeometry(0.5f, halfHeight, 0.5f), *gMaterial, density);
	PxFixedJoint* j1 = PxFixedJointCreate(*m_gPhysics, NULL, PxTransform(pos), link1, PxTransform(0, -halfHeight, 0));

	PxRigidDynamic* link2 = PxCreateDynamic(*m_gPhysics, 
		PxTransform(PxVec3(0,halfHeight*2,0)+pos+dir*halfLength, PxQuat(angle, rotate)), PxBoxGeometry(0.5f, 0.5f, halfLength), *gMaterial, density);
	PxFixedJoint* j2 = PxFixedJointCreate(*m_gPhysics, link1, PxTransform(0, halfHeight, 0), link2,
		PxTransform(PxVec3(0, 0, -halfLength), PxQuat(angle, -rotate)));

	PxRigidDynamic* link3 = PxCreateDynamic(*m_gPhysics, PxTransform(pos+dir*halfLength*2+PxVec3(0,halfHeight,0)),
		PxBoxGeometry(0.5f, halfHeight, 0.5f), *gMaterial, density);
	PxFixedJoint* j3 = PxFixedJointCreate(*m_gPhysics, link2, 
		PxTransform(PxVec3( 0, 0, halfLength), PxQuat(angle, -rotate)), link3, PxTransform(0, halfHeight, 0));

	PxFixedJoint* j4 = PxFixedJointCreate(*m_gPhysics, link3, PxTransform(0, -halfHeight, 0), NULL,
		PxTransform(pos+dir*halfLength*2));

	if (type == AllInPiece)
	{
		j2->setBreakForce(force, torque);
	}
	j1->setBreakForce(force, torque);
	j3->setBreakForce(force, torque);
	j4->setBreakForce(force, torque);

	m_gScene->addActor(*link1);
	m_gScene->addActor(*link2);
	m_gScene->addActor(*link3);
}

//加入随机
void TheCreator::CreateBanisters(physx::PxVec3 pos, physx:: PxVec3 dir, PxMaterial* gMaterial,
	float separate, physx::PxU32 num, float halfHeight, float halfLength, float density, float force, 
	float torque)
{
	dir = dir.getNormalized();
	CreateBanister(pos,dir,gMaterial,halfHeight, halfLength, density, force, torque,SomeInPiece);

	srand((unsigned)time(NULL));
	for (PxU32 i = 1; i < num; i++)
	{
		BreakType type=SomeInPiece;

		//1/2的几率全碎掉
		int r= rand() %2 ;
		if (r == 0)
		{
			type = AllInPiece;
		}
		pos += dir*halfLength*2+dir*separate;
		CreateBanister(pos,dir, gMaterial, halfHeight, halfLength, density, force, torque,type);
	}
}

void TheCreator::CreatePole(PxVec3 pos, PxMaterial* gMaterial,float radius,
	float halfHeight,float density,float force,float torque)
{
	PxRigidDynamic* link = PxCreateDynamic(*m_gPhysics,
		PxTransform(pos+PxVec3(0,halfHeight,0)), 
		PxBoxGeometry(radius, halfHeight,radius),
		*gMaterial, density);

	PxFixedJoint* j = PxFixedJointCreate(*m_gPhysics, NULL, PxTransform(pos), link, 
		PxTransform(PxVec3(0, -halfHeight, 0)));

	j->setBreakForce(force, torque);

	m_gScene->addActor(*link);
}

void TheCreator::CreatePoles(PxVec3 pos, PxVec3 dir, float separate, PxU32 num, PxMaterial* gMaterial, float halfXZ, float halfHeight, float density, float force, float torque)
{
	dir = dir.getNormalized();
	CreatePole(pos, gMaterial, halfXZ, halfHeight, density, force, torque);

	for (int i = 1; i < num; i++)
	{
		pos += dir * separate;
		CreatePole(pos, gMaterial, halfXZ, halfHeight, density, force, torque);
	}
}

