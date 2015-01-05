#ifndef TCAS_H
#define TCAS_H

bool TCAS1(bool a,bool b,bool c,bool d,bool e)
{
	return a&&(!b||!c)&&d||e;
}
bool TCAS2(bool a,bool b,bool c,bool d,bool e,bool f,bool h)
{
	return !(a&&b)&&(d&&!e&&f||!d&&e&&!f||!d&&!e&&!f)&&(a&&c&&(d||e)&&h||a&&(d||e)&&!h||b&&(e||f));
}
bool TCAS3(bool a,bool b,bool c,bool d,bool e,bool f,bool g)
{
	return !(c&&d)&&(!e&&f&&!g&&!a&&(b&&c||!b&&d));
}
bool TCAS4(bool a,bool b,bool c,bool d,bool e,bool f,bool h)
{
	return a&&c&&(d||e)&&h||a&&(d||e)&&!h||b&&(e||f);
}
bool TCAS5(bool a,bool b,bool c,bool d,bool e,bool f,bool g)
{
	return !e&&f&&!g&&!a&&(b&&c||!b&&d);
}
bool TCAS6(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h)
{
	return (!a&&b||a&&!b)&&!(c&&d)&&!(g&&h)&&((a&&c||b&&d)&&e&&(f&&g||!f&&h));
}
bool TCAS7(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h)
{
	return (a&&c||b&&d)&&e&&(f&&g||!f&&h);
}
bool TCAS8(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i)
{
	return (a&&((c||d||e)&&g||a&&f||c&&(f||g||h||i))||(a||b)&&(c||d||e)&&i)&&!(a&&b)&&!(c&&d)&&!(c&&e)&&!(d&&e)&&!(f&&g)&&!(f&&h)&&!(f&&i)&&!(g&&h)&&!(h&&i);
}
bool TCAS9(bool a,bool b,bool c,bool f,bool g,bool h,bool i,bool k,bool l)
{
	return a&&(!b||!c||b&&c&&!(!f&&g&&h&&!i||!g&&h&&i)&&!(!f&&g&&l&&k||!g&&!i&&k))||f;
}
bool TCAS10(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i)
{
	return a&&((c||d||e)&&g||a&&f||c&&(f||g||h||i))||(a||b)&&(c||d||e)&&i;
}
bool TCAS11(bool a,bool b,bool c,bool d,bool e,bool g,bool h,bool i,bool j,bool k)
{
	return (!a&&b||a&&!b)&&!(c&&d)&&!(g&&h)&&!(i&&j)&&((a&&c||b&&d)&&e&&(!i||!g&&!k||!j&&(!h||!k)));
}
bool TCAS12(bool a,bool b,bool c,bool d,bool e,bool g,bool h,bool i,bool j,bool k)
{
	return (a&&c||b&&d)&&e&&(!i||!g&&!k||!j&&(!h||!k));
}
bool TCAS13(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool j,bool k)
{
	return (!a&&b||a&&!b)&&!(c&&d)&&(f&&!g&&!h||!f&&g&&!h||!f&&!g&&!h)&&!(j&&k)&&((a&&c||b&&d)&&e&&(f||(i&&(g&&j||h&&k))));
}
bool TCAS14(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool j,bool k)
{
	return (a&&c||b&&d)&&e&&(f||(i&&(g&&j||h&&k)));
}
bool TCAS15(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool k,bool l,bool m)
{
	return (a&&(!d||!e||d&&e&&!(!f&&g&&h&&!i||!g&&h&&i)&&(!f&&g&&l&&k||!g&&!i&&k))||!(!f&&g&&h&&!i)&&!(!f&&g&&l&&k||!g&&!i&&k)&&(b||c&&!m||f))&&(a&&!b&&!c||!a&&b&&!c||!a&&!b&&c);
}
bool TCAS16(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool j,bool k,bool l)
{
	return a||b||c||!c&&!d&&e&&f&&!g&&!h||i&&(j||k)&&!l;
}
bool TCAS17(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool k,bool l,bool m)
{
	return (a&&(!d||!e||d&&e&&!(!f&&g&&h&&!i||!g&&h&&i)&&(!f&&g&&l&&k||!g&&!i&&k))||!(!f&&g&&h&&!i)&&!(!f&&g&&l&&k||!g&&!i&&k)&&(b||c&&!m||f));
}
bool TCAS18(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool j,bool k,bool l,bool m)
{
	return a&&!b&&!c&&d&&!e&&f&&(g||!g&&(h||i))&&!(j&&k||!j&&l||m);
}
bool TCAS19(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool j,bool k,bool l,bool m)
{
	return a&&!b&&!c&&(!(f&&(g||!g&&(h||i)))||f&&(g||!g&&(h||i))&&!d&&!e)&&!(j&&k||!j&&l&&!m);
}
bool TCAS20(bool a,bool b,bool c,bool d,bool e,bool f,bool g,bool h,bool i,bool j,bool k,bool l,bool m,bool n)
{
	return a&&!b&&!c&&(f&&(g||!g&&(h||i))&&(!e&&!n||d)||!n&&(j&&k||!j&&l&&!m));
}

#endif
