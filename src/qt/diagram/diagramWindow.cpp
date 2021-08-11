#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

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
	setWindowTitle(diagram.caption.c_str());

	actionsMenu = this->menuBar()->addMenu("&Actions");
	this->registerAction(
		actionsMenu->addAction("&Convert", [this] { this->convert(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("&Simplify", [this] { this->simplify(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("C&lear", [this] { this->clear(); }),
		[&diagram](QAction &action) { action.setEnabled(!diagram.vertices().empty()); }
	);
	
	this->registerAction(
		this->addToolbarAction("mini_trefoil.xpm", "Convert to knot", [this] { this->convert(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);

	addToolbarSeparator();
	auto addAction = [this, widget](const QString &icon, const QString &text, DiagramWidget::EditorMode mode) {
		QAction *action = this->addToolbarAction(icon, text, [this, mode] { this->setMode(mode); });
		action->setCheckable(true);
		this->registerAction(action, [widget, mode](QAction &action) {
			const bool canSet = widget->canSetEditorMode(mode);
			action.setChecked(canSet && widget->editorMode() == mode);
			action.setEnabled(canSet);
		});
	};
	addAction("diagram_mode_quick_drawing.svg", "Quick drawing", DiagramWidget::QUICK_DRAWING);
	addAction("diagram_mode_editing.svg", "Editing", DiagramWidget::EDITING);
	addAction("diagram_mode_moving.svg", "Moving diagram", DiagramWidget::MOVING);

	addToolbarSeparator();
	addToolbarAction("undo.svg", "Undo", [] {});
	addToolbarAction("redo.svg", "Redo", [] {});

	setWindowIcon(QPixmap((QString) getenv("KNOTEDITOR_PIXMAPS") + "/diagram.xpm"));

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

	isSaved = true;
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
	if (this->diagramWidget()->diagram.simplify(2)) {
		this->isSaved = false;
		this->centralWidget()->repaint();
	}
}

void diagramWindow::printIt(QPrinter *prn) {
	QPainter pnt;
	pnt.begin(prn);
	this->diagramWidget()->drawIt(pnt);
	pnt.end();
}
