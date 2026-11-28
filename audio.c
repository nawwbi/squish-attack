#include "cprocessing.h"
#include "audio.h"

// background music
CP_Sound main_menu_music;
CP_Sound dumpling_fight_music;
CP_Sound butter_fight_music;
CP_Sound boss_fight_music;
CP_Sound win_screen_music;
CP_Sound lose_screen_music;

// sound effects
CP_Sound button_click_sound;
CP_Sound button_hover_sound;
CP_Sound player_projectile_sound;
CP_Sound enemy_soundwave_sound;
CP_Sound boss_ultimate_sound;

// load all audio files
void Audio_Load(void)
{
    // background music
    main_menu_music = CP_Sound_LoadMusic("./Assets/main-menu.mp3");
    dumpling_fight_music = CP_Sound_LoadMusic("./Assets/dumpling-fight.mp3");
    butter_fight_music = CP_Sound_LoadMusic("./Assets/butter-fight.mp3");
    boss_fight_music = CP_Sound_LoadMusic("./Assets/boss-fight.mp3");
    win_screen_music = CP_Sound_LoadMusic("./Assets/win-screen.mp3");
    lose_screen_music = CP_Sound_LoadMusic("./Assets/lose-screen.mp3");

    // sound effects
    button_click_sound = CP_Sound_Load("./Assets/button-click.mp3");
    button_hover_sound = CP_Sound_Load("./Assets/button-hover.mp3");
    player_projectile_sound = CP_Sound_Load("./Assets/player-projectile.mp3");
    enemy_soundwave_sound = CP_Sound_Load("./Assets/enemy-soundwave.mp3");
    boss_ultimate_sound = CP_Sound_Load("./Assets/boss-ultimate.mp3");
}

// play main menu music
void Audio_Play_Main_Menu(void)
{
    CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
    CP_Sound_PlayMusic(main_menu_music);
}

// play bg music based on the current level
void Audio_Play_Level(int level)
{
    CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);

    if (level == 1)
    {
        CP_Sound_PlayMusic(dumpling_fight_music);
    }
    else if (level == 2)
    {
        CP_Sound_PlayMusic(butter_fight_music);
    }
    else if (level == 3)
    {
        CP_Sound_PlayMusic(boss_fight_music);
    }
}

// play win or lose screen music
void Audio_Play_End_Screen(bool player_won)
{
    CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);

    if (player_won == true)
    {
        CP_Sound_PlayMusic(win_screen_music);
    }
    else
    {
        CP_Sound_PlayMusic(lose_screen_music);
    }
}

// play button click sound
void Audio_Play_Button_Click(void)
{
    CP_Sound_Play(button_click_sound);
}

// play button hover sound
void Audio_Play_Button_Hover(void)
{
    CP_Sound_Play(button_hover_sound);
}

// play player projectile sound
void Audio_Play_Player_Projectile(void)
{
    CP_Sound_Play(player_projectile_sound);
}

// play enemy soundwave sound
void Audio_Play_Enemy_Soundwave(void)
{
    CP_Sound_Play(enemy_soundwave_sound);
}

// play boss ultimate sound
void Audio_Play_Boss_Ultimate(void)
{
    CP_Sound_Play(boss_ultimate_sound);
}

// stop background music only
void Audio_Stop_Music(void)
{
    CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
}

// stop all music and sound effects
void Audio_Stop_All(void)
{
    CP_Sound_StopAll();
}

// free all audio files
void Audio_Free(void)
{
    CP_Sound_Free(main_menu_music);
    CP_Sound_Free(dumpling_fight_music);
    CP_Sound_Free(butter_fight_music);
    CP_Sound_Free(boss_fight_music);
    CP_Sound_Free(win_screen_music);
    CP_Sound_Free(lose_screen_music);

    CP_Sound_Free(button_click_sound);
    CP_Sound_Free(button_hover_sound);
    CP_Sound_Free(player_projectile_sound);
    CP_Sound_Free(enemy_soundwave_sound);
    CP_Sound_Free(boss_ultimate_sound);
}
