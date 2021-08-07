#ifndef __DIAGRAMWINDOW_H__
#define __DIAGRAMWINDOW_H__

#include <QtWidgets/QMenu>

#include "DiagramWidget.h"
#include "../abstractWindow/abstractWindow.h"

class diagramWindow : public abstractWindow {

Q_OBJECT

private:
	QMenu *actionsMenu;

	void init(DiagramWidget *widget);

	const char *mask() const override {return "*.dgr";};

	void printIt(QPrinter*);
	void saveIt(std::ostream&);

	void setMode(DiagramWidget::EditingMode mode);
	
	void convert();
	void clear();
	void simplify();

public:
	diagramWindow(const rapidjson::Document &doc);
	diagramWindow();
	~diagramWindow();

	DiagramWidget *diagramWidget() const { return (DiagramWidget*)this->centralWidget(); }
	bool isEmpty() const override;

	friend class DiagramWidget;
};

#endif /* __DIAGRAMWINDOW_H__ */
