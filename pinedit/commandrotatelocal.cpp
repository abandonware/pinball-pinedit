/***************************************************************************
                          commandrotatelocal.cpp  -  description
                             -------------------
    begin                : Fri Apr 12 2002
    copyright            : (C) 2002 by Henrik Enqvist IB
    email                : henqvist@excite.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// qt includes
#include <qpainter.h>
// application includes
#include "commandrotatelocal.h"
#include "pineditdoc.h"
#include "view2d.h"
// emilia includes
#include "Private.h"
#include "Shape3D.h"
#include "EMath.h"

CommandRotateLocal::CommandRotateLocal(PinEditDoc * doc) : Command(doc) {
}

CommandRotateLocal::~CommandRotateLocal() {
}

void CommandRotateLocal::clearObjects() {
}

void CommandRotateLocal::execute(const CommandContext & context) {
	assert(context.shape != NULL);
	// build matrix stack for temporary translation
	// mtxB is global rotion matrix, mtxC fixes the translation in local rotation
	// mtxC is the final matrix
	Matrix mtxA = EMath::identityMatrix;
	Matrix mtxB = EMath::identityMatrix;
	Matrix mtxC = EMath::identityMatrix;
	Vertex3D vtxTA = {0,0,0}, vtxRA = {0,0,0};
	Vertex3D vtxTB = {0,0,0}, vtxRB = {0,0,0};
	// get local center of gravity
	p_Doc->getSelectedCenter(vtxTA);
	vtxTB.x = -vtxTA.x;
	vtxTB.y = -vtxTA.y;
	vtxTB.z = -vtxTA.z;
	switch (context.type) {
	case XY: vtxRA.z = (context.x2-context.x1)/PE_ROTATION_FACTOR; break;
	case XZ: vtxRA.y = (context.x2-context.x1)/PE_ROTATION_FACTOR; break;
	case ZY: vtxRA.x = (context.z2-context.z1)/PE_ROTATION_FACTOR; break;
	}
	EMath::getTransformationMatrix(mtxA, vtxTA, vtxRA);
	EMath::getTransformationMatrix(mtxB, vtxTB, vtxRB);
	EMath::matrixMulti(mtxB, mtxA, mtxC);

	int index = 0;
	Vertex3D * vtx = context.shape->getVertex3D(p_Doc->getSelectedVertex(index));
	Vertex3D vtxA;
	while (vtx != NULL) {
		EMath::applyMatrix(mtxC, *vtx, vtxA);
		*vtx = vtxA;
		// TODO rotation
		index++;
		vtx = context.shape->getVertex3D(p_Doc->getSelectedVertex(index));
	}

	//p_Context = new CommandContext(context);
	p_Doc->setModified(true);
	p_Doc->updateAll("polygon");
	p_Doc->pushUndo(this);
	cerr << "CommandRotateLocal::execute from " << context.x1 <<" "<< context.y1 <<" "<< context.z1 << 
		" to " << context.x2 <<"  "<< context.y2 <<" "<< context.z2 << endl;
}

void CommandRotateLocal::preview(const CommandContext & context, View2D * view2d) {
	if (context.shape == NULL) return;
	// build matrix stack for temporary translation
	// mtxB is global rotion matrix, mtxC fixes the translation in local rotation
	// mtxC is the final matrix
	
	// build matrix stack for temporary translation
	// mtxA is moves and rotates , mtxB fixes the translation to local rotation
	// mtxC is the final matrix
	Matrix mtxA = EMath::identityMatrix;
	Matrix mtxB = EMath::identityMatrix;
	Matrix mtxC = EMath::identityMatrix;
	Vertex3D vtxTA = {0,0,0}, vtxRA = {0,0,0};
	Vertex3D vtxTB = {0,0,0}, vtxRB = {0,0,0};
	
	switch (context.type) {
	case XY: vtxRA.z = (context.x2-context.x1)/PE_ROTATION_FACTOR; break;
	case XZ: vtxRA.y = (context.x2-context.x1)/PE_ROTATION_FACTOR; break;
	case ZY: vtxRA.x = (context.z2-context.z1)/PE_ROTATION_FACTOR; break;
	}
	
	p_Doc->getSelectedCenter(vtxTA);
	vtxTB.x = -vtxTA.x;
	vtxTB.y = -vtxTA.y;
	vtxTB.z = -vtxTA.z;
	
	EMath::getTransformationMatrix(mtxA, vtxTA, vtxRA);
	EMath::getTransformationMatrix(mtxB, vtxTB, vtxRB);
	EMath::matrixMulti(mtxB, mtxA, mtxC);
	
	// draw selected polygons
	view2d->getPainter()->setPen(Qt::green);
	int index = 0;
	Polygon * poly = p_Doc->getSelectedPolygon(index);
	while (poly != NULL) {
		view2d->drawPolygon(context.shape, poly, mtxC);
		index++;
		poly = p_Doc->getSelectedPolygon(index);
	}
	// draw selected vertices
	index = 0;
	Vertex3D * vtx = context.shape->getVertex3D(p_Doc->getSelectedVertex(index));
	Vertex3D vtxA;
	while (vtx != NULL) {
		// the matrix will rotate and reapply the translation
		view2d->drawVertex(context.shape, *vtx, mtxC);
		index++;
		vtx = context.shape->getVertex3D(p_Doc->getSelectedVertex(index));
	}
	//cerr << "drawed " << index << " temporary polygons" << endl;
	cerr << "CommandRotateLocal::preview" << endl;
}

void CommandRotateLocal::undo() {
	cerr << "CommandRotateLocal::undo not implemented" << endl;
}

Command * CommandRotateLocal::build() {
	return new CommandRotateLocal(p_Doc);
}
