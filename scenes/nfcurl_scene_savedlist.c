#include "gui/modules/button_menu.h"
#include "nfcurl_i.h"
#include "nfcurl_scene.h"

#include <storage/storage.h>
#include <toolbox/stream/file_stream.h>

// TODO: ****** migrate to use toolbox/key_dict.h?

static void nfcurl_scene_savedlist_button_menu_callback(void* context, int32_t index, InputType type) {
	NfcUrlApp* app = context;
	if(type == InputTypeRelease || type == InputTypeShort) {
		view_dispatcher_send_custom_event(app->view_dispatcher, index);
	}
}

void nfcurl_scene_savedlist_on_enter(void* context) {
	NfcUrlApp* app = context;
	Storage* storage = furi_record_open(RECORD_STORAGE);
	Stream* urlstream = file_stream_alloc(storage);
	if(!file_stream_open(urlstream, APP_DATA_PATH("urls"), FSAM_READ, FSOM_OPEN_EXISTING)) {
		FURI_LOG_E("nfcurl", "Error opening url save file");
	    goto cleanup;
	}

	FuriString* line = furi_string_alloc();
	size_t split;
	size_t index = 0;
	while(stream_read_line(urlstream, line) && index < 100) { // TODO: Don't have a hardcoded maximum
		split = furi_string_search_char(line, ':', 0);
		if (split == FURI_STRING_FAILURE) continue;
		app->urlpairs[index].name = furi_string_alloc();
		app->urlpairs[index].url = furi_string_alloc();
	    furi_string_set_n(app->urlpairs[index].name, line, 0, split);
		furi_string_set_n(app->urlpairs[index].url, line,
						  split + 1, furi_string_size(line) - split - 1);
		button_menu_add_item(app->button_menu,
							 furi_string_get_cstr(app->urlpairs[index].name),
							 index,
							 nfcurl_scene_savedlist_button_menu_callback,
							 ButtonMenuItemTypeCommon,
							 app);
		++index;
	}
	app->urlpairs_count = index;
	furi_string_free(line);

	button_menu_set_header(app->button_menu, "Saved URLs:");
	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewButtonMenu);
 cleanup:
	file_stream_close(urlstream);
	furi_record_close(RECORD_STORAGE);
}

bool nfcurl_scene_savedlist_on_event(void* context, SceneManagerEvent event) {
	NfcUrlApp* app = context;
	bool consumed = false;
	FURI_LOG_I("nfcurl", "Scene on event");
	if(event.type == SceneManagerEventTypeCustom) {
		FURI_LOG_I("nfcurl", "EventTypeCustom");
	    furi_string_set(app->url, app->urlpairs[event.event].url);
		nfcurl_create_tag(app);
		scene_manager_next_scene(app->scene_manager, NfcUrlSceneEmulate);
		consumed = true;
	}
	return consumed;
}

void nfcurl_scene_savedlist_on_exit(void* context) {
	NfcUrlApp* app = context;
	button_menu_reset(app->button_menu);

	for(size_t i = 0; i < app->urlpairs_count; ++i) {
		furi_string_free(app->urlpairs[i].name);
		furi_string_free(app->urlpairs[i].url);
	}
	app->urlpairs_count = 0;
}
