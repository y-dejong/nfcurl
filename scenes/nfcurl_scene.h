#pragma once

#include <gui/scene_manager.h>

#define ADD_SCENE(name, id) NfcUrlScene##id,
typedef enum {
#include "nfcurl_scene_config.h"
	NfcUrlSceneCount
} NfcUrlScene;
#undef ADD_SCENE

extern const SceneManagerHandlers nfcurl_scene_handlers;

// Generate scene on_enter handlers declaration
#define ADD_SCENE(name, id) void nfcurl_scene_##name##_on_enter(void*);
#include "nfcurl_scene_config.h"
#undef ADD_SCENE

// Generate scene on_event handlers declaration
#define ADD_SCENE(name, id) \
    bool nfcurl_scene_##name##_on_event(void* context, SceneManagerEvent event);
#include "nfcurl_scene_config.h"
#undef ADD_SCENE

// Generate scene on_exit handlers declaration
#define ADD_SCENE(name, id) void nfcurl_scene_##name##_on_exit(void* context);
#include "nfcurl_scene_config.h"
#undef ADD_SCENE
