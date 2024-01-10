#pragma once

#include <gui/gui.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>

#include <gui/modules/text_input.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>

#include <furi/core/string.h>

struct NFCUrlApp {
	SceneManager* scene_manager;
	ViewDispatcher* view_dispatcher;

	Gui* gui;
    TextInput* text_input;
	Popup* popup;
	Submenu* submenu;

    char prefix;
    FuriString* url;
	FuriString* name;
	char* text_buffer;

	uint8_t* uid;
    uint8_t* data;
};

typedef struct NFCUrlApp NFCUrlApp;

typedef enum {
	NFCUrlViewTextInput,
	NFCUrlViewPopup,
	NFCUrlViewSubmenu
} InfraredView;
