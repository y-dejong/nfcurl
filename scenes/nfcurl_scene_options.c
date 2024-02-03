#include "../nfcurl_i.h"
#include "nfcurl_scene.h"

enum SubmenuIndex {
	SubmenuIndexSave = 0,
	SubmenuIndexExport = 1
};

void nfcurl_scene_options_submenu_callback(void* context, uint32_t index) {
	NfcUrlApp* app = context;
	view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void nfcurl_scene_options_on_enter(void* context) {
	NfcUrlApp* app = context;
    submenu_add_item(app->submenu, "Save to URL List", SubmenuIndexSave, nfcurl_scene_options_submenu_callback, app);
	submenu_add_item(app->submenu, "Export to NFC File", SubmenuIndexExport, nfcurl_scene_options_submenu_callback, app);

	submenu_set_selected_item(
		app->submenu, scene_manager_get_scene_state(app->scene_manager, NfcUrlSceneCreated));
	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewSubmenu);
}

bool nfcurl_scene_options_on_event(void* context, SceneManagerEvent event) {
	NfcUrlApp* app = context;
	bool consumed = false;

	if(event.type == SceneManagerEventTypeCustom) {
		if(event.event == SubmenuIndexSave || event.event == SubmenuIndexExport) {
			scene_manager_next_scene(app->scene_manager, NfcUrlSceneSave);
			consumed = true;
		}
		scene_manager_set_scene_state(app->scene_manager, NfcUrlSceneOptions, event.event);
	}
	return consumed;
}

void nfcurl_scene_options_on_exit(void* context) {
	NfcUrlApp* app = context;
	submenu_reset(app->submenu);
}
