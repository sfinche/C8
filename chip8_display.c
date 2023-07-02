#include <epoxy/gl.h>
#include "gtk/gtk.h"
#include "debug.h"

// globals for now...
GLuint vbo = 0;
GLuint vao = 0;
GLuint shader_programme;

static bool on_render(GtkGLArea *area, GdkGLContext *context) {
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shader_programme);
  glBindVertexArray(vao);
  // draw points 0-3 from the currently bound VAO with current in-use shader
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glFlush();

  return false;
}

static void on_realize(GtkGLArea *area, GdkGLContext *context) {

  // write out opengl version info
  print_opengl_info();

  gtk_gl_area_make_current(area);

// buffers
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

// define shaders
  const char* vertex_shader =
"#version 400\n"
"in vec3 vp;"
"void main() {"
"  gl_Position = vec4(vp, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.6, 0.2, 0.2, 1.0);"
"}";

// compile shaders

GLuint vs = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vs, 1, &vertex_shader, NULL);
glCompileShader(vs);
GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fs, 1, &fragment_shader, NULL);
glCompileShader(fs);

// attach to program
shader_programme = glCreateProgram();
glAttachShader(shader_programme, fs);
glAttachShader(shader_programme, vs);
glLinkProgram(shader_programme);

};

GtkWidget *chip8_add_display(GtkWindow *window) {
    GtkWidget *gl_area = gtk_gl_area_new();
    g_signal_connect(gl_area, "render", G_CALLBACK(on_render), NULL);
    g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);
    gtk_window_set_child(window, gl_area);
    return gl_area;
}