#pragma once


extern "C" {
    int _write(int fd, char *ptr, size_t len) {
        (void) fd;
        return Serial.write(ptr, len);
    }
}
