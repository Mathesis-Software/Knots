#ifndef __DIAGRAMWINDOW_H__
#define __DIAGRAMWINDOW_H__

#include <QtWidgets/QMenu>

#include "DiagramWidget.h"
#include "../abstractWindow/abstractWindow.h"

class diagramWindow : public abstractWindow {

private:
	QMenu *actionsMenu;

	void init(DiagramWidget *widget);

	QString fileFilter() const override { return "Diagram files (*.dgr)"; }

	void printIt(QPrinter*) override;

	void setMode(DiagramWidget::EditorMode mode);

	void convert();
	void clear();
	void simplify();

	void rename() override;

public:
	diagramWindow(const rapidjson::Document &doc);
	diagramWindow();
	~diagramWindow();

	DiagramWidget *diagramWidget() const { return (DiagramWidget*)this->centralWidget(); }

	void saveIt(std::ostream&) override;
	bool isSaved() const override;

	bool isEmpty() const override;

	void updateActions() override;

	friend class DiagramWidget;
};

#endif /* __DIAGRAMWINDOW_H__ */
