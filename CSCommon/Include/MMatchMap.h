#ifndef _MMATCHMAP_H
#define _MMATCHMAP_H

#include "MMatchGlobal.h"

enum MMATCH_MAP 
{
	MMATCH_MAP_MANSION			= 0,
	MMATCH_MAP_PRISON			= 1,
	MMATCH_MAP_STATION			= 2,
	MMATCH_MAP_PRISON_II		= 3,
	MMATCH_MAP_BATTLE_ARENA		= 4,
	MMATCH_MAP_TOWN				= 5,
	MMATCH_MAP_DUNGEON			= 6,
	MMATCH_MAP_RUIN				= 7,
	MMATCH_MAP_ISLAND			= 8,
	MMATCH_MAP_GARDEN			= 9,
	MMATCH_MAP_CASTLE			= 10,
	MMATCH_MAP_FACTORY			= 11,
	MMATCH_MAP_PORT				= 12,
	MMATCH_MAP_LOST_SHRINE		= 13,
	MMATCH_MAP_STAIRWAY			= 14,
	MMATCH_MAP_SNOWTOWN			= 15,
	MMATCH_MAP_HALL				= 16,
	MMATCH_MAP_CATACOMB			= 17,
	MMATCH_MAP_JAIL				= 18,
	MMATCH_MAP_SHOWERROOM		= 19,
	MMATCH_MAP_HIGH_HAVEN		= 20,
	MMATCH_MAP_CITADEL			= 21,

	// 새맵 추가는 이 밑에 하세요.

	//	MMATCH_MAP_EVENT,					// 이벤트 전용맵
	MMATCH_MAP_RELAYMAP			= 22,

	//MMATCH_MAP_HALLOWEEN_TOWN	= 23,
	MMATCH_MAP_WEAPON_SHOP		= 23,
	MMATCH_MAP_MAX				= 512		// Custom: Raise the max maps to 512
};

/*
enum MMATCH_MAP 
{
	MMATCH_MAP_MANSION			= 0,
	MMATCH_MAP_RELAYMAP			= 1,
	MMATCH_MAP_PRISON			= 2,
	MMATCH_MAP_STATION			= 3,
	MMATCH_MAP_PRISON_II		= 4,
	MMATCH_MAP_BATTLE_ARENA		= 5,
	MMATCH_MAP_TOWN				= 6,
	MMATCH_MAP_DUNGEON			= 7,
	MMATCH_MAP_RUIN				= 8,
	MMATCH_MAP_ISLAND			= 9,
	MMATCH_MAP_GARDEN			= 10,
	MMATCH_MAP_CASTLE			= 11,
	MMATCH_MAP_FACTORY			= 12,
	MMATCH_MAP_PORT				= 13,
	MMATCH_MAP_LOST_SHRINE		= 14,
	MMATCH_MAP_STAIRWAY			= 15,
	MMATCH_MAP_SNOWTOWN			= 16,
	MMATCH_MAP_HALL				= 17,
	MMATCH_MAP_CATACOMB			= 18,
	MMATCH_MAP_JAIL				= 19,
	MMATCH_MAP_SHOWERROOM		= 20,
	MMATCH_MAP_HIGH_HAVEN		= 21,
	MMATCH_MAP_CITADEL			= 22,

	// 새맵 추가는 이 밑에 하세요.

	//	MMATCH_MAP_EVENT,					// 이벤트 전용맵

	MMATCH_MAP_HALLOWEEN_TOWN	= 23,
	MMATCH_MAP_WEAPON_SHOP		= 24,
	MMATCH_MAP_MAX				= 512		// Custom: Raise the max maps to 512
};
*/

/*
enum MMATCH_MAP 
{
	MMATCH_MAP_MANSION			= 0,
	MMATCH_MAP_PRISON			= 1,
	MMATCH_MAP_STATION			= 2,
	MMATCH_MAP_PRISON_II		= 3,
	MMATCH_MAP_BATTLE_ARENA		= 4,
	MMATCH_MAP_TOWN				= 5,
	MMATCH_MAP_DUNGEON			= 6,
	MMATCH_MAP_RUIN				= 7,
	MMATCH_MAP_ISLAND			= 8,
	MMATCH_MAP_GARDEN			= 9,
	MMATCH_MAP_CASTLE			= 10,
	MMATCH_MAP_FACTORY			= 11,
	MMATCH_MAP_PORT				= 12,
	MMATCH_MAP_LOST_SHRINE		= 13,
	MMATCH_MAP_STAIRWAY			= 14,
	MMATCH_MAP_SNOWTOWN			= 15,
	MMATCH_MAP_HALL				= 16,
	MMATCH_MAP_CATACOMB			= 17,
	MMATCH_MAP_JAIL				= 18,
	MMATCH_MAP_SHOWERROOM		= 19,
	MMATCH_MAP_HIGH_HAVEN		= 20,
	MMATCH_MAP_CITADEL			= 21,

	// 새맵 추가는 이 밑에 하세요.

	//	MMATCH_MAP_EVENT,					// 이벤트 전용맵
	MMATCH_MAP_RELAYMAP			= 22,

	MMATCH_MAP_HALLOWEEN_TOWN	= 23,
	MMATCH_MAP_WEAPON_SHOP		= 24,
	MMATCH_MAP_MAX				= 512		// Custom: Raise the max maps to 512
};
*/

#define MMATCH_MAP_COUNT	MMATCH_MAP_MAX			// 전체 맵 갯수

#define MMATCH_MAPNAME_RELAYMAP				"RelayMap"


class MMapDesc
{
private:
	const struct MapInfo
	{
		int			nMapID;							// map id
		char		szMapName[MAPNAME_LENGTH];		// 맵 이름
		char		szMapImageName[MAPNAME_LENGTH];	// 맵 이미지 이름
		char		szBannerName[MAPNAME_LENGTH];	// 베너 이름
		float		fExpRatio;						// 경험치 습득률
		int			nMaxPlayers;					// 최대 인원
		bool		bOnlyDuelMap;					// 듀얼맵 전용
		bool		bIsCTFMap;
	};


	// data
	MapInfo	m_MapVectors[MMATCH_MAP_COUNT];
	MMapDesc();
public:
	~MMapDesc() { }
	
	static MMapDesc* GetInstance();

	bool Initialize(const char* szFileName);
	bool Initialize(MZFileSystem* pfs, const char* szFileName);
	bool MIsCorrectMap(const int nMapID);
	

 
	bool IsMapOnlyDuel( const int nMapID);
	bool IsCTFMap( const int nMapID);
	int GetMapID( const int nMapID);
	const char* GetMapName(const int nMapID);
	const char* GetMapImageName(const char* szMapName);
	const char* GetBannerName(const char* szMapName);
	float GetExpRatio( const int nMapID); 
	int GetMaxPlayers( const int nMapID);

	int GetMapCount(){ return MMATCH_MAP_COUNT; }
};


inline MMapDesc* MGetMapDescMgr() 
{ 
	return MMapDesc::GetInstance();
}



#endif