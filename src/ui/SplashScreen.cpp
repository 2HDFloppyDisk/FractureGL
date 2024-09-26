#define STB_IMAGE_IMPLEMENTATION
#include "ui/SplashScreen.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

extern GLFWwindow* window;

// Load texture using stb_image
GLuint LoadTexture(const char* filePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, 0, 4); // Load image as RGBA
    if (!data) {
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}

void RenderSplashScreen(GLuint textureID, int windowWidth, int windowHeight, int imageWidth, int imageHeight, float alpha) {
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Calculate the aspect ratio of the image
    float imageAspect = (float)imageWidth / (float)imageHeight;

    // Determine the size of the image, keeping the aspect ratio
    int displayWidth = windowWidth * 0.5f;
    int displayHeight = static_cast<int>(displayWidth / imageAspect);
    if (displayHeight > windowHeight * 0.5f) {
        displayHeight = windowHeight * 0.5f;
        displayWidth = static_cast<int>(displayHeight * imageAspect);
    }

    // Calculate the position to center the image
    int xPos = (windowWidth - displayWidth) / 2;
    int yPos = (windowHeight - displayHeight) / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1); // Set orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set the alpha transparency
    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    // Render the image as a quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(xPos, yPos);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(xPos + displayWidth, yPos);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(xPos + displayWidth, yPos + displayHeight);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(xPos, yPos + displayHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND); // Disable blending after rendering

    // Reset the color to avoid affecting other renders
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ShowSplashScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to black
    // Load the OpenGL, FMOD, and engine logos
    GLuint openGLLogoTexture = LoadTexture("assets/images/opengl_logo.png");
    GLuint fmodLogoTexture = LoadTexture("assets/images/fmod_logo.png");
    GLuint engineLogoTexture = LoadTexture("assets/images/engine_logo.png");

    if (openGLLogoTexture == 0 || fmodLogoTexture == 0 || engineLogoTexture == 0) {
        return; // Exit if any texture loading failed
    }

    // Set logo durations (in seconds)
    float logoDuration = 5.0f; // Display each logo for 5 seconds
    float fadeDuration = 1.0f; // Duration of the fade-in and fade-out effect
    float startTime = glfwGetTime();

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Total splash screen duration for 3 logos
    float totalDuration = 3 * logoDuration;

    while (glfwGetTime() - startTime < totalDuration) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Determine which logo to display based on elapsed time
        float elapsedTime = glfwGetTime() - startTime;
        GLuint currentTexture;
        float logoStartTime;
        if (elapsedTime < logoDuration) {
            // Display the OpenGL logo
            currentTexture = openGLLogoTexture;
            logoStartTime = 0.0f;
        }
        else if (elapsedTime < 2 * logoDuration) {
            // Display the FMOD logo
            currentTexture = fmodLogoTexture;
            logoStartTime = logoDuration;
        }
        else {
            // Display the engine logo
            currentTexture = engineLogoTexture;
            logoStartTime = 2 * logoDuration;
        }

        // Calculate the fade-in and fade-out alpha value
        float logoElapsedTime = elapsedTime - logoStartTime;
        float alpha = 1.0f; // Default fully opaque
        if (logoElapsedTime < fadeDuration) {
            // Fade-in
            alpha = logoElapsedTime / fadeDuration;
        }
        else if (logoElapsedTime > (logoDuration - fadeDuration)) {
            // Fade-out
            alpha = (logoDuration - logoElapsedTime) / fadeDuration;
        }

        // Render the current logo with the calculated alpha value
        RenderSplashScreen(currentTexture, windowWidth, windowHeight, 727, 192, alpha);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up textures
    glDeleteTextures(1, &openGLLogoTexture);
    glDeleteTextures(1, &fmodLogoTexture);
    glDeleteTextures(1, &engineLogoTexture);
}
