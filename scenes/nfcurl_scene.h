#pragma once

#include <gui/scene_manager.h>

typedef enum {
	NFCUrlSceneInputInfo,
	NFCUrlSceneDoneMenu,
	NFCUrlSceneCount
} NFCUrlScene;

extern const SceneManagerHandlers nfcurl_scene_handlers;

void nfcurl_scene_inputinfo_on_enter(void* context);
bool nfcurl_scene_inputinfo_on_event(void* context, SceneManagerEvent event);
void nfcurl_scene_inputinfo_on_exit(void* context);

void nfcurl_scene_donemenu_on_enter(void* context);
bool nfcurl_scene_donemenu_on_event(void* context, SceneManagerEvent event);
void nfcurl_scene_donemenu_on_exit(void* context);
