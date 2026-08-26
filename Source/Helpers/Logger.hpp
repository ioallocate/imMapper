#pragma once

#include <chrono>
#include <memory>

class Logger {
public:
    template<typename... Args>
    inline void Log(const char* format, Args... args) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);

        tm local_tm;
        localtime_s(&local_tm, &time);

        printf(
            "\x1b[38;2;255;255;255m["
            "\x1b[38;2;245;245;245m%02d"
            "\x1b[38;2;235;235;235m/"
            "\x1b[38;2;225;225;225m%02d"
            "\x1b[38;2;215;215;215m/"
            "\x1b[38;2;205;205;205m%04d"
            "\x1b[38;2;195;195;195m "
            "\x1b[38;2;185;185;185m%02d"
            "\x1b[38;2;175;175;175m:"
            "\x1b[38;2;165;165;165m%02d"
            "\x1b[38;2;155;155;155m:"
            "\x1b[38;2;145;145;145m%02d"
            "\x1b[38;2;135;135;135m][info]"
            "\x1b[0m ",
            local_tm.tm_mon + 1,
            local_tm.tm_mday,
            local_tm.tm_year + 1900,
            local_tm.tm_hour,
            local_tm.tm_min,
            local_tm.tm_sec
        );

        printf("\x1b[37m");
        printf(format, args...);
        printf("\x1b[0m\n");
    }
};

inline auto Console = std::make_unique<class Logger>();
