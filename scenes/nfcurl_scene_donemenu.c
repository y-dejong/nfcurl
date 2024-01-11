#include "../nfcurl_i.h"

#include <toolbox/path.h>

#include <nfc/helpers/nfc_data_generator.h>
#include <nfc/protocols/mf_ultralight/mf_ultralight.h>

#define NFC_APP_FOLDER ANY_PATH("nfc")
#define NFC_APP_EXTENSION ".nfc"

enum SubmenuIndex {
	SubmenuIndexEmulate,
	SubmenuIndexSave
};

static void nfcurl_create_tag(void* context, uint32_t index) {
	UNUSED(index);
	NFCUrlApp* nfcurl = context;
	nfcurl->device  = nfc_device_alloc();

	nfc_data_generator_fill_data(NfcDataGeneratorTypeNTAG215, nfcurl->device);
    const MfUltralightData* data_original = nfc_device_get_data(nfcurl->device, NfcProtocolMfUltralight);
	MfUltralightData* data = malloc(sizeof(MfUltralightData));
	Iso14443_3aData* isodata = malloc(sizeof(Iso14443_3aData));
	memcpy(isodata, data_original->iso14443_3a_data, sizeof(Iso14443_3aData));
	memcpy(data, data_original, sizeof(MfUltralightData));
	data->iso14443_3a_data = isodata;

	// User data
	data->page[4].data[0] = 0x03; //TLV open
	data->page[4].data[2] = 0xc1; // NDEF header
	data->page[4].data[3] = 0x01; // Size of type
	size_t url_size = furi_string_size(nfcurl->url) + 1; //url + prefix size
	unsigned page_index, data_index;
	if(url_size < 254) {
		data->page[4].data[2] |= 0x10;
		data->page[5].data[0] = url_size;
		data->page[5].data[1] = 'U'; // RTD well-known type
		data->page[5].data[2] = 0x04; // URL prefix TODO let user select
	    page_index = 5;
		data_index = 3;
	} else {
		data->page[5].data[0] = url_size;
		data->page[5].data[1] = url_size >> 8;
		data->page[5].data[2] = url_size >> 16;
		data->page[5].data[3] = 'U';
		data->page[6].data[0] = 0x04;
		page_index = 6;
		data_index = 1;
	}

	const char* urlstr = furi_string_get_cstr(nfcurl->url);
	for(size_t i = 0; i < url_size-1; ++i) {
		data->page[page_index].data[data_index++] = *urlstr++;
	    if(data_index > 3) {
			data_index = 0;
			page_index++;
		}
	}
    data->page[page_index].data[data_index++] = 0xfe;
	data->page[4].data[1] = page_index*4 + data_index - (4*4 + 1) - 2; //TODO check math
	nfc_device_set_data(nfcurl->device, NfcProtocolMfUltralight, data);
    free(data);
	free(isodata);

	FuriString* path = furi_string_alloc_set_str(NFC_APP_FOLDER);
	path_append(path, furi_string_get_cstr(nfcurl->name));
	furi_string_cat_str(path, NFC_APP_EXTENSION);
	bool success = nfc_device_save(nfcurl->device, furi_string_get_cstr(path));

	//TODO make this a different function or just send an event
	// Result popup
	popup_set_context(nfcurl->popup, nfcurl);
	popup_set_header(nfcurl->popup, (success ? "NFC device created" : "Device creation failed"),
					 0, 0, AlignCenter, AlignCenter);
	popup_set_timeout(nfcurl->popup, 3000);
	view_dispatcher_switch_to_view(nfcurl->view_dispatcher, NFCUrlViewPopup);
	popup_enable_timeout(nfcurl->popup);
}

void nfcurl_scene_donemenu_on_enter(void* context) {
	NFCUrlApp* nfcurl = context;
	Submenu* submenu = nfcurl->submenu;

	submenu_add_item(submenu, "Emulate", SubmenuIndexEmulate, NULL, nfcurl);
	submenu_add_item(submenu, "Save", SubmenuIndexSave, nfcurl_create_tag, nfcurl);
	submenu_set_selected_item(submenu, SubmenuIndexEmulate);

	view_dispatcher_switch_to_view(nfcurl->view_dispatcher, NFCUrlViewSubmenu);
}

bool nfcurl_scene_donemenu_on_event(void* context, SceneManagerEvent event) {
	UNUSED(context);
	UNUSED(event);
	return false;
}

void nfcurl_scene_donemenu_on_exit(void* context) {
	NFCUrlApp* nfcurl = context;
	submenu_reset(nfcurl->submenu);
}
