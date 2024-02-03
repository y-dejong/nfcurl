#include "../nfcurl_i.h"
#include "nfcurl_scene.h"

#include <toolbox/path.h>
#include <storage/storage.h>
#include <toolbox/stream/file_stream.h>

#define NFC_APP_FOLDER ANY_PATH("nfc")
#define NFC_APP_EXTENSION ".nfc"

enum SubmenuIndex {
	SubmenuIndexSave = 0,
	SubmenuIndexExport = 1
};

static void nfcurl_scene_save_text_callback(void* context) {
	NfcUrlApp* app = context;
	view_dispatcher_send_custom_event(app->view_dispatcher, NfcUrlCustomEventTypeTextEditDone);
}

static void clear_text_buffer(char* buffer, uint32_t len) {
	for(unsigned i = 0; i < len; ++i) buffer[i] = '\0';
}

static bool nfcurl_export_nfc_file(NfcUrlApp* app) {
    FuriString* path = furi_string_alloc_set_str(NFC_APP_FOLDER);
	path_append(path, furi_string_get_cstr(app->name));
	furi_string_cat_str(path, NFC_APP_EXTENSION);

	//TODO change the path thing to a furi_string_printf?
	bool success = nfc_device_save(app->device, furi_string_get_cstr(path));
	furi_string_free(path);
	return success;
}

static bool nfcurl_save_url(NfcUrlApp* app) {
	UNUSED(app);
	bool success = true;
    FuriString* outpath = furi_string_alloc_set_str(APP_DATA_PATH("urls"));

	Storage* storage = furi_record_open(RECORD_STORAGE);
	Stream* urlstream = file_stream_alloc(storage);
	if(!file_stream_open(urlstream, furi_string_get_cstr(outpath), FSAM_READ_WRITE, FSOM_OPEN_ALWAYS)) {
		FURI_LOG_E("nfcurl", "Error opening url save file");
		success = false;
	}

	if(!stream_insert_format(urlstream, "%s:%s\n",
							 furi_string_get_cstr(app->name),
							 furi_string_get_cstr(app->url))) {
		FURI_LOG_E("nfcurl", "Error writing url to file");
		success = false;
	}

	file_stream_close(urlstream);
	furi_string_free(outpath);
	furi_record_close(RECORD_STORAGE);
	return success;
}

void nfcurl_scene_save_on_enter(void* context) {
	NfcUrlApp* app = context;
	text_input_set_header_text(app->text_input, "Enter Name");
    text_input_set_result_callback(app->text_input, nfcurl_scene_save_text_callback,
							   app, app->text_buffer, 200, true);

	popup_set_header(app->popup, "Saved", 95, 26, AlignCenter, AlignBottom);
	popup_set_text(app->popup, furi_string_get_cstr(app->name), 95, 30, AlignCenter, AlignTop);
	//scene_manager_set_scene_state(app->scene_manager, NfcUrlSceneSave, 0);
	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewTextInput);
}

bool nfcurl_scene_save_on_event(void* context, SceneManagerEvent event) {
	NfcUrlApp* app = context;
    bool consumed = false;
	bool success = false;

	if(event.type == SceneManagerEventTypeCustom) {
		if(event.event == NfcUrlCustomEventTypeTextEditDone) {
			furi_string_set_str(app->name, app->text_buffer);
			uint32_t index = scene_manager_get_scene_state(app->scene_manager, NfcUrlSceneOptions);
			if(index == SubmenuIndexSave){
				success = nfcurl_save_url(app);
				consumed = true;
			} else if(index == SubmenuIndexExport) {
				success = nfcurl_export_nfc_file(app);
				consumed = true;
			}
			popup_set_header(app->popup, (success ? "Saved": "Save Failed"), 95, 26, AlignCenter, AlignBottom);
		    view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewPopup);
			//scene_manager_set_scene_state(app->scene_manager, NfcUrlSceneSave, 1);
			consumed = true;
		}
	}
	return consumed;
}

void nfcurl_scene_save_on_exit(void* context) {
	NfcUrlApp* app = context;
    clear_text_buffer(app->text_buffer, 200);
	text_input_reset(app->text_input);
}
