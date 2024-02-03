#include "../nfcurl_i.h"
#include "nfcurl_scene.h"

enum SubmenuIndex {
	SubmenuIndexNew,
	SubmenuIndexSaved
};

void nfcurl_scene_start_submenu_callback(void* context, uint32_t index) {
	NfcUrlApp* app = context;
	view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void nfcurl_scene_start_on_enter(void* context) {
	NfcUrlApp* app = context;
	Submenu* submenu = app->submenu;

	submenu_add_item(submenu, "New", SubmenuIndexNew, nfcurl_scene_start_submenu_callback, app);
	submenu_add_item(submenu, "Saved", SubmenuIndexNew, nfcurl_scene_start_submenu_callback, app);

	submenu_set_selected_item(
		submenu, scene_manager_get_scene_state(app->scene_manager, NfcUrlSceneStart));
	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewSubmenu);
}

bool nfcurl_scene_start_on_event(void* context, SceneManagerEvent event) {
	NfcUrlApp* app = context;
	bool consumed = false;

	if(event.type == SceneManagerEventTypeCustom) {
		if(event.event == SubmenuIndexNew) {
			scene_manager_next_scene(app->scene_manager, NfcUrlSceneEnterUrl);
			consumed = true;
		} else if(event.event == SubmenuIndexSaved) {
			//scene_manager_next_scene(app->scene_manager, NfcUrlSceneSavedList);
			//consumed = true;
		}
		scene_manager_set_scene_state(app->scene_manager, NfcUrlSceneStart, event.event);
	}

	return consumed;
}

void nfcurl_scene_start_on_exit(void* context) {
	NfcUrlApp* app = context;
	submenu_reset(app->submenu);
}
