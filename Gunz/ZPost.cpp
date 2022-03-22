#include "stdafx.h"

#include "ZPost.h"
#include "MBlobArray.h"
#include "MMatchTransDataType.h"
#include "MMatchGlobal.h"
#include "ZGame.h"
#include "ZMyCharacter.h"
#include "ZGameClient.h"
#include "ZApplication.h"
#include "ZConfiguration.h"
#include "MMD5.h"
#include <Psapi.h>
#pragma comment (lib, "psapi.lib")

void ZPostUserOption()
{
	unsigned long nOptionFlags = 0;

	if (Z_ETC_REJECT_WHISPER)
		nOptionFlags |= MBITFLAG_USEROPTION_REJECT_WHISPER;
	if (Z_ETC_REJECT_INVITE)
		nOptionFlags |= MBITFLAG_USEROPTION_REJECT_INVITE;

	ZPOSTCMD1(MC_MATCH_USER_OPTION, MCmdParamUInt(nOptionFlags));
}


//For module list.
std::string GetSystemPathToDiskPath(char* szPath)
{
	char* pszPath = new char[512];

	strcpy(pszPath,szPath);

	std::string szNewPath = "";
	CHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';

	if (GetLogicalDriveStrings(512-1, szTemp)) 
	{
		CHAR szName[MAX_PATH];
		CHAR szDrive[3] = TEXT(" :");
		BOOL bFound = FALSE;
		CHAR* p = szTemp;

		do 
		{
			*szDrive = *p;

			if (QueryDosDevice(szDrive, szName, 512))
			{
				UINT uNameLen = strlen(szName);

				if (uNameLen < MAX_PATH) 
				{
					bFound = _tcsnicmp(pszPath, szName, 
						uNameLen) == 0;

					if (bFound) 
					{
						CHAR szTempFile[MAX_PATH];
						_snprintf_s(szTempFile,
							MAX_PATH,
							"%s%s",
							szDrive,
							pszPath+uNameLen);
						strncpy_s(pszPath, MAX_PATH+1, szTempFile, strlen(szTempFile));
					}
				}
			}
			// Go to the next NULL character.
			while (*p++);
		} while (!bFound && *p); // end of string
	}
	
	szNewPath = pszPath;
	delete [] pszPath;
	return szNewPath;
}

//Custom: See modules loaded into GunZ.
void ZPostMatchLog()
{
	PBYTE pb = NULL;
	MEMORY_BASIC_INFORMATION mbi;
	char ModuleFileName[256] = {0};

	std::stringstream os(std::stringstream::in | std::stringstream::out | std::stringstream::binary);

	while (VirtualQuery(pb, &mbi, sizeof(mbi)) == sizeof(mbi)) {
		int nLen;
		char szModName[MAX_PATH + 1];

		if (mbi.State == MEM_FREE)
			mbi.AllocationBase = mbi.BaseAddress;

		if    ((mbi.AllocationBase != mbi.BaseAddress) || (mbi.AllocationBase == NULL) || (mbi.Type != MEM_IMAGE)) {
			nLen = 0;
		} else {
			nLen = GetMappedFileName(GetCurrentProcess(), mbi.AllocationBase, szModName, sizeof(szModName));
		}
		if (nLen > 0) {
			unsigned int len = 0;
			// \Device\HarddiskVolume2\Users\JLG\AppData\Local\Temp\HLe6098.tmp 
			//("Device name is %s\n"), szFileName); 

			std::string modulepath = GetSystemPathToDiskPath(szModName);
			unsigned char szMD5Value[ 16 ] = {0, };
			MMD5 *pMD5 = new MMD5;

			std::string md5str = "";

			if (pMD5->md5_file((char*)modulepath.c_str(), szMD5Value) == 0) // true
			{
				//store in pkt
				md5str = pMD5->ToString(szMD5Value);
			}
			//store in pkt

			//Bytes to read
			len = modulepath.length();
			os.write((const char*)&len, sizeof(unsigned int));
			//	r3dOutToLog("%i\n", len);
			//Module name
			os.write((const char*)&modulepath[0], modulepath.length());
			//	r3dOutToLog("%s\n", modulepath.c_str());
			//Bytes to read
			len = md5str.length();
			os.write((const char*)&len, sizeof(unsigned int));
			//	r3dOutToLog("%i\n", len);
			//MD5 if avail
			os.write((const char*)&md5str[0], md5str.length());
			//	r3dOutToLog("%s\n", md5str.c_str());

			SAFE_DELETE(pMD5);
		}
		pb += mbi.RegionSize;
	}

	BYTE* dst = new BYTE[4096];
	DWORD dstSize = 4096;
	if(compress(dst, &dstSize, (BYTE*)&os.str()[0], os.str().length()) != Z_OK)
		return;

	int LenMod = dstSize;
	int tmpSize = os.str().length();

	void* pBlob = MMakeBlobArray(dstSize, 1);
	if(!pBlob) 
	{
		return;
	}

	memcpy(MGetBlobArrayElement(pBlob, 0), dst, dstSize);
	ZPOSTCMD1(MC_DEBUGINFO, MCommandParameterBlob(pBlob, MGetBlobArraySize(pBlob)))
	MEraseBlobArray(pBlob);

	os.clear();
	SAFE_DELETE_ARRAY(dst);

}


// 해커 리버싱 방해용도의 커맨드ID 반환 함수 : 자세한 주석은 헤더 참조
//#pragma optimize("", off)

int UncloakCmdId(int cloakedCmdId, int cloakFactor)
{
	return cloakedCmdId - cloakFactor;
}

//#pragma optimize("", on)