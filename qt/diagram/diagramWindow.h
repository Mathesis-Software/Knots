#ifndef __DIAGRAMWINDOW_H__
#define __DIAGRAMWINDOW_H__

#include <QtWidgets/QMenu>
#include <QtWidgets/QToolButton>

#include "DiagramWidget.h"
#include "../abstractWindow/abstractWindow.h"

class diagramWindow : public abstractWindow {

Q_OBJECT

private:
	std::map<QAction*,std::function<void(QAction&)>> actionsMap;

	QMenu *actionsMenu;

	QToolButton **actions;

	void init(DiagramWidget *widget);

	const char *mask() const override {return "*.dgr";};

	void printIt(QPrinter*);
	void saveIt(std::ostream&);

	void setMode(DiagramWidget::EditingMode mode);
	void registerAction(QAction *action, std::function<void(QAction&)> controller);
	
private slots:
	void convert();
	void clear();
	void simplify();

public:
	diagramWindow(const rapidjson::Document &doc);
	diagramWindow();
	~diagramWindow();

	DiagramWidget *diagramWidget() const { return (DiagramWidget*)this->centralWidget(); }
	bool isEmpty() const override;
	void updateActions();

	friend class DiagramWidget;
};

#endif /* __DIAGRAMWINDOW_H__ */
