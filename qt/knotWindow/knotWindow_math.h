#ifndef __KNOTWINDOW_MATH_H__
#define __KNOTWINDOW_MATH_H__

#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
//#include <qwindow.h>
#include <QtWidgets/qdialog.h>

#include "knotWindow.h"

class paramWindow : public QDialog {

	Q_OBJECT

private:
 
	knotWindow *Parent;
	int nLabels;
	class parameterLabel **pLabels;
	void closeEvent(QCloseEvent*);

public:

	paramWindow(knotWindow*);
	~paramWindow();
	void recompute();
};

class parameterLabel : public QWidget {

Q_OBJECT

private:

	QLabel *lbl;
	QCheckBox *chkbox;
	std::shared_ptr<Knot::Computable> computable;

private slots:

	void doit();

public:

	parameterLabel(QDialog*, std::shared_ptr<Knot::Computable>, int, int);
	~parameterLabel();

	void renew();
};

#endif /* __KNOTWINDOW_MATH_H__ */
