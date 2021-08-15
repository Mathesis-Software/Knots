#ifndef __KNOTWINDOW_MATH_H__
#define __KNOTWINDOW_MATH_H__

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>

#include "knotWindow.h"
#include "../../math/computables/computable.h"

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
	std::shared_ptr<KE::ThreeD::Computables::Computable> computable;

private slots:

	void doit();

public:

	parameterLabel(QDialog*, std::shared_ptr<KE::ThreeD::Computables::Computable>, int, int);
	~parameterLabel();

	void renew();
};

#endif /* __KNOTWINDOW_MATH_H__ */
