/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

#ifndef __DIAGRAM_EDITOR_H__
#define __DIAGRAM_EDITOR_H__

#include <vector>

#include "Diagram.h"

namespace KE::TwoD {

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
		this->serialize();
	}
	DiagramEditor(const rapidjson::Document &doc) : initialDiagram(doc), currentDiagram(new Diagram(doc)), indexInLog(0) {
		this->serialize();
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
	const std::list<Diagram::Crossing> &underCrossings(const Diagram::Edge &edge) const { return this->currentDiagram->underCrossings(edge); }
	std::map<Diagram::Edge,std::list<Diagram::Crossing>> allCrossings() const { return this->currentDiagram->allCrossings(); }
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
	rapidjson::Document serialize();

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

}

#endif /* __DIAGRAM_EDITOR_H__ */
