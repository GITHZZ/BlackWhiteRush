//
//  GameSoundResources.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-2-16.
//
//

#ifndef BlackWhiteRush_SoundResources_h
#define BlackWhiteRush_SoundResources_h

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define S_BACKGROUND   "main_theme.mp3"
#define S_JUMP         "jump.mp3"
#define S_DOUBLE_JUMP  "double_jump.mp3"
#define S_FEVER_START  "fever_start.mp3"
#define S_FEVER_END    "fever_end.mp3"
#define S_FEVER_1      "ingame_fever_1.mp3"
#define S_WAVE         "Wave.wav"
#define S_SPRINT       "sprint.wav"
#define S_BLOOD        "blood.wav"
#define S_GAIN         "gain.wav"
#define S_SELECT       "select.mp3"
#define S_OK           "ok.mp3"
#define S_RECIPROCAL   "reciprocal.wav"

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define S_BACKGROUND   "SingleBgMusic2.mp3"
#define S_JUMP         "jump.mp3"
#define S_FEVER_START  "fever_start.mp3"
#define S_FEVER_END    "fever_end.mp3"
#define S_FEVER_1      "ingame_fever_1.mp3"
#define S_SELECT       "select.mp3"
#define S_OK           "ok.mp3"
#endif

#endif
