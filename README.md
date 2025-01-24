# sample minui sdl2 pak

This is a sample minui sdl2 pak. It demonstrates how to use the minui libraries to create a simple app displays some text to the screen.

## Requirements

- A minui union toolchain
- Docker (this folder is assumed to be the contents of the toolchain workspace directory)
- `make`

## Tutorial

> [!IMPORTANT]
> The instructions here only build an app for a single platform - in this case, the Trimui Smart Pro/Trimui Brick. If you want to build for a different platform, you'll need to adjust the instructions below.

### Building

To start, clone a toolchain and run it. For this example, we'll use the `union-tg5040-toolchain` toolchain.

```shell
git clone https://github.com/shauninman/union-tg5040-toolchain
```

Once it is cloned, run `make shell` in that toolchain directory to build and start the toolchain. This will take a few minutes, depending largely on your network speed.

```shell
cd union-tg5040-toolchain
make shell
```

Once the shell is setup, you can clone this repository in the toolchain workspace directory. You can run the following commands inside of the toolchain shell or on your host machine (so long as it is run in the workspace directory).

```shell
git clone https://github.com/josegonzalez/minui-sample-sdl2-app
```

Once the app is cloned, we'll do a bit of setup. Lets start by ensuring the MinUI repository is available. This is necessary in order to pull in libraries from the MinUI codebase for handling cross-platform building.

```shell
cd minui-sample-sdl2-app
make minui
```

Now we build the app. This should be performed inside the toolchain shell.

```shell
make
```

### Using the pak

With the binary built, we can now start playing with it on your device.

#### Installing the pak

1. Mount your TrimUI Brick SD card.
2. Create a pak folder with the path name `/Tools/tg5040/SDL2 Sample.pak` on your SD card.
3. Copy the compiled `main-tg5040` binary to the pak folder.
4. Copy the `launch.sh` script to the pak folder.
5. Unmount your SD Card and insert it into your TrimUI Brick.

#### Using the pak

You can now launch the pak from your device. This will bring up a faux UI with the first seven letters of the Greek alphabet spelled out.

Pressing the Menu, A, or B button will exit the pak and bring you back to MinUI. If you press the B button, the pak will exit with an error code.

Logs for the app will be available at `/.userdata/tg5040/logs/SDL2 Sample.txt`.

## Code walkthrough

Generally speaking, the only functions you will need to update are `draw_screen` and `handle_input`.

- `handle_input` is the function that handles input. It is called whenever input is detected.
- `draw_screen` is the function that draws the screen. It is called whenever the screen needs to be redrawn.

The `handle_input` function is a good place to mutate app state. In this example, it merely says to redraw the app, but it may also be useful to mutate a pointer to some app state. The `draw_screen` function can then be used to interpret the state of the app and draw it accordingly to the screen.
