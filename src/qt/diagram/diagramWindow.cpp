#include <QtGui/QPainter>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "diagramWindow.h"
#include "../knot/knotWindow.h"

diagramWindow::diagramWindow(const rapidjson::Document &doc) {
	this->init(new DiagramWidget(this, doc));
}

diagramWindow::diagramWindow() {
	this->init(new DiagramWidget(this));
}

void diagramWindow::init(DiagramWidget *widget) {
	setCentralWidget(widget);
	const auto &diagram = widget->diagram;

	this->connect(widget, &DiagramWidget::setActionTip, [this](const QString &text) {
		if (text != QString::null) {
			this->statusBar()->showMessage(text);
		} else {
			this->statusBar()->clearMessage();
		}
	});
	this->connect(widget, &DiagramWidget::actionsUpdated, this, &diagramWindow::updateActions);

	actionsMenu = this->menuBar()->addMenu("Actions");
	this->registerAction(
		actionsMenu->addAction("Convert", [this] { this->convert(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("Simplify", [this] { this->simplify(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("Clear", [this] { this->clear(); }),
		[&diagram](QAction &action) { action.setEnabled(!diagram.vertices().empty()); }
	);

	this->registerAction(
		this->addToolbarAction("trefoil.png", "Convert to knot", [this] { this->convert(); }),
		[&diagram](QAction &action) {
			const bool enabled = diagram.isClosed();
			action.setEnabled(enabled);
			action.setToolTip(enabled ? "Converting to 3D knot" : "Converting to knot disabled until the diagram is closed.");
		}
	);

	addToolbarSeparator();
	auto addAction = [this, widget](const QString &icon, const QString &text, const QString &tooltip, const QString &disabledTooltip, DiagramWidget::EditorMode mode) {
		QAction *action = this->addToolbarAction(icon, text, [this, mode] { this->setMode(mode); });
		action->setCheckable(true);
		this->registerAction(action, [widget, mode, tooltip, disabledTooltip](QAction &action) {
			const bool canSet = widget->canSetEditorMode(mode);
			action.setChecked(canSet && widget->editorMode() == mode);
			action.setEnabled(canSet);
			action.setToolTip(canSet ? tooltip : disabledTooltip);
		});
	};
	addAction(
		"diagram_mode_quick_drawing.svg",
		"Quick drawing",
		"Quick drawing: move the mouse and click to set a new point. Right-click to close the diagram.",
		"Quick drawing disabled for closed diagram.",
		DiagramWidget::QUICK_DRAWING
	);
	addAction(
		"diagram_mode_editing.svg",
		"Editing",
		"Editing: choose point/edge/crossing and click to change it. Read status at the bottom of the window if unsure.",
		"Editing disabled for empty diagram.",
		DiagramWidget::EDITING
	);
	addAction(
		"diagram_mode_moving.svg",
		"Moving diagram",
		"Moving: click anywhere and move the diagram.",
		"Moving disabled for empty diagram.",
		DiagramWidget::MOVING
	);

	addToolbarSeparator();
	auto undo = this->registerAction(
		this->addToolbarAction("undo.svg", "Undo", [this, widget] {
			widget->diagram.undo();
			widget->repaint();
			widget->updateEditorMode();
			this->updateActions();
		}),
		[widget](QAction &action) { action.setEnabled(widget->diagram.canUndo()); }
	);
	undo->setShortcut(QKeySequence("Ctrl+Z"));
	auto redo = this->registerAction(
		this->addToolbarAction("redo.svg", "Redo", [this, widget] {
			widget->diagram.redo();
			widget->repaint();
			widget->updateEditorMode();
			this->updateActions();
		}),
		[widget](QAction &action) { action.setEnabled(widget->diagram.canRedo()); }
	);
	redo->setShortcut(QKeySequence("Ctrl+Shift+Z"));

	QPixmap pixmap(":images/diagram.svg");
	pixmap.setDevicePixelRatio(this->devicePixelRatio());
	this->setWindowIcon(pixmap);

	complete();

	this->updateActions();
}

diagramWindow::~diagramWindow() {
	delete actionsMenu;
}

void diagramWindow::setMode(DiagramWidget::EditorMode mode) {
	this->diagramWidget()->setEditorMode(mode);
	this->updateActions();
}

void diagramWindow::clear() {
	this->diagramWidget()->clear();
	this->updateActions();
}

void diagramWindow::convert() {
	if (!this->diagramWidget()->diagram.isClosed()) {
		QMessageBox::critical(this, "Error", "\nCannot convert non-closed diagram.\n");
		return;
	}

	if (this->diagramWidget()->diagram.vertices().size() <= 2) {
		QMessageBox::critical(this, "Error", "\nCannot convert diagram with less than three points.\n");
		return;
	}

	(new knotWindow(*this))->show();
}

void diagramWindow::saveIt(std::ostream &os) {
	const rapidjson::Document doc = this->diagramWidget()->diagram.save();
	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	doc.Accept(writer);
}

bool diagramWindow::isEmpty() const {
	const auto widget = this->diagramWidget();
	return widget == nullptr || widget->diagram.vertices().empty();
}

void diagramWindow::simplify() {
	if (this->diagramWidget()->diagram.simplify()) {
		this->centralWidget()->repaint();
	}
}

void diagramWindow::printIt(QPrinter *prn) {
	QPainter pnt;
	pnt.begin(prn);
	this->diagramWidget()->drawIt(pnt);
	pnt.end();
}

void diagramWindow::updateActions() {
	const auto widget = this->diagramWidget();
	if (widget) {
		setWindowTitle(widget->diagram.caption().c_str());
	}
	abstractWindow::updateActions();
}

void diagramWindow::rename() {
	auto &diagram = this->diagramWidget()->diagram;

	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename diagram", "New diagram name:", QLineEdit::Normal, diagram.caption().c_str(), &ok
	);
	if (ok) {
		diagram.setCaption(text.toStdString());
		this->updateActions();
	}
}

bool diagramWindow::isSaved() const {
	const auto widget = this->diagramWidget();
	return !widget || widget->diagram.isSaved();
}
