#include "KnotWidget.h"

const KE::GL::Color &KnotWidget::backgroundColor() const {
	const auto ref = this->knot.backgroundColor;
	return ref ? *ref : KE::GL::Color::white;
}
