/***************************************************************************
                          commandmove.h  -  description
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

#ifndef COMMANDMOVE_H
#define COMMANDMOVE_H

#include <command.h>

/** @author Henrik Enqvist IB */
class CommandMove : public Command  {
 public:
	CommandMove(PinEditDoc * doc);
	~CommandMove();
	Command * build();
	void undo();
	void clearObjects();
	void execute(const CommandContext & context);
	void preview(const CommandContext & context, View2D * view2d);
	virtual const char * type() { return "CommandMove"; };

};

#endif
