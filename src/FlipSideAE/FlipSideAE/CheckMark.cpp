#include <be/support/Debug.h>
#include "CheckMark.h"

CheckMark::CheckMark() :
	BBitmap(BRect(0,0,kImageWidth,kImageHeight),kImageColorSpace)
{
	SetBits(kImageBits,BitsLength(),0,kImageColorSpace);
}
