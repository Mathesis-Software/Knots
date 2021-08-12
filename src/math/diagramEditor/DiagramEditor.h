#ifndef __DIAGRAM_EDITOR_H__
#define __DIAGRAM_EDITOR_H__

#include "../diagram/Diagram.h"

namespace KE { namespace TwoD {

class DiagramEditor {

public:
	struct Command {
		virtual ~Command() {}
		virtual void play(Diagram &diagram) = 0;
	};

private:
	Diagram initialDiagram;
	std::shared_ptr<Diagram> currentDiagram;
	std::string saveCheckpoint;

	std::vector<std::shared_ptr<Command>> log;
	std::size_t indexInLog;

public:
	DiagramEditor() : currentDiagram(new Diagram), indexInLog(0) {
		this->save();
	}
	DiagramEditor(const rapidjson::Document &doc) : initialDiagram(doc), currentDiagram(new Diagram(doc)), indexInLog(0) {
		this->save();
	}

	void savePoint();
	bool canUndo() const;
	bool canRedo() const;
	void undo();
	void redo();

private:
	void trimLog();
	void addCommand(const std::shared_ptr<Command> &command, bool savePoint);

public:
	const Diagram &diagram() const { return *this->currentDiagram; }
	const std::string &caption() const { return this->currentDiagram->caption; }
	void setCaption(const std::string &caption);

	const std::list<std::shared_ptr<Diagram::Vertex>> &vertices() const { return this->currentDiagram->vertices(); }
	std::list<Diagram::Edge> edges() const { return this->currentDiagram->edges(); }
	const std::list<Diagram::Crossing> &crossings(const Diagram::Edge &edge) const { return this->currentDiagram->crossings(edge); }
	bool isClosed() const { return this->currentDiagram->isClosed(); }

	std::shared_ptr<Diagram::Vertex> findVertex(const FloatPoint &pt, float maxDistance) const {
		return this->currentDiagram->findVertex(pt, maxDistance);
	}
	std::shared_ptr<Diagram::Edge> findEdge(const FloatPoint &pt, float maxDistance) const {
		return this->currentDiagram->findEdge(pt, maxDistance);
	}
	std::shared_ptr<Diagram::Crossing> findCrossing(const FloatPoint &pt, float maxDistance) const {
		return this->currentDiagram->findCrossing(pt, maxDistance);
	}

	bool isSaved() const;
	rapidjson::Document save();

	std::shared_ptr<Diagram::Vertex> addVertex(int x, int y);
	std::shared_ptr<Diagram::Vertex> addVertex(const Diagram::Edge &edge, int x, int y);
	bool canRemoveVertex(const std::shared_ptr<Diagram::Vertex> &vertex) const;
	void removeVertex(const std::shared_ptr<Diagram::Vertex> &vertex);
	void moveVertex(const std::shared_ptr<Diagram::Vertex> &vertex, int x, int y, bool storeCommand);
	bool canRemoveEdge(const std::shared_ptr<Diagram::Edge> &edge) const;
	void removeEdge(const std::shared_ptr<Diagram::Edge> &edge);
	std::shared_ptr<Diagram::Crossing> flipCrossing(Diagram::Crossing &crossing);

	void shift(int dx, int dy, bool storeCommand);
	void close();
	void clear();
	bool simplify();
};

}}

#endif /* __DIAGRAM_EDITOR_H__ */
