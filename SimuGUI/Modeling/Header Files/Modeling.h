#pragma once

/*
* @date : 2019/01/04
* @author : jihang
*/

#ifndef MODELING_H
#define MODELING_H

#include "imode.h"

#include "cwidgets.h"
#include "ToolDragStack.h"
#include "DropLabel.h"
#include "DataTypeStack.h"
#include "ModelInfoStack.h"
#include "InterfaceInfoStack.h"

class Modeling : public IMode {

	Q_OBJECT

public:
	explicit Modeling(QWidget *parent = 0);

private:
	void createWindow();
	void createToolDragStack();
	void createDropLabel();
	void createDateTypeStack();
	void createModelInfoStack();
	void createInterfaceInfoStack();

public slots:
	void slotReceiveMessage(QString);

private:
	ToolDragStack *m_pToolDragStack;

	DropLabel *m_pDropLabel;

	DataTypeStack *m_pDataTypeStack;

	ModelInfoStack *m_pModelInfoStack;

	InterfaceInfoStack *m_pInterfaceInfoStack;
};
#endif // MODELING_H