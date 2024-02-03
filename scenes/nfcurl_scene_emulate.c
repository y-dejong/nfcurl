#include "../nfcurl_i.h"
#include <nfcurl_icons.h>

#include <nfc/protocols/mf_ultralight/mf_ultralight.h>

void nfcurl_scene_emulate_on_enter(void* context) {
	NfcUrlApp* app = context;
	widget_add_icon_element(app->widget, 0, 3, &I_NFC_dolphin_emulation_47x61);
	widget_add_string_element(app->widget, 90, 13, AlignCenter, AlignTop, FontPrimary, "Emulating...");

	const MfUltralightData* data = nfc_device_get_data(app->device, NfcProtocolMfUltralight);
	app->listener = nfc_listener_alloc(app->nfc, NfcProtocolMfUltralight, data);
	nfc_listener_start(app->listener, NULL, NULL);

	view_dispatcher_switch_to_view(app->view_dispatcher, NfcUrlViewWidget);
	notification_message(app->notifications, &sequence_blink_start_magenta);
}

bool nfcurl_scene_emulate_on_event(void* context, SceneManagerEvent event) {
	UNUSED(context);
	UNUSED(event);
	return false;
}

void nfcurl_scene_emulate_on_exit(void* context) {
	NfcUrlApp* app = context;

	nfc_listener_stop(app->listener);
    nfc_listener_free(app->listener);

	widget_reset(app->widget);
	notification_message(app->notifications, &sequence_blink_stop);
	UNUSED(app);
}
