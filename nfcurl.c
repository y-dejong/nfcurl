#include "nfcurl_i.h"
#include "scenes/nfcurl_scene.h"

#include <furi.h>

static bool nfcurl_custom_event_callback(void* context, uint32_t event) {
	furi_assert(context);
    NFCUrlApp* nfcurl = context;
	return scene_manager_handle_custom_event(nfcurl->scene_manager, event);
}

static bool nfcurl_back_event_callback(void* context) {
	furi_assert(context);
	NFCUrlApp* nfcurl = context;
	return scene_manager_handle_back_event(nfcurl->scene_manager);
}

static NFCUrlApp* nfcurl_alloc() {
	NFCUrlApp* nfcurl = malloc(sizeof(NFCUrlApp));

    nfcurl->scene_manager = scene_manager_alloc(&nfcurl_scene_handlers, nfcurl);

	nfcurl->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(nfcurl->view_dispatcher);
	view_dispatcher_set_event_callback_context(nfcurl->view_dispatcher, nfcurl);
	view_dispatcher_set_custom_event_callback(nfcurl->view_dispatcher, nfcurl_custom_event_callback);
	view_dispatcher_set_navigation_event_callback(nfcurl->view_dispatcher, nfcurl_back_event_callback);

	nfcurl->text_input = text_input_alloc();
	view_dispatcher_add_view(nfcurl->view_dispatcher, NFCUrlViewTextInput, text_input_get_view(nfcurl->text_input));

	nfcurl->popup = popup_alloc();
	view_dispatcher_add_view(nfcurl->view_dispatcher, NFCUrlViewPopup, popup_get_view(nfcurl->popup));

	nfcurl->submenu = submenu_alloc();
	view_dispatcher_add_view(nfcurl->view_dispatcher, NFCUrlViewSubmenu, submenu_get_view(nfcurl->submenu));

	nfcurl->url = furi_string_alloc();
	nfcurl->name = furi_string_alloc();
	nfcurl->text_buffer = malloc(200);

	return nfcurl;
}

static void nfcurl_free(NFCUrlApp* nfcurl) {
	view_dispatcher_remove_view(nfcurl->view_dispatcher, NFCUrlViewTextInput);
	text_input_free(nfcurl->text_input);
	view_dispatcher_remove_view(nfcurl->view_dispatcher, NFCUrlViewPopup);
    popup_free(nfcurl->popup);

	furi_record_close(RECORD_GUI);
	view_dispatcher_free(nfcurl->view_dispatcher);
	scene_manager_free(nfcurl->scene_manager);

	furi_string_free(nfcurl->url);
	furi_string_free(nfcurl->name);
	free(nfcurl->text_buffer);
	free(nfcurl);
}

int32_t nfcurl_app(void* p) {
	UNUSED(p);
    NFCUrlApp* nfcurl = nfcurl_alloc();

	nfcurl->gui = furi_record_open(RECORD_GUI);
	view_dispatcher_attach_to_gui(nfcurl->view_dispatcher, nfcurl->gui, ViewDispatcherTypeFullscreen);
	scene_manager_next_scene(nfcurl->scene_manager, NFCUrlSceneInputInfo);
	view_dispatcher_run(nfcurl->view_dispatcher);

	nfcurl_free(nfcurl);
    return 0;
}
