#include "Zuma_Prefix.pch"

#include "CurveData.h"
#include "Board.h"
#include "Bullet.h"
#include "CurveMgr.h"
#include "WayPoint.h"
#include "DataSync.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int gVersion = 3;
float INV_SUBPIXEL_MULT = 0.01f;

CurveData::CurveData()
{
    Clear();
}

bool CurveData::Save(const std::string &theFilePath, bool smallVersion)
{
    // TODO: doesn't seem to be present in any .exe?

    return false;
}

bool CurveData::Load(const std::string &theFilePath)
{
    Clear();

    DataReader aReader;
    if (!aReader.OpenFile(theFilePath))
    {
        mErrorString = "Failed to open file";
        return false;
    }

    char aBuf[5];
    aBuf[3] = 0;
    aBuf[4] = 0;

    aReader.ReadBytes(aBuf, 4);
    if (strcmp(aBuf, "CURV") != 0)
    {
        mErrorString = "Invalid file header";
        return false;
    }

    mVersion = aReader.ReadLong();
    if (mVersion < 1 || mVersion > gVersion)
    {
        mErrorString = "Invalid file version";
        return false;
    }

    bool smallVersion;
    bool hasTunnels;

    if (mVersion <= 2)
    {
        smallVersion = false;
        hasTunnels = true;
    }
    else
    {
        smallVersion = aReader.ReadBool();
        hasTunnels = aReader.ReadBool();
    }

    if (smallVersion)
    {
        mEditType = 0;
    }
    else
    {
        mEditType = aReader.ReadLong();
        ulong aBufferSize = aReader.ReadLong();

        if (aBufferSize > 1000000)
        {
            mErrorString = "File is corrupt";
            return false;
        }

        uchar *aByteArray = new uchar[aBufferSize];
        aReader.ReadBytes(aByteArray, aBufferSize);
        mEditData.WriteBytes(aByteArray, aBufferSize);
        delete[] aByteArray;
    }

    ulong aSize = aReader.ReadLong();
    if (aSize > 0)
    {
        if (mVersion > 1)
        {
            mPointList.push_back(PathPoint());
            PathPoint &aStartPoint = mPointList.back();

            aStartPoint.x = aReader.ReadFloat();
            aStartPoint.y = aReader.ReadFloat();

            if (hasTunnels)
            {
                aStartPoint.mInTunnel = aReader.ReadBool();
                aStartPoint.mPriority = aReader.ReadByte();
            }

            float ox = aStartPoint.x;
            float oy = aStartPoint.y;

            for (ulong i = 0; i < aSize - 1; i++)
            {
                mPointList.push_back(PathPoint());
                PathPoint &aPoint = mPointList.back();

                signed char dx = aReader.ReadByte();
                signed char dy = aReader.ReadByte();

                aPoint.x = dx * INV_SUBPIXEL_MULT + ox;
                aPoint.y = dy * INV_SUBPIXEL_MULT + oy;

                if (hasTunnels)
                {
                    aPoint.mInTunnel = aReader.ReadBool();
                    aPoint.mPriority = aReader.ReadByte();
                }

                ox = aPoint.x;
                oy = aPoint.y;
            }
        }
        else
        {
            for (ulong i = 0; i < aSize; i++)
            {
                mPointList.push_back(PathPoint());
                PathPoint &aPoint = mPointList.back();

                aPoint.x = aReader.ReadFloat();
                aPoint.y = aReader.ReadFloat();
                aPoint.mInTunnel = aReader.ReadBool();
                aPoint.mPriority = aReader.ReadByte();
            }
        }
    }

    return true;
}

void CurveData::Clear()
{
    mPointList.clear();
    mEditType = 0;
    mEditData.Clear();
}