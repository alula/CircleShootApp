#ifndef __BLENDEDIMAGE_H__
#define __BLENDEDIMAGE_H__

namespace Sexy
{

    class Graphics;
    class Image;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class BlendedImage
    {
    protected:
        Image *mImages[4][4];

    public:
        BlendedImage(Image *theImage, const Rect &theSrcRect);
        virtual ~BlendedImage();

        void Draw(Graphics *g, float x, float y);
    };

} // namespace Sexy

#endif