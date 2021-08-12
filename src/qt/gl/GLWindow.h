#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include "../abstractWindow/abstractWindow.h"
#include "GLWidget.h"

class GLWindow : public abstractWindow {

private:
	void printIt(QPrinter*);

protected:
	void repaint3d();
	virtual void rotate(int direction);

public:
	GLWindow();
	GLWidget *glWidget() { return (GLWidget*)this->centralWidget(); }
};

#endif /* __GLWINDOW_H__ */
