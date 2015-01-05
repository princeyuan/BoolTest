#ifndef INPUT_H
#define INPUT_H

bool input1(bool a1,bool a2,bool a3,bool a4,bool a5)
{
	return a1&&!a5||(!a2||!a3||!a1)&&a4||a5;
}
bool input2(bool a,bool b,bool c,bool d,bool e)
{
	return a&&(!b||!c)&&d||e;
}

#endif
