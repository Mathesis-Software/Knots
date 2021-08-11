#ifndef __DIAGRAM_EDITOR_H__
#define __DIAGRAM_EDITOR_H__

#include "../diagram/Diagram.h"

namespace KE { namespace TwoD {

class DiagramEditor {

private:
	Diagram _diagram;

public:
	DiagramEditor() {}
	DiagramEditor(const rapidjson::Document &doc) : _diagram(doc) {}

	const Diagram &diagram() const { return this->_diagram; }
	const std::string &caption() const { return this->_diagram.caption; }

	const std::list<std::shared_ptr<Diagram::Vertex>> &vertices() const { return this->_diagram.vertices(); }
	std::list<Diagram::Edge> edges() const { return this->_diagram.edges(); }
	const std::list<Diagram::Crossing> &crossings(const Diagram::Edge &edge) const { return this->_diagram.crossings(edge); }
	bool isClosed() const { return this->_diagram.isClosed(); }

	std::shared_ptr<Diagram::Vertex> findVertex(const FloatPoint &pt, float maxDistance) const {
		return this->_diagram.findVertex(pt, maxDistance);
	}
	std::shared_ptr<Diagram::Edge> findEdge(const FloatPoint &pt, float maxDistance) const {
		return this->_diagram.findEdge(pt, maxDistance);
	}
	std::shared_ptr<Diagram::Crossing> findCrossing(const FloatPoint &pt, float maxDistance) const {
		return this->_diagram.findCrossing(pt, maxDistance);
	}

	rapidjson::Document save() const { return this->_diagram.save(); }

	std::shared_ptr<Diagram::Vertex> addVertex(int x, int y);
	std::shared_ptr<Diagram::Vertex> addVertex(const Diagram::Edge &edge, int x, int y);
	void removeVertex(const std::shared_ptr<Diagram::Vertex> &vertex);
	void moveVertex(const std::shared_ptr<Diagram::Vertex> &vertex, int x, int y);
	std::shared_ptr<Diagram::Crossing> flipCrossing(Diagram::Crossing &crossing);

	void shift(int dx, int dy);
	void close();
	void clear();
	bool simplify();
};

}}

#endif /* __DIAGRAM_EDITOR_H__ */
