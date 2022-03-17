#include "Zuma_Prefix.pch"

#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/MemoryImage.h>

#include "BlendedImage.h"

#include <cmath>

using namespace Sexy;

BlendedImage::BlendedImage(Sexy::Image *theImage, const Rect &theSrcRect)
{
    int theWidth = theSrcRect.mWidth + 3;
    int theHeight = theSrcRect.mHeight + 3;

    MemoryImage memoryImage;
    memoryImage.Create(theWidth, theHeight);

    Graphics graphics(&memoryImage);
    graphics.DrawImage(theImage, 1, 1, theSrcRect);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            MemoryImage *memoryImage2 = new MemoryImage();
            memoryImage2->Create(theWidth, theHeight);

            Graphics graphics2(memoryImage2);

            graphics2.DrawImageF(&memoryImage, (float)i / 4.0f * 0.9f + 0.1f, (float)j / 4.0f * 0.9f + 0.1f);

            mImages[i][j] = memoryImage2;
        }
    }
}

BlendedImage::~BlendedImage()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            delete mImages[i][j];
        }
    }
}

void BlendedImage::Draw(Graphics *g, float x, float y)
{
    int idxX = (int)((x - floorf(x)) * 4.0f);
    int idxY = (int)((y - floorf(y)) * 4.0f);

    g->DrawImage(mImages[idxX][idxY], (int)x, (int)y);
}