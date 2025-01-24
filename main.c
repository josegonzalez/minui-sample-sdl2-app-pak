#include <stdio.h>
#include <unistd.h>
#include <msettings.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "api.h"
#include "utils.h"

FILE *log_file = NULL;

// log_info logs a message to stdout for debugging purposes
void log_info(const char *msg)
{
    // Set stdout to unbuffered mode
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("%s\n", msg);
}

// AppState holds the current state of the application
struct AppState
{
    int redraw;                  // whether the screen needs to be redrawn
    int quitting;                // whether the app should exit
    int exit_code;               // the exit code to return
    int show_brightness_setting; // whether to show the brightness or hardware state
};

// handle_input interprets input events and mutates app state
void handle_input(struct AppState *state)
{
    PAD_poll();
    if (PAD_anyPressed())
    {
        log_info("input press detected");
        char buf[256];
        snprintf(buf, sizeof(buf), "button press: %d -- %d:%d", pad.just_pressed, BTN_B, BTN_MENU);
        log_info(buf);
        state->redraw = 1;
    }

    if (PAD_anyJustReleased())
    {
        log_info("input release detected");
        char buf[256];
        snprintf(buf, sizeof(buf), "button release: %d -- %d:%d", pad.just_released, BTN_B, BTN_MENU);
        log_info(buf);
        state->redraw = 1;
    }

    if (PAD_justReleased(BTN_A) || PAD_justReleased(BTN_B) || PAD_justReleased(BTN_MENU))
    {
        log_info("quitting...");
        state->redraw = 0;
        state->quitting = 1;
        if (PAD_justReleased(BTN_B))
        {
            state->exit_code = EXIT_FAILURE;
        }
    }
}

// draw_screen interprets the app state and draws it to the screen
void draw_screen(SDL_Surface *screen, struct AppState *state)
{
    // draw the button group on the button-right
    GFX_blitButtonGroup((char *[]){"B", "ERROR", "A", "QUIT", NULL}, 1, screen, 1);

    // the rest of the function is just for drawing your app to the screen
    const char *greek_letters[] = {
        "Alpha", "Beta", "Gamma", "Delta",
        "Epsilon", "Zeta", "Eta"};

    for (int i = 0; i < 7; i++)
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "Rendering '%s'...", greek_letters[i]);
        log_info(buf);
        SDL_Surface *text = TTF_RenderUTF8_Blended(font.large, greek_letters[i], COLOR_WHITE);

        SDL_Rect pos = {
            0,                                     // center horizontally
            (screen->h - (7 * 80)) / 2 + (i * 80), // distribute vertically with 40px spacing
            text->w,
            text->h};
        SDL_BlitSurface(text, NULL, screen, &pos);
        SDL_FreeSurface(text);
    }

    // don't forget to reset the should_redraw flag
    state->redraw = 0;
}

// main is the entry point for the app
int main(int argc, char *argv[])
{
    // set the cpu speed to the menu speed
    // this is done here to ensure we downclock
    // the menu (no need to draw power unnecessarily)
    PWR_setCPUSpeed(CPU_SPEED_MENU);

    // initialize:
    // - the screen, allowing us to draw to it
    // - input from the pad/joystick/buttons/etc.
    // - power management
    // - sync hardware settings (brightness, hdmi, speaker, etc.)
    SDL_Surface *screen = GFX_init(MODE_MAIN);
    PAD_init();
    PWR_init();
    InitSettings();

    // Initialize app state
    struct AppState state = {
        .redraw = 1,
        .quitting = 0,
        .exit_code = EXIT_SUCCESS,
        .show_brightness_setting = 0};

    // get initial wifi state
    int was_online = PLAT_isOnline();

    while (!state.quitting)
    {
        // handle turning the on/off screen on/off
        // as well as general power management
        PWR_update(&state.redraw, NULL, NULL, NULL);

        // check if the device is on wifi
        // redraw if the wifi state changed
        // and then update our state
        int is_online = PLAT_isOnline();
        if (was_online != is_online)
        {
            state.redraw = 1;
        }
        was_online = is_online;

        // handle any input events
        handle_input(&state);

        // redraw the screen if there has been a change
        if (state.redraw)
        {
            // clear the screen at the beginning of each loop
            GFX_clear(screen);

            // draw the hardware information in the top-right
            GFX_blitHardwareGroup(screen, state.show_brightness_setting);

            // draw the setting hints
            if (state.show_brightness_setting)
            {
                GFX_blitHardwareHints(screen, state.show_brightness_setting);
            }

            // your draw logic goes here
            draw_screen(screen, &state);

            // Not sure what this does just yet
            GFX_flip(screen);
        }
        else
        {
            // Not sure what this does just yet
            GFX_sync();
        }
    }

    // cleanup in reverse order
    QuitSettings();
    PWR_quit();
    PAD_quit();
    GFX_quit();

    // exit the program
    return state.exit_code;
}