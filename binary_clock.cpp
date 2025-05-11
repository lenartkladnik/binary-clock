#include <iostream>
#include <ctime>
#include <iomanip>
#include <bitset>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <bits/stdc++.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

const int bitSize = 6;
const std::vector<std::string> indicatorChars = {" 🯱 ", " 🯲 ", " 🯴 ", " 🯸 ", "🯱🯶 ", "🯳🯲 "};

void help(std::string progName) {
    std::cout << "Binary Clock - Displays the current time in binary format.\n";
    std::cout << "Usage: " + progName + " [OPTION]\n";
    std::cout << "Options:\n";
    std::cout << "  -i,  --indicators         Show the powers of two next to the binary representation.\n";
    std::cout << "  -t   --time-indicators    Show the hrs, min, sec indicators under each.\n";
    std::cout << "  -nt, --no-top             Remove the line that starts the program.\n";
    std::cout << "  -hr, --horizontal         Display the clock horizontally.\n";
    std::cout << "  -h,  --help               Display this help message and exit.\n";
}

void setNonBlocking(bool enable) {
    #ifndef _WIN32
        struct termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);

        if (enable) {
            ttystate.c_lflag &= ~ICANON;
            ttystate.c_lflag &= ~ECHO;
            ttystate.c_cc[VMIN] = 0;
            ttystate.c_cc[VTIME] = 0;
        } else {
            ttystate.c_lflag |= ICANON;
            ttystate.c_lflag |= ECHO;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    #endif
}

bool isKeyPressed() {
    #ifdef _WIN32
        return _kbhit() != 0;
    #else
        struct timeval tv = {0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        
        return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) == 1;
    #endif
}

char getInputChar() {
    #ifdef _WIN32c
        return _getch();
    #else
        char ch = 0;
        if (read(STDIN_FILENO, &ch, 1) != 1) {
            ch = 0;
        }
        return ch;
    #endif
}

void binTime(std::vector<std::string> full, std::vector<std::string> empty, int height, bool indicators, bool vertical, bool timeIndicator) {
    std::time_t now = std::time(nullptr);

    std::tm* localTime = std::localtime(&now);

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    std::bitset<bitSize> binHour(hour);
    std::bitset<bitSize> binMinute(minute);
    std::bitset<bitSize> binSecond(second);

    if (vertical){
        for (int i = bitSize - 1; i >= 0; i--){
            for (int j = 0; j < height; j++){
                if (j == std::round(height / 2) && indicators){
                    std::cout << indicatorChars[i];
                }else if (indicators){
                    std::cout << std::string(3, ' ');
                }

                if (static_cast<int>(binHour[i]) == 1){
                    std::cout << full[j];
                }else{
                    std::cout << empty[j];
                }

                if (static_cast<int>(binMinute[i]) == 1){
                    std::cout << full[j];
                }else{
                    std::cout << empty[j];
                }

                if (static_cast<int>(binSecond[i]) == 1){
                    std::cout << full[j] << std::endl;
                }else{
                    std::cout << empty[j] << std::endl;
                }
            }
        }

        std::string filler = indicators ? std::string(3, ' '): "";

        if (timeIndicator){
            std::cout << filler << "  h    m    s  ";
        }
    }else{
        for (int i = 0; i < height; i++){
            if (i == std::round(height / 2) && timeIndicator){
                std::cout << "h";
            }else if (timeIndicator){
                std::cout << " ";
            }

            for (int j = 0; j < bitSize; j++){
                if (static_cast<int>(binHour[j]) == 1){
                    std::cout << full[i];
                }else{
                    std::cout << empty[i];
                }
            }

            std::cout << std::endl;
        }
        
        for (int i = 0; i < height; i++){
            if (i == std::round(height / 2) && timeIndicator){
                std::cout << "m";
            }else if (timeIndicator){
                std::cout << " ";
            }
            
            for (int j = 0; j < bitSize; j++){
                if (static_cast<int>(binMinute[j]) == 1){
                    std::cout << full[i];
                }else{
                    std::cout << empty[i];
                }
            }

            std::cout << std::endl;
        }

        for (int i = 0; i < height; i++){
            if (i == std::round(height / 2) && timeIndicator){
                std::cout << "s";
            }else if (timeIndicator){
                std::cout << " ";
            }

            for (int j = 0; j < bitSize; j++){
                if (static_cast<int>(binSecond[j]) == 1){
                    std::cout << full[i];
                }else{
                    std::cout << empty[i];
                }
            }

            std::cout << std::endl;
        }

        if (indicators){
            std::string filler = timeIndicator ? " ": "";
            std::cout << filler;
            for (int i = 0; i < bitSize; i++){
                std::cout << ' ' << indicatorChars[i] << ' ';
            }
        }
    }
}

int main(int argc, char *argv[]){
    // ┏━━━┓
    // ┃   ┃
    // ┗━━━┛
    // ▗▄▄▄▖
    // ▐███▌
    // ▝▀▀▀▘  

    std::vector<std::string> empty = {"┏━━━┓", "┃   ┃", "┗━━━┛"};
    std::vector<std::string> full = {"▗▄▄▄▖", "▐███▌", "▝▀▀▀▘"};

    int height = std::min(full.size(), empty.size());

    #ifndef _WIN32
        setNonBlocking(true);
    #endif

    char ch = 0;

    bool indicators = false;
    bool no_top = false;
    bool vertical = true;
    bool t_ind = false;

    for (int i = 0; i < argc; i++){
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help"){
            help(std::string(argv[0]));

            return 0;
        } 
        else if (arg == "-i" || arg == "--indicators"){
            indicators = true;
        }
        else if (arg == "-nt" || arg == "--no-top"){
            no_top = true;
        }
        else if (arg == "-hr" || arg == "--horizontal"){
            vertical = false;
        }
        else if (arg == "-t" || arg == "--time-indicators"){
            t_ind = true;
        }
        else if (arg == "-inv" || arg == "--inverted"){
            full = {"┏━━━┓", "┃   ┃", "┗━━━┛"};
            empty = {"▗▄▄▄▖", "▐███▌", "▝▀▀▀▘"};
        }
    }

    if (no_top){
        std::cout << "\x1b[A\x1b[2K";
    }
    
    int jump_h = vertical ? height * bitSize : height * height;

    while (true) {
        binTime(full, empty, height, indicators, vertical, t_ind);

        if (isKeyPressed()) {
            ch = getInputChar();
            if (ch == 'q') {
                if (!vertical && indicators || vertical && t_ind){
                    std::cout << std::endl;
                }
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "\x1b[" + std::to_string(jump_h) + "A\x1b[999999999D";
    }

    #ifndef _WIN32
        setNonBlocking(false);
    #endif

    return 0;
}
