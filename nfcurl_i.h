#pragma once

#include <gui/gui.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>

#include <gui/modules/widget.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include "views/text_input.h"

#include <furi/core/string.h>

#include <nfc/nfc_device.h>
#include <nfc/nfc_listener.h>

#include <notification/notification_messages.h>

struct NfcUrlApp {
	SceneManager* scene_manager;
	ViewDispatcher* view_dispatcher;

	Gui* gui;
	Widget* widget;
    TextInput* text_input;
	Popup* popup;
	Submenu* submenu;

    FuriString* url;
	FuriString* name;
	FuriString* path;
	char* text_buffer;

	Nfc* nfc;
	NfcDevice* device;
	NfcListener* listener;

	NotificationApp* notifications;
};

typedef struct NfcUrlApp NfcUrlApp;

typedef enum {
	NfcUrlViewTextInput,
	NfcUrlViewPopup,
	NfcUrlViewSubmenu,
	NfcUrlViewWidget
} NfcUrlView;

enum NfcUrlCustomEventType{
	NfcUrlCustomEventTypeTextEditDone
};

void nfcurl_create_tag(NfcUrlApp* app);
