#ifndef __CURVEDATA_H__
#define __CURVEDATA_H__

#include <list>
#include <string>
#include <vector>

#include "SexyAppFramework/Buffer.h"


namespace Sexy
{

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct PathPoint
{
	float x,y;
	unsigned char mPriority;
	bool mInTunnel;

	PathPoint() : mInTunnel(false), mPriority(0) { }
	PathPoint(float tx, float ty) : x(tx), y(ty), mInTunnel(false), mPriority(0) { }
};
typedef std::vector<PathPoint> PathPointList;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CurveData
{
public:
	PathPointList mPointList;
	int mEditType;
	int mVersion;
	Buffer mEditData;
	std::string mErrorString;

	bool Fail(const std::string &theString);

public:
	CurveData();
	bool Save(const std::string &theFilePath, bool smallVersion = false);
	bool Load(const std::string &theFilePath);

	void Clear();
};

}

#endif