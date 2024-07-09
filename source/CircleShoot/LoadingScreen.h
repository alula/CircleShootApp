#ifndef __LOADINGSCREEN_H__
#define __LOADINGSCREEN_H__

#include <SexyAppFramework/Widget.h>
#include <SexyAppFramework/HyperlinkWidget.h>
#include <SexyAppFramework/ButtonListener.h>

namespace Sexy
{

    class LoadingScreen : public Widget, public ButtonListener
    {
    private:
        std::string mRegistrationStatusString;
        HyperlinkWidget *mClickToPlayLink;
        HyperlinkWidget *mRegisterLink;
        double mLoadingProgress;
        bool unk5;
        std::string unk6;
        bool unk7;
        bool unk8;

    public:
        LoadingScreen();
        virtual ~LoadingScreen();

        virtual void Resize(int theX, int theY, int theWidth, int theHeight);
        virtual void Draw(Graphics *g);
        virtual void Update();
        virtual void AddedToManager(WidgetManager *theWidgetManager);
        virtual void RemovedFromManager(WidgetManager *theWidgetManager);
        virtual void ButtonDepress(int theId);
        void SetRegistered();

        void SyncRegistrationStuff();
    };

};

#endif