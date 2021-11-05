/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DiagramEditor.h"

namespace KE::TwoD {

namespace {

struct SavePointCommand : public DiagramEditor::Command {
	void play(Diagram &) override {
	}
};

std::shared_ptr<DiagramEditor::Command> savePointCommand(new SavePointCommand());

}

void DiagramEditor::savePoint() {
	if (this->indexInLog > 0 && this->log[this->indexInLog - 1] != savePointCommand) {
		this->trimLog();
		this->log.push_back(savePointCommand);
		this->indexInLog += 1;
	}
}

void DiagramEditor::trimLog() {
	if (this->indexInLog < this->log.size()) {
		this->log.erase(this->log.begin() + this->indexInLog, this->log.end());
	}
}

void DiagramEditor::addCommand(const std::shared_ptr<Command> &command, bool savePoint) {
	this->trimLog();
	this->log.push_back(command);
	this->indexInLog += 1;
	if (savePoint) {
		this->savePoint();
	}
}

bool DiagramEditor::canUndo() const {
	return this->indexInLog > 0;
}

bool DiagramEditor::canRedo() const {
	return this->indexInLog < this->log.size();
}

void DiagramEditor::undo() {
	if (this->indexInLog == 0) {
		return;
	}
	for (this->indexInLog -= 1; this->indexInLog > 0; this->indexInLog -= 1) {
		if (this->log[this->indexInLog - 1] == savePointCommand) {
			break;
		}
	}
	auto replacement = std::make_shared<Diagram>(this->initialDiagram.serialize());
	for (std::size_t index = 0; index < this->indexInLog; index += 1) {
		this->log[index]->play(*replacement);
	}
	this->currentDiagram = replacement;
}

void DiagramEditor::redo() {
	for (; this->indexInLog < this->log.size(); this->indexInLog += 1) {
		const auto &command = this->log[this->indexInLog];
		if (command == savePointCommand) {
			this->indexInLog += 1;
			break;
		}
		command->play(*this->currentDiagram);
	}
}

}
