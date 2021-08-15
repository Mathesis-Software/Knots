#ifndef __KNOTEDITOR_H__
#define __KNOTEDITOR_H__

#include "../knot/Knot.h"

namespace KE { namespace ThreeD {

class KnotWrapper {

private:
	Knot knot;

public:
	KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : knot(diagram, width, height) {}
	KnotWrapper(const rapidjson::Document &doc) : knot(doc) {}

	Knot::Snapshot snapshot() const { return this->knot.snapshot(); }
	const std::string &caption() const { return this->knot.caption; }

	void decreaseEnergy() { this->knot.decreaseEnergy(); }
	void setCaption(const std::string &caption) { this->knot.caption = caption; } 
	void setLength(double length) { this->knot.setLength(length); }
	void center() { this->knot.center(); }
	void normalize(std::size_t numberOfPoints) { this->knot.normalize(numberOfPoints); }
	rapidjson::Document serialize(const double matrix[3][3]) const { return this->knot.serialize(matrix); }
};

}}

#endif /* __KNOTEDITOR_H__ */
