#include"MissionManager.h"

MissionManager::MissionManager()
{
}

MissionManager::~MissionManager()
{
}

void MissionManager::AddMission(PxVec3 StartPos, PxVec3 EndPos, MissionType _Type, std::string Description)
{
	MissionList.push_back(new Mission(StartPos, EndPos, Description, TotalNum, _Type));
	TotalNum += 1;
}

void MissionManager::DeleteMission(int _ID)
{

	for (vector<Mission*>::iterator iter = MissionList.begin(); iter != MissionList.end(); iter++)
	{
		if ((*iter)->ID == _ID)
		{
			MissionList.erase(iter);
			return;
		}
	}
}

void MissionManager::UpdateAllMission(double deltatime)
{
	for (vector<Mission*>::iterator iter = MissionList.begin(); iter != MissionList.end(); iter++)
	{
		(*iter)->UpdateTrigger();
		(*iter)->UpdateTimer(deltatime);
	}
}