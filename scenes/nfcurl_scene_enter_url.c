#include "../nfcurl_i.h"
#include "nfcurl_scene.h"

static void nfcurl_scene_enter_url_text_callback(void* context) {
	NfcUrlApp* app = context;
	view_dispatcher_send_custom_event(app->view_dispatcher, NfcUrlCustomEventTypeTextEditDone);
}

static void clear_text_buffer(char* buffer, uint32_t len) {
	for(unsigned i = 0; i < len; ++i) buffer[i] = '\0';
}

void nfcurl_scene_enter_url_on_enter(void* context) {
	NfcUrlApp* app = context;
	text_input_set_header_text(app->text_input, "Enter URL");
	text_input_set_result_callback(app->text_input, nfcurl_scene_enter_url_text_callback,
								   app, app->text_buffer, 200, false);
	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewTextInput);
}

bool nfcurl_scene_enter_url_on_event(void* context, SceneManagerEvent event) {
	NfcUrlApp* app = context;
	bool consumed = false;

	if(event.type == SceneManagerEventTypeCustom) {
		if(event.event == NfcUrlCustomEventTypeTextEditDone) {
			furi_string_set_str(app->url, app->text_buffer);
			scene_manager_next_scene(app->scene_manager, NfcUrlSceneCreated);
			consumed = true;
		}
	}
	return consumed;
}

void nfcurl_scene_enter_url_on_exit(void* context) {
	NfcUrlApp* app = context;
	UNUSED(clear_text_buffer);
	//clear_text_buffer(app->text_buffer, 200);
	text_input_reset(app->text_input);
}
