#pragma once
#include "SwebPage.h"
// pointer na pamat, ktoru ma naplnit webstrankou
// Struct SwebPage *Page
class LoadPage_Interface
{
public:
	LoadPage_Interface(struct sWebPage *page);
	~LoadPage_Interface();
	
private:

	void set_parameter(char *type, char *value);
};

