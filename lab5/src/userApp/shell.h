#ifndef __SHELL_H__
#define __SHELL_H__

#include "../myOS/userInterface.h"

//#define NULL (void*)0
#define SIZE 10

int getCmdline(unsigned char *buf, int limit);

int listCmds();

void addNewCmd(	unsigned char *cmd, 
		int (*func)(int argc, unsigned char **argv), 
		void (*help_func)(void), 
		unsigned char* description);

void help_help(void);

int help(int argc, unsigned char **argv);

int split2Words(unsigned char *cmdline, unsigned char **argv, int limit);

void initShell(void);

void startShell(void);

#endif