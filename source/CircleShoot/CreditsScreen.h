#ifndef __CREDITSSCREEN_H__
#define __CREDITSSCREEN_H__

#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/HyperlinkWidget.h>
#include <SexyAppFramework/ButtonListener.h>
#include <SexyAppFramework/CheckboxListener.h>

namespace Sexy
{
    class CircleCheckbox;
    class CircleButton;
    class SpriteMgr;
    class Image;

    class CreditsScreen : public Widget, public ButtonListener
    {
    private:
    public:
        CreditsScreen(bool happyEnd);
        virtual ~CreditsScreen();

        virtual void Draw(Graphics *g);
        virtual void Update();
        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        virtual void ButtonDepress(int theId);
        virtual void KeyChar(char theChar);

        // Unknown layout, assumed it's the same as SpriteMgr::StarInfo
        struct StarInfo
        {
            float x, y;
            float vy;
            unsigned int mColor;
        };

        int mScroll;
        typedef std::list<StarInfo> StarList;
        StarList mStarInfoList; // Unused
        SpriteMgr *mSpriteMgr;
        CircleButton *mWhateverButton;
        Image *mBackgroundImage;
    };

};

#endif