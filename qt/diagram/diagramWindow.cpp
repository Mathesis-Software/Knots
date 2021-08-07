#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPixmap>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "diagramWindow.h"
#include "../knotWindow/knotWindow.h"

diagramWindow::diagramWindow(const rapidjson::Document &doc) {
	this->init(new DiagramWidget(this, doc));
	actions[0]->setChecked(false);
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
		actionsMenu->addAction("&Convert", this, SLOT(convert())),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("&Simplify", this, SLOT(simplify())),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("C&lear", this, SLOT(clear())),
		[&diagram](QAction &action) { action.setEnabled(!diagram.vertices().empty()); }
	);
	
	this->registerAction(
		this->addToolbarAction("mini_trefoil.xpm", "Convert to knot", [this] { this->convert(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);

	addToolBarSeparator();
	actions = new QToolButton*[6];
	actions[0] = addToolBarButton("draw_diagram.xpm", "Draw diagram", 0);
	actions[1] = addToolBarButton("add_point.xpm", "Add point", 0);
	actions[2] = addToolBarButton("move_point.xpm", "Move point", 0);
	actions[3] = addToolBarButton("delete_point.xpm", "Delete point", 0);
	actions[4] = addToolBarButton("change_crossing.xpm", "Change crossing", 0);
	actions[5] = addToolBarButton("move_diagram.xpm", "Move diagram", 0);

	QButtonGroup *grp = new QButtonGroup;
	for (int i = 0; i < 6; i++) {
		actions[i]->setCheckable(true);
		grp->addButton(actions[i]);
		this->connect(actions[i], &QToolButton::pressed, [=](){ this->setMode(static_cast<DiagramWidget::EditingMode>(i)); });
	}

	grp->setExclusive(false);

	actions[0]->setChecked(true);

	setWindowIcon(QPixmap((QString) getenv("KNOTEDITOR_PIXMAPS") + "/diagram.xpm"));

	complete();

	this->updateActions();
}

diagramWindow::~diagramWindow() {
	delete actionsMenu;
	delete[] actions;
}

void diagramWindow::setMode(DiagramWidget::EditingMode mode) {
	if (!this->diagramWidget()->setEditingMode(mode)) {
		actions[mode]->toggle();
		return;
	}

	for (int mo = 0; mo < 6; ++mo) {
		if (static_cast<DiagramWidget::EditingMode>(mo) != mode) {
			actions[mo]->setChecked(false);
		}
	}
}

void diagramWindow::clear() {
	this->diagramWidget()->clear();
	for (int mo = 0; mo < 6; ++mo) {
		actions[mo]->setChecked(mo == 0);
	}

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
