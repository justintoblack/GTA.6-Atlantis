#pragma once
#ifndef _ACTIONMAP_H
#define _ACTIONMAP_H

#include<PxPhysicsAPI.h>
#include "../Render/Camera.h"
#include "../Utils/Mathf.h"

extern Snippets::Camera* sCamera;
class ActionMap
{
private:
	//bool _qKeyDown;
	bool isMKeyDown=false;
	int lastX; int lastY;
	bool needToPass = false;
public:
	ActionMap();
	void(*EditKeyEvent)();
	virtual void InputAction();
};

class CharacterActionMap :public ActionMap
{
private:
	physx::PxController* m_controller;
	Snippets::Camera* m_camera;
	physx::PxVec2 arrowKey;
	float moveSpeed;
	bool isSpaceKeyDown;
	bool isLeftButtonDown;
public:
	void(*SpaceKeyEvent)();
	void(*ShiftKeyEvent)(bool isPress);

	void (*LeftButtonDownEvent)();
	void (*LeftButtonEvent)();


	void InputAction();
	void SetActionMap(physx::PxController* newController, Snippets::Camera* camera, float speed);
	physx::PxVec2 GetArrowKeyValue();
};

class VehicleActionMap : public ActionMap
{
private:
	physx::PxVehicleDrive4W* m_controller;
public:
	void (*release)();
	void (*WKeyEvent)();
	void (*SKeyEvent)();
	void (*AKeyEvent)();
	void (*DKeyEvent)();
	void (*EKeyEvent)();
	void (*ReleaseWKeyEvent)();
	void (*ReleaseEKeyEvent)();



	void InputAction();
	void SetActionMap(physx::PxVehicleDrive4W* newController);
};

class EditActionMap :public ActionMap
{
private:
	physx::PxVec2 arrowKey;
public:
	void InputAction();
	physx::PxVec2 GetArrowKeyValue();
};

#endif // !_ACTIONMAP_H
