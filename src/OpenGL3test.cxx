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
    shaderProgram = 0;
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

     
    if (gl_version_major >= 3 && !shaderProgram) {
     // Upload vertices (1st four values in a row) and colours (following four values)
      /* float vertexData[] = { 
         -0.5, -0.5, 0.5,       0.882f, 0.035f, 0.086,
         -0.5,  0.5, 0.5,       0.882f, 0.035f, 0.086,
          0.5,  0.5, 0.5,       0.882f, 0.035f, 0.086,
          0.5, -0.5, 0.5,       0.882f, 0.035f, 0.086,


         -0.5, -0.5, -0.5,       0.960f, 0.698f, 0.004f,
         -0.5,  0.5, -0.5,       0.960f, 0.698f, 0.004f,
          0.5,  0.5, -0.5,       0.960f, 0.698f, 0.004f,
          0.5, -0.5, -0.5,       0.960f, 0.698f, 0.004f,

         -0.5,  0.5,  0.5,       0.196f, 0.6f, 0.0f,
         -0.5, -0.5,  0.5,       0.196f, 0.6f, 0.0f,
         -0.5, -0.5, -0.5,       0.196f, 0.6f, 0.0f,
         -0.5,  0.5, -0.5,       0.196f, 0.6f, 0.0f,

         0.5,   0.5,   0.5,       0.0, 0.6, 0.5,
         0.5,  -0.5,   0.5,       0.0, 0.6, 0.5,
         0.5,  -0.5,  -0.5,       0.0, 0.6, 0.5,
         0.5,   0.5,  -0.5,       0.0, 0.6, 0.5,

          -0.5,  0.5,   0.5,       0.960f, 0.698f, 1.0f,
          0.5,   0.5,   0.5,       0.960f, 0.698f, 1.0f,
         -0.5,  0.5,  -0.5,       0.960f, 0.698f, 1.0f,
          0.5,  0.5,  -0.5,       0.960f, 0.698f, 1.0f,


           -0.5,  -0.5,   0.5,       0.25f, 0.3f, 1.0f,
          0.5,   -0.5,   0.5,       0.25f, 0.3f, 1.0f,
         -0.5,  -0.5,  -0.5,       0.25f, 0.3f, 1.0f,
          0.5,  -0.5,  -0.5,       0.25f, 0.3f, 1.0f,
      };

      unsigned int indexData[] = {0, 1, 2, 2, 3, 0, 
                                  4, 5, 6, 6, 4, 7,
                                  8, 9, 10, 11, 8, 10,
                                  12, 13, 14, 12, 14, 15,
                                  16, 17, 18, 19, 18, 17,
                                  20, 21, 22, 23, 21, 22};*/
        
      //sizeOfIndexData = sizeof(indexData);

      Assimp::Importer importer;
     const aiScene *scene = importer.ReadFile("../models/stanfordbunny.obj", aiProcess_Triangulate | aiProcess_FlipUVs |
                                    aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);

      //float vertexData = scene->mMeshes[0]->mVertices;
      //testVertices = std::vector<float>();
      //aiMesh *mesh = scene->mMeshes[0];

      //testNormals = std::vector<float>();
	  //testIndices = std::vector<unsigned int>();

     

      
      /* for (int k = 0; k < scene->mNumMeshes; k++) {
		  for (int i = 0; i < scene->mMeshes[k]->mNumVertices; i++) {
			testVertices.push_back(scene->mMeshes[k]->mVertices[i].x);
			testVertices.push_back(scene->mMeshes[k]->mVertices[i].y);
			testVertices.push_back(scene->mMeshes[k]->mVertices[i].z);

			testVertices.push_back(scene->mMeshes[k]->mNormals[i].x);
			testVertices.push_back(scene->mMeshes[k]->mNormals[i].y);
			testVertices.push_back(scene->mMeshes[k]->mNormals[i].z);
		  }
		  for (int i = 0; i < scene->mMeshes[k]->mNumFaces; i++) {
			for (int j = 0; j < scene->mMeshes[k]->mFaces[i].mNumIndices; j++) {
			  testIndices.push_back(scene->mMeshes[k]->mFaces[i].mIndices[j]);
			}
		  }
      }*/

        //uiStuff->speedInput->value(speed);
      GLuint  vs;
      GLuint  fs;
      int Mslv, mslv; // major and minor version numbers of the shading language
      sscanf((char*)glGetString(GL_SHADING_LANGUAGE_VERSION), "%d.%d", &Mslv, &mslv);
      add_output("Shading Language Version=%d.%d\n",Mslv, mslv);

      const char *vss_format = "#version 330 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "layout (location = 1) in vec3 aNormal;\n"
                               "layout (location = 2) in vec2 aTex;\n"
                               "out vec4 colourV;\n"
                               "out vec4 lightDir;\n"
                                "out vec4 normal;\n"
                               "out vec2 TexCoord;\n"
                               "uniform float xOffset;\n"
                               "uniform float yOffset;\n"
                               "uniform mat4 model;\n"
                               "uniform mat4 view;\n"
                               "uniform mat4 projection;\n"
                               "void main()\n"
                               "{\n"

                                "TexCoord = aTex;\n"
                                "normal = vec4(aNormal.x, aNormal.y, aNormal.z, 1.0);"
                               "gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                               "colourV = vec4(1.0, 1.0, 1.0, 1.0);\n"
                               "lightDir = normalize(vec4(10.0, 10.0, 10.0, 1.0) - gl_Position);\n"
                               "}\0";

      char vss_string[1024]; const char *vss = vss_string;
      snprintf(vss_string, 1024, vss_format, Mslv, mslv);
      const char *fss_format="#version %d%d\n\
      in vec4 colourV;\
      out vec4 fragColour;\
      in vec4 lightDir;\
      in vec4 normal;\
in vec2 TexCoord;\
uniform sampler2D ourTexture;\
      void main(void)\
      {\
        float diff = max(dot(normal, lightDir), 0.0);\
      fragColour = texture(ourTexture, TexCoord);\
      }\0";
      char fss_string[512]; const char *fss = fss_string;
      snprintf(fss_string, 512, fss_format, Mslv, mslv);
      GLint err; GLchar CLOG[1000]; GLsizei length;
      vs = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vs, 1, &vss, NULL);
      glCompileShader(vs);
      glGetShaderiv(vs, GL_COMPILE_STATUS, &err);
      if (err != GL_TRUE) {
        glGetShaderInfoLog(vs, sizeof(CLOG), &length, CLOG);
        add_output("vs ShaderInfoLog=%s\n",CLOG);
        std::cout << CLOG << std::endl;
        }
      fs = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fs, 1, &fss, NULL);
      glCompileShader(fs);
      glGetShaderiv(fs, GL_COMPILE_STATUS, &err);
      if (err != GL_TRUE) {
        glGetShaderInfoLog(fs, sizeof(CLOG), &length, CLOG);
        add_output("fs ShaderInfoLog=%s\n",CLOG);
        std::cout << CLOG << std::endl;
        }
      // Attach the shaders
      shaderProgram = glCreateProgram();
      glAttachShader(shaderProgram, vs);
      glAttachShader(shaderProgram, fs);
      //glBindFragDataLocation(shaderProgram, 0, "fragColour");
      glLinkProgram(shaderProgram);
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &err);
      if (err != GL_TRUE) {
        glGetProgramInfoLog(shaderProgram, sizeof(CLOG), &length, CLOG);
        add_output("link log=%s\n", CLOG);
      }
      // Get pointers to uniforms and attributes
      //positionUniform = glGetUniformLocation(shaderProgram, "p");
      //colourAttribute = glGetAttribLocation(shaderProgram, "colour");
      //positionAttribute = glGetAttribLocation(shaderProgram, "position");
      glDeleteShader(vs);
      glDeleteShader(fs);
     
      //glGenVertexArrays(1, &vertexArrayObject);
      //glBindVertexArray(vertexArrayObject);

      std::vector<std::string> textures = {"lambert3_Base_color.png", "lambert1_Base_color.png",
                                           "lambert2_Base_color.png"};
      meshes = std::vector<Mesh>();
      for (int i = 0; i < scene->mNumMeshes; i++) {
        Mesh newMesh = Mesh(scene->mMeshes[i], textures[i], uiStuff);
        meshes.push_back(newMesh);
      }


      /*glGenBuffers(1, &vertexBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
      glBufferData(GL_ARRAY_BUFFER, testVertices.size()*sizeof(float), testVertices.data(), GL_STATIC_DRAW);

      glGenBuffers(1, &elementBuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, testIndices.size()*sizeof(unsigned int), testIndices.data(), GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                            (void *)(3 * sizeof(float)));
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);*/
      //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(3*sizeof(float)));
      //glEnableVertexAttribArray(1);

    }
    else if ((!valid())) {
      glViewport(0, 0, pixel_w(), pixel_h());
      valid(1);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (shaderProgram) {
      //GLfloat p[]={0,0};
      //glUniform2fv(positionUniform, 1, (const GLfloat *)&p);


        //int vertexCount = (sizeof(vertexData)/sizeof(float))/6;
      //int vertexCount = testVertices.size();


      //glDrawArrays(GL_TRIANGLES, 0, vertexCount);
      glUseProgram(shaderProgram);
      int uniformLocation = glGetUniformLocation(shaderProgram, "xOffset");
      //glUniform1f(uniformLocation, sin(time));
      int uniformLocation2 = glGetUniformLocation(shaderProgram, "yOffset");
      //glUniform1f(uniformLocation2, cos(time));

       model = glm::mat4(1.0f);
      // model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -5.0f));
      model = glm::rotate(glm::mat4(1.0f), rotationAngle * ((3.14159265f)/180),
                          glm::vec3(0.0f, 1.0f, 0.0f));
       model = glm::rotate(model, verticalRotationAngle * ((3.14159265f) / 180),
                           glm::vec3(1.0f, 0.0f, 0.0f));

       model = glm::scale(model, glm::vec3(cubeScale, cubeScale, cubeScale));


      view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, cameraZoom));

      //view = glm::rotate(view, 45 * (3.14159265f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
      view = glm::rotate(view, 45 * (3.14159265f / 180), glm::vec3(0.0f, 1.0f, 0.0f));

      //projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

      projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, -100.0f, 1000.0f);
      projection = glm::transpose(projection);
      
      float s = zScale / orthoSize;
      projection[0][2] = +s * sin((3.14159265f/180) * -angle);
      projection[1][2] = -s * cos((3.14159265f/180) * -angle);
      projection[0][3] = -zOffset * projection[0][2];
      projection[1][3] = -zOffset * projection[1][2];
      projection = glm::transpose(projection);


      glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 


      int uniformLocation3 = glGetUniformLocation(shaderProgram, "model");
      glUniformMatrix4fv(uniformLocation3, 1, GL_FALSE, glm::value_ptr(model));

      int uniformLocation4 = glGetUniformLocation(shaderProgram, "view");
      glUniformMatrix4fv(uniformLocation4, 1, GL_FALSE, glm::value_ptr(view));

      int uniformLocation5 = glGetUniformLocation(shaderProgram, "projection");
      glUniformMatrix4fv(uniformLocation5, 1, GL_FALSE, glm::value_ptr(projection));

      //glDrawElements(GL_TRIANGLES, testVertices.size(), GL_UNSIGNED_INT, nullptr);
      //glDrawElements(GL_TRIANGLES, testIndices.size(), GL_UNSIGNED_INT, nullptr);

      for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw();
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
     // redraw();
    }

    //redraw();

    int retval = Fl_Gl_Window::handle(event);
    if (retval) return retval;

    /* if (event == FL_PUSH && gl_version_major >= 3) {
      static float factor = 1.1;
      GLfloat data[4];
      glGetBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(GLfloat), data);
      if (data[0] < -0.88 || data[0] > -0.5) factor = 1/factor;
      data[0] *= factor;
      glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(GLfloat), data);
      glGetBufferSubData(GL_ARRAY_BUFFER, 24*sizeof(GLfloat), 4*sizeof(GLfloat), data);
      data[0] *= factor;
      glBufferSubData(GL_ARRAY_BUFFER, 24*sizeof(GLfloat), 4*sizeof(GLfloat), data);
      //redraw();
      add_output("push  Fl_Gl_Window::pixels_per_unit()=%.1f\n", pixels_per_unit());
      return 1;
    }*/

    return retval;
  }
  void SimpleGL3Window::reset(void) {
    shaderProgram = 0;
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
  unsigned char *img = stbi_load("danstestfile.png", &width, &height, &channels, 0);
  width = uiStuff->cube->w();
  height = uiStuff->cube->h();
  // channels = 3;
  char *pixelData = new char[3 * width * height];
  std::cout << "Loaded image with width: " << width << " height: " << height
            << " channels: " << channels << std::endl;

  Assimp::Importer importer;


  // glBindVertexArray(vertexArrayObject);

  // directionsToCapture = 2;

  char *totalPixelData = new char[3 * (width) * (height * directionsToCapture)];

  char *gridPixelData = new char[3 * (width) * (height * directionsToCapture)];

  char *upsideDownPixelData = new char[3 * width * height * directionsToCapture];


  for (int i = 0; i < directionsToCapture; i++) {
    int currentDegrees = i * (360.0f / directionsToCapture);
    rotationAngle = currentDegrees;
    verticalRotationAngle = 0;
    model = glm::mat4(1.0f);
    model = glm::rotate(glm::mat4(1.0f), rotationAngle * ((3.14159265f) / 180),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, verticalRotationAngle * ((3.14159265f) / 180),
                        glm::vec3(1.0f, 0.0f, 0.0f));

    model = glm::scale(model, glm::vec3(cubeScale, cubeScale, cubeScale));

    int uniformLocation3 = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(uniformLocation3, 1, GL_FALSE, glm::value_ptr(model));

    int uniformLocation4 = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniformLocation4, 1, GL_FALSE, glm::value_ptr(view));

    int uniformLocation5 = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(uniformLocation5, 1, GL_FALSE, glm::value_ptr(projection));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glDrawElements(GL_TRIANGLES, testIndices.size(), GL_UNSIGNED_INT, nullptr);
    for (int i = 0; i < meshes.size(); i++) {
      meshes[i].Draw();
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
  std::string fileName = "sprites/danssprite.png";
 // stbi_write_png(fileName.c_str(), width, height*directionsToCapture, channels, &totalPixelData[0], width * 3);
  //stbi_write_png(fileName.c_str(), width*amountOfGridRows, height*amountOfGridRows, channels,
    //             &gridPixelData[0], width * 3 * amountOfGridRows);
  stbi_write_png(fileName.c_str(), width * amountOfGridRows, height * amountOfGridRows, channels,
                 &upsideDownPixelData[0], width * 3 * amountOfGridRows);

 


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
