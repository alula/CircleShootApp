#ifndef __CREATEUSERDIALOG_H__
#define __CREATEUSERDIALOG_H__

#include "CircleDialog.h"

#include <SexyAppFramework/EditListener.h>

namespace Sexy
{
	class WidgetManager;
	class CircleButton;
	class Graphics;
	class EditWidget;

	class CreateUserDialog : public CircleDialog, EditListener
	{
	public:
		CreateUserDialog(bool rename);
		virtual ~CreateUserDialog();

		virtual void Resize(int theX, int theY, int theWidth, int theHeight);
		virtual int GetPreferredHeight(int theWidth);

		virtual void AddedToManager(WidgetManager *theWidgetManager);
		virtual void RemovedFromManager(WidgetManager *theWidgetManager);

		virtual void EditWidgetText(int theId, const std::string &theString);
		virtual bool AllowChar(int theId, char theChar);

		void SetName(const std::string &theName);
		std::string GetName();

		EditWidget *mNameInput;
	};
};

#endif