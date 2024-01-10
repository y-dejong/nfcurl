#include "nfcurl_scene.h"

void (*const nfcurl_on_enter_handlers[])(void*) = {
	nfcurl_scene_inputinfo_on_enter,
	nfcurl_scene_donemenu_on_enter,
};

bool (*const nfcurl_on_event_handlers[])(void* context, SceneManagerEvent event) = {
	nfcurl_scene_inputinfo_on_event,
	nfcurl_scene_donemenu_on_event,
};

void (*const nfcurl_on_exit_handlers[])(void*) = {
	nfcurl_scene_inputinfo_on_exit,
	nfcurl_scene_donemenu_on_exit
};

const SceneManagerHandlers nfcurl_scene_handlers = {
	.on_enter_handlers = nfcurl_on_enter_handlers,
	.on_event_handlers = nfcurl_on_event_handlers,
	.on_exit_handlers = nfcurl_on_exit_handlers,
	.scene_num = NFCUrlSceneCount
};
