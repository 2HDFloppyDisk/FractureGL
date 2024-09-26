#ifndef FMOD_MANAGER_H
#define FMOD_MANAGER_H

#include <FMOD/fmod.hpp>

class FMODManager {
public:
    static FMODManager& getInstance() {
        static FMODManager instance;  // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    FMOD::System* getSystem() {
        return fmodSystem;
    }

    void init() {
        if (fmodSystem == nullptr) {
            FMOD::System_Create(&fmodSystem);
            fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);
        }
    }

    void update() {
        if (fmodSystem) {
            fmodSystem->update();
        }
    }

    void cleanup() {
        if (fmodSystem) {
            fmodSystem->close();
            fmodSystem->release();
            fmodSystem = nullptr;
        }
    }

private:
    FMOD::System* fmodSystem = nullptr;

    // Private constructor to prevent instantiation
    FMODManager() {}

    // Delete copy constructor and assignment operator to prevent copies
    FMODManager(FMODManager const&) = delete;
    void operator=(FMODManager const&) = delete;
};

#endif
