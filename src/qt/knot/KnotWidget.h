#ifndef __KNOTWIDGET_H__
#define __KNOTWIDGET_H__

#include "../gl/GLWidget.h"
#include "../../math/knotWrapper/KnotWrapper.h"

class KnotWidget : public GLWidget {

private:
	const KE::ThreeD::KnotWrapper &knot;

public:
	KnotWidget(QWidget *parent, const KE::ThreeD::KnotWrapper &knot) : GLWidget(parent), knot(knot) {}
	const KE::GL::Color &backgroundColor() const override;
};

#endif /* __KNOTWIDGET_H__ */
