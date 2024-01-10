#include "../nfcurl_i.h"
#include "nfcurl_scene.h"

static void nfcurl_scene_inputinfo_text_callback(void* context) {
	NFCUrlApp* nfcurl = context;
	if(!nfcurl->url) {
		nfcurl->url = furi_string_alloc_set_str(nfcurl->text_buffer);
		furi_string_cat_str(nfcurl->url, ".com");
		text_input_set_header_text(nfcurl->text_input, "Enter Name");
	    for(unsigned i = 0; i < 200; ++i) nfcurl->text_buffer[i] = '\0';
	} else {
		nfcurl->name = furi_string_alloc_set_str(nfcurl->text_buffer);
		scene_manager_next_scene(nfcurl->scene_manager, NFCUrlSceneDoneMenu);
	}
}

void nfcurl_scene_inputinfo_on_enter(void* context) {
    NFCUrlApp* nfcurl = context;
	TextInput* text_input = nfcurl->text_input;
	text_input_set_header_text(text_input, "Enter URL");

	view_dispatcher_switch_to_view(nfcurl->view_dispatcher, NFCUrlViewTextInput);

	text_input_set_result_callback(nfcurl->text_input, nfcurl_scene_inputinfo_text_callback,
								   nfcurl, nfcurl->text_buffer, 200, true);
}

bool nfcurl_scene_inputinfo_on_event(void* context, SceneManagerEvent event) {
	UNUSED(context);
	UNUSED(event);
	return false;
}

void nfcurl_scene_inputinfo_on_exit(void* context) {
	UNUSED(context);
}
