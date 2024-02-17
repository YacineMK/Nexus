# Nexus

### Project setup

1. **Download sdl2**

2. **Fork the repository and clone the repository to your local machine:**

   ```shell
   git clone https://github.com/YacineMK/smail
   ```
3. **installing dependecies**
   1. **Fedora**
      ```shell
      sudo dnf install SDL2 SDL2_image-devel SDL2_ttf
      ```

   3. **Arch**
      ```shell
      sudo pacman -S install sdl2 sdl2_image sdl2_ttf
      ```
5. **Run code**

   ```shell
   gcc -o myprogram main.c logic.h logic.c draw.c -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm
   ```

   ```shell
   ./mymyprograme
   ```

