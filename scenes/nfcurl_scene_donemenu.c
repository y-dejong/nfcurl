#include "../nfcurl_i.h"

#include <storage/storage.h>
#include <flipper_format/flipper_format.h>
#include <toolbox/path.h>

#define NFC_APP_FOLDER ANY_PATH("nfc")
#define NFC_APP_EXTENSION ".nfc"

enum SubmenuIndex {
	SubmenuIndexEmulate,
	SubmenuIndexSave
};

static void nfcurl_generate_data(NFCUrlApp* nfcurl) {
    uint8_t* data = nfcurl->data;
    uint8_t* uid = nfcurl->uid;
	data[0] = uid[0];
	data[1] = uid[1];
	data[2] = uid[2];
	data[3] = uid[0] ^ uid[1] ^ uid[2] ^ 0x88;
	data[4] = uid[3];
	data[5] = uid[4];
	data[6] = uid[5];
    data[7] = uid[6];
	data[8] = uid[3] ^ uid[4] ^ uid[5] ^ uid[6];
	data[9] = 0x48;
	data[10] = 0x00;
	data[11] = 0x00;
	data[12] = 0xe1;
	data[13] = 0x10;
	data[14] = 0x3e;
	data[15] = 0x00;

	// User data
    uint8_t* cursor = data + 16;
	*cursor++ = 0x03; //TLV open
    uint8_t* messagesize = cursor++; //Save TLV size pointer for later
	*cursor++ = 0xc1;
	*cursor++ = 0x01; //Size of Type
	size_t size = furi_string_size(nfcurl->url) + 1; // +1 for prefix
	if(size < 254) {
		*(cursor-2) |= 0x10;
		*cursor++ = size;
	} else {
		for(unsigned i = 0; i < 3; ++i) *cursor++ = size >> (8*i);
	}
	*cursor++ = 'U'; // RTD Well-known type
	nfcurl->prefix = 0x04; // TODO replace with menu
	*cursor++ = nfcurl->prefix;
	const char* urlstr = furi_string_get_cstr(nfcurl->url);
	for(size_t i = 0; i < size-1; ++i) *cursor++ = *urlstr++;
	*cursor++ = 0xfe;
	*messagesize = cursor - messagesize - 2; // TODO check accuracy
    uint8_t* dataend = nfcurl->data + 540;
	while(cursor < dataend) *cursor++ = '\0';

	// Footer data and dynamic lock
	cursor = data + 130*4 + 3;
	const uint8_t footerdata[] = {0xbd, 0x04, 0x00, 0x00, 0xff,
							  0x00, 0x05, 0x00, 0x00, 0xff,
							  0xff, 0xff, 0xff};
	for(unsigned i = 0; i < 13; ++i) {
		cursor[i] = footerdata[i];
	}
}

static void nfcurl_create_tag(void* context, uint32_t index) {
	UNUSED(index);
	NFCUrlApp* nfcurl = context;
	FuriString* path = furi_string_alloc_set_str(NFC_APP_FOLDER);
	path_append(path, furi_string_get_cstr(nfcurl->name));
	furi_string_cat_str(path, NFC_APP_EXTENSION);
	Storage* storage = furi_record_open(RECORD_STORAGE);
	FlipperFormat* ff = flipper_format_file_alloc(storage);

    uint8_t uid[] = {0x04, 0xb7, 0x96, 0x81, 0x27, 0xc3, 0x86}; //TODO randomize last 6 bytes?
	for(unsigned i = 0; i < 7; ++i) nfcurl->uid[i] = uid[i];
    uint8_t signature[32];
	for(unsigned i = 0; i < 32; ++i) signature[i] = 0x00;
	uint8_t mifare_version[8] = {0x00, 0x04, 0x04, 0x02, 0x01, 0x00, 0x11, 0x03};
	uint8_t atqa[] = {0x00, 0x44};
	uint8_t sak[] = {0x00};

	nfcurl_generate_data(nfcurl);

	bool success = false;
	bool shouldbreak = false;
	do {
	    if(!flipper_format_file_open_always(ff, furi_string_get_cstr(path))) break;
		if(!flipper_format_write_header_cstr(ff, "Flipper NFC device", 4)) break;
		if(!flipper_format_write_string_cstr(ff, "Device type", "NTAG/Ultralight")) break;
		if(!flipper_format_write_hex(ff, "UID", nfcurl->uid, 7)) break;
		if(!flipper_format_write_hex(ff, "ATQA", atqa, 2)) break;
		if(!flipper_format_write_hex(ff, "SAK", sak, 1)) break;
		if(!flipper_format_write_string_cstr(ff, "Data format version", "2")) break;
		if(!flipper_format_write_string_cstr(ff, "NTAG/Ultralight type", "NTAG215")) break;
		if(!flipper_format_write_hex(ff, "Signature", signature, 32)) break;
		if(!flipper_format_write_hex(ff, "Mifare version", mifare_version, 8)) break;
		if(!flipper_format_write_string_cstr(ff, "Counter 0", "0")) break;
		if(!flipper_format_write_string_cstr(ff, "Tearing 0", "00")) break;
		if(!flipper_format_write_string_cstr(ff, "Counter 1", "0")) break;
		if(!flipper_format_write_string_cstr(ff, "Tearing 1", "00")) break;
		if(!flipper_format_write_string_cstr(ff, "Counter 2", "0")) break;
		if(!flipper_format_write_string_cstr(ff, "Tearing 2", "00")) break;
		if(!flipper_format_write_string_cstr(ff, "Pages total", "135")) break;
		if(!flipper_format_write_string_cstr(ff, "Pages read", "135")) break;
		FuriString* page_key = furi_string_alloc();
		for(unsigned i = 0; i < 135; ++i) {
			furi_string_printf(page_key, "Page %u", i);
			if(!flipper_format_write_hex(ff, furi_string_get_cstr(page_key), nfcurl->data + i * 4, 4)) {
				shouldbreak = true;
				break;
			}
		}
		furi_string_free(page_key);
		if(shouldbreak) break;
		if(!flipper_format_write_string_cstr(ff, "Failed authentication attempts", "0")) break;
	    success = true;
	} while(false);

	FuriString* popupheader = furi_string_alloc_set_str(success ? "NFC device created" : "Device creation failed");
	popup_set_context(nfcurl->popup, nfcurl);
	popup_set_header(nfcurl->popup, furi_string_get_cstr(popupheader),
					 30, 30, AlignCenter, AlignCenter);
	popup_set_timeout(nfcurl->popup, 3000);
	view_dispatcher_switch_to_view(nfcurl->view_dispatcher, NFCUrlViewPopup);
	popup_enable_timeout(nfcurl->popup);

	flipper_format_free(ff);
	furi_record_close(RECORD_STORAGE);
	furi_string_free(path);
	//TODO: furi_string_free(popupheader)
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
