#include <be/kernel/fs_info.h>
#include <be/storage/Volume.h>
#include <be/support/Debug.h>
#include "VolumeMenuItem.h"

VolumeMenuItem::VolumeMenuItem(BVolume* volume) : BMenuItem("",NULL)
{
	PRINT(("VolumeMenuItem::VolumeMenuItem(BVolume*)\n"));

	char volume_name[B_FILE_NAME_LENGTH];
	volume->GetName(volume_name);
	SetLabel(volume_name);
	device = volume->Device();
}

VolumeMenuItem::VolumeMenuItem(fs_info* info) : BMenuItem("",NULL)
{
	PRINT(("VolumeMenuItem::VolumeMenuItem(fs_info*)\n"));

	SetLabel(info->volume_name);
	device = info->dev;
}

VolumeMenuItem::~VolumeMenuItem()
{
	PRINT(("VolumeMenuItem::~VolumeMenuItem()\n"));
}

dev_t 
VolumeMenuItem::GetDevice()
{
	PRINT(("VolumeMenuItem::GetDevice()\n"));

	return device;
}
