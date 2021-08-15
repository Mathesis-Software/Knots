#ifndef __KNOTEDITOR_H__
#define __KNOTEDITOR_H__

#include "../knot/Knot.h"

namespace KE { namespace ThreeD {

class KnotWrapper {

private:
	Knot _knot;

public:
	KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : _knot(diagram, width, height) {}
	KnotWrapper(const rapidjson::Document &doc) : _knot(doc) {}

	const Knot &knot() const { return this->_knot; }
	Knot::Snapshot snapshot() const { return this->_knot.snapshot(); }

	void decreaseEnergy() { this->_knot.decreaseEnergy(); }
	void setCaption(const std::string &caption) { this->_knot.caption = caption; } 
	void setLength(double length) { this->_knot.setLength(length); }
	void center() { this->_knot.center(); }
	void normalize(std::size_t numberOfPoints) { this->_knot.normalize(numberOfPoints); }
};

}}

#endif /* __KNOTEDITOR_H__ */
