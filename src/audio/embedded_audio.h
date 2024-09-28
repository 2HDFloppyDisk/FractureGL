#ifndef EMBEDDED_AUDIO_H
#define EMBEDDED_AUDIO_H

#include <map>
#include <string>
#include <cstddef>  // For size_t

extern std::map<std::string, unsigned char*> audioFiles;
extern std::map<std::string, size_t> audioFileSizes;

extern unsigned char embeddedMP3_1[];
extern size_t embeddedMP3_1_Size;

extern unsigned char embeddedMP3_2[];
extern size_t embeddedMP3_2_Size;

extern unsigned char embeddedMP3_3[];
extern size_t embeddedMP3_3_Size;

#endif // EMBEDDED_AUDIO_H
