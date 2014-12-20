/*
 * =====================================================================================
 *
 *	Filename:  label.h
 *
 * 	Description:  the file of label header
 *
 *	Version:  1.0
 *	Created:  12/28/11 20:30:10
 *	Revision:  none
 * 	Compiler:  gcc
 *
 *	Author:  fangyunjiang (fang), 42550564@qq.com
 *	Company:  star-net
 *
 * =====================================================================================
 */

#ifndef _LABEL_H_2012_
#define _LABEL_H_2012_

#include "win.h"

typedef struct _LABEL
{
	ALL_HAVE_ATRRIBUTE
	int lines;
	char **value;
}LABEL, *PLABEL;


LABEL *create_label(LABEL *label,  void *parent, char *value);
LABEL *create_label_ex(LABEL *label,   void *parent, char *value, int x, int y, int w, int h);
int create_simple_label( void *parent, char *value, int x, int y);
int delete_label(LABEL *label);
int show_label(LABEL *label);

#endif /*_LABEL_H_2012_*/

