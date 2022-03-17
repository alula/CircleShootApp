#ifndef __IMAGEFONT_H__
#define __IMAGEFONT_H__

#include "Font.h"
#include "DescParser.h"
#include "SharedImage.h"

namespace Sexy
{

class SexyAppBase;
class Image;

class CharData
{
public:
	Rect					mImageRect;
	Point					mOffset;
	char					mKerningOffsets[256];
	int						mWidth;
	int						mOrder;

public:
	CharData();
};

class FontData;

class FontLayer
{
public:	
	FontData*				mFontData;
	StringVector			mRequiredTags;
	StringVector			mExcludedTags;	
	CharData				mCharData[256];	
	Color					mColorMult;
	Color					mColorAdd;
	SharedImageRef			mImage;	
	int						mDrawMode;
	Point					mOffset;
	int						mSpacing;
	int						mMinPointSize;
	int						mMaxPointSize;
	int						mPointSize;
	int						mAscent;
	int						mAscentPadding; // How much space is above the avg uppercase char
	int						mHeight;		// 	
	int						mDefaultHeight; // Max height of font character image rects	
	int						mLineSpacingOffset; // This plus height should get added between lines
	int						mBaseOrder;

public:
	FontLayer(FontData* theFontData);
	FontLayer(const FontLayer& theFontLayer);
};

typedef std::list<FontLayer> FontLayerList;
typedef std::map<std::string, FontLayer*> FontLayerMap;
typedef std::list<Rect> RectList;

class FontData : public DescParser
{
public:
	bool					mInitialized;
	int						mRefCount;
	SexyAppBase*			mApp;		

	int						mDefaultPointSize;
	uchar					mCharMap[256];	
	FontLayerList			mFontLayerList;
	FontLayerMap			mFontLayerMap;

	std::string				mSourceFile;
	std::string				mFontErrorHeader;	

public:
	virtual bool			Error(const std::string& theError);

	bool					GetColorFromDataElement(DataElement *theElement, Color &theColor);
	bool					DataToLayer(DataElement* theSource, FontLayer** theFontLayer);
	virtual bool			HandleCommand(const ListDataElement& theParams);

public:
	FontData();
	virtual ~FontData();

	void					Ref();
	void					DeRef();

	bool					Load(SexyAppBase* theSexyApp, const std::string& theFontDescFileName);
	bool					LoadLegacy(Image* theFontImage, const std::string& theFontDescFileName);
};

class ActiveFontLayer
{
public:
	FontLayer*				mBaseFontLayer;

	Image*					mScaledImage;
	bool					mOwnsImage;
	Rect					mScaledCharImageRects[256];

public:
	ActiveFontLayer();
	ActiveFontLayer(const ActiveFontLayer& theActiveFontLayer);
	virtual ~ActiveFontLayer();
};

typedef std::list<ActiveFontLayer> ActiveFontLayerList;

class RenderCommand
{
public:
	Image*					mImage;
	int						mDest[2];
	int						mSrc[4];
	int						mMode;
	Color					mColor;
	RenderCommand*			mNext;
};

typedef std::multimap<int, RenderCommand> RenderCommandMap;

class ImageFont : public Font
{
public:	
	FontData*				mFontData;
	int						mPointSize;
	StringVector			mTagVector;

	bool					mActiveListValid;
	ActiveFontLayerList		mActiveLayerList;
	double					mScale;
	bool					mForceScaledImagesWhite;

public:
	virtual void			GenerateActiveFontLayers();
	virtual void			DrawStringEx(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect* theClipRect, RectList* theDrawnAreas, int* theWidth);

public:
	ImageFont(SexyAppBase* theSexyApp, const std::string& theFontDescFileName);
	ImageFont(Image *theFontImage); // for constructing your own image font without a file descriptor
	ImageFont(const ImageFont& theImageFont);
	virtual ~ImageFont();

	// Deprecated
	ImageFont(Image* theFontImage, const std::string& theFontDescFileName);
	//ImageFont(const ImageFont& theImageFont, Image* theImage);
	
	virtual int				CharWidth(char theChar);
	virtual int				CharWidthKern(char theChar, char thePrevChar);
	virtual int				StringWidth(const SexyString& theString);
	virtual void			DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect);

	virtual Font*			Duplicate();

	virtual void			SetPointSize(int thePointSize);
	virtual int				GetPointSize();
	virtual void			SetScale(double theScale);
	virtual int				GetDefaultPointSize();
	virtual bool			AddTag(const std::string& theTagName);	
	virtual bool			RemoveTag(const std::string& theTagName);
	virtual bool			HasTag(const std::string& theTagName);
	virtual std::string		GetDefine(const std::string& theName);

	virtual void			Prepare();
};

}

#endif //__IMAGEFONT_H__