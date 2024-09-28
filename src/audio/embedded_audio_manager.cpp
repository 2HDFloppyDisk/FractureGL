#include "audio/embedded_audio.h"

std::map<std::string, unsigned char*> audioFiles = {
    { "Diatribe by Oliver Michael - Parhelion", embeddedMP3_1 },
    { "Once and for All by The Robbery Continues", embeddedMP3_2 },
    { "El Rio Fluye by El Rio Fluye", embeddedMP3_3 }
};

std::map<std::string, size_t> audioFileSizes = {
    { "Diatribe by Oliver Michael - Parhelion", embeddedMP3_1_Size },
    { "Once and for All by The Robbery Continues", embeddedMP3_2_Size },
    { "El Rio Fluye by El Rio Fluye", embeddedMP3_3_Size }
};

unsigned char* GetAudioData(const std::string& audioID) {
    if (audioFiles.count(audioID)) {
        return audioFiles[audioID];
    }
    return nullptr;  // Return null if audioID is not found
}

size_t GetAudioSize(const std::string& audioID) {
    if (audioFileSizes.count(audioID)) {
        return audioFileSizes[audioID];
    }
    return 0;  // Return 0 if audioID is not found
}
