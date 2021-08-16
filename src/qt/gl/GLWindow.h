#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include "../abstractWindow/abstractWindow.h"

class GLWidget;

class GLWindow : public abstractWindow {

private:
	void printIt(QPrinter*);

public:
	GLWindow();
	GLWidget *glWidget() const;
};

#endif /* __GLWINDOW_H__ */
