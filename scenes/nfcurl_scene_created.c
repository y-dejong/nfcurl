#include "../nfcurl_i.h"
#include "nfcurl_scene.h"

static void nfcurl_scene_created_widget_callback(GuiButtonType result, InputType type, void* context) {
	NfcUrlApp* app = context;
	if(type == InputTypeShort) {
		view_dispatcher_send_custom_event(app->view_dispatcher, result);
	}
}

void nfcurl_scene_created_on_enter(void* context) {
	NfcUrlApp* app = context;

	nfcurl_create_tag(app);

    widget_add_text_box_element(app->widget, 0, 0, 128, 23, AlignCenter, AlignCenter, "NFC Device Created", false);
	widget_add_text_box_element(app->widget, 0, 26, 128, 23, AlignCenter, AlignCenter, furi_string_get_cstr(app->path), false);
	widget_add_button_element(app->widget, GuiButtonTypeLeft, "Exit", nfcurl_scene_created_widget_callback, app);
	widget_add_button_element(app->widget, GuiButtonTypeRight, "Use", nfcurl_scene_created_widget_callback, app);
	widget_add_button_element(app->widget, GuiButtonTypeCenter, "Options", nfcurl_scene_created_widget_callback, app);

	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewWidget);
}

bool nfcurl_scene_created_on_event(void* context, SceneManagerEvent event) {
	NfcUrlApp* app = context;
	bool consumed = false;
	if(event.type == SceneManagerEventTypeCustom) {
		switch(event.event) {
	    case GuiButtonTypeLeft:
		    scene_manager_stop(app->scene_manager);
			view_dispatcher_stop(app->view_dispatcher);
		    consumed = true;
			break;
		case GuiButtonTypeRight:
		    scene_manager_next_scene(app->scene_manager, NfcUrlSceneEmulate);
			consumed = true;
			break;
		case GuiButtonTypeCenter:
			scene_manager_next_scene(app->scene_manager, NfcUrlSceneOptions);
			consumed = true;
			break;
		}
	}
	return consumed;
}

void nfcurl_scene_created_on_exit(void* context) {
	NfcUrlApp* app = context;
	widget_reset(app->widget);
}
