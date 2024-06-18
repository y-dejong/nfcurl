#include "../nfcurl_i.h"
#include "gui/modules/button_menu.h"
#include "gui/scene_manager.h"
#include "input/input.h"
#include "nfcurl_scene.h"

#define PREFIX_COUNT 9
const char* prefix_names[] = {
	"None",
	"http://",
	"https://",
	"tel:",
	"mailto:",
	"ftp://",
	"sftp://",
	"smb://",
	"telnet://"
};

const char prefixes[] = {
	0, 3, 4, 5, 6, 0x0d, 0x0a, 0x0b, 0x10
};

static void nfcurl_scene_prefix_button_menu_callback(void* context, int32_t index, InputType type) {
	NfcUrlApp* app = context;
	if(type == InputTypeRelease || type == InputTypeShort) {
		app->prefix = prefixes[index];
		view_dispatcher_send_custom_event(app->view_dispatcher, index);
	}
}

void nfcurl_scene_prefix_on_enter(void* context) {
	NfcUrlApp* app = context;
	for(int32_t i = 0; i < PREFIX_COUNT; ++i) {
		button_menu_add_item(app->button_menu,
							 prefix_names[i],
							 i,
							 nfcurl_scene_prefix_button_menu_callback,
							 ButtonMenuItemTypeCommon,
							 app);
	}
	button_menu_set_header(app->button_menu, "URL Prefix:");
	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewButtonMenu);
}

bool nfcurl_scene_prefix_on_event(void* context, SceneManagerEvent event) {
	NfcUrlApp* app = context;
	bool consumed = false;
	if(event.type == SceneManagerEventTypeCustom) {
		scene_manager_next_scene(app->scene_manager, NfcUrlSceneEnterUrl);
	}
	return consumed;
}

void nfcurl_scene_prefix_on_exit(void* context) {
	NfcUrlApp* app = context;
    button_menu_reset(app->button_menu);
}
