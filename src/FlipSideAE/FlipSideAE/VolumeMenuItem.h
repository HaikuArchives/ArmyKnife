#ifndef __VOLUME_MENU_ITEM_H__
#define __VOLUME_MENU_ITEM_H__

#include <be/interface/MenuItem.h>

class BMessage;
class BVolume;
struct fs_info;

class VolumeMenuItem : public BMenuItem 
{
	public:
		VolumeMenuItem(BVolume* volume);
		VolumeMenuItem(fs_info* info);
		~VolumeMenuItem();
		dev_t GetDevice();
	private:
		dev_t device;
};

#endif
