//
// Tiny OpenGL v3 demo program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2022 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

/*#include <stdarg.h>
#include <FL/Fl.H>
#include <FL/platform.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#if defined(__APPLE__)
#  include <OpenGL/gl3.h> // defines OpenGL 3.0+ functions
#else
#  if defined(_WIN32)
#    define GLEW_STATIC 1
#  endif
#  include <GL/glew.h>
#endif
#include <FL/gl.h> // for gl_texture_reset()

*/
#include "SimpleGL3Window.h"
#include "math.h"
#include "MyCubeViewUI.h"

#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <fstream>
#include <string>

#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>


  SimpleGL3Window::SimpleGL3Window(int x, int y, int w, int h, const char* name) :  Fl_Gl_Window(x, y, w, h) {
    mode(FL_RGB8 | FL_DOUBLE | FL_OPENGL3);
    unlitShaderProgram = 0;
    gouraudShaderProgram = 0;
    gl_version_major = 0;

    auto thisIsMyFunction = testFunction;
    auto lol = (Fl_Timeout_Handler)&thisIsMyFunction;
    Fl::add_timeout(1.0/60.0, testFunction, (void*)this);
  }
  void SimpleGL3Window::testFunction(void *data) {
    SimpleGL3Window *thing = (SimpleGL3Window *)data;
    thing->redraw();
    Fl::repeat_timeout(1.0 / 60.0, testFunction, data);
  }
  void SimpleGL3Window::draw(void) {

      glEnable(GL_DEPTH_TEST);
      static float time = 0;
      time += speed;

     
    if (gl_version_major >= 3 && !hasDoneInit) {

       hasDoneInit = true;

     unlitShaderProgram = CreateShaderProgram("../src/vertex.vert", "../src/unlit.frag");
     gouraudShaderProgram = CreateShaderProgram("../src/vertex.vert", "../src/gouraud.frag");
     ActivateShaderProgram(gouraudShaderProgram);

      std::vector<std::string> textures = {"lambert3_Base_color.png", "lambert1_Base_color.png",
                                           "lambert2_Base_color.png"};
    }
    else if ((!valid())) {
      glViewport(0, 0, pixel_w(), pixel_h());
      valid(1);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (unlitShaderProgram && gouraudShaderProgram) {
      int uniformLocation = glGetUniformLocation(activeShaderProgram, "xOffset");
      int uniformLocation2 = glGetUniformLocation(activeShaderProgram, "yOffset");

       model = glm::mat4(1.0f);
      model = glm::rotate(glm::mat4(1.0f), yAxisModelRotation * ((3.14159265f)/180),
                          glm::vec3(0.0f, 1.0f, 0.0f));
       model = glm::rotate(model, xAxisModelRotation * ((3.14159265f) / 180),
                           glm::vec3(1.0f, 0.0f, 0.0f));
       model = glm::rotate(model, zAxisModelRotation * ((3.14159265f) / 180), 
                           glm::vec3(0.0f, 0.0f, 1.0f));

       model = glm::scale(model, glm::vec3(cubeScale, cubeScale, cubeScale));


      view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, cameraZoom));

      view = glm::rotate(view, 45 * (3.14159265f / 180), glm::vec3(0.0f, 1.0f, 0.0f));

      projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, -100.0f, 1000.0f);
      projection = glm::transpose(projection);
      
      // Turning the ortho projection into an oblique one.
      float s = zScale / orthoSize;
      projection[0][2] = +s * sin((3.14159265f/180) * -angle);
      projection[1][2] = -s * cos((3.14159265f/180) * -angle);
      projection[0][3] = -zOffset * projection[0][2];
      projection[1][3] = -zOffset * projection[1][2];
      projection = glm::transpose(projection);

      glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 

      int uniformLocation3 = glGetUniformLocation(activeShaderProgram, "model");
      glUniformMatrix4fv(uniformLocation3, 1, GL_FALSE, glm::value_ptr(model));

      int uniformLocation4 = glGetUniformLocation(activeShaderProgram, "view");
      glUniformMatrix4fv(uniformLocation4, 1, GL_FALSE, glm::value_ptr(view));

      int uniformLocation5 = glGetUniformLocation(activeShaderProgram, "projection");
      glUniformMatrix4fv(uniformLocation5, 1, GL_FALSE, glm::value_ptr(projection));

      if (currentModel != nullptr) {
		  currentModel->Draw();
      }
    }
    Fl_Gl_Window::draw(); // Draw FLTK child widgets.
  }
  int SimpleGL3Window::handle(int event) {
    static int first = 1;
    if (first && event == FL_SHOW && shown()) {
      first = 0;
      make_current();
#ifndef __APPLE__
      GLenum err = glewInit(); // defines pters to functions of OpenGL V 1.2 and above
#  ifdef FLTK_USE_WAYLAND
      // glewInit returns GLEW_ERROR_NO_GLX_DISPLAY with Wayland
      // see https://github.com/nigels-com/glew/issues/273
      if (fl_wl_display() && err == GLEW_ERROR_NO_GLX_DISPLAY) err = GLEW_OK;
#  endif
      if (err) Fl::warning("glewInit() failed returning %u", err);
      else add_output("Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
      const uchar *glv = glGetString(GL_VERSION);
      add_output("GL_VERSION=%s\n", glv);
      sscanf((const char *)glv, "%d", &gl_version_major);
      if (gl_version_major < 3) {
        add_output("\nThis platform does not support OpenGL V3 :\n"
                   "FLTK widgets will appear but the programmed "
                   "rendering pipeline will not run.\n");
        mode(mode() & ~FL_OPENGL3);
      }
    }

    int retval = Fl_Gl_Window::handle(event);
    if (retval) return retval;

    return retval;
  }
  void SimpleGL3Window::reset(void) {
    //shaderProgram = 0;
    gl_texture_reset();
  }
  //};


void SimpleGL3Window::toggle_double(Fl_Widget *wid, void *data) {
  static bool doublebuff = true;
  doublebuff = !doublebuff;
  SimpleGL3Window *glwin = (SimpleGL3Window*)data;
  int flags = glwin->mode();
  if (doublebuff) flags |= FL_DOUBLE; else flags &= ~FL_DOUBLE;
  glwin->reset();
  glwin->hide();
  glwin->mode(flags);
  glwin->show();
}




void SimpleGL3Window::output_win(SimpleGL3Window *gl)
{
  output = new Fl_Text_Display(300,0,500, 280);
  Fl_Light_Button *lb = new Fl_Light_Button(300, 280, 500, 20, "Double-Buffered");
  //lb->callback(toggle_double);
  lb->user_data(gl);
  lb->value(1);
  output->buffer(new Fl_Text_Buffer());
}


void SimpleGL3Window::add_output(const char *format, ...)
{
  /* va_list args;
  char line_buffer[10000];
  va_start(args, format);
  vsnprintf(line_buffer, sizeof(line_buffer)-1, format, args);
  va_end(args);
  output->buffer()->append(line_buffer);
  output->scroll(10000, 0);
  output->redraw();*/
}


void SimpleGL3Window::button_cb(Fl_Widget *widget, void *) {
  add_output("run %s callback\n", widget->label());
}

void SimpleGL3Window::add_widgets(Fl_Gl_Window *g) {
  Fl::set_color(FL_FREE_COLOR, 255, 255, 255, 140); // partially transparent white
  g->begin();
  // Create here widgets to go above the GL3 scene
  Fl_Button* b = new Fl_Button( 0, 0, 60, 30, "button");
  b->color(FL_FREE_COLOR);
  b->box(FL_DOWN_BOX );
 // b->callback(button_cb, NULL);
  Fl_Button* b2 = new Fl_Button( 0, 170, 60, 30, "button2");
  b2->color(FL_FREE_COLOR);
  b2->box(FL_BORDER_BOX );
 // b2->callback(button_cb, NULL);
  g->end();
}

void SimpleGL3Window::WriteToImage() {
  std::ofstream myFile;
  // myFile.open("danstestfile.bmp");

  char red[3] = {255, 255, 255};

  int width, height, channels;
  channels = 3;
  unsigned char *img = stbi_load("danstestfile.png", &width, &height, &channels, 0);
  width = uiStuff->cube->w();
  height = uiStuff->cube->h();
  // channels = 3;
  char *pixelData = new char[3 * width * height];
  std::cout << "Loaded image with width: " << width << " height: " << height
            << " channels: " << channels << std::endl;


  // directionsToCapture = 2;

  char *totalPixelData = new char[3 * (width) * (height * directionsToCapture)];

  char *gridPixelData = new char[3 * (width) * (height * directionsToCapture)];

  char *upsideDownPixelData = new char[3 * width * height * directionsToCapture];


  for (int i = 0; i < directionsToCapture; i++) {
    int currentDegrees = i * (360.0f / directionsToCapture);
    yAxisModelRotation = currentDegrees;
    xAxisModelRotation = 0;
    model = glm::mat4(1.0f);
    model = glm::rotate(glm::mat4(1.0f), yAxisModelRotation * ((3.14159265f) / 180),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, xAxisModelRotation * ((3.14159265f) / 180),
                        glm::vec3(1.0f, 0.0f, 0.0f));

    model = glm::scale(model, glm::vec3(cubeScale, cubeScale, cubeScale));

    int uniformLocation3 = glGetUniformLocation(activeShaderProgram, "model");
    glUniformMatrix4fv(uniformLocation3, 1, GL_FALSE, glm::value_ptr(model));

    int uniformLocation4 = glGetUniformLocation(activeShaderProgram, "view");
    glUniformMatrix4fv(uniformLocation4, 1, GL_FALSE, glm::value_ptr(view));

    int uniformLocation5 = glGetUniformLocation(activeShaderProgram, "projection");
    glUniformMatrix4fv(uniformLocation5, 1, GL_FALSE, glm::value_ptr(projection));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glDrawElements(GL_TRIANGLES, testIndices.size(), GL_UNSIGNED_INT, nullptr);
    if (currentModel != nullptr) {
		currentModel->Draw();
    }
    Fl_Gl_Window::draw();
    // glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, &totalPixelData[0+(width * height
    // * i * 3)]);

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE,
                 &totalPixelData[width * height * i * 3]);


    // for (int x = 0; x < 300; x++) {
    //  totalPixelData[x] = pixelData[x];
    //}
    // std::string filePath = "sprites/" + fileName;
  }

  /*
  for (int currentRow = 0; currentRow < height; currentRow++) {
          for (int i = 0; i < directionsToCapture; i++) {
                for (int j = 0; j < width * 3; j++) {
                //	flippedPixelData[j+(width*3*currentRow)+(width*3*i)] =
  totalPixelData[(i*width*height*3)+j+(width*3*currentRow)]; flippedPixelData[j +
  (width*3*directionsToCapture*currentRow)+(width * 3 * i)] = totalPixelData[(i * width * height *
  3) + j + (width * 3 * currentRow)];
                }
          }
  }
  */

  int amountOfGridRows = sqrt(directionsToCapture);
 

  /* for (int currentRow = 0; currentRow < height; currentRow++) {
    int currentGridRow = 0;
    for (int i = 0; i < directionsToCapture; i++) {
      if ((i % amountOfGridRows) == 0 && i != 0) {
        currentGridRow += 1;
      }
      for (int j = 0; j < width * 3; j++) {
        flippedPixelData[j + (width * 3 * amountOfGridRows * currentRow) +
                         (width * 3 * (i % amountOfGridRows)) +
                         (width * amountOfGridRows * 3 * height * currentGridRow)] =
            totalPixelData[(i * width * height * 3) + j + (width * 3 * currentRow)];
      }
    }
  } */


  for (int currentRow = 0; currentRow < height; currentRow++) {
    int currentGridRow = 0;
    for (int i = 0; i < directionsToCapture; i++) {
      if ((i % amountOfGridRows) == 0 && i != 0) {
        currentGridRow += 1;
      }
      for (int j = 0; j < width * 3; j++) {
        gridPixelData[(j + (width * 3 * amountOfGridRows * currentRow) +
                          (width * 3 * (i % amountOfGridRows)) +
                          (width * amountOfGridRows * 3 * height * currentGridRow))] =
            totalPixelData[((i * width * height * 3) + j + (width * 3 * currentRow))];
      }
    }
  }
  int totalArraySize = width * height * 3 * directionsToCapture;
  for (int i = width * height * directionsToCapture; i >= 0; i--) {

    int actualindex0 = (i*3) - 3;
    int actualindex1 = (i*3)- 2;
    int actualindex2 = (i*3) - 1;

    upsideDownPixelData[totalArraySize - i*3] = gridPixelData[actualindex0];
    upsideDownPixelData[(totalArraySize - (i*3)) + 1] = gridPixelData[actualindex1];
    upsideDownPixelData[(totalArraySize - (i * 3)) + 2] = gridPixelData[actualindex2];
  }

  stbi_flip_vertically_on_write(0);

  std::string fileName = "danssprite.png";
 // stbi_write_png(fileName.c_str(), width, height*directionsToCapture, channels, &totalPixelData[0], width * 3);
  //stbi_write_png(fileName.c_str(), width*amountOfGridRows, height*amountOfGridRows, channels,
    //             &gridPixelData[0], width * 3 * amountOfGridRows);
  if (stbi_write_png(fileName.c_str(), width * amountOfGridRows, height * amountOfGridRows, channels,
      &upsideDownPixelData[0], width * 3 * amountOfGridRows) > 0) {
      std::cout << "Successfully generated sprite sheet." << std::endl;
  }
  else {
      std::cout << "ERROR could not generate sprite sheet." << std::endl;
  }


  /* rotationAngle = 0;
  verticalRotationAngle = 0;
  model = glm::mat4(1.0f);
  // model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -5.0f));
  model = glm::rotate(glm::mat4(1.0f), rotationAngle * ((3.14159265f) / 180),
                      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, verticalRotationAngle * ((3.14159265f) / 180),
                      glm::vec3(1.0f, 0.0f, 0.0f));
  glClearColor(0.0f, 0.0f, 0.0f, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //int uniformLocation3 = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(uniformLocation3, 1, GL_FALSE, glm::value_ptr(model));
  glDrawElements(GL_TRIANGLES, sizeOfIndexData / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
  Fl_Gl_Window::draw();

  //myFile.close();*/
}

unsigned int SimpleGL3Window::CreateShaderProgram(std::string vertexSourcePath, std::string fragmentSourcePath)
{
    std::string vertexShaderSource;
    if (!ReadFile(vertexSourcePath, vertexShaderSource)) {
        return 0;
    }
    const char* vsSource = vertexShaderSource.c_str();
    std::string fragmentShaderSource;
    if (!ReadFile(fragmentSourcePath, fragmentShaderSource)) {
        return 0;
    }
    const char* fsSource = fragmentShaderSource.c_str();

    GLint err; GLchar CLOG[1000]; GLsizei length;

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &err);
    if (err != GL_TRUE) {
        glGetShaderInfoLog(vs, sizeof(CLOG), &length, CLOG);
        add_output("vs ShaderInfoLog=%s\n", CLOG);
        std::cout << CLOG << std::endl;
    }
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &err);
    if (err != GL_TRUE) {
        glGetShaderInfoLog(fs, sizeof(CLOG), &length, CLOG);
        add_output("fs ShaderInfoLog=%s\n", CLOG);
        std::cout << CLOG << std::endl;
    }
    // Attach the shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    //glBindFragDataLocation(shaderProgram, 0, "fragColour");
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &err);
    if (err != GL_TRUE) {
        glGetProgramInfoLog(shaderProgram, sizeof(CLOG), &length, CLOG);
        add_output("link log=%s\n", CLOG);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}

void SimpleGL3Window::LoadModel(std::string pathToModel)
{
    currentModel = new Model(pathToModel, uiStuff);
}

void SimpleGL3Window::DeleteCurrentModel()
{
    if (currentModel != nullptr) {
		delete currentModel;
        currentModel = nullptr;
    }
}

bool SimpleGL3Window::ReadFile(std::string filePath, std::string &contents)
{
    std::string line;
    std::string fileContents;

    std::ifstream file;
    file.open(filePath);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            fileContents.append(line + '\n');
        }
        file.close();

        contents = fileContents;
        return true;
    }
    else {
        std::cout << "Could not open " << filePath << " file." << std::endl;
        return false;
    }
}

void SimpleGL3Window::ActivateShaderProgram(unsigned int shaderProgram)
{
    activeShaderProgram = shaderProgram;
    glUseProgram(shaderProgram);
}

void SimpleGL3Window::ActivateGouraud()
{
    ActivateShaderProgram(gouraudShaderProgram);
}

void SimpleGL3Window::ActivateUnlit()
{
    ActivateShaderProgram(unlitShaderProgram);
}

/*
int main(int argc, char **argv)
{
  Fl::use_high_res_GL(1);
  Fl_Window *topwin = new Fl_Window(800, 300);
  SimpleGL3Window *win = new SimpleGL3Window(0, 0, 300, 300);
  win->end();
  output_win(win);
  add_widgets(win);
  topwin->end();
  topwin->resizable(win);
  topwin->label("Click GL panel to reshape");
  topwin->show(argc, argv);
  Fl::run();
}
*/
