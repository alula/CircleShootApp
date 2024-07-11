#ifndef __USERDIALOG_H__
#define __USERDIALOG_H__

#include "CircleDialog.h"

#include <SexyAppFramework/ListListener.h>

namespace Sexy
{
	class WidgetManager;
	class CircleButton;
	class Graphics;
	class ListWidget;
	class ScrollbarWidget;

	class UserDialog : public CircleDialog, ListListener
	{
	public:
		UserDialog();
		virtual ~UserDialog();

		virtual void Resize(int theX, int theY, int theWidth, int theHeight);
		virtual int GetPreferredHeight(int theWidth);

		virtual void AddedToManager(WidgetManager *theWidgetManager);
		virtual void RemovedFromManager(WidgetManager *theWidgetManager);

		virtual void ListClicked(int theId, int theIdx, int theClickCount);
		virtual void ButtonDepress(int theId);

		std::string GetSelName();
		void FinishDeleteUser();
		void FinishRenameUser(const std::string &theNewName);

		ListWidget *mUserList;
		ScrollbarWidget *mScrollbar;
		CircleButton *mRenameButton;
		CircleButton *mDeleteButton;
	};
};

#endif