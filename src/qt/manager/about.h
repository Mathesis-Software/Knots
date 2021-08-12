#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <QtWidgets/QWidget>

class AboutWindow : public QWidget {

public:
	AboutWindow(QWidget*);
	void showMe();

private:
	void mousePressEvent(QMouseEvent*) override;
};

#endif /* __ABOUT_H__ */
