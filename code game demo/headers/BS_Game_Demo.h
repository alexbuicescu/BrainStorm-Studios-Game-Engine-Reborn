#ifndef BS_GAME_DEMO_H_INCLUDED
#define BS_GAME_DEMO_H_INCLUDED


class BS_Game_Demo
{
private:
    static void loadCoinsAndDeaths();

    static void update_input();
    static void update_physics();
    static void update_game();
    static void render_game();

    static long current_time;
    static int FPS;

public:
    static void run_main_loop();
	static bool initialize_media();
    virtual ~BS_Game_Demo() = 0;
};


#endif // BS_GAME_DEMO_H_INCLUDED
