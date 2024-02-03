#include "nfcurl_scene.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(name, id) nfcurl_scene_##name##_on_enter,
void (*const nfcurl_on_enter_handlers[])(void*) = {
#include "nfcurl_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(name, id) nfcurl_scene_##name##_on_event,
bool (*const nfcurl_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "nfcurl_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(name, id) nfcurl_scene_##name##_on_exit,
void (*const nfcurl_on_exit_handlers[])(void* context) = {
#include "nfcurl_scene_config.h"
};
#undef ADD_SCENE

const SceneManagerHandlers nfcurl_scene_handlers = {
	.on_enter_handlers = nfcurl_on_enter_handlers,
	.on_event_handlers = nfcurl_on_event_handlers,
	.on_exit_handlers = nfcurl_on_exit_handlers,
	.scene_num = NfcUrlSceneCount
};
