# smail

### Project setup

1. **Download sdl2**

2. **Fork the repository and clone the repository to your local machine:**

   ```shell
   git clone https://github.com/<your_github_username>/smail
   ```
3. **download libs**
   1. **Fedora**
      ```shell
      sudo dnf install SDL2
      ```
      ```shell
      sudo dnf install SDL2_image-devel
      ```
   2. **Arch**
      ```shell
      sudo dnf install sdl2
      ```
      ```shell
      sudo dnf install sdl2_image
      ```
   

5. **Run code**

   ```shell
   gcc startpage.c -o myprograme -lSDL2 -lSDL2_ttf -lSDL2_image
   ```

   ```shell
   ./mymyprograme
   ```
