#pragma once

#include <gui/gui.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>

#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include "views/text_input.h"

#include <furi/core/string.h>

#include <nfc/nfc_device.h>

struct NFCUrlApp {
	SceneManager* scene_manager;
	ViewDispatcher* view_dispatcher;

	Gui* gui;
    TextInput* text_input;
	Popup* popup;
	Submenu* submenu;

    FuriString* url;
	FuriString* name;
	char* text_buffer;

	NfcDevice* device;
};

typedef struct NFCUrlApp NFCUrlApp;

typedef enum {
	NFCUrlViewTextInput,
	NFCUrlViewPopup,
	NFCUrlViewSubmenu
} InfraredView;
