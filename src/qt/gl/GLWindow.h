#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include "../abstractWindow/abstractWindow.h"

class GLWidget;

class GLWindow : public abstractWindow {

private:
	void printIt(QPrinter*);

protected:
	void repaint3d();

public:
	GLWindow();
	GLWidget *glWidget();
};

#endif /* __GLWINDOW_H__ */
