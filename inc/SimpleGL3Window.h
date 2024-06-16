#pragma once
#include <stdarg.h>
#include <FL/Fl.H>
#include <FL/platform.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include "SimpleGL3Window.h"
#if defined(__APPLE__)
  #ifndef GL_SILENCE_DEPRECATION
    #define GL_SILENCE_DEPRECATION 1
  #endif
#  include <OpenGL/gl3.h> // defines OpenGL 3.0+ functions
#else
#  if defined(_WIN32)
#    define GLEW_STATIC 1
#  endif
#  include <GL/glew.h>
#endif
#include <FL/gl.h> // for gl_texture_reset()


#include "glm/glm.hpp"
#include <vector>
#include "Model.h"


class MyCubeViewUI;
class SimpleGL3Window : public Fl_Gl_Window {
    GLuint activeShaderProgram;
    GLuint unlitShaderProgram;
    GLuint gouraudShaderProgram;
    GLuint vertexArrayObject;
    GLuint vertexBuffer;
    GLuint elementBuffer;
    GLint positionUniform;
    GLint colourAttribute;
    GLint positionAttribute;
    int gl_version_major;
    Fl_Text_Display* output; // shared between output_win() and add_output()
    int sizeOfIndexData;

    bool hasDoneInit = false;
public:
    MyCubeViewUI* uiStuff;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    float speed = 0.01;

    // Exposing model rotation for UI
    float xAxisModelRotation = 0;
    float yAxisModelRotation = 0;
    float zAxisModelRotation = 0;

    Model* currentModel = nullptr;

    float cubeScale = 1.0f;
    int directionsToCapture = 8;

    float zScale = 1;
    float zOffset = -5.0f;
    float angle = 0;

    float cameraZoom = -5.0f;
    float orthoSize = 5.0f;

    
    
    SimpleGL3Window(int x, int y, int w, int h, const char* name);// : Fl_Gl_Window(x, y, w, h)
    void draw(void) FL_OVERRIDE;
    int handle(int event) FL_OVERRIDE;
    void reset(void);
    //void add_output(const char* format, ...);
    void toggle_double(Fl_Widget* wid, void* data);
    void output_win(SimpleGL3Window* gl);
    void add_output(const char* format, ...);
    void button_cb(Fl_Widget* widget, void*);
    void add_widgets(Fl_Gl_Window* g);

    void WriteToImage();

    unsigned int CreateShaderProgram(std::string vertexSourcePath, std::string fragmentSourcePath);

    void LoadModel(std::string pathToModel);
    void DeleteCurrentModel();


    bool ReadFile(std::string filePath, std::string &contents);

    void ActivateShaderProgram(unsigned int shaderProgram);
    void ActivateGouraud();
    void ActivateUnlit();


    void static testFunction(void* data);
};
