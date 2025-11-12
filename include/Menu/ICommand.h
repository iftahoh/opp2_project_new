#pragma once

/*
ICommand interface representing a command in the command pattern.
*/

class	ICommand {
public:
	virtual ~ICommand() = default;
	virtual void execute() = 0;
	
};