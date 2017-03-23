#pragma once
class SwebPage
{
public:
	SwebPage();
	~SwebPage();

	struct sWebPage
	{
		int size_haeder;
		int size_array;
		int size_body;
		char *header;
		char *array;
		char *body;
	} *page;

	//a
private:

	
};
